#include <iostream>
#include <vector>
#include <algorithm>

#include "TRandom.h"
#include "TStopwatch.h"

using namespace std;

void print(vector<int> vec, vector<int>::iterator it);

void bubble(){

  // Fill vector with random integers:
  TRandom *random = new TRandom();
  random->SetSeed(7);
  
  vector<int> unsorted_ints;
  vector<int>::iterator vit;

  for(int i=0; i<20; i++){
    int temp = random->Integer(65539);

    if(find(unsorted_ints.begin(), unsorted_ints.end(), temp) != unsorted_ints.end()){}
    else{ unsorted_ints.push_back(temp); }
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //                                  BUBBLE SORT                                        //
  /////////////////////////////////////////////////////////////////////////////////////////

  // Outer loop - worst scenario is the entire vector needs to be inverted, i.e. x[3]={3,2,1} 
  // then requires 2 iterations to make it sorted. Therefore, outer loop needs to be [0,size-1].
  bool swapped;
  int iterations = 0;
  TStopwatch *time = new TStopwatch();
  for(unsigned int i=0; i<(unsorted_ints.size()-1); i++){
    swapped = false;
    for(unsigned int j=0; j<(unsorted_ints.size()-1-i); j++){
      if( unsorted_ints[j] > unsorted_ints[j+1] ){
	swap(unsorted_ints[j],unsorted_ints[j+1]);
	swapped = true;
      }
    }
    // If the item has not been swapped, then break bc it is okay where it is.
    if(!swapped) break;
    //print(unsorted_ints,vit);
    iterations++;
  }
  time->Stop();
  cout << "Sorting took " << iterations << " iterations, and " << time->RealTime() << " seconds. " << endl;
}

void swap(int a, int b){
  int temp = a;
  a = b;
  b = temp;
}

void print(vector<int> vec, vector<int>::iterator it){
  for(it=vec.begin(); it!=vec.end(); it++){
    cout << *it << " "; 
  }
  cout << "\n" << endl;
}
