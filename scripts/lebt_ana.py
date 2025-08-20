
# INSERT_YOUR_CODE
import pandas as pd

def load_lebt_vs_yield_sheet(sheet_name, file_path='misc/lebt_vs_yield.xlsx'):
    """
    misc 폴더의 lebt_vs_yield 엑셀 파일에서 지정한 시트(sheet_name)를 불러와 데이터프레임으로 반환합니다.
    """
    try:
        df = pd.read_excel(file_path, sheet_name=sheet_name)
        print(f"'{sheet_name}' 시트에서 데이터프레임을 성공적으로 불러왔습니다.")
        return df
    except Exception as e:
        print(f"엑셀 파일 또는 시트 로딩 중 오류 발생: {e}")
        return None

# INSERT_YOUR_CODE
import matplotlib.pyplot as plt

def plot_lebt_and_yields(df):
    """
    df의 'LEBT' 열과 그 뒤의 다른 열들을 run을 x축으로 하여 두 개의 y축(좌: LEBT, 우: 나머지 yield들)으로 한꺼번에 플롯합니다.
    'target' 열은 무시합니다.
    x축은 중간 틱 없이 정수로 끊기게 표시합니다.
    선 없이 마커만 그립니다.
    양쪽 y축 모두 0부터 시작합니다.
    """
    if df is None or 'Run' not in df.columns or 'LEBT' not in df.columns:
        print("데이터프레임에 'Run' 또는 'LEBT' 컬럼이 없습니다.")
        return

    # 'target' 열은 무시
    cols = [col for col in df.columns if col not in ['Run', 'Target', 'Time']]
    if 'LEBT' not in cols:
        print("'LEBT' 컬럼이 없습니다.")
        return

    # LEBT와 나머지 yield 열 분리
    y_lebt = df['LEBT'] # * df['Time'] # LEBT는 초당 카운트이므로 시간을 곱해줘야 함
    x_run = df['Run']
    yield_cols = [col for col in cols if col != 'LEBT']
    # 일드 컬럼 전체를 time 컬럼으로 나누기
    for col in yield_cols:
        df[col] = df[col] / df['Time']

    fig, ax1 = plt.subplots(figsize=(10,6))

    # LEBT plot (좌측 y축) - 선 없이 마커만
    ax1.plot(x_run, y_lebt, 'x-', color='tab:red', label='LEBT')
    ax1.set_xlabel('Run')
    ax1.set_ylabel('LEBT(Integrated)', color='tab:red')
    ax1.tick_params(axis='y', labelcolor='tab:red')
    ax1.set_ylim(bottom=0)  # 좌측 y축 0부터 시작

    # x축을 정수로만, 중간 틱 없이 설정
    import matplotlib.ticker as ticker
    min_run = int(x_run.min())
    max_run = int(x_run.max())
    ax1.xaxis.set_major_locator(ticker.MaxNLocator(integer=True, prune=None))
    ax1.set_xticks(range(min_run, max_run+1))
    ax1.tick_params(axis='x', which='major', length=7)

    # 우측 y축
    ax2 = ax1.twinx()
    for col in yield_cols:
        ax2.plot(x_run, df[col], 'o-', label=col)  # 선 없이 마커만
    ax2.set_ylabel('Yield', color='tab:blue')
    ax2.tick_params(axis='y', labelcolor='tab:blue')
    ax2.set_ylim(bottom=0)  # 우측 y축 0부터 시작

    # 범례
    lines1, labels1 = ax1.get_legend_handles_labels()
    lines2, labels2 = ax2.get_legend_handles_labels()
    ax2.legend(lines1 + lines2, labels1 + labels2, loc='best')

    plt.title(f'Average LEBT vs Yields/sec for {df["Target"].iloc[0]} Runs')
    plt.tight_layout()
    plt.show()

def plot_lebt_vs_yield_scatter(df_list):
    """
    여러 개의 DataFrame(df_list)에서 LEBT와 각 isotope yield/sec 간의 상관관계를 한 번에 산점도로 그립니다.
    x축: yield/sec (isotope별)
    y축: LEBT
    DataFrame마다 컬럼 구조가 다를 수 있으므로, 모든 df에서 가능한 isotope 컬럼을 자동으로 수집합니다.
    """
    import matplotlib.pyplot as plt

    if not df_list or not isinstance(df_list, list):
        print("DataFrame 리스트가 필요합니다.")
        return

    # 모든 df에서 isotope yield 컬럼을 수집 (Run, Target, Time, LEBT 제외)
    isotope_set = set()
    for df in df_list:
        if df is not None:
            isotope_set.update([col for col in df.columns if col not in ['Run', 'Target', 'Time', 'LEBT']])
    isotope_cols = sorted(list(isotope_set))
    if not isotope_cols:
        print("isotope yield 컬럼이 없습니다.")
        return

    plt.figure(figsize=(8, 6))
    colors = plt.get_cmap('tab10', len(isotope_cols))

    for idx, col in enumerate(isotope_cols):
        x_all = []
        y_all = []
        for df in df_list:
            if df is not None and col in df.columns and 'LEBT' in df.columns and 'Time' in df.columns:
                # NaN 값은 제외
                valid = df[[col, 'LEBT', 'Time']].dropna()
                if not valid.empty:
                    #x_all.extend((valid[col] / valid['Time']).tolist())
                    x_all.extend(valid[col].tolist())
                    y_all.extend((valid['LEBT'] * valid['Time']).tolist())
        if x_all and y_all:
            plt.scatter(x_all, y_all, label=col, color=colors(idx), alpha=0.8)

    plt.xlim(left=0)
    plt.ylim(bottom=0)
    plt.xlabel('Yield')
    plt.ylabel('Integrated LEBT')
    plt.title(f'LEBT vs Yield ({df_list[0]["Target"].iloc[0]})')
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.5)
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    isos = ['18O', '19O', '20O', '21O']
    df_list = []
    for iso in isos:
        df = load_lebt_vs_yield_sheet(iso)
        df_list.append(df)

    for df in df_list:
        plot_lebt_vs_yield_scatter([df])
