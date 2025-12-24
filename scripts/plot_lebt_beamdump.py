import argparse
import math

import matplotlib.dates as mdates
import matplotlib.pyplot as plt
import pandas as pd

from beam_data import BeamData
from control_data import ControlData


def _parse_col(value: str):
    """
    --beamdump-col 인자를 int(컬럼 인덱스) 또는 str(컬럼명)으로 파싱합니다.
    """
    try:
        return int(value)
    except ValueError:
        return value


def _ensure_datetime_index(df: pd.DataFrame) -> pd.DataFrame:
    """
    ControlData의 df는 보통 DatetimeIndex지만, 혹시 아닐 경우를 대비해 보정합니다.
    """
    if isinstance(df.index, pd.DatetimeIndex):
        return df.sort_index()

    # 첫 번째 컬럼이 시간이라고 가정(=ControlData.re_indexing_control_data 방식)
    time_col = df.columns[0]
    df = df.copy()
    df[time_col] = pd.to_datetime(df[time_col])
    df = df.set_index(time_col).sort_index()
    return df


def _slice_time(df: pd.DataFrame, start_dt: pd.Timestamp, end_dt: pd.Timestamp) -> pd.DataFrame:
    df = _ensure_datetime_index(df)
    # loc slicing은 DatetimeIndex에서 inclusive
    return df.loc[start_dt:end_dt]


def _get_control_series(
    control: ControlData,
    col: int | str,
    start_dt: pd.Timestamp,
    end_dt: pd.Timestamp,
    label: str | None = None,
) -> pd.DataFrame:
    """
    control.df에서 지정된 컬럼의 시계열을 (datetime, value) 형태로 반환합니다.
    """
    df = _slice_time(control.df, start_dt, end_dt)
    if df.empty:
        return pd.DataFrame(columns=["datetime", "value"])

    if isinstance(col, int):
        if col < 0 or col >= len(df.columns):
            raise IndexError(
                f"--beamdump-col={col} 가 범위를 벗어났습니다. (0 ~ {len(df.columns) - 1})"
            )
        col_name = df.columns[col]
    else:
        col_name = col
        if col_name not in df.columns:
            raise KeyError(f"'{col_name}' 컬럼이 ControlData에 없습니다.")

    out = df[[col_name]].copy()
    out = out.rename(columns={col_name: "value"})
    out["datetime"] = out.index
    out = out[["datetime", "value"]]
    out.attrs["label"] = label if label is not None else str(col_name)
    return out


def _get_lebt_series(
    beam: BeamData,
    date_start: str,
    time_start: str,
    time_end: str,
    date_end: str | None = None,
) -> pd.DataFrame:
    df = beam.get_data_list("LEBT", date_start, time_start, time_end, date_end)
    if df is None or df.empty:
        return pd.DataFrame(columns=["datetime", "value"])
    out = df[["datetime", "LEBT"]].rename(columns={"LEBT": "value"}).copy()
    out.attrs["label"] = "LEBT"
    return out


def _maybe_resample(df: pd.DataFrame, rule: str | None) -> pd.DataFrame:
    """
    rule 예: '1S', '2S', '100ms', '1min'
    """
    if df is None or df.empty or rule is None:
        return df

    out = df.copy()
    out["datetime"] = pd.to_datetime(out["datetime"])
    out = out.set_index("datetime").resample(rule).mean(numeric_only=True).reset_index()
    out.attrs.update(df.attrs)
    return out


def _print_control_columns(control: ControlData):
    df = control.df
    print("=== ControlData columns ===")
    for i, c in enumerate(df.columns):
        print(f"{i:3d}: {c}")


def _date_list_from_range(date_start: str, date_end: str) -> list[str]:
    start = pd.to_datetime(date_start).normalize()
    end = pd.to_datetime(date_end).normalize()
    if end < start:
        raise ValueError("date-end가 date-start보다 빠릅니다.")
    return [d.strftime("%Y-%m-%d") for d in pd.date_range(start, end, freq="D")]


def _parse_dates_arg(dates: str) -> list[str]:
    # "YYYY-MM-DD,YYYY-MM-DD,..." 형태
    items = [x.strip() for x in dates.split(",") if x.strip()]
    if not items:
        raise ValueError("--dates가 비어있습니다.")

    # validate + normalize
    out = [pd.to_datetime(s).strftime("%Y-%m-%d") for s in items]

    # 중복 제거(순서 유지)
    seen = set()
    uniq = []
    for d in out:
        if d not in seen:
            uniq.append(d)
            seen.add(d)
    return uniq


