#include "stdafx.h"
#include "VEBPEnumeration.h"

int main()
{
	cout << "asdfasdf" << endl;
	ofstream EBPFile;
	EBPFile.open("EBPFile.txt");
	VEBPEnumeartion(EBPFile);
	EBPFile.close();
	cout << "Finished...hit Enter to exit.." << endl;
	getchar();
	return 0;
}
