#include "stdafx.h"
#include "HEBPEnumeration.h"
#include "EBPOperations.h"

int ii = 0;
void HEBPEnumeration(vector<vector<int>> VEBP, ofstream &myFile)
{
	//recieve message from VEBP enumeration
	//cout << "VEBP: VEBP:///////////////";printEBP(VEBP);
	
	int VENumb = 0;
	int HENumb = 0;
	vector<vector<int>> VEBPPrime(M, vector<int>(N, 0));
	VEBPPrimeAndVENumbFromVEBP(VEBP, VEBPPrime, VENumb);
	
	vector<int> maxHEi(M-1,0);
	vector<int> minHEi(M-1,0);
	maxMinHeiFromVEBPPrime(VEBPPrime, maxHEi, minHEi);
	
	int leftMaxHEi = 0;
	int leftMinHEi = 0;
	for (int i = 1; i < M - 1; i++)
	{
		leftMaxHEi += maxHEi[i];
		leftMinHEi += minHEi[i];
	}


	HENumb = M*N - 1 - VENumb;

	vector<vector<int>> CCNumbMatrix(M, vector<int>(N + 1, 0));
	CCNumbMatFromVEBPPrime(CCNumbMatrix, VEBPPrime);
	
	vector<vector<int>> HEBP;
	distinctHEBPEnumeartion(0, CCNumbMatrix, maxHEi, minHEi, leftMaxHEi, leftMinHEi, HENumb, VENumb, HEBP, VEBP, myFile);
	//getchar();
}

void maxMinHeiFromVEBPPrime(vector<vector<int>> VEBPPrime, vector<int> &maxHEi, vector<int> &minHEi)
{
	for (int i = 1; i < M-1; i++)
	{
		int maxNumb = N;
		int minNumb = N;
		for (int j = 1; j < N; j++)
		{
			if (VEBPPrime[i + 1][j] == 1)
			{
				minNumb -= 1;
				if (VEBPPrime[i][j] == 1)
					maxNumb -= 1;
			}
		}
		maxHEi[i] = maxNumb;
		minHEi[i] = minNumb;
	}
}

void CCNumbMatFromVEBPPrime(vector<vector<int>> &CCNumbMatrix, vector<vector<int>> VEBPPrime)
{
	int CCNumbIndex = 0;
	for (int column = 0; column < VEBPPrime.size(); column++)
	{
		for (int row = 0; row < VEBPPrime[column].size(); row++)
		{
			if (VEBPPrime[column][row] == 1)
			{
				CCNumbMatrix[column][row + 1] = CCNumbMatrix[column][row];
			}
			else
			{
				CCNumbIndex++;
				CCNumbMatrix[column][row + 1] = CCNumbIndex;
			}
		}
	}
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
			VEBPPrime[sectionNumb][binNumb + 1] = 1;
		}
	}
}


