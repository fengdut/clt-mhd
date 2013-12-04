#include"Output.h"
#include"stdlib.h"
#include<iostream>

Output::Output()
{
	m_h5file=0;
	char dfile[]="Output_CLT.h5";
	strcpy(m_h5name,dfile);
	Openhdf5();
}

Output::Output(char * hdf5name)
{	
	m_h5file=0;
	strcpy(m_h5name,hdf5name);
	Openhdf5();
}

void Output::Openhdf5()
{
	using namespace H5;	
	using namespace std;	
	m_h5file	=	new H5File(m_h5name,H5F_ACC_TRUNC);
	cout<<"Open HDF5 file:"<<m_h5name<<endl;
}

Output::~Output()
{
	if(m_h5file!=0)
	{
		m_h5file->close();
		delete m_h5file;
	}
}

H5::H5File* Output::GetH5()
{
	return m_h5file;
}

