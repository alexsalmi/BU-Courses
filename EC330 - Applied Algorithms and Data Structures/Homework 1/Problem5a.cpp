/**
 ** Created Alexander Salmi
 ** BUID: UXXXXXXXX
 **/

#include "findSingle.h"
#include <iostream>
using namespace std;

int main()
{
  int arr[] = {1, 7, 54, 4, 2, 54, 1, 7, 2};
  int n = sizeof(arr) / sizeof(arr[0]);
  cout << "The integer that occurs only once is " << findSingle(arr, n) << ".\n";
  return 0;
}

int findSingle(int arr[], int arr_size){
	// Variable to know whether the number is in the array twice
	bool isDouble = false;
	// Nested loop through array to compare all values
	for(int i=0; i<arr_size; i++){
		for(int j=0; j<arr_size; j++){
			// If it is not the same element and they have the same value, update isDouble
			if(i!=j && arr[i]==arr[j]){
				isDouble = true;
				break;
			}
		}
		// If there aren\t two of the number return, otherwise continue loop
		if(!isDouble) return arr[i];
		else isDouble = false;
	}
}
