#ifndef _EQUILIBRIUM_
#define _EQUILIBRIUM_

#include"MHDType.h"
#include"Mesh.h"
#include"H5Cpp.h"

//2 D equilibrium
class Equilibrium
{
public:
	Equilibrium();
	~Equilibrium();

	int ReadNova();
	int ReadNovaqpg(int iPsi,int &nPsi, double *pPsi, double *pq, double *pP, double * pg);

	void WriteEqu();
	void WriteEquHdf5(H5::H5File * pfile);
	Mesh* GetMesh()
		{return &m_mesh;};
	MHDType * GetEqu()
		{return &m_MHDequ;};
private:
	double m_Raxis;
	int m_mPsi;
	int m_mTheta;
	Mesh m_mesh;
	int m_mR;
	int m_mZ;
	MHDType m_MHDequ;

};



#endif


