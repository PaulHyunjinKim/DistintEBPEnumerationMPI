#include "stdafx.h"
#include "VEBPEnumeration.h"
#include "HEBPEnumeration.h"
#include "EBPOperations.h"

void VEBPEnumeartion(MPI_Datatype mpi_vebp_type)
{
	int OneBitsNumber = 0;
	int startNumberOfBitsInVEBP = ceilf((float)(M*N - 1) / 2);
	VEBP_type send;
	if (M != N)
		startNumberOfBitsInVEBP = N - 1;
	int receiver = 1;
	for (OneBitsNumber = startNumberOfBitsInVEBP; OneBitsNumber <= N*N - N; OneBitsNumber++)//enumerate VEBP based on # 1bits on VEBP
	{		
		send.firstNumb = OneBitsNumber;
		send.secondNumb = 2;
		send.thirdNumb = 3;
		
		sprintf_s(send.myFile, sizeof(send.myFile), "EBPFileMPI_%d.txt", receiver);
		//cout << "i " << send.myFile << endl;
		MPI_Send(&send, 1, mpi_vebp_type, receiver, 0, MPI_COMM_WORLD);
		receiver++;
		
		//
		////send message to VEBPEnumeration...//
		//SetOneBitNumberOnEachSectionVEBP(OneBitsNumber, 0, result, 0, myFile);
	}
	/*send.firstNumb = -1;
	MPI_Send(&send, 1, mpi_vebp_type, 1, 0, MPI_COMM_WORLD);*/

}

void SetOneBitNumberOnEachSectionVEBP(int LeftOneBitsNumber, int currentRow, vector<int> result, int LastOneBitsNumber, ofstream &myFile, double &numberOfOutputs)
{
	currentRow += 1;
	LeftOneBitsNumber -= LastOneBitsNumber;

	if (currentRow > N - 1)
	{
		bool equalSwitch = true;
		if (checkIFDistinctBitsComb(result, equalSwitch))
			distinctVEBPEnumeration(result, myFile, numberOfOutputs, equalSwitch);
	}
	else
	{
		int MinNumber = max(1, LeftOneBitsNumber - M*(N - 1 - currentRow));
		int MaxNumber = min(M, LeftOneBitsNumber - (N - 1 - currentRow));
		for (LastOneBitsNumber = MinNumber; LastOneBitsNumber <= MaxNumber; LastOneBitsNumber++)
		{
			result.push_back(LastOneBitsNumber);
			SetOneBitNumberOnEachSectionVEBP(LeftOneBitsNumber, currentRow, result, LastOneBitsNumber, myFile, numberOfOutputs);
			result.pop_back();
		}
	}
}

bool checkIFDistinctBitsComb(vector<int> result, bool &equalSwitch)
{
	bool checkedResult = true;

	for (int i = 0; i < (M - 1) / 2; i++)
	{
		int bitNumb1 = result[i];
		int bitNumb2 = result[M - 2 - i];
		if (bitNumb1 != bitNumb2)
		{
			equalSwitch = false;
			if (bitNumb1 < bitNumb2)
				checkedResult = false;
			break;
		}
	}


	return checkedResult;
}

void distinctVEBPEnumeration(vector<int> result, ofstream &myFile, double &numberOfOutputs, bool equalSwitch)
{
	//cout << "result " << endl;
	
	vector<vector<vector<int>>> VEBPSet;
	///combinatorics based on #1bits in each section of VEBP///
	for (vector<int>::iterator it = result.begin(); it != result.end(); ++it)
	{		
		int K = *it;
		vector<int> resultForCombinations;
		vector<vector<int>> VEBPi;
		//cout << K << " ";
		allCombinationsOfKInN(1, K, M, resultForCombinations, VEBPi);
		
		VEBPSet.push_back(VEBPi);
	}
	//cout << endl;
	
	vector<vector<int>> VEBP;
	completeEachVEBP(0, VEBPSet, VEBP, myFile, numberOfOutputs, equalSwitch);
	//cout << endl;
}



void completeEachVEBP(int sectionNumber, vector<vector<vector<int>>> VEBPSet, vector<vector<int>> VEBP, ofstream &myFile, double &numberOfOutputs, bool equalSwitch)
{
	if (sectionNumber == N - 1)
	{
		
		if (!equalSwitch)
		{
			if (EqualEBP(VEBP, LargerEBP(VEBP, inverseEBP(VEBP, M))))
			{
				//myFile << numberOfOutputs << endl;
				//numberOfOutputs++;
				//writeEBP(VEBP, myFile);
				HEBPEnumeration(VEBP, myFile);
			}		
		}
		else
		{
			if (checkIfDistinct(VEBP))
			{
				//myFile << numberOfOutputs << endl;
				//numberOfOutputs++;
				//cout << "number of outputs from VEBP " << ii << endl;
				//printEBP(VEBP);
				//writeEBP(VEBP, myFile);
				//send message to HEBP enumeration..//
				HEBPEnumeration(VEBP, myFile);
				//////////
			}
		}
	}
	else
	{
		for (int i = 0; i < VEBPSet[sectionNumber].size(); i++)
		{
			VEBP.push_back(VEBPSet[sectionNumber][i]);
			
			completeEachVEBP(sectionNumber + 1, VEBPSet, VEBP, myFile, numberOfOutputs, equalSwitch);

			VEBP.pop_back();
		}
	}
}

