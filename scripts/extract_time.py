import subprocess
import glob
import csv
import os
import re

MIDCHECK_EXEC = './midcheck'
DATA_DIR = 'rawdata/KO_24_30'
CSV_FILE = 'midcheck_summary.csv'

def parse_output(output):
    lines = output.strip().split('\n')
    result = {}
    for line in lines:
        if line.startswith('Start Time'):
            result['start_time'] = line.split(':', 1)[1].strip()
        elif line.startswith('Stop Time'):
            result['stop_time'] = line.split(':', 1)[1].strip()
        elif line.startswith('Elapsed second'):
            result['elapsed_sec'] = line.split(':', 1)[1].strip().split()[0]
    return result

def extract_run_number(filename):
    # 예: run1234.mid.lz4 → 1234
    match = re.search(r'run(\d+)\.mid\.lz4', filename)
    if match:
        return match.group(1)
    return filename  # 매칭 안 되면 파일명 전체 반환

def main():
    files = sorted(glob.glob(os.path.join(DATA_DIR, 'run*.mid.lz4')))
    with open(CSV_FILE, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['run_number', 'start_time', 'stop_time', 'elapsed_sec'])
        for f in files:
            try:
                proc = subprocess.run([MIDCHECK_EXEC, f], capture_output=True, text=True, check=True)
                result = parse_output(proc.stdout)
                run_number = extract_run_number(os.path.basename(f))
                writer.writerow([
                    run_number,
                    result.get('start_time', ''),
                    result.get('stop_time', ''),
                    result.get('elapsed_sec', '')
                ])
                print(f"완료: {f}")
            except Exception as e:
                print(f"실패: {f} - {e}")

if __name__ == '__main__':
    main()