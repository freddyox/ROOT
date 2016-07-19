#include "TString.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TObject.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TEllipse.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TRandom.h"
#include "TColor.h"
#include "TStyle.h"
#include "TBox.h"
#include "TLatex.h"

#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

//////////////////////////////////////////////////
// Global containers - poor coding habit,
// but this is a small program and I'm lazy today
//////////////////////////////////////////////////

// Containers for Cursor movements and controlling colors
vector<TEllipse*> PMTs_clicked;
map<int,TEllipse*> PMTs; // key is PMT unique ID, value is PMT
vector<int> PMT_color, PMT_colorhv;
vector<double> PMT_alpha, PMT_alphahv;
map<int,vector<int> > HV_click; // key is click, value is list of pmts
int click = 0;
// key is row number in excel sheet
map<int,int> column_map;
map<int,int> row_map;
map<int,int> serial_map;
map<int,int> HV_map;

// key is HV #
map<int,Int_t> HV_Colors;
map<int,double> HV_Alphas;
map<TString,int> alpha_map;
map<int, TString> alpha_map_back;
map<int,vector<int> > HV_unique;


///////////////////////////////////////////////////

void rich_pmt() {
// Colors
 vector<Int_t> colors;
 colors.push_back(0);
 //colors.push_back(920);
 colors.push_back(632);
 colors.push_back(416);
 colors.push_back(600);
 colors.push_back(400);
 colors.push_back(616);
 colors.push_back(432);
 colors.push_back(800);
 colors.push_back(820);
 colors.push_back(840);
 colors.push_back(860);
 colors.push_back(880);
 colors.push_back(990);

 TString Line;
 ifstream master("master_sheet.csv");
 TFile *myfile = new TFile("RICH_layout.root","RECREATE");
 
 // Need a mapping between HV numbering scheme and letters. In the spreadsheet,
 // I gave bundles of 16 a letter instead of lump summing them all into 17-32
 TString alphabet[26] = { "A","B","C","D","E","F","G","H","I","J","K","L",
			  "M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z" }; 

 for( int i=0; i<26; i++ ){
   int temp = 65 + i;
   alpha_map[ alphabet[i] ] = temp;
   alpha_map_back[ temp ] = alphabet[i];
 }

 alpha_map_back[0] = "17-31";

 // Plots
 TH1D *hHVnum = new TH1D("hHVnum"," ", 82, -0.5, 81.5 );
 hHVnum->SetXTitle("HV Group #");
 hHVnum->SetTitle("0=TBD or 17-32, anything > 64 belongs to HV Module #2");
 
 // Checks to see if there are duplicate SN numbers:
 set<int> SN_numbers;
 set<int> SN_duplicates;
 int count = 0;

 set<int> HV_set;

 // Analyze the Spreadsheet
 if( master.is_open() ) {
   while( Line.ReadLine(master) ) {
     TObjArray *temp = Line.Tokenize(",");
     int SN  = (( (TObjString*) (*temp)[0])->GetString()).Atoi();
     TString HV_str = ( (TObjString*) (*temp)[1])->GetString();
     int Row = (( (TObjString*) (*temp)[2])->GetString()).Atoi();
     int Col = (( (TObjString*) (*temp)[3])->GetString()).Atoi();
     
     // Key is excel row number
     column_map[count] = Col;
     row_map[count] = Row;
     serial_map[count] = SN;

     // Fill HV Containers:
     if( HV_str != "TBD" && HV_str != "17-32" ) {
       int HV = 0;
       if( alpha_map.find( HV_str ) == alpha_map.end() ) {
	 HV = HV_str.Atoi();
       } else {
	 HV = alpha_map[ HV_str ];
       }
       HV_map[count] = HV;
       HV_set.insert( HV );
       hHVnum->Fill( HV );

     } else {
       HV_map[count] = 0;
       HV_set.insert( 0 );
       hHVnum->Fill( 0 );
     }

     count++;

     // This is a check to see if I made any SN input errors
     if( SN_numbers.find(SN) == SN_numbers.end() ) {
       SN_numbers.insert( SN );
     } else {
       SN_duplicates.insert( SN ); 
       cout << "Warning: Duplicate SN " << SN << endl;
     }
   }
 }
 else { 
   cerr << "ERROR: Could not open master!" << endl; 
 }
 
 // Lets assign colors to every PMT HV Group:
 TRandom r;
 Int_t col, col_wheel;
 double alph;
 set<int>::iterator sit;
 int p_or_m;
 for( sit=HV_set.begin(); sit!=HV_set.end(); sit++ ){
   col = (r.Rndm() * (colors.size()-2)) + 1;
   alph = r.Rndm(); 

   p_or_m = pow(-1,r.Rndm()*2+1); // either 1 or -1

   if( p_or_m == -1 ) {
     col_wheel = r.Rndm() * 11;
   } else {
     col_wheel = r.Rndm() * 5;
   }

   // If HV group is unassigned, just give it white:
   if( *sit == 0 ) { 
     col = 0; 
     alph = 0.0;
     col_wheel = 0;
   }

   int temp = colors[col] + col_wheel;

   HV_Alphas[*sit] = alph;
   HV_Colors[*sit] = temp;
 }

 // RICH PMT ARRAY:
 // box dimensions are 147.4 by 62.8 cm
 // diameter of a pmt = 18.6 mm
 // distance between two adjacent pmt centers is 23.3 mm
 int count_pmts = 0;

 Double_t w = 1000;
 Double_t h = 1000;
 TCanvas* c = new TCanvas("c", "PMT_Array", w, h);
 c->SetWindowSize(w + (w - c->GetWw()), h + (h - c->GetWh()));

 Double_t radius = 0.0126/2.0;
 Double_t off = 0.0158/2.0;
 TVector3 origin(0.025, 0.1, 0.0);
 TVector3 offset( 0.0, -radius, 0.0 );

 for(unsigned int i=0; i<column_map.size(); i++){
   int col = column_map[i];
   int row = row_map[i];
   int hv = HV_map[i];

   HV_unique[hv].push_back(i);

   TVector3 pmt_pos(0.0,0.0,0.0);
   TVector3 position(2.0*radius*col, 2.0*radius*row, 0.0 );

   // If we are dealing with an even Column, an offset is needed:
   if( col % 2 == 0 ) {
     pmt_pos = position + offset;
   } else {
     pmt_pos = position;
   }
   pmt_pos += origin;

   TEllipse *PMT = new TEllipse(pmt_pos.X(),pmt_pos.Y(),radius,radius );
   PMT->SetUniqueID(count_pmts);
   PMT->SetFillColorAlpha(HV_Colors[hv], HV_Alphas[hv]);
   PMTs[count_pmts] = PMT;
   PMT->Draw(); 
   count_pmts++;
 }
 
 myfile->Write();

 cout << "# of rows in master = " << count << endl;
 cout << "# of pmts placed    = " << count_pmts << endl;

 // Make a color legend:
 map<int,int>::iterator mit;
 double length = 0.014;
 double blx = 0.18;
 double bly = 0.90;
 double vert_off = length*1.8;
 int dummy = 0;
 int column = 2;
 int index = 0;

 for(mit=HV_Colors.begin(); mit!=HV_Colors.end(); mit++){
   if( index%16==0 && index>0 ) { 
     blx = 0.18*column; 
     bly = 0.90;
     dummy = 0;
     column++;
   }

   double x1 = blx;
   double y1 = bly - dummy*vert_off;

   double x2 = x1 + length;
   double y2 = y1 + length;
   TBox *legbox = new TBox(x1,y1,x2,y2);

   if(mit->first==0){
     legbox->SetLineColor(kBlack);
   } else {
     legbox->SetFillColorAlpha(mit->second,HV_Alphas[mit->second]);
   }
   legbox->SetUniqueID(mit->first);
   legbox->Draw();

   // Make the text
   TLatex l;
   l.SetTextSize(0.014);
   double x = x1 + length*2.0;
   double y = y1;
   if(mit->first>0 && mit->first <65 ){
     l.DrawLatex(x,y,Form("%d",mit->first));
   } else {
     l.DrawLatex(x,y, alpha_map_back[ mit->first ]); 
   }
   dummy++;
   index++;
 }

 // Control mouse clicks:
 c->AddExec("ex","PMT_Number()");
}

