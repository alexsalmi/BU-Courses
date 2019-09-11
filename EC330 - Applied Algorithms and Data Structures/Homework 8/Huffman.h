// Author: Alexander Salmi
// BUID: UXXXXXXXX
#include <unordered_map>
#include <queue>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

// Defining pair type for priority queue
typedef pair<int, char> pairVal;

class HuffmanNode{
public:
	HuffmanNode* zero; 	//Left node (zero)
	HuffmanNode* one;  	//Right node (one)
	char symbol;		//Character held in node

	HuffmanNode(char s){
		zero = NULL;
		one = NULL;
		symbol = s;
	}

	HuffmanNode(HuffmanNode* z, HuffmanNode* o, char s){
		zero = z;
		one = o;
		symbol = s;
	}
};


class Huffman{
public:
	HuffmanNode* root;

	Huffman();

	void buildHuffmanTree(char characters[], int freq[], int& size);

	void printCodes();

	void decodeText(string filename);
};
