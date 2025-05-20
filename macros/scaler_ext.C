#ifndef __CLING__
#include "TCanvas.h"
#include "TH2.h"
#include "kobra.hxx"
#endif

void scaler_ext() {
    bool flagCut = true;

    const int n = 8;
    const char* z[] = {"o", "o", "o", "o", "o", "ne", "ne", "ne"};

    const char* cut[] = {"o18 || f20", "o19 || f21",   "o19 || f21",   "f22 || o20",
                         "n18 || o20", "mg28 || na26", "mg29 || na27", "mg29 || na27"};

    int zz[] = {8, 8, 8, 8, 8, 10, 10, 10};
    int a[] = {18, 19, 20, 21, 22, 24, 25, 26};
    double brho[] = {1.2959, 1.3198, 1.3734, 1.3879, 1.4600, 1.2646443, 1.3201144, 1.3741834};

    std::vector<int> expt[] = {KOBRA::o18, KOBRA::o19,  KOBRA::o20,  KOBRA::o21,
                               KOBRA::o22, KOBRA::ne24, KOBRA::ne25, KOBRA::ne26};

    KOBRA* ko;
    for (int i = 0; i < n; i++) {
        ko = new KOBRA(KOBRA::Expt::Phys, expt[i]);
        ko->ExtractScalerInfo(true, Form("misc/scaler_ext_%d%s.csv", a[i], z[i]));
        delete ko;
    }
}