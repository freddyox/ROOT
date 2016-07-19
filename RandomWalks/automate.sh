#!/bin/bash

#root -l -b << EOF
#.x RandomWalk.cpp
#EOF

#sed -i 's/SetSeed(4)/SetSeed(5)/g' RandomWalk.cpp
#sed -i 's/random_walk3D_4.root/random_walk3D_5.root/g' RandomWalk.cpp

#root -l -b << EOF
#.x RandomWalk.cpp
#EOF

#sed -i 's/SetSeed(5)/SetSeed(6)/g' RandomWalk.cpp
#sed -i 's/random_walk3D_5.root/random_walk3D_6.root/g' RandomWalk.cpp

#root -l -b << EOF
#.x RandomWalk.cpp
#EOF

#sed -i 's/SetSeed(6)/SetSeed(7)/g' RandomWalk.cpp
#sed -i 's/random_walk3D_6.root/random_walk3D_7.root/g' RandomWalk.cpp

#root -l -b << EOF
#.x RandomWalk.cpp
#EOF

root -l -b << EOF
TFile f("random_walk3D_4.root");
  RW_hist->SetMarkerStyle(6);
  RW_hist->SetMarkerColor(8);
  RW_hist->SetXTitle("X");
  RW_hist->SetYTitle("Y");
  RW_hist->SetZTitle("Z");
RW_hist->Draw();

TFile g("random_walk3D_5.root");
 RW_hist->SetMarkerColor(4);
RW_hist->Draw("same");

TFile h("random_walk3D_6.root");
 RW_hist->SetMarkerColor(2);
RW_hist->Draw("same");

TFile i("random_walk3D_7.root");
 RW_hist->SetMarkerColor(7);
RW_hist->Draw("same");
c1->Rotate(40,0);
c1->SaveAs("text.png");
EOF
exit
