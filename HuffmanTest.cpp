// HuffmanTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <map>
#include <list>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

struct TreeNode
{
	TreeNode()
	{
	}

	TreeNode(bool leaf, TreeNode* left, TreeNode* right, int val, int likelihood)
	{
		Leaf = leaf; 
		Left = left; 
		Right = right; 
		Val = val; 
		Likelihood = likelihood; 
	}

	bool Leaf; 
	TreeNode *Left;
	TreeNode *Right;
	short Val; 
	int Likelihood; 
};

struct BranchOrKey
{
	BranchOrKey(TreeNode* branch, int key, int likelihood)
	{
		Branch = branch; 
		Key = key; 
		Likelihood = likelihood; 
	}

	BranchOrKey()
	{
	}

	const bool operator==(const BranchOrKey& other)
	{
		return other.Branch == Branch &&
			other.Key == Key &&
			other.Likelihood == Likelihood;
	}

	TreeNode* Branch; 
	int Key; 
	int Likelihood; 
};

bool RemoveAndGetLowest(std::list<BranchOrKey>& keys, BranchOrKey *out)
{
	int lowestLikelihood = -1; 
	BranchOrKey best; 
	bool found = false; 
	for(auto itr = keys.begin(); itr != keys.end(); itr++)
	{
		BranchOrKey current = *itr; 
		if(current.Likelihood < lowestLikelihood || lowestLikelihood == -1)
		{
			best = current; 
			lowestLikelihood = current.Likelihood; 
			found = true; 
		}
	}

	if (found)
	{
		keys.remove(best);
		*out = best;
	}

	return found; 
}

int PickTwoLowest(TreeNode** left, TreeNode** right, std::list<BranchOrKey>& keys)
{
	int numberReturned = 0; 
	if(keys.size() >= 2)
	{
		BranchOrKey lowest; 
		if (RemoveAndGetLowest(keys, &lowest))
		{
			if (lowest.Branch != NULL)
				*left = lowest.Branch;
			else
			{
				*left = new TreeNode(true, NULL, NULL, lowest.Key, lowest.Likelihood);
			}
			numberReturned++;
		}

		BranchOrKey secondLowest; 
		if (RemoveAndGetLowest(keys, &secondLowest))
		{
			if (secondLowest.Branch != NULL)
			{
				*right = secondLowest.Branch;
			}
			else
			{
				*right = new TreeNode(true, NULL, NULL, secondLowest.Key, 
					secondLowest.Likelihood);
			}
			numberReturned++; 
		}
	}

	return numberReturned; 
}

void PrintTree(std::map<char,std::list<int>>& dict, std::list<int> bits, 
	TreeNode* root, int indent)
{
	if (!root->Leaf)
	{
		std::list<int> leftBits(bits); 
		leftBits.push_back(1); 
		PrintTree(dict, leftBits, root->Left, indent + 1);
		std::list<int> rightBits(bits); 
		rightBits.push_back(0); 
		PrintTree(dict, rightBits, root->Right, indent + 1);
	}
	else
	{
		dict[root->Val] = bits; 
	}
}

void Encode(std::vector<short> phrase, char** buffer, int* bufferLength)
{
	std::map<short,int> freq; 
	for (int c = 0; c < phrase.size(); c++)
	{
		freq[phrase[c]]++; 
	}

	std::list<BranchOrKey> keys;
	for (auto itr = freq.begin(); itr != freq.end(); itr++)
	{
		keys.push_back(BranchOrKey(NULL, (*itr).first, (*itr).second)); 
	}

	TreeNode* currentRoot = NULL;
	for (;;)
	{
		TreeNode *leftNode, *rightNode;
		int numberReturned = PickTwoLowest(&leftNode, &rightNode, keys);

		TreeNode* parent = new TreeNode(false, leftNode, rightNode, -1,
			leftNode->Likelihood + rightNode->Likelihood);
		currentRoot = parent;

		keys.push_back(BranchOrKey(parent, -1, parent->Likelihood));

		if (keys.size() == 1)
		{
			break;
		}
	} 

	std::list<int> bits; 
	std::map<char, std::list<int>> dict; 
	PrintTree(dict, bits, currentRoot, 0);

	//int bitsUsed = 0; 
	//for (int c = 0; c < phrase.size(); c++)
	//{
	//	std::list<int> bits = dict[phrase[c]]; 
	//	for (auto itr = bits.begin(); itr != bits.end(); itr++)
	//	{
	//		//std::cout << (*itr); 
	//		bitsUsed++; 
	//	}
	//}
	//std::cout << std::endl << (phrase.size() * 16) / (bitsUsed * 1.0) 
	//	<< "x compression" << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	char* buffer; 
	int length; 
	std::ifstream in("deltas.csv"); 

	std::vector<short> phrase; 
	short val; 
	while (in >> val)
	{
		phrase.push_back(val); 
	}

	Encode(phrase, &buffer, &length);

	std::cout << "done" << std::endl; 
	getchar(); 

	return 0;
}

