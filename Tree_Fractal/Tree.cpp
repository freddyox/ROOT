#include "Tree.hh"
#include "TApplication.h"
#include <cmath>
#include <iostream>

Tree::Tree(){
  fDisx = 1300;
  fDisy = 1000;
  xhat = TVector2(1.0,0.0);
  yhat = TVector2(0.0,1.0);
  fCanvas = new TCanvas("c1","Tree Fractal",0,1000,fDisx,fDisy);
  fCanvas->SetFillColor(1);
  deg2rad = TMath::Pi()/180.0;
  fIter = 15;

  fLength = 0.3;
  fOrigin = TVector2(0.5,0.5);
  fOffsetY = fLength;
  GenerateTree();
}

void Tree::GenerateTree(){
  TLine *trunk = new TLine( fOrigin.X(), fOrigin.Y() - fLength/2.0 - fOffsetY,
			    fOrigin.X(), fOrigin.Y()+fLength/2.0 - fOffsetY );
  trunk->SetLineColor(kOrange+2);

  for( fTheta = 0; fTheta < 361; fTheta++ ) {
    int dummy = 0; 
    fBranches.clear();
    fBranches.push_back( trunk );
    fStorage.clear();
    fTemp.clear();
    fStorage = fBranches;

    while( dummy < fIter ) {
    
      if( dummy > 0 ){
	fStorage.clear();
	fStorage = fTemp;
	fTemp.clear();
      }

      for( fVit=fStorage.begin(); fVit!=fStorage.end(); fVit++ ){
	// Grab Coordinates:
	TVector2 bot( (*fVit)->GetX1(), (*fVit)->GetY1() );
	TVector2 top( (*fVit)->GetX2(), (*fVit)->GetY2() );
      
	double length = (top-bot).Mod();

	// Lets generate two more with these dimensions:
	double next_length = length*0.65;

	// Build a "normal"
	TVector2 nhat = (top-bot).Unit();
	double ang_wrt_x = (TMath::ACos(nhat*xhat))/deg2rad;

	// Next starting point
	TVector2 nstart = top;

	for( int i=1; i<3; i++ ){
	  double ang  =  ang_wrt_x + pow(-1,i)*fTheta;
	  if( nhat*yhat < 0 ) { ang = -ang_wrt_x + pow(-1,i)*fTheta;}

	  TVector2 next_nhat( cos(ang*deg2rad), sin(ang*deg2rad) );
	  TVector2 end = nstart + next_length*next_nhat;
	  TLine *branch = new TLine( nstart.X(), nstart.Y(), end.X(), end.Y() );
	  if( dummy >= 0 && dummy <=1 ) {
	    branch->SetLineColor(kGreen+3);
	  }
	  if( dummy >= 2 && dummy <=3 ) {
	    branch->SetLineColor(kGreen+1);
	  }
	  if( dummy >= 4 && dummy < fIter-2 ) {
	    branch->SetLineColor(kGreen);
	  }
	  if( dummy == fIter-2){
	    branch->SetLineColor(kRed);
	  }
	  if( dummy == fIter-1 ) {
	    branch->SetLineColor(kPink+10);
	  }
	  fBranches.push_back( branch );
	  fTemp.push_back( branch );
	}	    
      }
      dummy++;
    }
    fCanvas->Clear();
    Draw();
    TString temp;
    temp.Form("%d",fTheta);
    TString title = "Screenshots1/FractalTree_" + temp + ".png";
    fCanvas->SaveAs(title);
  }
}

void Tree::Draw(){
  if( !fBranches.empty() ){
    for( fVit=fBranches.begin(); fVit!=fBranches.end(); fVit++){
      (*fVit)->Draw("gl");
    }
  }
 }

int main(int argc, char **argv ) {
  TApplication theApp("App",&argc, argv);
  new Tree();
  theApp.Run();
  return 0; 
}
