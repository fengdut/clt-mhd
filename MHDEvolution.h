#ifndef _MHDEVOLUTION_H
#define _MHDEVOLUTION_H

#include"MHDType.h"
#include"Mesh.h"
#include"H5Cpp.h"
#include"Equilibrium.h"

class MHDEvolution
{
public:
	 MHDEvolution(Equilibrium * pEqu, double dt, double gamma);
	~MHDEvolution();
	void EquTo3D();
        void WriteMHDHdf5(H5::H5File * pfile);
	void GetRHS();
private: 
	MHDEvolution();

private:

	Mesh * m_pMesh;
	MHDType m_MHD;
	Equilibrium * m_pEqu;
	double m_dt;	//time step of MHD evolution
	double m_gamma; //gamma (1/1.5)	
	double *m_RHS_rho;
	double *m_RHS_p;
	double *m_RHS_vR,*m_RHS_vZ,*m_RHS_vPhi;
	double *m_RHS_BR,*m_RHS_BZ,*m_RHS_BPhi;
	double *m_RHS_tempR, *m_RHS_tempZ,*m_RHS_tempPhi;
};



#endif


