#include"MHDEvolution.h"
#include"Operator.h"
MHDEvolution::MHDEvolution()
{
	
}

MHDEvolution::MHDEvolution(Equilibrium *pequ, double dt, double gamma)
{
	m_pEqu = pequ;
	m_pMesh= pequ->GetMesh();

	int NT	=m_pMesh->m_mR*m_pMesh->m_mZ*m_pMesh->m_mPhi;
	m_MHD.SetNT(NT);
	EquTo3D();
	m_dt	=	dt;
	m_gamma =	gamma;
	m_RHS_rho = new double[NT];
	m_RHS_p	= new double[NT];
	m_RHS_vR= new double[NT];
	m_RHS_vZ= new double[NT];
	m_RHS_vPhi=new double[NT];
	m_RHS_BR = new double[NT];
	m_RHS_BZ = new double[NT];
	m_RHS_BPhi=new double[NT];
	m_RHS_tempR=new double[NT];
	m_RHS_tempZ=new double[NT];
	m_RHS_tempPhi=new double[NT];

	memset(m_RHS_rho,0,sizeof(double)*NT);
	memset(m_RHS_p,0,sizeof(double)*NT);
	memset(m_RHS_vR,0,sizeof(double)*NT);
	memset(m_RHS_vZ,0,sizeof(double)*NT);
	memset(m_RHS_vPhi,0,sizeof(double)*NT);
	memset(m_RHS_BR,0,sizeof(double)*NT);
	memset(m_RHS_BZ,0,sizeof(double)*NT);
	memset(m_RHS_BPhi,0,sizeof(double)*NT);
	memset(m_RHS_tempR,0,sizeof(double)*NT);
	memset(m_RHS_tempZ,0,sizeof(double)*NT);
	memset(m_RHS_tempPhi,0,sizeof(double)*NT);
}

MHDEvolution::~MHDEvolution()
{
	delete[] m_RHS_rho;
	delete[] m_RHS_p;
	delete[] m_RHS_vR;
	delete[] m_RHS_vZ;
	delete[] m_RHS_vPhi;
	delete[] m_RHS_BR;
	delete[] m_RHS_BZ;
	delete[] m_RHS_BPhi;	
	delete[] m_RHS_tempR;
	delete[] m_RHS_tempZ;
	delete[] m_RHS_tempPhi;
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

void MHDEvolution::WriteMHDHdf5(H5::H5File * pfile)
{
	using namespace H5;
	hsize_t dimsf[3];
	dimsf[0]=m_pMesh->m_mPhi;
	dimsf[1]=m_pMesh->m_mR;
	dimsf[2]=m_pMesh->m_mZ;
	DataSpace dataspace(3,dimsf);
	IntType datatype(PredType::NATIVE_DOUBLE);
	
	pfile->createGroup("/Status");
	DataSet drho = pfile->createDataSet("/Status/rho",datatype,dataspace);
        DataSet dp   = pfile->createDataSet("/Status/p",datatype,dataspace);
        DataSet dBR  = pfile->createDataSet("/Status/BR",datatype,dataspace);
        DataSet dBZ  = pfile->createDataSet("/Status/BZ",datatype,dataspace);
        DataSet dBPhi= pfile->createDataSet("/Status/BPhi",datatype,dataspace);
        DataSet dJR  = pfile->createDataSet("/Status/JR",datatype,dataspace);
        DataSet dJZ  = pfile->createDataSet("/Status/JZ",datatype,dataspace);
        DataSet dJPhi= pfile->createDataSet("/Status/JPhi",datatype,dataspace);
        DataSet dpsi = pfile->createDataSet("/Status/Psi",datatype,dataspace);

	drho.write(m_MHD.rho,PredType::NATIVE_DOUBLE);
        dp.write(m_MHD.p,PredType::NATIVE_DOUBLE);
        dBR.write(m_MHD.BR,PredType::NATIVE_DOUBLE);
        dBZ.write(m_MHD.BZ,PredType::NATIVE_DOUBLE);
        dBPhi.write(m_MHD.BPhi,PredType::NATIVE_DOUBLE);
        dJR.write(m_MHD.JR,PredType::NATIVE_DOUBLE);
        dJZ.write(m_MHD.JZ,PredType::NATIVE_DOUBLE);
        dJPhi.write(m_MHD.JPhi,PredType::NATIVE_DOUBLE);
        dpsi.write(m_MHD.Psi,PredType::NATIVE_DOUBLE);
	
	
	pfile->createGroup("/RHS");
	DataSet dRHS_rho = pfile->createDataSet("/RHS/RHS_rho",datatype,dataspace);
	dRHS_rho.write(m_RHS_rho,PredType::NATIVE_DOUBLE);
	DataSet dRHS_divB= pfile->createDataSet("/RHS/RHS_divB",datatype,dataspace);
	dRHS_divB.write(m_RHS_BR,PredType::NATIVE_DOUBLE);

}

void MHDEvolution::GetRHS()
{
	double * ipV[3];
	double * opV[3];
	Operator opt(m_pMesh);
//density equation RHS: - div dot (rho v)
//m_RHS_tempR/Z/Phi=rho*vR/Z/Phi	
	ipV[0]=m_MHD.vR;
	ipV[1]=m_MHD.vZ;
	ipV[2]=m_MHD.vPhi;
	opV[0]=m_RHS_tempR;
	opV[1]=m_RHS_tempZ;
	opV[2]=m_RHS_tempPhi;

	opt.STimesV(m_MHD.rho,ipV,opV);
//m_RHS_rho=div(rho*v)
	opt.Divergence(opV,m_RHS_rho);
	opt.aTimesS(-1,m_RHS_rho,m_RHS_rho);

//test Divergence operator
	ipV[0]=m_MHD.BR;
	ipV[1]=m_MHD.BZ;
	ipV[2]=m_MHD.BPhi;
	opV[0]=m_RHS_BR;
	opV[1]=m_RHS_BZ;
	opV[2]=m_RHS_BPhi;
	opt.Divergence(ipV,m_RHS_BR);	
	
//Energy equation RHS: -

	
}



