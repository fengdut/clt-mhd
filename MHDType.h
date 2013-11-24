#ifndef _MHDTYPE_H
#define _MHDTYPE_H

struct MeshType
{
	int mR;
	int mZ;
	int mPhi;
	double * R;
	double * dR;
	double * Z;
	double * dZ;
	double * Phi;
	double * zPhi;
};

struct MHDType
{
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
};


class Mesh
{
public:
	Mesh();
	Mesh(char * filename);
	~Mesh();
private:
MeshType m_meshdata;
	
};

#endif






