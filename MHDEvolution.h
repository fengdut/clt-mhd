#ifndef _MHDEVOLUTION_H
#define _MHDEVOLUTION_H

#include"MHDType.h"
#include"Mesh.h"
#include"H5Cpp.h"
#include"Equilibrium.h"

class MHDEvolution
{
public:
	 MHDEvolution(Equilibrium * pEqu);
	~MHDEvolution();
	void EquTo3D();
	
private: 
	MHDEvolution();

private:

	Mesh * m_pMesh;
	MHDType m_MHD;
	Equilibrium * m_pEqu;

};



#endif


