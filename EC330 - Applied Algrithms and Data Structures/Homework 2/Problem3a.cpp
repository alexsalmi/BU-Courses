/**
 ** Created by Alexander Salmi
 ** BUID: UXXXXXXXX
 **/

#include <cmath>
#include <iostream>

using namespace std;

int kthDigit(int k);

int kthDigit(int k){
	int n_count = 1;
	int currentnum = 0;
	int iterate;
	double digits;

	if(k==1){return 0;}

	// Loop until done
	while(n_count < k){
		currentnum++;
		digits=0;
		iterate = currentnum;

		// Find out how many digits are in the current number in the sequence
		while(iterate>=1){
			digits++;
			iterate = iterate/10;
		}

		iterate = currentnum;
		// If n_count + digits > k, the kth digit is the sequence is in the current number
		if(n_count+digits >= k){
			int diff  = k - n_count;
			// Find the correct digit in the current number and return it
			for(int i=digits-diff; i>0; i--){
				iterate = floor(iterate/10);
			}
			return iterate%10;
		}
		// If the kth digit is not in the current number, increase the n_count
		// to show how many digits in the sequence have been checked
		else{
			n_count +=digits;
		}

	}
}
