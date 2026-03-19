import argparse
import math

import matplotlib.dates as mdates
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
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


def _parse_offset(value: str) -> pd.Timedelta:
    """
    오프셋 문자열을 pandas Timedelta로 변환합니다.
    예: '0s', '+2s', '-150ms', '1min'
    """
    v = (value or "").strip()
    if v == "":
        return pd.Timedelta(0)
    return pd.Timedelta(v)


def _shift_series_time(df: pd.DataFrame, offset: pd.Timedelta) -> pd.DataFrame:
    if df is None or df.empty or offset == pd.Timedelta(0):
        return df
    out = df.copy()
    out["datetime"] = pd.to_datetime(out["datetime"]) + offset
    out.attrs.update(df.attrs)
    return out


def _abs_timedelta(td: pd.Timedelta) -> pd.Timedelta:
    return td if td >= pd.Timedelta(0) else -td


def _parse_timedelta_pair(value: str) -> tuple[pd.Timedelta, pd.Timedelta]:
    """
    "-5s,5s" 같은 형태를 (min, max) Timedelta로 파싱합니다.
    """
    parts = [p.strip() for p in value.split(",")]
    if len(parts) != 2:
        raise ValueError("--auto-offset-range는 'min,max' 형식이어야 합니다. 예: -5s,5s")
    a = pd.Timedelta(parts[0])
    b = pd.Timedelta(parts[1])
    if b < a:
        a, b = b, a
    return a, b


def _get_lebt_series_by_dt(beam: BeamData, start_dt: pd.Timestamp, end_dt: pd.Timestamp) -> pd.DataFrame:
    """
    BeamData의 self.df에서 datetime 범위로 직접 필터링하여 LEBT 시계열을 반환합니다.
    (오프셋 때문에 날짜 경계를 넘을 수 있어 문자열 기반 get_data_list 대신 사용)
    """
    if not hasattr(beam, "df") or beam.df is None or beam.df.empty:
        return pd.DataFrame(columns=["datetime", "value"])

    df = beam.df
    if "datetime" not in df.columns or "LEBT" not in df.columns:
        return pd.DataFrame(columns=["datetime", "value"])

    if not pd.api.types.is_datetime64_any_dtype(df["datetime"]):
        df = df.copy()
        df["datetime"] = pd.to_datetime(df["datetime"])

    mask = (df["datetime"] >= start_dt) & (df["datetime"] <= end_dt) & df["LEBT"].notna()
    out = df.loc[mask, ["datetime", "LEBT"]].rename(columns={"LEBT": "value"}).copy()
    out.attrs["label"] = "LEBT"
    return out


