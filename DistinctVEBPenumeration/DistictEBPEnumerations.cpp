#include "stdafx.h"
#include "VEBPEnumeration.h"




int main(int argc, char* argv[])
{

	MPI_Init(&argc, &argv);

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

		VEBP_type send;
		send.myFile = (char*)malloc(20);//"EBPFileMPI.txt";
		sprintf_s(send.myFile, 20, "EBPFileMPI_%d.txt", 1);
		//send.myFile = tempChar;
		//cout << "i " << send.myFile << endl;
		//send.myFile = "EBPFileMPI_%d.txt";
		//strcpy_s(send.myFile, 20, tempChar);
		send.myFile = "asdf.txt";//////////////////////////////no work with sprintf......try to use sstream..
		cout << "i " << send.myFile << endl;
		MPI_Send(&send, 1, mpi_vebp_type, 1, 0, MPI_COMM_WORLD);


		cout << "rank: " << rank << endl;
		//VEBPEnumeartion(mpi_vebp_type);
		cout <<"process "<<rank<< " Finished...hit Enter to exit.." << endl;
	}
	else if (rank >= 1 && rank <= 3)
	{
		cout << "rank: " << rank << endl;
		VEBP_type recv;
		
		MPI_Recv(&recv, 1, mpi_vebp_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		cout << recv.myFile << endl;

		//ofstream EBPFile;
		//EBPFile.open(recv.myFile);
		/*while (recv.firstNumb != -1)
		{
			vector<int> result;
			SetOneBitNumberOnEachSectionVEBP(recv.firstNumb, 0, result, 0, EBPFile);
			MPI_Recv(&recv, 1, mpi_vebp_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}*/
						
		//EBPFile.close();
		


		cout << "process " << rank << " Finished...hit Enter to exit.." << endl;
	}
	
	MPI_Finalize();
	
	//getchar();
	
	return 0;
}