void distinctHEBPEnumeartion(int currentColumn, vector<vector<int>> CCNumbMatrix, vector<int> maxHEi, vector<int> minHEi, int leftMaxHEi, int leftMinHEi, int HENumb, int VENumb, vector<vector<int>> HEBP, vector<vector<int>> VEBP, ofstream &myFile)//enumerate all distinct HEBP based on VEBP
{
	
	if (currentColumn > M - 2)
	{
		//cout << "CCNumbMAtrix: "; printEBP(CCNumbMatrix); cout << endl;
		//cout << "final CC#://////////////////// "; printEBPi(CCNumbMatrix[currentColumn]); cout << endl;

		//end..finish enumeration.. after check symmetry.. print or store HEBP

		//cout << "HEBP: ";printEBP(HEBP);
		//cout << "VEBP: "; printEBP(VEBP);
		
		bool HEBPIsDistinct = true;
		HEBPIsDistinct = checkIfDistinctHEBP(VEBP, HEBP);
		//cout << "mirroredHEBPIsDistinct " << HEBPIsDistinct << endl;
		if((M == N) && (M % 2 == 1)  && (VENumb == (M*N - 1) / 2) && HEBPIsDistinct)
		{
			HEBPIsDistinct = checkRotationalSymmetryOfHEBP(VEBP, HEBP);
		}
		/*cout << "beforeVEBP: "; printEBP(VEBP);
		cout << "beforeHEBP: "; printEBP(HEBP);
		cout << "rotateHEBPIsDistinct " << HEBPIsDistinct << endl;*/
		if (HEBPIsDistinct)
		{
			/*ii++;
			cout << ii << endl;*/
			writeEBP(VEBP, myFile);
			writeEBP(HEBP, myFile);
			/*cout << "VEBP: "; printEBP(VEBP);
			cout << "HEBP: "; printEBP(HEBP);*/
		}
	}
	else 
	{
		map<int, vector<vector<int>>> HEBPiTree;


		HEBPiTree = generateTreeForHEBPi(CCNumbMatrix[currentColumn], CCNumbMatrix[currentColumn+1]);
		//cout << "hebpiTREE"; printHEBPiTree(HEBPiTree);

		int size1Numb = HEBPiTree.size();
		int size2Numb = 0;
		for (map<int, vector<vector<int>>>::iterator it = HEBPiTree.begin(); it != HEBPiTree.end(); it++)
			size2Numb += it->second.size();
		
		/*int leftMaxHEi = 0;
		int leftMinHEi = 0;
		for (int i = currentColumn + 1; i < M - 1; i++)
		{
			leftMaxHEi += maxHEi[i];
			leftMinHEi += minHEi[i];
		}*/


		/*int MinNumber = max(size1Numb, HENumb - N*(M - 1 - (currentColumn + 1)));
		int MaxNumber = min(size2Numb, HENumb - (M - 1 - (currentColumn + 1)));*/

		int MinNumber = max(size1Numb, HENumb - leftMaxHEi);
		int MaxNumber = min(size2Numb, HENumb - leftMinHEi);
		
		for (int HENumbi = MinNumber; HENumbi <= MaxNumber; HENumbi++)
		{
			vector<int> HEBPi;
			generateHEBPi(maxHEi, minHEi, leftMaxHEi, leftMinHEi, VENumb, HENumb, HENumbi, HENumbi,  0, currentColumn, CCNumbMatrix, size1Numb, size2Numb, HEBPiTree, HEBPi, HEBP, VEBP, myFile);
		}
		
	}
}


void generateHEBPi(vector<int> maxHEi, vector<int> minHEi, int leftMaxHEi, int leftMinHEi, int VENumb, int HENumb, int HENumbi, int leftHENumbi, int indexInMapKey, int currentColumn, vector<vector<int>> CCNumbMatrix, int size1Numb, int size2Numb, map<int, vector<vector<int>>> HEBPiTree, vector<int> HEBPi, vector<vector<int>> HEBP, vector<vector<int>> VEBP, ofstream &myFile)
{
	if (indexInMapKey > HEBPiTree.size()-1)
	{
		/*cout << "hebp"<<currentColumn<<": " << endl;
		printEBPi(HEBPi);*/

		//vector<int>::iterator minValue = min_element(CCNumbMatrix[currentColumn + 1].begin() + 1, CCNumbMatrix[currentColumn + 1].end());
		bool isError = newCCNumb_iplus1(currentColumn, CCNumbMatrix, HEBPi);

		HENumb -= HENumbi;
		if (currentColumn < M - 2)
		{
			leftMaxHEi -= maxHEi[currentColumn + 1];
			leftMinHEi -= minHEi[currentColumn + 1];
		}
		HEBP.push_back(HEBPi);
		
		if (isError)
		{
			printEBP(VEBP);
			printEBP(HEBP);
			cout << "HENumb: " << HENumb << endl;
			printHEBPiTree(HEBPiTree);
			printEBP(CCNumbMatrix);
		}
		distinctHEBPEnumeartion(currentColumn + 1, CCNumbMatrix, maxHEi, minHEi, leftMaxHEi, leftMinHEi, HENumb, VENumb, HEBP, VEBP, myFile);
		//HEBP.pop_back();
		//end
	}
	else
	{
		map<int, vector<vector<int>>>::iterator iter = HEBPiTree.begin();
		advance(iter, indexInMapKey);
		
		vector<vector<int>> treeInEachCC = iter->second;
		int sizeOFEachCC = treeInEachCC.size();
		int minNumber = max(1, leftHENumbi - (size2Numb - sizeOFEachCC));
		int maxNumber = min(sizeOFEachCC, leftHENumbi - (size1Numb - 1));

		for (int oneBitsInEachCC = minNumber; oneBitsInEachCC <= maxNumber; oneBitsInEachCC++)
		{
			//comb
			vector<int> resultForCombinations;
			vector<vector<int>> combSetInEachCC;
			allCombinationsOfKInN(1, oneBitsInEachCC, sizeOFEachCC, resultForCombinations, combSetInEachCC);
			
			leftHENumbi -= oneBitsInEachCC;
			size2Numb -= sizeOFEachCC;
			size1Numb -= 1;
			//cout << leftHENumbi << " " << size2Numb << " " << size1Numb << endl;
			for (vector<vector<int>>::iterator comb = combSetInEachCC.begin(); comb != combSetInEachCC.end(); comb++)
			{		
				//cout << "comb: ";printEBPi(*comb);
				vector<vector<int>> CCCombInHEBPiSet;
				vector<int> CCCombInHEBPi;
				combinationOneBitForEachCCinHEBPTree(treeInEachCC, 0, *comb, CCCombInHEBPiSet, CCCombInHEBPi);
				//cout << "combSet: ";printEBP(CCCombInHEBPiSet);
				for (vector<vector<int>>::iterator eachCombInSet = CCCombInHEBPiSet.begin(); eachCombInSet != CCCombInHEBPiSet.end(); eachCombInSet++)
				{
					for (vector<int>::iterator it = eachCombInSet->begin(); it != eachCombInSet->end(); it++)
					{
						HEBPi.push_back(*it);
					}
					generateHEBPi(maxHEi, minHEi, leftMaxHEi, leftMinHEi, VENumb, HENumb, HENumbi,  leftHENumbi, indexInMapKey + 1, currentColumn, CCNumbMatrix, size1Numb, size2Numb, HEBPiTree, HEBPi, HEBP, VEBP, myFile);
					for (vector<int>::iterator it = eachCombInSet->begin(); it != eachCombInSet->end(); it++)
					{
						HEBPi.pop_back();
					}
				}	
			}
		}
	}
}

