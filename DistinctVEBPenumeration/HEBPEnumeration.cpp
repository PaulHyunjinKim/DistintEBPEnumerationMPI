#include "stdafx.h"
#include "HEBPEnumeration.h"
#include "EBPOperations.h"

void HEBPEnumeration(vector<vector<int>> VEBP)
{
	//recieve message from VEBP enumeration
	printEBP(VEBP);
	
	int VENumb = 0;
	int HENumb = 0;
	vector<vector<int>> VEBPPrime(M, vector<int>(N-1,0));
	VEBPPrimeAndVENumbFromVEBP(VEBP, VEBPPrime, VENumb);//initial VEBPPrime

	vector<int> CCNumb;
	for (int NumberOfCC = 0; NumberOfCC <= N; NumberOfCC++)
		CCNumb.push_back(NumberOfCC);//first CC#

	//TODO: initial CC# with VEBPPrimei
	vector<int> VEBPPrimei = VEBPPrime[0];
	HENumb = M*N - 1 - VENumb;
	CCNumb[1] = 1; CCNumb[2] = 1; CCNumb[3] = 1;//assume..111
	distinctHEBPEnumeartion(0, CCNumb, VEBPPrime, HENumb);
	getchar();
}

void VEBPPrimeAndVENumbFromVEBP(vector<vector<int>> VEBP, vector<vector<int>> &VEBPPrime, int &VENumb)
{
	for (int i = 0; i < VEBP.size(); i++)
	{
		for (int j = 0; j < VEBP[i].size(); j++)
		{
			int sectionNumb = VEBP[i][j] - 1;
			int binNumb = i;

			VENumb++;
			VEBPPrime[sectionNumb][binNumb] = 1;
		}
	}
}


void distinctHEBPEnumeartion(int currentColumn, vector<int> CCNumb, vector<vector<int>> VEBPPrime, int HENumb)//enumerate all distinct HEBP based on VEBP
{
	if (currentColumn > M - 1)
	{
		//end..finish enumeration.. after check symmetry.. print or store HEBP//
	}
	else 
	{
		map<int, vector<vector<int>>> HEBPiTree;
		vector<int> VEBPPrime_iplus1(N,0);

		for (int indexOfVEBPPrime = 0; indexOfVEBPPrime < VEBPPrime[currentColumn+1].size(); indexOfVEBPPrime++)
			VEBPPrime_iplus1[indexOfVEBPPrime+1] = VEBPPrime[currentColumn+1][indexOfVEBPPrime]; //00
		
		cout << "CCNumb: "; printEBPi(CCNumb); cout << endl;
		cout << "VEBPPrim_i+1: "; printEBPi(VEBPPrime_iplus1); cout<< endl;

		HEBPiTree = generateTreeForHEBPi(CCNumb, VEBPPrime_iplus1);
		printHEBPiTree(HEBPiTree);

		int size1Numb = HEBPiTree.size();
		int size2Numb = 0;
		for (map<int, vector<vector<int>>>::iterator it = HEBPiTree.begin(); it != HEBPiTree.end(); it++)
			size2Numb += it->second.size();

		int MinNumber = max(size1Numb, HENumb - N*(M - 1 - currentColumn));
		int MaxNumber = min(size2Numb, HENumb - (M - 1 - currentColumn));

		for (int HENumbi = MinNumber; HENumbi <= MaxNumber; HENumbi++)
		{
			generateHEBPi(HENumbi, 0, 0, currentColumn, CCNumb, VEBPPrime, size1Numb, size2Numb, HEBPiTree);
		}
		
	}
}


void generateHEBPi(int HENumbi, int leftHENumbi, int indexInMapKey, int currentColumn, vector<int> CCNumb, vector<vector<int>> VEBPPrime, int size1Numb, int size2Numb, map<int, vector<vector<int>>> HEBPiTree)
{
	if (indexInMapKey > HEBPiTree.size()-1)
	{
		//end
	}
	else
	{

	}
}

map<int, vector<vector<int>>> generateTreeForHEBPi(vector<int> CCNumb, vector<int> VEBPPrime_iplus1)
{
	map<int, vector<vector<int>>> resultTree;
	for (int i = 1; i < CCNumb.size(); i++)
	{
		bool Connected = true;
		bool sameCCNumb = false;
		
		for (int j = i; j >= 1; j--)
		{
			if (VEBPPrime_iplus1[j-1] == 0)
				Connected = false;
			if (CCNumb[i] == CCNumb[j - 1])
			{
				sameCCNumb = true;
				break;
			}
		}

		
		if (!sameCCNumb)
		{
			
			vector<vector<int>> newBranchOfRoot;
			vector<int> newBranchOfsameCCNumb;
			newBranchOfsameCCNumb.push_back(i);
			newBranchOfRoot.push_back(newBranchOfsameCCNumb);

			resultTree.insert(pair<int, vector<vector<int>>>(CCNumb[i], newBranchOfRoot));
		}
		else
		{
			if (Connected)
			{
				
				int lastIndexOfGrandChildNodeOfRoot = resultTree[CCNumb[i]].size()-1;
				resultTree[CCNumb[i]][lastIndexOfGrandChildNodeOfRoot].push_back(i);
			}
			else
			{
				vector<int> newBranchOfsameCCNumb;
				newBranchOfsameCCNumb.push_back(i);
				resultTree[CCNumb[i]].push_back(newBranchOfsameCCNumb);
			}
		}
	}

	return resultTree;
}

void printHEBPiTree(map<int, vector<vector<int>>> HEBPiTree)
{
	///print tree to see if tree is correct
	for (map<int, vector<vector<int>>>::iterator it = HEBPiTree.begin(); it != HEBPiTree.end(); it++)
	{
		cout << it->first << " --> ";
		for (vector<vector<int>>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{
			cout << " + ";
			for (vector<int>::iterator it3 = it2->begin(); it3 != it2->end(); it3++)
			{
				cout << "--> " << *it3 << endl;
			}

		}
	}
}