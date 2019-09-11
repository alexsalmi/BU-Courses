// Author: ALexander Salmi
// BUID: UXXXXXXXX

#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <list>

using namespace std;

void sortLetterCounts(std::vector<int> *letterCounts, string *chars);

string sortByFreq(string s) {
	std::vector<int> letterCounts(26, 0);			// Stores counts for each letter
	string chars = "abcdefghijklmnopqrstuvwxyz";	// To keep track of which letter corresponds to each count
	int i, j;

	// Count number of instances for each letter
	for(i=0; i<s.length(); i++)
		letterCounts[s[i]-'a']++;

	// Sort the counts of each letter
	sortLetterCounts(&letterCounts, &chars);

	// Append each letter as many times as they occur in the input string
	s = "";
	for(i=0; i<26; i++){
		for(j=0; j<letterCounts[i]; j++){
			s += chars[i];
		}
	}

	return s;
}


/*
	Auxillary/utility functions, if you need to implement any, go here.
*/

// Insertion sort to sort the counts of each letter
// chars[] is also sorted corresponding to the letterCount[] sort, so that we know which letter count corresponds to which letter
void sortLetterCounts(std::vector<int> *letterCounts, string *chars){
	int i, j, value, charValue;
	for(i=1; i<26; i++){
		value = letterCounts->at(i);
		charValue = chars->at(i);
		j = i-1;
		while(j>=0 && letterCounts->at(j)>value){
			letterCounts->at(j+1) = letterCounts->at(j);
			chars->at(j+1) = chars->at(j);
			j = j-1;
		}
		letterCounts->at(j+1) = value;
		chars->at(j+1) = charValue;
	}
}
