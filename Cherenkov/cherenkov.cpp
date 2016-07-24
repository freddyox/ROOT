#include <iostream>
#include <vector>
#include <stdio.h>

#include "TFile.h"
#include "TGraph.h"
#include "TGLabel.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TLegend.h"

// July 19, 2016 - written by R.F. Obrecht.
//////////////////////////////////////////////////////////////////
//                 Simple Cherenkov Analysis                    //
//////////////////////////////////////////////////////////////////

// Program calculates Cherenkov emission angles for protons,
// kaons, pions, and protons for a range of momentum p (GeV) given
// material index of refraction. Good for three materials, 4 
// particles but starts to get clunky after that.

// The idea is to calculate the threshold momentum for cherenkov
// emission, then fill the plot from there.

using namespace std;

void cherenkov(){
  TFile *myfile = new TFile("spectrum_cherenkov.root","RECREATE");

  TString names[] = {"Kaon","Pion","Proton","Electron"};
  TString nmaterials[] = {"Aerogel","C4F10", "CF4"};

  TString mat_root[] = {"Aerogel", "C_{4}F_{10}","CF_{4}"};
  TString particles[] = {"K","#pi","p","e^{-}"};

  double mass_kaon = 493.667 / 1000.0; // GeV
  double mass_pion = 139.57 / 1000.0;  // GeV
  double mass_proton = 938.57 / 1000.0;// GeV
  double mass_e = 0.511 / 1000.0;      // GeV

  double n_aero = 1.0304;              // index of ref. for aerogel 
  double n_C4F10 = 1.00137;              // index of ref. for C4F10
  double CF4 = 1.0005;              // index of ref for CF4

  ///////////////////////////////////////////////////////////////////
  //           Only need to add physical constants here
  double masses[] = {mass_kaon, mass_pion, mass_proton, mass_e};
  double index_of_ref[] = {n_aero, n_C4F10, CF4};                 
  ///////////////////////////////////////////////////////////////////
  
  int m_size = sizeof(masses)/sizeof(*masses);
  vector<double> mass;
  for(int i=0; i<m_size; i++){
    mass.push_back( masses[i] );
  }

  int n_size = sizeof(index_of_ref)/sizeof(*index_of_ref);
  vector<double> refractive_index;
  for(int i=0; i<n_size; i++){
    refractive_index.push_back( index_of_ref[i] );
  }

  vector<double> momentum;
  vector<double> theta,theta_deg;

  Int_t colors[4] = {kRed, kMagenta, kBlue, kGreen+2};
  vector<TGraph*> graphs;

  for( unsigned int n=0; n<refractive_index.size(); n++ ){
    for(unsigned int m=0; m<mass.size(); m++){
      momentum.clear();
      theta.clear();
    
      // Calculate the threshold momentum:
      double mom_threshold = sqrt( mass[m]*mass[m] / (pow(refractive_index[n],2) - 1) );
      cout << "Threshold for " << nmaterials[n] << ": " << names[m] << " = " << mom_threshold << endl;

      bool first_try = true;

      for(int i=0; i<1000; i++) { // p = momentum in GeV, index represents momentum
	double p = double(i)/40.0;
   
	if( p>mom_threshold && first_try) {
	  p = mom_threshold+0.001;
	  first_try = false;
	}

	if( p>mom_threshold ){
	
	  double mass_over_p = mass[m]/p;
	  double beta = pow( (1 + pow(mass_over_p,2)),-0.5 );
	  double ratio = 1/(refractive_index[n]*beta);
	  double th = acos( ratio );
      
	  momentum.push_back(p);
	  theta.push_back( th );
	}
      }
  
      // Make the particle TGraphs:
      TGraph *gtemp = new TGraph( momentum.size(),
				  &(momentum[0]),
				  &(theta[0]) );
      TString gname = "Cherenkov Spectrum";
      gtemp->SetTitle(gname);
      gtemp->SetName(gname);

      gtemp->GetXaxis()->SetTitle("Momentum (GeV)");
      gtemp->GetYaxis()->SetTitle("#theta_{c} (rad)");
      gtemp->GetYaxis()->SetRangeUser(0.0, 0.26);

      gtemp->SetLineColor(colors[m]);
      gtemp->SetMarkerColor(colors[m]);

      // Make gas different
      switch(n){
      case 1:  gtemp->SetLineStyle(7);
	break;
      case 2:  gtemp->SetLineStyle(10);
	gtemp->SetLineWidth(2);
	break;
      default: gtemp->SetLineStyle(1);
	break;
      }

      gtemp->Write();
      graphs.push_back( gtemp );
    }
  }

  // Combine all TGraphs:
  TCanvas *c1 = new TCanvas("c1","Cherenkov_Spectrum",600,500);

  // Draw the TGraphs:
  graphs[graphs.size()-1]->Draw();
  vector<TGraph*>::reverse_iterator rit;
  for(rit=graphs.rbegin()+1; rit!=graphs.rend(); rit++){
    (*rit)->Draw("same");
  }

  // Make the TLegend:
  TString header;
  int nnames = sizeof(mat_root)/sizeof(*mat_root);
  for(int i=0; i<nnames; i++){
    header += mat_root[i];
    if(i<nnames-1) header += "     ";
  }
  TLegend *leg = new TLegend(0.65,0.5,0.9,0.75);
  leg->SetNColumns(refractive_index.size());
  leg->SetHeader(header);
  
  // Add in the TLegend Entries:
  vector<TGraph*>::iterator it;
  for(unsigned int i=0; i<mass.size(); i++){
    for(int j=0; j<nnames; j++){
      int index = i + j*mass.size();
      leg->AddEntry(graphs[index],particles[i],"lp");
    }
  }

  leg->Draw("same");

  // Write the ROOT file:
  myfile->Write();
}