def _plot_one_panel(
    ax1: plt.Axes,
    lebt: pd.DataFrame,
    dump: pd.DataFrame,
    beamdump_label: str,
    title: str,
    show_ylabel_left: bool,
    show_ylabel_right: bool,
):
    ax2 = ax1.twinx()

    h_lebt = None
    h_dump = None

    if not lebt.empty:
        h_lebt = ax1.plot(lebt["datetime"], lebt["value"], lw=1.0, color="tab:blue")[0]
    if not dump.empty:
        h_dump = ax2.plot(dump["datetime"], dump["value"], lw=1.0, color="tab:red", alpha=0.85)[0]

    ax1.set_title(title)
    ax1.grid(True, alpha=0.25)

    # 날짜별 subplot에서는 시간만 보여주는게 깔끔함
    ax1.xaxis.set_major_locator(mdates.AutoDateLocator(minticks=3, maxticks=6))
    ax1.xaxis.set_major_formatter(mdates.DateFormatter("%H:%M"))

    if show_ylabel_left:
        ax1.set_ylabel("LEBT")
    else:
        ax1.set_ylabel("")

    if show_ylabel_right:
        ax2.set_ylabel(beamdump_label)
    else:
        ax2.set_ylabel("")

    return h_lebt, h_dump


def main():
    parser = argparse.ArgumentParser(
        description="BeamData(LEBT)와 ControlData(BeamDump)를 같은 플롯에 표시합니다."
    )
    parser.add_argument("--date-start", required=True, help="시작 날짜 (YYYY-MM-DD)")
    parser.add_argument("--time-start", required=True, help="시작 시간 (HH:MM:SS)")
    parser.add_argument("--time-end", required=True, help="끝 시간 (HH:MM:SS)")
    parser.add_argument("--date-end", default=None, help="끝 날짜 (YYYY-MM-DD), 기본: date-start")
    parser.add_argument(
        "--dates",
        default=None,
        help="콤마로 구분한 날짜 리스트(YYYY-MM-DD,YYYY-MM-DD,...). 지정하면 date-start/date-end 대신 이 목록을 사용",
    )
    parser.add_argument(
        "--ncols",
        type=int,
        default=3,
        help="여러 날짜를 서브플롯으로 그릴 때 컬럼 개수 (기본: 3)",
    )
    parser.add_argument(
        "--beamdump-col",
        default="9",
        help="ControlData에서 BeamDump 컬럼 (기본: 9; 숫자는 인덱스, 문자열은 컬럼명)",
    )
    parser.add_argument(
        "--beamdump-label",
        default="BeamDump",
        help="오른쪽 y축 라벨/legend 이름",
    )
    parser.add_argument(
        "--resample",
        default=None,
        help="두 데이터 모두 리샘플링(평균) 간격. 예: 1S, 2S, 100ms, 1min",
    )
    parser.add_argument(
        "--out",
        default=None,
        help="저장 파일 경로 (예: misc/lebt_beamdump.png). 없으면 화면에 표시",
    )
    parser.add_argument(
        "--list-control-columns",
        action="store_true",
        help="ControlData 컬럼 인덱스/이름을 출력하고 종료",
    )
    args = parser.parse_args()

    beam = BeamData()
    control = ControlData()

    if args.list_control_columns:
        _print_control_columns(control)
        return

    # 날짜 목록 결정
    if args.dates is not None:
        date_list = _parse_dates_arg(args.dates)
        date_start = date_list[0]
        date_end = date_list[-1]
    else:
        date_end = args.date_end if args.date_end is not None else args.date_start
        date_list = _date_list_from_range(args.date_start, date_end)
        date_start = args.date_start

    start_dt = pd.to_datetime(f"{date_start} {args.time_start}")
    end_dt = pd.to_datetime(f"{date_end} {args.time_end}")
    if end_dt < start_dt:
        raise ValueError("끝 시간이 시작 시간보다 빠릅니다. date-end/time-end를 확인해주세요.")

    beamdump_col = _parse_col(args.beamdump_col)

    # 날짜가 여러 개면 한 figure 안에 서브플롯으로 배치
    if len(date_list) > 1:
        ncols = max(1, int(args.ncols))
        nrows = math.ceil(len(date_list) / ncols)
        fig, axes = plt.subplots(
            nrows=nrows,
            ncols=ncols,
            figsize=(5.2 * ncols, 3.2 * nrows),
            sharex=False,
            squeeze=False,
        )

        legend_handles = {"LEBT": None, "BeamDump": None}

        for idx, d in enumerate(date_list):
            r, c = divmod(idx, ncols)
            ax1 = axes[r][c]

            day_start_dt = pd.to_datetime(f"{d} {args.time_start}")
            day_end_dt = pd.to_datetime(f"{d} {args.time_end}")
            if day_end_dt < day_start_dt:
                raise ValueError(
                    "한 날짜 내에서 time-end가 time-start보다 빠릅니다. (자정 넘어가는 구간은 현재 미지원)"
                )

            lebt = _get_lebt_series(beam, d, args.time_start, args.time_end, None)
            dump = _get_control_series(
                control, beamdump_col, day_start_dt, day_end_dt, label=args.beamdump_label
            )
            lebt = _maybe_resample(lebt, args.resample)
            dump = _maybe_resample(dump, args.resample)

            show_ylabel_left = (c == 0)
            show_ylabel_right = (c == ncols - 1)
            h_lebt, h_dump = _plot_one_panel(
                ax1=ax1,
                lebt=lebt,
                dump=dump,
                beamdump_label=args.beamdump_label,
                title=d,
                show_ylabel_left=show_ylabel_left,
                show_ylabel_right=show_ylabel_right,
            )

            if legend_handles["LEBT"] is None and h_lebt is not None:
                legend_handles["LEBT"] = h_lebt
            if legend_handles["BeamDump"] is None and h_dump is not None:
                legend_handles["BeamDump"] = h_dump

        # 남은 subplot 숨기기
        for j in range(len(date_list), nrows * ncols):
            r, c = divmod(j, ncols)
            axes[r][c].axis("off")

        # 공통 라벨/타이틀
        fig.suptitle(
            f"LEBT vs {args.beamdump_label} (daily subplots, {args.time_start}~{args.time_end})",
            y=0.995,
        )
        fig.tight_layout(rect=(0, 0.03, 1, 0.97))

        # 공통 legend (가능할 때만)
        handles = []
        labels = []
        if legend_handles["LEBT"] is not None:
            handles.append(legend_handles["LEBT"])
            labels.append("LEBT")
        if legend_handles["BeamDump"] is not None:
            handles.append(legend_handles["BeamDump"])
            labels.append(args.beamdump_label)
        if handles:
            fig.legend(handles, labels, loc="lower right")

        if args.out:
            fig.savefig(args.out, dpi=150, bbox_inches="tight")
        else:
            plt.show()
        return

    # 날짜가 1개면 기존처럼 한 장으로
    lebt = _get_lebt_series(beam, date_start, args.time_start, args.time_end, date_end)
    dump = _get_control_series(control, beamdump_col, start_dt, end_dt, label=args.beamdump_label)
    lebt = _maybe_resample(lebt, args.resample)
    dump = _maybe_resample(dump, args.resample)

    if lebt.empty and dump.empty:
        raise RuntimeError("지정한 기간에 LEBT/BeamDump 데이터가 모두 없습니다.")

    fig, ax1 = plt.subplots(figsize=(12, 5))
    ax2 = ax1.twinx()

    lines = []
    labels = []

    if not lebt.empty:
        l1 = ax1.plot(lebt["datetime"], lebt["value"], lw=1.2, color="tab:blue")[0]
        lines.append(l1)
        labels.append(lebt.attrs.get("label", "LEBT"))
        ax1.set_ylabel("LEBT")

    if not dump.empty:
        l2 = ax2.plot(dump["datetime"], dump["value"], lw=1.2, color="tab:red", alpha=0.85)[0]
        lines.append(l2)
        labels.append(dump.attrs.get("label", "BeamDump"))
        ax2.set_ylabel(args.beamdump_label)

    ax1.set_xlabel("Time")
    ax1.xaxis.set_major_locator(mdates.AutoDateLocator(minticks=4, maxticks=10))
    ax1.xaxis.set_major_formatter(mdates.DateFormatter("%m-%d %H:%M"))
    fig.autofmt_xdate()

    if lines:
        ax1.legend(lines, labels, loc="upper left")

    ax1.grid(True, alpha=0.3)
    ax1.set_title(f"LEBT vs {args.beamdump_label} ({start_dt} ~ {end_dt})")

    plt.tight_layout()
    if args.out:
        fig.savefig(args.out, dpi=150, bbox_inches="tight")
    else:
        plt.show()


if __name__ == "__main__":
    main()


