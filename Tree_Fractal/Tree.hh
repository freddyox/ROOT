#ifndef TREE_HH
#define TREE_HH
#include "TCanvas.h"
#include "TFile.h"
#include "TLine.h"
#include "TPolyLine.h"
#include "TMath.h"
#include "TVector2.h"

#include <vector>

class Tree{
private:
  double deg2rad;
  double fDisx, fDisy;
  TCanvas *fCanvas;
  int fIter;
 
  double fLength;
  TVector2 fOrigin;
  double fOffsetY;

  int fTheta;
  TVector2 yhat;
  TVector2 xhat;

  TFile *fFile;

  std::vector<TLine*>::iterator fVit;
  std::vector<TLine*> fBranches;
  std::vector<TLine*> fStorage, fTemp;

public:
  Tree();
  ~Tree(){;}

  void Draw();
  void GenerateTree();
};
#endif
