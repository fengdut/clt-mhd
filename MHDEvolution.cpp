#include"MHDEvolution.h"

MHDEvolution::MHDEvolution()
{
	
}

MHDEvolution::~MHDEvolution()
{
	
}

MHDEvolution::MHDEvolution(Equilibrium *pequ)
{
	m_pEqu = pequ;
	m_pMesh= pequ->GetMesh();

	int NT	=m_pMesh->m_mR*m_pMesh->m_mZ*m_pMesh->m_mPhi;
	m_MHD.SetNT(NT);
}

void MHDEvolution::EquTo3D()
{
	MHDType *pmhdequ=m_pEqu->GetEqu();
	int MRMZ = m_pMesh->m_mR*m_pMesh->m_mZ;
	
	int MRMZs=sizeof(double)*MRMZ;
	for(int i=0;i<m_pMesh->m_mPhi;i++)
	{
		memcpy(m_MHD.rho+i*MRMZ,pmhdequ->rho,MRMZs);
		memcpy(m_MHD.p+i*MRMZ,pmhdequ->p,MRMZs);
		memcpy(m_MHD.BR+i*MRMZ,pmhdequ->BR,MRMZs);
		memcpy(m_MHD.BZ+i*MRMZ,pmhdequ->BZ,MRMZs);
		memcpy(m_MHD.BPhi+i*MRMZ,pmhdequ->BPhi,MRMZs);
		memcpy(m_MHD.JR+i*MRMZ,pmhdequ->JR,MRMZs);	
		memcpy(m_MHD.JZ+i*MRMZ,pmhdequ->JZ,MRMZs);
		memcpy(m_MHD.JPhi+i*MRMZ,pmhdequ->JPhi,MRMZs);
		memcpy(m_MHD.Psi+i*MRMZ,pmhdequ->Psi,MRMZs);
	}			
}




