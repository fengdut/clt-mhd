#ifndef _OUTPUT_H
#define _OUTPUT_H

#include<iostream>
#include"H5Cpp.h"

class Output
{

public:
	
	Output();
	Output(char * hdf5name);	
	~Output();
	void Openhdf5();
	H5::H5File * GetH5();
private:
	char m_h5name[100];
	H5::H5File *m_h5file;
};

#endif
