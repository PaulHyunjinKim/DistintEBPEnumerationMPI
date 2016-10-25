#pragma once
void VEBPEnumeartion(MPI_Datatype mpi_vebp_type);
void SetOneBitNumberOnEachSectionVEBP(int LeftOneBitsNumber, int currentColumn, vector<int> result, int LastOneBitsNumber, ofstream &myFile);
void distinctVEBPEnumeration(vector<int> result, ofstream &myFile);
void completeEachVEBP(int sectionNumber, vector<vector<vector<int>>> VEBPSet, vector<vector<int>> VEBP, ofstream &myFile);