def _get_control_series_by_dt(
    control: ControlData,
    col: int | str,
    start_dt: pd.Timestamp,
    end_dt: pd.Timestamp,
    label: str | None = None,
) -> pd.DataFrame:
    """
    ControlData를 dt 범위로 잘라 특정 컬럼(BeamDump 등) 시계열을 반환합니다.
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


def _merge_by_timestamp(
    lebt: pd.DataFrame,
    dump: pd.DataFrame,
    tolerance: str,
    direction: str = "nearest",
) -> pd.DataFrame:
    """
    LEBT(datetime, value)와 BeamDump(datetime, value)를 timestamp 기준으로 병합합니다.

    - 기본은 pandas.merge_asof로 "가장 가까운 시간"을 매칭합니다.
    - tolerance 예: '2s', '500ms', '1min'
    """
    if lebt is None or dump is None or lebt.empty or dump.empty:
        return pd.DataFrame(columns=["datetime", "LEBT", "BeamDump"])

    a = lebt.copy()
    b = dump.copy()
    a["datetime"] = pd.to_datetime(a["datetime"])
    b["datetime"] = pd.to_datetime(b["datetime"])
    a = a.sort_values("datetime").rename(columns={"value": "LEBT"})[["datetime", "LEBT"]]
    b = b.sort_values("datetime").rename(columns={"value": "BeamDump"})[["datetime", "BeamDump"]]

    tol = pd.Timedelta(tolerance)
    merged = pd.merge_asof(
        a,
        b,
        on="datetime",
        direction=direction,
        tolerance=tol,
    )
    merged = merged.dropna(subset=["LEBT", "BeamDump"]).reset_index(drop=True)
    return merged


def _collect_series_for_offset_search(
    beam: BeamData,
    control: ControlData,
    date_list: list[str],
    time_start: str,
    time_end: str,
    beamdump_col: int | str,
    beamdump_label: str,
    offset_lebt: pd.Timedelta,
    offset_dump: pd.Timedelta,
    pad: pd.Timedelta,
    resample_rule: str | None,
) -> tuple[pd.DataFrame, pd.DataFrame]:
    """
    오프셋 자동 추정을 위해 날짜별 구간을 모아서 (LEBT, BeamDump) 시계열을 반환합니다.
    - pad만큼 양쪽으로 확장해서 가져와서, 후보 오프셋을 적용했을 때도 데이터가 빠지지 않게 합니다.
    - 반환되는 df의 datetime은 "표시 기준"(=오프셋 적용 후)입니다. (추가 delta는 나중에 적용)
    """
    lebt_all = []
    dump_all = []

    for d in date_list:
        day_start_dt = pd.to_datetime(f"{d} {time_start}")
        day_end_dt = pd.to_datetime(f"{d} {time_end}")
        if day_end_dt < day_start_dt:
            raise ValueError(
                "한 날짜 내에서 time-end가 time-start보다 빠릅니다. (자정 넘어가는 구간은 현재 미지원)"
            )

        # raw fetch window를 pad만큼 확장
        lebt_raw_start = day_start_dt - offset_lebt - pad
        lebt_raw_end = day_end_dt - offset_lebt + pad
        dump_raw_start = day_start_dt - offset_dump - pad
        dump_raw_end = day_end_dt - offset_dump + pad

        lebt = _get_lebt_series_by_dt(beam, lebt_raw_start, lebt_raw_end)
        dump = _get_control_series_by_dt(
            control, beamdump_col, dump_raw_start, dump_raw_end, label=beamdump_label
        )

        # base offset 적용(표시 기준으로 변환)
        lebt = _shift_series_time(lebt, offset_lebt)
        dump = _shift_series_time(dump, offset_dump)

        lebt = _maybe_resample(lebt, resample_rule)
        dump = _maybe_resample(dump, resample_rule)

        if not lebt.empty:
            lebt_all.append(lebt)
        if not dump.empty:
            dump_all.append(dump)

    lebt_all_df = pd.concat(lebt_all, ignore_index=True) if lebt_all else pd.DataFrame()
    dump_all_df = pd.concat(dump_all, ignore_index=True) if dump_all else pd.DataFrame()
    return lebt_all_df, dump_all_df


def _estimate_best_offset(
    lebt_df: pd.DataFrame,
    dump_df: pd.DataFrame,
    target: str,
    delta_min: pd.Timedelta,
    delta_max: pd.Timedelta,
    step: pd.Timedelta,
    tolerance: str,
    metric: str,
    min_matches: int,
) -> tuple[pd.Timedelta, float, int]:
    """
    delta를 스캔해서 metric이 최대가 되는 오프셋을 찾습니다.
    metric:
      - 'pearson': Pearson r 최대
      - 'abs_pearson': |Pearson r| 최대
    """
    if lebt_df is None or dump_df is None or lebt_df.empty or dump_df.empty:
        raise RuntimeError("자동 오프셋 추정에 필요한 데이터가 부족합니다. (LEBT/BeamDump가 비어있음)")

    if step <= pd.Timedelta(0):
        raise ValueError("--auto-offset-step은 0보다 커야 합니다.")

    # inclusive range
    deltas = pd.timedelta_range(delta_min, delta_max, freq=step)
    if len(deltas) == 0:
        raise ValueError("auto-offset range/step 설정으로 스캔할 delta가 없습니다.")

    best_delta = pd.Timedelta(0)
    best_r = float("nan")
    best_score = float("-inf")
    best_n = 0

    for delta in deltas:
        if target == "beamdump":
            dump_s = _shift_series_time(dump_df, delta)
            merged = _merge_by_timestamp(lebt_df, dump_s, tolerance=tolerance)
        else:
            lebt_s = _shift_series_time(lebt_df, delta)
            merged = _merge_by_timestamp(lebt_s, dump_df, tolerance=tolerance)

        n = len(merged)
        if n < min_matches:
            continue
        r = merged["LEBT"].corr(merged["BeamDump"])
        if pd.isna(r):
            continue

        score = abs(r) if metric == "abs_pearson" else r
        if score > best_score:
            best_score = score
            best_delta = pd.Timedelta(delta)
            best_r = float(r)
            best_n = int(n)

    if best_score == float("-inf"):
        raise RuntimeError(
            f"오프셋 스캔 결과 유효한 매칭이 없습니다. (min_matches={min_matches}, tolerance={tolerance}를 완화해보세요)"
        )
    return best_delta, best_r, best_n


def _plot_correlation(
    df: pd.DataFrame,
    kind: str,
    beamdump_label: str,
    title: str,
    gridsize: int = 60,
    alpha: float = 0.35,
    bins: str | int | None = "log",
    cmap: str = "viridis",
    mincnt: int = 1,
    log_norm: bool = True,
    range_xy: tuple[float, float, float, float] | None = None,
    vmin: float | None = None,
    vmax: float | None = None,
):
    """
    df columns: datetime, LEBT, BeamDump
    kind: 'scatter' | 'hexbin' | 'hist2d'
    """
    if df is None or df.empty:
        raise RuntimeError("병합된 DF가 비어있습니다. (tolerance/기간/컬럼을 확인해주세요)")

    fig, ax = plt.subplots(figsize=(6.2, 5.5))
    x = df["LEBT"].to_numpy()
    y = df["BeamDump"].to_numpy()

    if kind == "scatter":
        ax.scatter(x, y, s=8, alpha=alpha, linewidths=0)
    elif kind == "hexbin":
        extent = None
        if range_xy is not None:
            xmin, xmax, ymin, ymax = range_xy
            extent = (xmin, xmax, ymin, ymax)
        hb = ax.hexbin(
            x,
            y,
            gridsize=gridsize,
            bins=bins,
            mincnt=mincnt,
            cmap=cmap,
            extent=extent,
            vmin=vmin,
            vmax=vmax,
        )
        fig.colorbar(hb, ax=ax, label="counts")
    elif kind == "hist2d":
        hist_range = None
        if range_xy is not None:
            xmin, xmax, ymin, ymax = range_xy
            hist_range = [[xmin, xmax], [ymin, ymax]]
        norm = LogNorm() if log_norm else None
        h = ax.hist2d(
            x,
            y,
            bins=(gridsize, gridsize),
            range=hist_range,
            cmap=cmap,
            norm=norm,
            cmin=mincnt,
            vmin=vmin,
            vmax=vmax,
        )
        fig.colorbar(h[3], ax=ax, label="counts")
    else:
        raise ValueError("--corr-kind는 scatter|hexbin|hist2d 중 하나여야 합니다.")

    ax.set_xlabel("LEBT")
    ax.set_ylabel(beamdump_label)
    ax.set_title(title)
    ax.grid(True, alpha=0.2)
    plt.tight_layout()
    return fig


def _parse_corr_bins(value: str) -> str | int | None:
    """
    - 'log' -> 'log' (hexbin의 bins='log')
    - 'none'/'linear' -> None
    - 정수 -> int
    """
    v = value.strip().lower()
    if v in ("log",):
        return "log"
    if v in ("none", "linear"):
        return None
    return int(v)


def _parse_corr_range(value: str) -> tuple[float, float, float, float]:
    """
    "xmin,xmax,ymin,ymax" 형태를 파싱합니다.
    """
    parts = [p.strip() for p in value.split(",")]
    if len(parts) != 4:
        raise ValueError("--corr-range는 'xmin,xmax,ymin,ymax' 형식이어야 합니다.")
    xmin, xmax, ymin, ymax = map(float, parts)
    if xmax <= xmin or ymax <= ymin:
        raise ValueError("--corr-range의 min/max 관계가 올바르지 않습니다.")
    return xmin, xmax, ymin, ymax


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
        "--offset-lebt",
        default="0s",
        help="LEBT 타임스탬프 오프셋 (예: +2s, -150ms, 0s). 값만큼 datetime을 이동",
    )
    parser.add_argument(
        "--offset-beamdump",
        default="0s",
        help="BeamDump 타임스탬프 오프셋 (예: +2s, -150ms, 0s). 값만큼 datetime을 이동",
    )
    parser.add_argument(
        "--auto-offset",
        action="store_true",
        help="두 데이터의 상대 타임 오프셋을 자동으로 추정해서 적용합니다(스캔 기반).",
    )
    parser.add_argument(
        "--auto-offset-target",
        default="beamdump",
        choices=["beamdump", "lebt"],
        help="어느 쪽 타임스탬프를 이동시켜 정렬할지 (기본: beamdump)",
    )
    parser.add_argument(
        "--auto-offset-range",
        default="-5s,5s",
        help=(
            "스캔할 오프셋 범위 'min,max' (예: -5s,5s). 값이 '-'로 시작하므로 "
            "쉘/argparse 오해를 피하려면 --auto-offset-range='-5s,5s' 또는 "
            "--auto-offset-range "
            "\"-5s,5s\" 처럼 입력하세요."
        ),
    )
    parser.add_argument(
        "--auto-offset-step",
        default="100ms",
        help="스캔 스텝 (예: 100ms, 1s) (기본: 100ms)",
    )
    parser.add_argument(
        "--auto-offset-metric",
        default="abs_pearson",
        choices=["pearson", "abs_pearson"],
        help="최적화 기준 (기본: abs_pearson)",
    )
    parser.add_argument(
        "--auto-offset-min-matches",
        type=int,
        default=50,
        help="오프셋 평가에 필요한 최소 매칭 샘플 수 (기본: 50)",
    )
    parser.add_argument(
        "--auto-offset-only",
        action="store_true",
        help="자동 오프셋만 추정해서 출력하고 종료",
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
        "--correlation",
        action="store_true",
        help="LEBT와 BeamDump를 timestamp 기준으로 병합한 뒤 2D correlation 플롯을 그림",
    )
    parser.add_argument(
        "--corr-kind",
        default="hexbin",
        choices=["scatter", "hexbin", "hist2d"],
        help="correlation 플롯 종류 (기본: hexbin)",
    )
    parser.add_argument(
        "--tolerance",
        default="2s",
        help="merge_asof 매칭 허용 시간차 (예: 2s, 500ms, 1min) (기본: 2s)",
    )
    parser.add_argument(
        "--corr-gridsize",
        type=int,
        default=150,
        help="hexbin/hist2d 해상도(클수록 더 촘촘함) (기본: 150)",
    )
    parser.add_argument(
        "--corr-bins",
        default="log",
        help="hexbin의 bins 옵션: log | none | <int> (기본: log)",
    )
    parser.add_argument(
        "--corr-log",
        action="store_true",
        help="hist2d에서 color scale을 로그로 표시(LogNorm). (기본: on)",
    )
    parser.add_argument(
        "--corr-no-log",
        action="store_true",
        help="hist2d에서 로그 컬러 스케일을 끔",
    )
    parser.add_argument(
        "--corr-cmap",
        default="viridis",
        help="컬러맵 이름 (기본: viridis)",
    )
    parser.add_argument(
        "--corr-mincnt",
        type=int,
        default=1,
        help="최소 카운트(이보다 작은 bin은 표시 안 함) (기본: 1)",
    )
    parser.add_argument(
        "--corr-range",
        default=None,
        help="2D 히스토그램 범위: 'xmin,xmax,ymin,ymax' (예: -1e-6,5e-5,0,1). 지정 시 자동 스케일 대신 고정",
    )
    parser.add_argument(
        "--corr-vmin",
        type=float,
        default=None,
        help="컬러바 최소값(선택)",
    )
    parser.add_argument(
        "--corr-vmax",
        type=float,
        default=None,
        help="컬러바 최대값(선택)",
    )
    parser.add_argument(
        "--save-merged-csv",
        default=None,
        help="병합된 DF를 CSV로 저장 (예: misc/lebt_beamdump_merged.csv)",
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
    offset_lebt = _parse_offset(args.offset_lebt)
    offset_dump = _parse_offset(args.offset_beamdump)

    # 자동 오프셋 추정(필요하면 먼저 수행)
    if args.auto_offset:
        delta_min, delta_max = _parse_timedelta_pair(args.auto_offset_range)
        step = pd.Timedelta(args.auto_offset_step)
        pad = max(_abs_timedelta(delta_min), _abs_timedelta(delta_max))
        # merge_asof tolerance도 고려해서 약간 더 넉넉히
        pad = pad + pd.Timedelta(args.tolerance)

        # auto-offset은 base offset이 적용된 "표시 기준" 시계열 위에서 추가 delta를 찾습니다.
        lebt_all_df, dump_all_df = _collect_series_for_offset_search(
            beam=beam,
            control=control,
            date_list=date_list,
            time_start=args.time_start,
            time_end=args.time_end,
            beamdump_col=beamdump_col,
            beamdump_label=args.beamdump_label,
            offset_lebt=offset_lebt,
            offset_dump=offset_dump,
            pad=pad,
            resample_rule=args.resample,
        )

        best_delta, best_r, best_n = _estimate_best_offset(
            lebt_df=lebt_all_df,
            dump_df=dump_all_df,
            target=args.auto_offset_target,
            delta_min=delta_min,
            delta_max=delta_max,
            step=step,
            tolerance=args.tolerance,
            metric=args.auto_offset_metric,
            min_matches=args.auto_offset_min_matches,
        )

        if args.auto_offset_target == "beamdump":
            offset_dump = offset_dump + best_delta
        else:
            offset_lebt = offset_lebt + best_delta

        print(
            f"[auto-offset] target={args.auto_offset_target}, best_delta={best_delta}, "
            f"metric={args.auto_offset_metric}, r={best_r:.4f}, n={best_n}"
        )
        print(f"[auto-offset] applied offsets: offset_lebt={offset_lebt}, offset_beamdump={offset_dump}")

        if args.auto_offset_only:
            return

    # correlation 모드: 여러 날짜여도 전체 기간을 한 DF로 합쳐서 2D 플롯
    if args.correlation:
        lebt_all = []
        dump_all = []

        for d in date_list:
            day_start_dt = pd.to_datetime(f"{d} {args.time_start}")
            day_end_dt = pd.to_datetime(f"{d} {args.time_end}")
            if day_end_dt < day_start_dt:
                raise ValueError(
                    "한 날짜 내에서 time-end가 time-start보다 빠릅니다. (자정 넘어가는 구간은 현재 미지원)"
                )

            # 오프셋 적용을 위해 조회 구간을 반대로 보정해서 가져온 뒤, datetime을 +offset으로 이동
            lebt = _get_lebt_series_by_dt(beam, day_start_dt - offset_lebt, day_end_dt - offset_lebt)
            dump = _get_control_series_by_dt(
                control,
                beamdump_col,
                day_start_dt - offset_dump,
                day_end_dt - offset_dump,
                label=args.beamdump_label,
            )
            lebt = _shift_series_time(lebt, offset_lebt)
            dump = _shift_series_time(dump, offset_dump)
            lebt = _maybe_resample(lebt, args.resample)
            dump = _maybe_resample(dump, args.resample)
            if not lebt.empty:
                lebt_all.append(lebt)
            if not dump.empty:
                dump_all.append(dump)

        lebt_all_df = pd.concat(lebt_all, ignore_index=True) if lebt_all else pd.DataFrame()
        dump_all_df = pd.concat(dump_all, ignore_index=True) if dump_all else pd.DataFrame()

        merged = _merge_by_timestamp(lebt_all_df, dump_all_df, tolerance=args.tolerance)
        if args.save_merged_csv:
            merged.to_csv(args.save_merged_csv, index=False)

        title = f"2D correlation (tolerance={args.tolerance})"
        range_xy = _parse_corr_range(args.corr_range) if args.corr_range else None
        bins = _parse_corr_bins(args.corr_bins)
        log_norm = True if args.corr_log else (False if args.corr_no_log else True)
        fig = _plot_correlation(
            merged,
            kind=args.corr_kind,
            beamdump_label=args.beamdump_label,
            title=title,
            gridsize=args.corr_gridsize,
            bins=bins,
            cmap=args.corr_cmap,
            mincnt=args.corr_mincnt,
            log_norm=log_norm,
            range_xy=range_xy,
            vmin=args.corr_vmin,
            vmax=args.corr_vmax,
        )

        if args.out:
            fig.savefig(args.out, dpi=150, bbox_inches="tight")
        else:
            plt.show()
        return

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

            lebt = _get_lebt_series_by_dt(beam, day_start_dt - offset_lebt, day_end_dt - offset_lebt)
            dump = _get_control_series_by_dt(
                control,
                beamdump_col,
                day_start_dt - offset_dump,
                day_end_dt - offset_dump,
                label=args.beamdump_label,
            )
            lebt = _shift_series_time(lebt, offset_lebt)
            dump = _shift_series_time(dump, offset_dump)
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
    lebt = _get_lebt_series_by_dt(beam, start_dt - offset_lebt, end_dt - offset_lebt)
    dump = _get_control_series_by_dt(
        control,
        beamdump_col,
        start_dt - offset_dump,
        end_dt - offset_dump,
        label=args.beamdump_label,
    )
    lebt = _shift_series_time(lebt, offset_lebt)
    dump = _shift_series_time(dump, offset_dump)
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


