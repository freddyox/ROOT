#include "TH3D.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TPolyLine3D.h"
#include "TRandom.h"
#include "TFile.h"
#include "TVector3.h"
#include "TVector2.h"
#include "TCanvas.h"
#include <cmath>
// Feb 4, 2016 - Freddy Obrecht - UConn
// Quick code to simulate random walk in 2/3 Dimensions. Just for fun.

void RandomWalk(void){
  TFile *myfile = new TFile("random_walk3D_7.root","RECREATE");
  
  TRandom *random = new TRandom();
  random->SetSeed(7);

  Double_t dX = 1.1;
  Double_t dX2 = 0.01;
  Int_t N = 10000000;

  TVector3 pos  (0.0, 0.0, 0.0);
  TVector3 step (0.0, 0.0, 0.0);
  TVector3 pos2 (0.0, 0.0, 0.0);
  TVector3 step2(0.0, 0.0, 0.0);
  
  Double_t xbin = 60.0;
  Double_t ybin = 60.0;
  Double_t zbin = 60.0;
  Double_t xbin2 = 40.0;
  Double_t ybin2 = 40.0;

  TH3D *RW_hist = new TH3D( "RW_hist", "", 100, -xbin, xbin, 100, -ybin, ybin, 100, -zbin, zbin );
  TH2D *RW_hist_2D = new TH2D( "RW_hist_2D", "", 100, -xbin2, xbin2, 100, -ybin2, ybin2 );
 
  TPolyLine3D *l = new TPolyLine3D(N);

  RW_hist->Fill( pos.X(),pos.Y(),pos.Z() );

  for(Int_t i=0; i<N; i++ ) {
    Int_t direction = 1+random->Integer(6);
    switch(direction) {
    case 1 : step.SetX( dX );
      break;
    case 2 : step.SetX( -dX );
      break;
    case 3 : step.SetY( dX );
      break;
    case 4 : step.SetY( -dX );
      break;
    case 5 : step.SetZ( dX );
      break;
    case 6 : step.SetZ( -dX );
      break;
    default :
      break;
    }
    pos += step;
    l->SetPoint( i, pos.X(), pos.Y(), pos.Z() );
    // Check to see if it goes out of Bounds.
    if( pos.X() > xbin ) {
      pos.SetX( -pos.X() );
    }
    if( pos.X() < -xbin ) {
      pos.SetX( pos.X() );
    }
    if( pos.Y() > ybin ) {
      pos.SetY( -pos.Y() );
    }
    if( pos.Y() < ybin ) {
      pos.SetY( pos.Y() );
    } 
    if( pos.Z() > zbin ) {
      pos.SetZ( -pos.Z() );
    }
    if( pos.Z() < zbin ){
      pos.SetZ( pos.Z() );
    }
    RW_hist->Fill( pos.X(), pos.Y(), pos.Z() );
    
    // Two Dimensional Walk
    Int_t d2 = 1+random->Integer(4);
    switch(d2) {
    case 1 : step2.SetX( dX2);
      break;
    case 2 : step2.SetX( -dX2 );
      break;
    case 3 : step2.SetY( dX2 );
      break;
    case 4 : step2.SetY( -dX2 );
      break;
    default :
      break;
    }
    pos2 += step2;
    // Check to see if it goes out of Bounds.
    if( pos2.X() > xbin2 ) {
      pos2.SetX( -pos2.X() );
    }
    if( pos2.X() < -xbin2 ) {
      pos2.SetX( pos2.X() );
    }
    if( pos2.Y() > ybin2 ) {
      pos2.SetY( -pos2.Y() );
    }
    if( pos2.Y() < ybin2 ) {
      pos2.SetY( pos2.Y() );
    } 
    RW_hist_2D->Fill( pos2.X(), pos2.Y() );
  }
  l->Draw();

  RW_hist->SetMarkerStyle(6);
  RW_hist->SetMarkerColor(8);
  RW_hist->SetXTitle("X");
  RW_hist->SetYTitle("Y");
  RW_hist->SetZTitle("Z");

  RW_hist_2D->SetMarkerStyle(6);
  RW_hist_2D->SetMarkerColor(8);
  RW_hist_2D->SetXTitle("X");
  RW_hist_2D->SetYTitle("Y");
  myfile->Write();
}