bool newCCNumb_iplus1(int currentColumn, vector<vector<int>> &CCNumbMatrix,  vector<int> HEBPi)
{
	bool error = false;
	map<int, vector<int>> CCNumbTree;
	map<int, vector<int>>::iterator it1;
	map<int, vector<int>>::iterator it2;
	initCCNumbTree(CCNumbTree, CCNumbMatrix, currentColumn);

	/*printEBP(CCNumbMatrix);

	for (map<int, vector<int>>::iterator it = CCNumbTree.begin(); it != CCNumbTree.end(); it++)
	{
		cout << it->first << " --> ";
		for (vector<int>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{
			cout <<"+ "<<  *it2 << endl;
			
		}
	}*/

	for (vector<int>::iterator edgeIndex = HEBPi.begin(); edgeIndex != HEBPi.end(); edgeIndex++)
	{
		it1 = CCNumbTree.find(CCNumbMatrix[currentColumn][*edgeIndex]);
		it2 = CCNumbTree.find(CCNumbMatrix[currentColumn+1][*edgeIndex]);
		/*if(it2 == CCNumbTree.end())
			cout << "it1 "<< CCNumbMatrix[currentColumn][*edgeIndex]<<" it2 "<< CCNumbMatrix[currentColumn + 1][*edgeIndex] << endl;*/

		if (CCNumbMatrix[currentColumn][*edgeIndex] < CCNumbMatrix[currentColumn + 1][*edgeIndex])
		{
			
			for (vector<int>::iterator iterator = it2->second.begin(); iterator != it2->second.end(); iterator++)
			{
				it1->second.push_back(*iterator);
				//int indexInCCNumb = *iterator - N;
				int indexInCCNumb = 0;
				int column = 0;
				if (*iterator > N)//currentColumn+1
				{
					column = currentColumn + 1;
					indexInCCNumb = *iterator - N;
				}
					
				else//currentColumn
				{
					column = currentColumn;
					indexInCCNumb = *iterator;
				}
				
				CCNumbMatrix[column][indexInCCNumb] = it1->first;
			}
			CCNumbTree.erase(it2);
		}
		
		else if (CCNumbMatrix[currentColumn][*edgeIndex] > CCNumbMatrix[currentColumn + 1][*edgeIndex])
		{
			
			for (vector<int>::iterator iterator = it1->second.begin(); iterator != it1->second.end(); iterator++)
			{
				it2->second.push_back(*iterator);
				//int indexInCCnumb = *iterator;
				int indexInCCNumb = 0;
				int column = 0;
				if (*iterator > N)//currentColumn+1
				{
					column = currentColumn + 1;
					indexInCCNumb = *iterator - N;
				}

				else//currentColumn
				{
					column = currentColumn;
					indexInCCNumb = *iterator;
				}
				CCNumbMatrix[column][indexInCCNumb] = it2->first;
			}
			CCNumbTree.erase(it1);
		}
		else
		{
			cout << "ERROR!!!!! loop is appeared!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			error = true;
		}
			
	}
	return error;
	
}

