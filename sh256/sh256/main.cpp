// sh256.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>


using std::string;
using std::cout;
using std::endl;

void hugefile_rw();
void hugefile_mmap_rw();
int main(int argc, char *argv[])
{
	string input = "SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U SUBSTREET ONE STREET JINKE BARANGAY U";
//	string output1 = sha256(input);


	hugefile_rw();
//	hugefile_mmap_rw();
//	cout << "sha256('" << input << "'):" << output1 << endl;
	return 0;
}
