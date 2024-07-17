#include "curry.hxx"

ClassImp(CURRY)


CURRY* CURRY::curry = NULL;

CURRY* CURRY::GetInstance() {

  if (!curry)
    curry = new CURRY();

  return curry;}


Bool_t CURRY::Run(TPad *pad, Double_t *x, Double_t *y, Bool_t *end){
  if (!pad) return false;
  pad->cd();

  pad->AddExec("ex_xval","CURRY::GetInstance()->GetEvent()");
  if (auto obj = gPad->WaitPrimitive("xval")) {
    gPad->GetListOfPrimitives()->Remove(obj);
    delete obj; }
  else {
    fX = fY = 0;
    return false;}

  if (!x && !y) {
    std::cout << "[xval] X: " << fX << ", Y: " << fY << "\n"; }
  else {
    if (x) *x = fX;
    if (y) *y = fY;
    if (end) *end = fEnd;}
  return true;
}

Bool_t CURRY::Run(Double_t *x, Double_t *y, Bool_t *end){
  return Run(static_cast<TPad*>(gPad), x, y, end); }

void CURRY::GetEvent() {

  const int event = gPad->GetEvent();
  if (event != kButton1Up && event != kButton2Up) return;

  gPad->DeleteExec("ex_xval");
  const Int_t px = gPad->GetEventX();
  const Double_t xx = gPad->AbsPixeltoX(px);
  const Double_t x = gPad->PadtoX(xx);
  const Int_t py = gPad->GetEventY();
  const Double_t yy = gPad->AbsPixeltoY(py);
  const Double_t y = gPad->PadtoY(yy);  

  fX = x; fY = y;
  if (event == kButton2Up) fEnd = true;
  else fEnd = false;
  gPad->GetListOfPrimitives()->AddLast(new TNamed("xval","xval"));
}
