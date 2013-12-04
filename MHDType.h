#ifndef _MHDTYPE_H
#define _MHDTYPE_H


class MHDType
{
public:
	MHDType();
	MHDType(int NT);
	~MHDType();
	void SetNT(int NT);
public:
	int m_NT; //total grid number;
	double * rho;
	double * p;
	double * vR;
	double * vZ;
	double * vPhi;
	double * BR;
	double * BZ;
	double * BPhi;
	double * JR;
	double * JZ;
	double * JPhi;
	double * Psi;

};



#endif






