#include "stdafx.h"
#include "VEBPEnumeration.h"
#include <ctime>



int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	
	clock_t begin = clock();



	int rank;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	int nitems = 4;
	int blocklengths[4] = { 1, 1, 1, 25 };
	MPI_Datatype types[4] = { MPI_INT, MPI_INT, MPI_INT, MPI_CHAR };
	MPI_Datatype mpi_vebp_type;
	MPI_Aint offsets[4];
	offsets[0] = offsetof(VEBP_type, firstNumb);
	offsets[1] = offsetof(VEBP_type, secondNumb);
	offsets[2] = offsetof(VEBP_type, thirdNumb);
	offsets[3] = offsetof(VEBP_type, myFile);

	MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_vebp_type);
	MPI_Type_commit(&mpi_vebp_type);

	

	if (rank == 0)
	{

		//cout << "rank: " << rank << endl;
		VEBPEnumeartion(mpi_vebp_type);
		cout <<"process "<<rank<< " Finished...hit Enter to exit.." << endl;
	}
	else if (rank >= 1 && rank <= 9)
	{
		//cout << "rank: " << rank << endl;
		VEBP_type recv;
		
		MPI_Recv(&recv, 1, mpi_vebp_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		cout << "size 2 " << recv.firstNumb << endl;
		
		cout << recv.myFile << endl;

		ofstream EBPFile;
		EBPFile.open(recv.myFile);

		double numberOfOutputs = 1;
		vector<int> result;
		SetOneBitNumberOnEachSectionVEBP(recv.firstNumb, 0, result, 0, EBPFile, numberOfOutputs);
		
		
		EBPFile.close();
		


		cout << "process " << rank << " Finished...hit Enter to exit.." << endl;
	}
	
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "rank " << rank << " elapsed time "<<elapsed_secs << endl;

	MPI_Finalize();
	
	//getchar();
	
	return 0;
}
