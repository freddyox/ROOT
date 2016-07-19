#include <iostream>
#include "TMath.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TString.h"
#include "TF1.h"
#include "TFile.h"
#include <cmath>

// Silly Binomial Distribution Program - 5/5/2016

Double_t poissonf(Double_t*x,Double_t*par)                                         
{                                                                              
  return par[0]*TMath::Poisson(x[0],par[1]);
}  

void Bin(){
  TFile *myfile = new TFile("Prob_Distributions.root","RECREATE");

  int ntries = 200;
  double p = 0.5;
  double q = 1-p;
  double x[ntries+1],y[ntries+1];
  int dummy = 0;
  // Mean and sigma of a Binomial:
  double bin_mean = ntries*p;
  double bin_sigma = sqrt(ntries*p*q);

  for(int i=0; i<=ntries; i++){
    double b_coeff = TMath::Binomial(ntries,i);
    double prob = pow(p,i)*pow(q,ntries-i);

    if(b_coeff < 10e100){
      dummy++;
      x[i] = i;
      y[i] = b_coeff*prob;
    }
  }

   TGraph *gr = new TGraph(dummy,x,y);
   gr->Draw("AB");
   gr->Write();

   TString p_str, q_str, ntries_str;
   p_str.Form("%f",p);
   q_str.Form("%f",q);
   ntries_str.Form("%d",ntries);

   while(p_str.EndsWith("0")){
     p_str.Remove(TString::kTrailing,'0');
   }
   while(q_str.EndsWith("0")){
     q_str.Remove(TString::kTrailing,'0');
   }

   TString gtitle = "Binomial, p = " + p_str + ", q = " 
     + q_str + ", N = " + ntries_str;
   gr->SetTitle(gtitle);

   if(ntries*p>5) {
     TF1 *gfit = new TF1("gfit","gaus", bin_mean-3*bin_sigma, bin_mean+3*bin_sigma);
     gr->Fit(gfit); 

     TH1D *hdiff = new TH1D("hdiff", "", 100, 0.0, 1.0);
     for(int i=0; i<=ntries; i++){
       if(y[i] > 0.0001) {
	 double diff = fabs(y[i]-gfit->Eval(i));
	 hdiff->Fill( 100.0*(diff/y[i]) );
       }
     }
     std::cout << "Binomial mean = " << bin_mean << std::endl;
     std::cout << "Expo mean     = " << gfit->GetParameter(1) << std::endl;
     std::cout << "Binomial sig = " << bin_sigma << std::endl;
     std::cout << "Expo sig     = " << gfit->GetParameter(2) << std::endl;
     hdiff->SetTitle("prob>0.0001 cut");
     hdiff->SetXTitle("(Binomial-gaus)/Binomial (%)");
     gfit->Delete();
   } else {
     TF1 *pfit = new TF1("pfit", poissonf,0.0, bin_mean+3*bin_sigma,2);
     pfit->SetParameter(0,1);
     pfit->SetParameter(1,bin_mean);
     gr->Fit(pfit);

     std::cout << "Np is small compared to N --- " << ntries*p << " vs. " << ntries << std::endl;
     std::cout << "This is better approximated by a Poisson rather than a Gaussian!" << std::endl;
   }


   // Clean up
   myfile->Write();
}
