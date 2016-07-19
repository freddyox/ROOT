#include <iostream>
#include <vector>

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
// kaons, pions, and protons for a range of momentum p (GeV).
// The idea is to calculate the threshold momentum for cherenkov
// emission, then fill the plot from there.

using namespace std;

void cherenkov(){
  TFile *myfile = new TFile("spectrum_cherenkov.root","RECREATE");

  TString names[4] = {"Kaon","Pion","Proton","Electron"};

  double mass_kaon = 493.667 / 1000.0; // GeV
  double mass_pion = 139.57 / 1000.0;  // GeV
  double mass_proton = 938.57 / 1000.0;// GeV
  double mass_e = 0.511 / 1000.0;      // GeV
  double n_aero = 1.0304;              // index of ref. for aerogel 
  double n_gas = 1.00137;              // index of ref. for C4F10

  double masses[4] = {mass_kaon,mass_pion,mass_proton,mass_e};
  double index_of_ref[2] = {n_aero,n_gas};

  vector<double> momentum;
  vector<double> theta;

  Int_t colors[4] = {kRed, kMagenta, kBlue, kGreen};
  vector<TGraph*> graphs;

  for(unsigned int m=0; m<4; m++){
    momentum.clear();
    theta.clear();
    
    // Calculate the thresholds momentum:
    double mom_threshold = sqrt( masses[m]*masses[m] / (n_aero*n_aero - 1) );
    cout << "Threshold " << names[m] << " = " << mom_threshold << endl;

    bool first_try = true;

    for(int i=0; i<1000; i++) { // p = momentum in GeV, index represents momentum
      double p = double(i)/40;
   
      if( p>mom_threshold && first_try) {
	p = mom_threshold+0.001;
	first_try = false;
      }

      if( p>mom_threshold ){
	
	double mass_over_p = masses[m]/p;
	double beta = pow( (1 + pow(mass_over_p,2)),-0.5 );
	double ratio = 1/(index_of_ref[0]*beta);
	double th = acos( ratio );
      
	momentum.push_back(p);
	theta.push_back( th );
      }
    }
  
    TGraph *gtemp = new TGraph( momentum.size(),
				&(momentum[0]),
				&(theta[0]) );
    TString gname = "Cherenkov Spectrum";
    gtemp->SetTitle(gname);
    gtemp->SetName(gname);
    // Need TAxis.h:
    gtemp->GetXaxis()->SetTitle("Momentum (GeV)");
    gtemp->GetYaxis()->SetTitle("#theta_{c} (rad)");
    gtemp->GetYaxis()->SetRangeUser(0.0, 0.26);

    gtemp->SetLineColor(colors[m]);
    gtemp->SetMarkerColor(colors[m]);

    gtemp->Write();
    graphs.push_back( gtemp );
  }

  TCanvas *c1 = new TCanvas("c1","Cherenkov_Spectrum",600,500);
  graphs[3]->Draw();
  graphs[2]->Draw("same");
  graphs[1]->Draw("same");
  graphs[0]->Draw("same");
  TLegend *leg = new TLegend(0.5,0.5,0.8,0.7);
  leg->AddEntry(graphs[3],"e^{_}","lp");
  leg->AddEntry(graphs[2],"p","lp");
  leg->AddEntry(graphs[1],"#pi","lp");
  leg->AddEntry(graphs[0],"K","lp");
  leg->Draw("same");

  myfile->Write();
}


