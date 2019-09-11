/**
 ** Created by Alexander Salmi
 ** BUID: UXXXXXXXX
 **/

#include <iostream>
#include <vector>

using namespace std;

int sumTo333(vector<int> nums);

int sumTo333(vector<int> nums){
	int closest_sum;
	int closest_diff;
	int sum;
	int diff;

	// Loop through the vector with 3 iterators to compare all numbers
	for(int i=0; i< nums.size(); i++){
		for(int j=i+1; j<nums.size(); j++){
			for(int k=j+1; k<nums.size(); k++){
				// Sum the three numbers
				sum = nums.at(i) + nums.at(j) + nums.at(k);

				// Find the difference between the sum and 333
				if(sum>333){diff = sum-333;}
				else{diff = 333-sum;}

				// If the current sum is closer to 333 than the past closest sum, update
				if(diff < closest_diff){
					closest_diff = diff;
					closest_sum = sum;
				}
				// If they are equally far from 333, update with larger of the sums
				else if(diff == closest_diff && sum != closest_sum){
					if(sum > closest_sum){
						closest_sum == sum;
					}
				}
				// If the sum is exactly 333, return the sum
				else if(diff == 0){
					return sum;
				}
			}
		}
	}

	return closest_sum;
}
