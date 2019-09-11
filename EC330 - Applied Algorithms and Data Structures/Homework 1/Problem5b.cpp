/**
 ** Created by Alexander Salmi
 ** BUID: UXXXXXXXX
 **/

#include "sumDigit.h"
#include <cmath>
#include <iostream>

using namespace std;

int main() {
  int n = 12345;
  cout << sumDigit(n) << "\n";
  return 0;
}

int sumDigit(int n){
	// Variable to save new number
	int num=0;

	// Loop untl return
	while(true){
		// While there are still digits left in num, keep looping
		while(n>=1){
			// Add first digit and move digits to the right
			num += n%10;
			n = floor(n/10);
		}
		// If the number is one digit, return. Else continue loop
		if(num<10) return num;
		else{
			n = num;
			num = 0;
		}
	}
}
