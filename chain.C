{
    // TChain 객체 생성
    TChain* chain = new TChain("kobra");

    const char* directory = ".";
    // 디렉토리에서 파일 목록 가져오기
    TSystemDirectory dir(directory, directory);
    TList* files = dir.GetListOfFiles();
    
    if (files) {
        TSystemFile* file;
        TString fname;
        TIter next(files);

        // 파일 목록 순회
        while ((file = (TSystemFile*)next())) {
            fname = file->GetName();
            // ROOT 파일만 선택하고 "kobra" TTree를 가진 파일을 TChain에 추가
            if (!file->IsDirectory() && fname.EndsWith(".root")) {
                TString filePath = TString::Format("%s/%s", directory, fname.Data());
                TFile* f = TFile::Open(filePath);
                if (f && f->Get("kobra")) {
                    chain->Add(filePath);
                    std::cout << "Added file: " << filePath.Data() << std::endl;
                }
                delete f;
            }
        }
    }

    // 체인에 추가된 파일의 수 출력
    std::cout << "Total files in chain: " << chain->GetNtrees() << std::endl;

    // 여기에 TChain을 이용해 원하는 작업을 추가하세요.
    // 예: chain->Draw("some_branch");
}