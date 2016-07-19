#include "Snowflake.hh"

#include "TF1.h"
#include "TMath.h"
#include "TGraph.h"
#include "TText.h"
#include <cmath>
#include <iostream>
#include "TApplication.h"

Snowflake::Snowflake(){
  canvas = new TCanvas("c1","Koch Snowflake",0,1000,1000,1000);
  canvas->SetFillColor(1);
  theta = 60.0;
  deg2rad = TMath::Pi()/180.0;
  iters = 10;
  x_hat = TVector2(1,0);
  y_hat = TVector2(0,1);

  mag = 0.7;
  centerx = (1.0-mag)/2.0;
  centery = (1-mag*sqrt(3)/2)/2+0.1;
  TVector2 initial(centerx,centery);

  TFile *myfile = new TFile("Asymmetric.root","RECREATE");

  fTotalArea = 0.0;
  InitialTriangle(mag,initial);
  GenerateFlake();
  Draw();
  Plots();
  myfile->Write();
}

void Snowflake::InitialTriangle(double mag, TVector2 initial){
  for(int i=0; i<3; i++){
    TVector2 hat(cos( 2*i*theta*deg2rad ), sin( 2*i*theta*deg2rad) );
    TVector2 final = initial + mag*hat;

    TLine *temp = new TLine(initial.X(),initial.Y(),final.X(),final.Y());
    temp->SetLineColor(kMagenta-3);
    initial = final;    

    // Insert initial lines into our vector for analysis
    lines.push_back(temp);
    snowflake.push_back(temp);
  }
  double temp = mag;
  Area(temp,0.0,1);
}

void Snowflake::GenerateFlake() {
  int dummy = 0;
  bool noiteration = true;
 
  while(dummy < iters) {
    noiteration = false;
    int counter = 0;
    int howmany = 0;
    if(dummy > 0) {
      howmany = lines.size();
      lines.clear();
      lines = snowflake;
    }

    snowflake.clear();
    double length;

    for(vit = lines.begin(); vit != lines.end(); vit++){
      TLine* temp = *vit;
      TVector2 start(temp->GetX1(), temp->GetY1());
      TVector2 end(temp->GetX2(), temp->GetY2());
      length = (end - start).Mod();
      TVector2 nhat = (end - start).Unit();

      TVector2 tri_i = start + 2.0*length/3.0*nhat;

      double ang = (TMath::ACos(nhat*x_hat))/deg2rad;

      CutUp(temp);

      for(int i=1; i<3; i++){
	counter++;

    	double temp_ang = (ang - 2.0*i*theta)*deg2rad;
    	if(nhat*y_hat<0){ temp_ang = (2.0*ang - 2.0*i*theta)*deg2rad; }
       
    	TVector2 hat( cos( temp_ang ), sin( temp_ang ) );
    	TVector2 tri_e = tri_i + length/3.0 * hat;
    
    	TLine *next = new TLine( tri_i.X(), tri_i.Y(), tri_e.X(), tri_e.Y() );
    	next->SetLineColor(kMagenta-3);
    	snowflake.push_back(next);

    	tri_i = tri_e;
      } 
    }   
    Area(length,double(dummy),howmany);
    dummy++;
    Analyze(dummy);
  }

  if( noiteration ) {
    std::cout << "\n";
    std::cout << "***********************************************************" << std::endl;
    std::cout << "No iterations have been performed.\n";
    std::cout << "Initial triangle perimeter = " << 3.0*mag << std::endl;
  }
}

void Snowflake::CutUp(TLine* temp){
  TVector2 start(temp->GetX1(), temp->GetY1());
  TVector2 end(temp->GetX2(), temp->GetY2());
  double length = (end-start).Mod();
  TVector2 nhat = (end-start).Unit();

  TVector2 f1 = start + length/3.0*nhat;
  TVector2 f2 = start + 2.0*length/3.0*nhat;

  TLine *first  = new TLine(start.X(),start.Y(),f1.X(),f1.Y());
  TLine *second = new TLine(f2.X(),f2.Y(),end.X(),end.Y()); 
  first->SetLineColor(kMagenta-3);
  second->SetLineColor(kMagenta-3);
  snowflake.push_back(first);
  snowflake.push_back(second);
}

void Snowflake::Area(double a, double ind, int n){
  std::cout << a << "  " << ind << "   " << n << std::endl;
  double area = a*a*sqrt(3)/4.0;
  fTotalArea += area*n;
  fArea.push_back( fTotalArea );
  fIndexArea.push_back(ind);
}

void Snowflake::Analyze(int iteration){
  double total_length = 0.0;
  for(vit=snowflake.begin(); vit!=snowflake.end(); vit++){
    TLine* temp = *vit;
    TVector2 start(temp->GetX1(), temp->GetY1());
    TVector2 end(temp->GetX2(), temp->GetY2());
    double length = (end-start).Mod();
    total_length+=length;
  }
  std::cout << "\n";
  std::cout << "*************************************************************" << std::endl;

  std::cout << "Iteration: " << iteration << std::endl;
  std::cout << "Initial triangle perimeter = " << 3.0*mag << std::endl;
  std::cout << "Total Length of perimeter = " << total_length << std::endl;
  fIndex.push_back(double(iteration));
  fPerimeter.push_back(total_length);
}

void Snowflake::Plots(){
  TGraph *gtemp = new TGraph(fIndex.size(),&(fIndex[0]),&(fPerimeter[0]));
  gtemp->SetName("Perimeter");
  gtemp->SetTitle("Fractal Perimeter vs. Iteration #");
  gtemp->SetMarkerStyle(21);
  gtemp->SetMarkerColor(kGreen+3);
  gtemp->Write();

  std::vector<double>::iterator vit1 = fArea.begin();
  fArea.erase(vit1);
  vit1 = fIndexArea.begin();
  fIndexArea.erase(vit1);

  TGraph *gtemp1 = new TGraph(fIndexArea.size(),&(fIndexArea[0]),&(fArea[0]));
  gtemp1->SetName("Area");
  gtemp1->SetTitle("Fractal Area vs. Iteration #");
  gtemp1->SetMarkerStyle(21);
  gtemp1->SetMarkerColor(kGreen+3);

  fNSides = 3.0 * pow(4,iters);
  fSmallSide = mag / pow(3,iters);
  double area = sqrt(3)/4.0 * mag * mag;
  double largen = 8.0*area/5.0;
  std::cout <<"\n";

  std::cout << "After " << iters << " iterations, the fractal now has " << fNSides 
	    << " sides." << std::endl;
  std::cout << "Started with a size of " << mag << ", now the fractal has sides equal to " 
	    << fSmallSide << "." << std::endl;
  std::cout << "Area_intial = " << area << std::endl;
  std::cout << "Area_final = " << fTotalArea << std::endl;
  std::cout << "Area as n->inf = " <<  largen << std::endl;

  gtemp1->Write();

}

void Snowflake::Save(int index){
  canvas->Clear();
  TString ind_str;
  ind_str.Form("%d",index);
  TString title = "AsymmetricFlake" + ind_str + ".png";
  canvas->SaveAs(title);
}

void Snowflake::Draw(){
  for( vit=snowflake.begin(); vit != snowflake.end(); vit++ ) {
    (*vit)->Draw("gl");
  }
}

int main(int argc, char **argv ) {
  TApplication theApp("App",&argc, argv);
  new Snowflake();
  theApp.Run();
  return 0; 
}
