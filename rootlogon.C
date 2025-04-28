{
#ifdef _WIN32_
    gSystem->Load("libkobra.dll");
#elif __APPLE__
    gSystem->Load("libkobra.dylib");
#else
    gSystem->Load("libkobra.so");
#endif
}
