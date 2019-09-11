// Author: Alexander Salmi
// BUID: UXXXXXXXX
#include "Huffman.h"

Huffman::Huffman(){
	root = new HuffmanNode(' ');
}

void Huffman::buildHuffmanTree(char characters[], int freq[], int& size){
	priority_queue <pairVal, vector<pairVal>, greater<pairVal> > priorityQueue;
	int i;

	if(size==0)
		return;

	// Fill the priority queue with all of the characters and their weights
	for(i=0; i<size; i++){
		priorityQueue.push(make_pair<int&,char&>(freq[i], characters[i]));
	}

	// Put the two first characters in the queue in the root's children
	root->zero = new HuffmanNode(priorityQueue.top().second);
	priorityQueue.pop();
	// If there is only one character, make an empty node
	if(size==1){
		root->one = new HuffmanNode(' ');
		priorityQueue.pop();
	}
	else{
		root->one = new HuffmanNode(priorityQueue.top().second);
		priorityQueue.pop();
	}

	// Build the rest of the huffman tree from the bottom up, making the previous root the
	// 'one' child of the new root, and make the next character in the queue the 'zero' child
	for(i=0; i<size-2; i++){
		HuffmanNode* z = new HuffmanNode(priorityQueue.top().second);
		root = new HuffmanNode(z, root, ' ');
		priorityQueue.pop();
	}
}

void Huffman::printCodes(){
	HuffmanNode* currentNode = root;
	string code = "";
	cout << "Huffman Code\tCharacter\n";
	cout << "--------------------------\n";

	// Print each character and it's code
	while(currentNode->zero != NULL){
		// Print the symbol held in the '0' child (there will always be one)
		cout << code << "0" << "\t\t" << currentNode->zero->symbol << endl;
		// Move down to the '1' child, where the rest of the characters are held
		code.append("1");
		if(currentNode->one != NULL)
			currentNode = currentNode->one;
	}

	// Print the final character and its key
	if(currentNode->symbol != ' ')
		cout << code << "\t\t" << currentNode->symbol << "\n\n";
}

void Huffman::decodeText(string filename){
	ifstream inFile;
	HuffmanNode* currentNode = root;
	int i;
	string code;

	// Open file
	inFile.open(filename);

	// Check if file is open, then start printing decodeed message
	if(inFile.is_open()){
		cout << "The decoded text is: ";
		// Get the first line of the file, where the encoded message is stored
		getline(inFile, code);

		for(char& c: code){
			// If the next character in the encoded message is a 0, and there is a symbol at the 0 node, print it
			if(c=='0'&&currentNode->zero!=NULL){
				if(currentNode->zero->symbol!=' '){
					cout << currentNode->zero->symbol;
					currentNode = root;
				}
				// If there is no symbol at the 0 node, go down into the 0 node
				//(this should never be reached, as there should always be a symbol in the 0 node if current node is not a leaf)
				else
					currentNode = currentNode->zero;
			}
			// If the next character in the encoded message is a 1, and there is a symbol at the 1 node, print it
			else if(c=='1'&&currentNode->one!=NULL){
				if(currentNode->one->symbol!=' '){
					cout << currentNode->one->symbol;
					currentNode = root;
				}
				// If there is no symbol at the 1 node, go down into the 1 node
				else
					currentNode = currentNode->one;
			}
			else
				return;
		}
	}
}