void initCCNumbTree(map<int, vector<int>> &CCNumbTree, vector<vector<int>> CCNumbMatrix, int currentColumn)
{
	
	for (int column = currentColumn; column <= currentColumn + 1; column++)
	{
		for (int indexInCCnumb = 1; indexInCCnumb <= N; indexInCCnumb++)
		{
			int ccNumb = CCNumbMatrix[column][indexInCCnumb];
			int indexInTree = indexInCCnumb + (column - currentColumn)*N;
			map<int, vector<int>>::iterator it;
			it = CCNumbTree.find(ccNumb);
			if (it != CCNumbTree.end())
				it->second.push_back(indexInTree);
			else
				CCNumbTree.insert(pair<int,vector<int>>(ccNumb, vector<int>(1, indexInTree)));
		}
	}
}

void combinationOneBitForEachCCinHEBPTree(vector<vector<int>> treeInEachCC, int indexOfEachGroupInCC, vector<int> comb, vector<vector<int>> &CCCombInHEBPiSet, vector<int> CCCombInHEBPi)
{
	if (indexOfEachGroupInCC > comb.size() - 1)
	{
		CCCombInHEBPiSet.push_back(CCCombInHEBPi);
	}
	else
	{
		int indexOfOneBitInCC = comb[indexOfEachGroupInCC] - 1;
		vector<int> resultForCombinations;
		vector<vector<int>> combInEachGroupInCCSet;
		allCombinationsOfKInN(1, 1, treeInEachCC[indexOfOneBitInCC].size(), resultForCombinations, combInEachGroupInCCSet);
		for (vector<vector<int>>::iterator eachComb = combInEachGroupInCCSet.begin(); eachComb != combInEachGroupInCCSet.end(); eachComb++)
		{
			int indexOFOneBitInHEBPi = treeInEachCC[indexOfOneBitInCC][eachComb[0][0] - 1];
			CCCombInHEBPi.push_back(indexOFOneBitInHEBPi);
			combinationOneBitForEachCCinHEBPTree(treeInEachCC, indexOfEachGroupInCC + 1, comb, CCCombInHEBPiSet, CCCombInHEBPi);
			CCCombInHEBPi.pop_back();
		}
	}
}

map<int, vector<vector<int>>> generateTreeForHEBPi(vector<int> CCNumb_i, vector<int> CCNumb_iplus1)
{
	map<int, vector<vector<int>>> resultTree;
	for (int i = 1; i < CCNumb_i.size(); i++)
	{
		bool Connected = true;
		bool sameCCNumb = false;
		
		for (int j = i; j >= 1; j--)
		{
			if (CCNumb_iplus1[j-1] != CCNumb_iplus1[j])
				Connected = false;
			if (CCNumb_i[i] == CCNumb_i[j - 1])
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

			resultTree.insert(pair<int, vector<vector<int>>>(CCNumb_i[i], newBranchOfRoot));
		}
		else
		{
			if (Connected)
			{
				
				int lastIndexOfGrandChildNodeOfRoot = resultTree[CCNumb_i[i]].size()-1;
				resultTree[CCNumb_i[i]][lastIndexOfGrandChildNodeOfRoot].push_back(i);
			}
			else
			{
				vector<int> newBranchOfsameCCNumb;
				newBranchOfsameCCNumb.push_back(i);
				resultTree[CCNumb_i[i]].push_back(newBranchOfsameCCNumb);
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