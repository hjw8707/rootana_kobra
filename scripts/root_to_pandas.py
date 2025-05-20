#!/usr/bin/env python3

import uproot
import pandas as pd
import awkward as ak
import numpy as np
import sys
import os
import argparse

def clean_branch_name(branch):
    """브랜치 이름에서 문제가 될 수 있는 문자를 제거합니다."""
    return branch.split('/')[-1].split('.')[-1]

def get_array_length(arr):
    """배열의 길이를 안전하게 가져옵니다."""
    if isinstance(arr, (np.ndarray, list)):
        return len(arr)
    elif isinstance(arr, ak.Array):
        return len(ak.flatten(arr, axis=None))
    return 0

def pad_or_trim_array(arr, target_length):
    """배열의 길이를 target_length에 맞게 조정합니다."""
    if isinstance(arr, ak.Array):
        arr = ak.to_numpy(arr)

    current_length = len(arr)
    if current_length == target_length:
        return arr
    elif current_length < target_length:
        # 배열이 더 짧은 경우 NaN으로 패딩
        padded = np.full(target_length, np.nan, dtype=float)
        padded[:current_length] = arr
        return padded
    else:
        # 배열이 더 긴 경우 자르기
        return arr[:target_length]

def root_to_pandas(root_file_path, tree_name="tree", selected_branches=None):
    """
    ROOT 파일의 tree를 pandas DataFrame으로 변환합니다.

    Parameters:
    -----------
    root_file_path : str
        ROOT 파일의 경로
    tree_name : str
        변환할 tree의 이름 (기본값: "tree")
    selected_branches : list
        변환할 브랜치 이름 목록 (None인 경우 모든 브랜치 변환)

    Returns:
    --------
    pandas.DataFrame
        변환된 DataFrame
    """
    try:
        # ROOT 파일 열기
        with uproot.open(root_file_path) as file:
            # tree 가져오기
            tree = file[tree_name]

            # 사용 가능한 브랜치 출력
            print(f"\n사용 가능한 브랜치:")
            all_branches = tree.keys()
            for branch in all_branches:
                print(f"- {branch}")

            # 선택된 브랜치 처리
            if selected_branches:
                branches = []
                for pattern in selected_branches:
                    matching = [b for b in all_branches if pattern in b]
                    if not matching:
                        print(f"경고: '{pattern}'와 일치하는 브랜치를 찾을 수 없습니다.")
                    branches.extend(matching)
                if not branches:
                    print("오류: 선택된 브랜치가 없습니다.")
                    return None
                print(f"\n선택된 브랜치:")
                for branch in branches:
                    print(f"- {branch}")
            else:
                branches = all_branches

            # 데이터 변환을 위한 딕셔너리 초기화
            data_dict = {}
            branch_lengths = {}

            # 첫 번째 패스: 각 브랜치의 길이 확인
            for branch in branches:
                try:
                    branch_data = tree[branch].array()
                    length = get_array_length(branch_data)
                    if length > 0:
                        branch_lengths[branch] = length
                except Exception as e:
                    print(f"경고: 브랜치 '{branch}' 길이 확인 중 에러 발생: {str(e)}")
                    continue

            if not branch_lengths:
                print("경고: 유효한 데이터가 있는 브랜치를 찾을 수 없습니다.")
                return None

            # 가장 긴 브랜치 길이 찾기
            max_length = max(branch_lengths.values())
            print(f"\n브랜치 길이 정보:")
            print(f"- 최대 길이: {max_length}")
            print(f"- 전체 브랜치 수: {len(branches)}")
            print(f"- 유효한 브랜치 수: {len(branch_lengths)}")

            # 두 번째 패스: 데이터 변환
            for branch in branches:
                try:
                    if branch not in branch_lengths:
                        print(f"경고: 브랜치 '{branch}' 건너뜀 (데이터 없음)")
                        continue

                    branch_data = tree[branch].array()
                    clean_name = clean_branch_name(branch)

                    # 데이터 타입에 따른 처리
                    if isinstance(branch_data, ak.Array):
                        if branch_data.ndim > 1:
                            try:
                                # 다차원 배열의 경우 첫 번째 요소만 선택
                                flattened = ak.to_numpy(branch_data[:, 0])
                                data_dict[clean_name] = pad_or_trim_array(flattened, max_length)
                            except Exception:
                                print(f"경고: 브랜치 '{branch}' 다차원 데이터 처리 실패")
                                continue
                        else:
                            flattened = ak.to_numpy(branch_data)
                            data_dict[clean_name] = pad_or_trim_array(flattened, max_length)
                    else:
                        data_dict[clean_name] = pad_or_trim_array(branch_data, max_length)

                    # 길이 정보 출력
                    if branch_lengths[branch] != max_length:
                        print(f"정보: 브랜치 '{branch}' 길이 조정됨 ({branch_lengths[branch]} → {max_length})")

                except Exception as e:
                    print(f"경고: 브랜치 '{branch}' 처리 중 에러 발생: {str(e)}")
                    continue

            # DataFrame 생성
            if data_dict:
                try:
                    df = pd.DataFrame(data_dict)

                    # DataFrame이 비어있는지 확인
                    if df.empty:
                        print("경고: 변환된 DataFrame이 비어있습니다.")
                        return None

                    print(f"\n성공적으로 변환된 컬럼 수: {len(df.columns)}")
                    return df
                except Exception as e:
                    print(f"DataFrame 생성 중 에러 발생: {str(e)}")
                    return None
            else:
                print("경고: 변환할 수 있는 데이터가 없습니다.")
                return None

    except Exception as e:
        print(f"파일 읽기 중 에러 발생: {str(e)}")
        return None

def main():
    parser = argparse.ArgumentParser(description='ROOT 파일을 pandas DataFrame으로 변환')
    parser.add_argument('root_file', help='ROOT 파일 경로')
    parser.add_argument('--tree', default='tree', help='트리 이름 (기본값: tree)')
    parser.add_argument('--branches', nargs='+', help='변환할 브랜치 이름 또는 패턴 (여러 개 지정 가능)')
    parser.add_argument('--output', help='출력 파일 경로 (기본값: 입력 파일과 같은 이름에 .csv 확장자)')

    args = parser.parse_args()

    if not os.path.exists(args.root_file):
        print(f"파일을 찾을 수 없습니다: {args.root_file}")
        sys.exit(1)

    df = root_to_pandas(args.root_file, args.tree, args.branches)

    if df is not None:
        print("\nDataFrame 정보:")
        print(df.info())
        print("\n처음 5개 행:")
        print(df.head())

        # DataFrame이 비어있지 않은 경우에만 저장
        if not df.empty:
            output_file = args.output if args.output else os.path.splitext(args.root_file)[0] + ".csv"
            df.to_csv(output_file, index=False)
            print(f"\nDataFrame이 다음 파일로 저장되었습니다: {output_file}")
        else:
            print("\n경고: 데이터가 비어있어 CSV 파일을 저장하지 않았습니다.")

if __name__ == "__main__":
    main()