void PMT_Number() {

  int event = gPad->GetEvent();
  TObject *select = gPad->GetSelected();

  int px = gPad->GetEventX();
  int py = gPad->GetEventY();
  
  // Make sure cursor is on the screen, otherwise seg break
  if( px > 0 && py > 0 && px < 1000 && py < 1000 ) {

    if( event != 11 ) return;
    if( !select ) return;
    if( select->InheritsFrom(TEllipse::Class()) ){
      TEllipse *pmtclick = (TEllipse*)select;

      int hv = HV_map[pmtclick->GetUniqueID()];
      int color = HV_Colors[ hv ];
      double alpha = HV_Alphas[ hv ];

      pmtclick->SetFillColor(3);
      PMTs_clicked.push_back(pmtclick);

      PMT_color.push_back( color );
      PMT_alpha.push_back( alpha );

      // If we click more than once, return ellipse to previous HV color:
      if( PMTs_clicked.size()>1 ) {
	PMTs_clicked[0]->SetFillColorAlpha(PMT_color[0], PMT_alpha[0]);
	PMTs_clicked.erase( PMTs_clicked.begin() );
	PMT_color.erase( PMT_color.begin() );
	PMT_alpha.erase( PMT_alpha.begin() );
      }
      cout << "SN = " << serial_map[pmtclick->GetUniqueID()] << ", row = " << row_map[pmtclick->GetUniqueID()] 
	   << ", col = " << column_map[pmtclick->GetUniqueID()] << ", HV Group = " << HV_map[pmtclick->GetUniqueID()]
	   << " = " << alpha_map_back[ hv ] << endl;
    }
    if( select->InheritsFrom(TBox::Class()) ){
      TBox *box = (TBox*)select;
      int hv = box->GetUniqueID();
      int color = HV_Colors[ hv ];
      double alpha = HV_Alphas[ hv ];

      PMT_colorhv.push_back( color );
      PMT_alphahv.push_back( alpha );

      vector<int> list = HV_unique[hv];
      HV_click[click] = list;

      for(unsigned int i=0; i<list.size(); i++ ){
	PMTs[ list[i] ]->SetFillColor(3);	
      }
      if( HV_click.size()>1 ) {
      	vector<int> old = HV_click[click-1];
      	for(unsigned int i=0; i<old.size(); i++ ){
      	  PMTs[ old[i] ]->SetFillColorAlpha(PMT_colorhv[0], PMT_alphahv[0]);	
      	 }
	PMT_colorhv.erase( PMT_colorhv.begin() );
	PMT_alphahv.erase( PMT_alphahv.begin() );
      	HV_click.erase(click-1);
      }
      click++;
    }
  }
}
