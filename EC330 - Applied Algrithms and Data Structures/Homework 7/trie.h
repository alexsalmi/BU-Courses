// Author: Alexander Salmi
// BUID: UXXXXXXXX

#include <string>

using namespace std;

const int ALPHABET_SIZE = 26; // Consider only lowercase letters for this problem

struct TrieNode {
	TrieNode *children[ALPHABET_SIZE];
	int val;
	TrieNode() {
		val = 0; // Default
		for (int i=0;i<ALPHABET_SIZE;i++) {
			children[i] = NULL;
		}
	}
};

class Trie {

public:
	TrieNode *root;
	int map_size; // size of the map
	int tree_size; // size of the tree
	Trie() {root = new TrieNode(); map_size = 0; tree_size = 1;}

	/*
		Insert (key, val) into the trie. If key is already present, update the corresponding value.
		Update map_size and tree_size accordingly. If key is already present, these numbers should not change.
	*/
	void insert(string key, int val) {
		TrieNode* trieParse = root;

		// Traverse down the tree, adding new nodes if they do not exist yet, and updating the tree size
		for(int i=0; i<key.length(); i++){
			if(trieParse->children[key[i]-'a'] == NULL){
				trieParse->children[key[i]-'a'] = new TrieNode();
				tree_size++;
			}

			trieParse = trieParse->children[key[i]-'a'];
		}

		// If there is no value set at this key yet, update the map size
		if(trieParse->val==0)
			map_size++;

		// Update the value at the key
		trieParse->val = val;
	}

	/*
		Search key and return the corresponding value. Return 0 if key is not present in the trie.
	*/
	int search(string key) {
		TrieNode* trieParse = root;

		// Traverse down the tree until you get to the key
		// If the key does not exist, return 0
		for(int i=0; i<key.length(); i++){
			if(trieParse->children[key[i]-'a'] == NULL)
				return 0;

			trieParse = trieParse->children[key[i]-'a'];
		}

		// Return the value at the key
		return trieParse->val; // Modify the return value appropriately in your implementation
	}

	/*
		Remove key from the trie. Update map_size and tree_size accordingly.
		If the node corresponding to the key is a leaf node, you should remove the node.
		If the node corresponding to the key is not a leaf node, you should update its value accordingly but should not remove the node.
	*/
	void remove(string key) {
		TrieNode* trieParse  = root;
		int i, j, length = key.length();
		bool isEmpty;

		// Traverse down the tree until you get to the key
		for(i=0; i<length; i++){
			if(trieParse->children[key[i]-'a'] == NULL)
				return;
			trieParse = trieParse->children[key[i]-'a'];
		}

		// If there is a val at the key, set it to 0, and update map size
		if(trieParse->val>0){
			trieParse->val = 0;
			map_size--;
		}

		// Check if the key's node has children or not
		isEmpty = true;
		for(i=0; i<ALPHABET_SIZE; i++){
			if(!(trieParse->children[i]==NULL))
				isEmpty = false;
		}

		// If the node does not have children, delete it and its parents bottom up until the next parent node with a val
		if(isEmpty){
			for(i=length-1; i>=0; i--){
				trieParse = root;
				// Traverse down to the next node
				for(j=0; j<i; j++)
					trieParse = trieParse->children[key[j]-'a'];

				// Update the next node (NULL if it has no val and its children are empty, stays the same if not)
				trieParse->children[key[i]-'a'] = removeHelper(trieParse->children[key[i]-'a']);

				// If the next node is not NULL, this means a node that is in use has been hit, stop deleting nodes
				if(!(trieParse->children[key[i]-'a']==NULL))
					return;
			}
		}
	}

	TrieNode* removeHelper(TrieNode* trieParse){
		bool isEmpty = true;
		// Check if the node's children are empty
		for(int i=0; i<ALPHABET_SIZE; i++){
			if(!(trieParse->children[i]==NULL))
				isEmpty = false;
		}

		// If the node has no val and it's children are empty, set it to null and update tree size, otherwise keep it the same.
		if(isEmpty && (trieParse->val == 0)){
			tree_size--;
			return NULL;
		}
		else
			return trieParse;
	}
};
