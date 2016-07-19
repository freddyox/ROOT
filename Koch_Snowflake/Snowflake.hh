#ifndef SNOWFLAKE_HH
#define SNOWFLAKE_HH
#include "TCanvas.h"
#include "TLine.h"
#include "TVector2.h"
#include "TFile.h"

#include <vector>

class Snowflake{
private:
  // Initialize:
  double centerx, centery;
  double mag, theta, deg2rad;
  TCanvas *canvas;
  int iters;

  TVector2 x_hat,y_hat;
  std::vector<TLine*> lines,snowflake;
  std::vector<TLine*>::iterator vit;

  TFile *myfile;
  std::vector<double> fIndex;
  std::vector<double> fPerimeter;
  std::vector<double> fArea;
  std::vector<double> fIndexArea;
  double fTotalArea;
  long fNSides;
  double fSmallSide;
public:
  Snowflake();
  ~Snowflake(){;}
  void Draw();
  void Save(int);
  void InitialTriangle(double,TVector2);
  void GenerateFlake();
  void CutUp(TLine*);

  void Analyze(int);
  void Plots();

  void Area(double,double,int);
  
};
#endif
