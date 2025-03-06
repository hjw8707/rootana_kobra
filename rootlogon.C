{
#ifdef _WIN32_
    gSystem->Load("libanadict.dll");
#elif __APPLE__
    gSystem->Load("libanadict.dylib");
#else
    gSystem->Load("libanadict.so");
#endif
}
