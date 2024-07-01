#ifndef TCanvasManager_h
#define TCanvasManager_h

#include "TCanvas.h"

#include "TClonesArray.h"
#include "TH1.h"
#include <string>

class TCanvasManager {

public:
static TCanvasManager* GetInstance();

void Clear();

void AddCanvas(const char* name, const char* title, int xdiv = 0, int ydiv = 0);
TCanvas* GetCanvas(const char* name);

void AddHistToCanvas(const char* name, std::vector<TH1*> list);
void AddHistToCanvas(const char* name, std::vector<std::string> list);

void AddCanvasAndHistFromFile(const char* file);

private:
TCanvasManager();
~TCanvasManager();

static TCanvasManager* instance;

TClonesArray* canvases;

};
#endif