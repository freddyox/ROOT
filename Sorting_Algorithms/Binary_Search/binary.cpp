#include <iostream>
#include <vector>
#include <stdio.h>

// If found, returns the position within the vector.
// Otherwise, returns -1.
template <typename T>
int binary(std::vector<T> vec, T x){
  int lo = 0;
  int hi = vec.size();

  while( lo <= hi ){
    int mid = lo + (hi-lo)/2;
    if( vec[mid] == x ) return mid;

    if( vec[mid] < x ) lo = mid + 1;

    else hi = mid - 1;  
  }
  return -1;
}

int main(){
  
  std::vector<double> test;
  for(int i=0; i<10; i++){
    test.push_back(i*3*.5);
  }
  
  int result = binary(test,1.5);
  (result == -1) ? printf("Did not find.")
    : printf("Element is present at index %d", result);
  if( result!=-1 ){
    std::cout << "\n";
    std::cout << "value = " << test[result] << std::endl;
  }

  return 0;
}
