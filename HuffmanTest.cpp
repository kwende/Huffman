// HuffmanTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <map>
#include <list>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

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
	char Val; 
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
	int lowestLikelihood = 100000; 
	BranchOrKey best; 
	bool found = false; 
	for(auto itr = keys.begin(); itr != keys.end(); itr++)
	{
		BranchOrKey current = *itr; 
		if(current.Likelihood < lowestLikelihood)
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

void PrintTree(TreeNode* root, int indent, std::ofstream& out)
{
	if (!root->Leaf)
	{
		for (int c = 0; c < indent; c++) out << "\t"; 
		out << "[" << root->Likelihood << "]" << std::endl;

		PrintTree(root->Left, indent+1, out); 
		PrintTree(root->Right, indent+1, out); 
	}
	else
	{
		for (int c = 0; c < indent; c++) out << "\t";
		out << root->Val << ":" << root->Likelihood << std::endl; 
	}
}

void Encode(std::string phrase, char** buffer, int* bufferLength)
{
	std::map<char,int> freq; 
	for (int c = 0; c < phrase.length(); c++)
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
}

int _tmain(int argc, _TCHAR* argv[])
{
	char* buffer; 
	int length; 
	Encode("Hello World", &buffer, &length); 

	//TreeNode* nodes = new TreeNode[sizeof(TreeNode) * 512]; 
	//memset((char*)nodes, 0, sizeof(TreeNode) * 512); 

	//std::list<BranchOrKey> keys; 
	//keys.push_back(BranchOrKey(NULL, 1, 15)); 
	//keys.push_back(BranchOrKey(NULL, 2, 5)); 
	//keys.push_back(BranchOrKey(NULL, 3, 20));
	//keys.push_back(BranchOrKey(NULL, 4, 7)); 
	//keys.push_back(BranchOrKey(NULL, 5, 45));
	//keys.push_back(BranchOrKey(NULL, 6, 10));

	//TreeNode* currentRoot = NULL; 
	//for (;;)
	//{
	//	TreeNode* leftNode, rightNode;
	//	int numberReturned = PickTwoLowest(&leftNode, &rightNode, keys);

	//	TreeNode* parent = new TreeNode(false, leftNode, rightNode, -1,
	//		leftNode->Likelihood + rightNode->Likelihood); 
	//	currentRoot = parent; 

	//	keys.push_back(BranchOrKey(parent, -1, parent->Likelihood)); 

	//	if (keys.size() == 1)
	//	{
	//		std::cout << "Done" << std::endl; 
	//		break; 
	//	}
	//}



	//getchar(); 

	return 0;
}

