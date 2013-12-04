#include"Equilibrium.h"
#include<iostream>
#include<fstream>
#include"Interp.h"
#include"Operator.h"
#include"H5Cpp.h"
#include"Vector2d.h"

Equilibrium::Equilibrium()
{
	m_mesh.GetmRmZ(m_mR,m_mZ);
	int NT=m_mR*m_mZ;
	m_MHDequ.SetNT(NT);	
}

Equilibrium::~Equilibrium()
{
	
}

int Equilibrium::ReadNova()
{
	using namespace std;
	char nova_psi_xz[]="psi_xz.dat";
	ifstream psixz(nova_psi_xz,ios::in);
	if(psixz.fail())
	{
		cerr<<"can not find file:"<<nova_psi_xz<<endl;
		return 1;
	}
	string tmpstr;
	char buf[500];
	cout<<"Reading "<<nova_psi_xz<<endl;
	psixz.getline(buf,500,'\n');
	double xplmin, xplmax, zplmax, arad, x_zero, xmagax, xmaj, xzmax, xatpi, xofset, a_ratio;
	/*xplmin: Rmin
	xplmax: Rmax
	zplmax: Zmax
	arad: a
	x_zero: R0
	xmagax: R(maxis) magnetic axis
	xmaj: ~R0
	xzmax: R(Zmax)
	xatpi: R(theta=Pi)
	xofset: axis ofset of magnetic axis
	a_ratio: R0/a		*/
	
	psixz>>xplmin>>xplmax>>zplmax>>arad>>x_zero>>xmagax>>xmaj>>xzmax>>xatpi>>xofset>>a_ratio;
	m_Raxis	=	xmagax;	
	cout<<"Rmin\t"<<"Rmax\t"<<"Zmax\t"<<"a\t"<<"R0\t"<<"Raxis\t"<<"R(Zmax)\t"
		<<"R(theta=Pi)\t"<<"xofset\t"<<"R0/a"<<endl;	
	cout<<xplmin<<'\t'<<xplmax<<'\t'<<zplmax<<'\t'<<arad<<'\t'<<x_zero<<'\t'<<xmagax<<'\t'<<xmaj<<'\t'<<xzmax
		<<'\t'<<xatpi<<'\t'<<xofset<<'\t'<<a_ratio<<endl;
	m_mesh.CreateMesh(xplmin,xplmax,-1*zplmax,zplmax);
	m_mesh.Meshinfo();
	int ipsi,itheta;
	int ipsi0,ipsi1,itheta0,itheta1;
	double tpsi,tR,tZ,tBR,tdBRdR,tdBRdZ,tBZ,tdBZdR,tdBZdZ;
	ipsi0=ipsi1=2;
	itheta0=itheta1=3;
	while(!psixz.eof())
	{
		psixz>>ipsi>>itheta>>tpsi>>tR>>tZ>>
			tBR>>tdBRdR>>tdBRdZ>>tBZ>>tdBZdR>>tdBZdZ;
		ipsi1 = ipsi>ipsi1 ? ipsi:ipsi1;
		itheta1 = itheta>itheta1 ? itheta:itheta1;
	}
	int npsi=ipsi1-ipsi0+1;
	int ntheta=itheta1-itheta0+1;
	int totalgrid=npsi*ntheta;	
	cout<<"psi grid: "<<npsi<<", theta grid: "<<ntheta<<", total grid: "<<totalgrid<<endl;
 
	double * psi, * R, * Z, * BR, * dBRdR, * dBRdZ, *BZ, *dBZdR, *dBZdZ;
 	psi	=	new double[totalgrid+1]; //"+1" for the axis point
	R	=	new double[totalgrid+1];
	Z	=	new double[totalgrid+1];
	BR	=	new double[totalgrid+1];
	BZ	=	new double[totalgrid+1];
	dBRdR	=	new double[totalgrid+1];
	dBRdZ	=	new double[totalgrid+1];
	dBZdR	=	new double[totalgrid+1];
	dBZdZ	=	new double[totalgrid+1];
	
	psixz.clear();	
	psixz.seekg(0,psixz.beg);
	psixz.getline(buf,500,'\n');
	psixz.getline(buf,500,'\n');
	for(int i=0;i<totalgrid;i++)
	{
		psixz>>ipsi>>itheta>>psi[i]>>R[i]>>Z[i]>>BR[i]>>dBRdR[i]>>dBRdZ[i]>>
			BZ[i]>>dBZdR[i]>>dBZdZ[i];				
	}		
	psixz.close();
	
	int nPsi;
	double *pPsi,*pq,*pP,*pg;
	pPsi	=	new double[ipsi1];
	pq	=	new double[ipsi1];
	pP	=	new double[ipsi1];
	pg	=	new double[ipsi1];
	if(ReadNovaqpg(ipsi1,nPsi,pPsi,pq,pP,pg))
	{
		cerr<<"read qpg error"<<endl;	
	}
	//axis point
	psi[totalgrid]	=pPsi[0];
	R[totalgrid]	=xmagax;
	Z[totalgrid]	=0;
	BR[totalgrid]	=0;
	BZ[totalgrid]	=0;
	
        InterData2D(R,Z,psi,totalgrid+1,m_mesh.m_R,m_mesh.m_Z,m_mR,m_mZ,m_MHDequ.Psi);
	InterData2D(R,Z,BR,totalgrid+1,m_mesh.m_R,m_mesh.m_Z,m_mR,m_mZ,m_MHDequ.BR); 
	for(int i=0;i<m_mR;i++)
	for(int j=0;j<m_mZ/2;j++)
		m_MHDequ.BR[i*m_mZ+j]=m_MHDequ.BR[i*m_mZ+j]*-1;
	InterData2D(R,Z,BZ,totalgrid+1,m_mesh.m_R,m_mesh.m_Z,m_mR,m_mZ,m_MHDequ.BZ);	
	
	double *R1D	=	new double[nPsi];
	R1D[0]	=	xmagax;
	for(int i=1;i<nPsi;i++)
	{
		R1D[i]=R[(i-1)*ntheta];
	}
	InterData2DPsi(pPsi,pP,nPsi,m_mR,m_mZ,m_MHDequ.Psi,m_MHDequ.p);
	InterData2DPsi(pPsi,pg,nPsi,m_mR,m_mZ,m_MHDequ.Psi,m_MHDequ.BPhi);
	//g to Bphi
	for(int i=0;i<m_mR;i++)
	for(int j=0;j<m_mZ;j++)
	{
		m_MHDequ.BPhi[i*m_mZ+j]=m_MHDequ.BPhi[i*m_mZ+j]	*	x_zero/m_mesh.m_R[i];
	}		
	Operator opt(&m_mesh);	
	opt.CurlEqu(m_MHDequ.BR,m_MHDequ.BZ,m_MHDequ.BPhi,m_MHDequ.JR,m_MHDequ.JZ,m_MHDequ.JPhi);	

	double * griddBRdZ	=	new double[m_mR*m_mZ];
	double * griddBZdR	=	new double[m_mR*m_mZ];
	InterData2D(R,Z,dBRdZ,totalgrid,m_mesh.m_R,m_mesh.m_Z,m_mR,m_mZ,griddBRdZ);
 	InterData2D(R,Z,dBZdR,totalgrid,m_mesh.m_R,m_mesh.m_Z,m_mR,m_mZ,griddBZdR);

	for(int i=0;i<m_mR;i++)
        for(int j=0;j<m_mZ;j++)
        {
                m_MHDequ.JPhi[i*m_mZ+j]= griddBRdZ[i*m_mZ+j]-griddBZdR[i*m_mZ+j];
        }

	delete[] R1D;
	delete[] pPsi;
	delete[] pq;
	delete[] pP;
	delete[] pg;
	delete[] psi;
	delete[] R;
	delete[] Z;
	delete[] BR;
	delete[] dBRdR;
	delete[] dBRdZ;
	delete[] BZ;
	delete[] dBZdR;
	delete[] dBZdZ;
	return 0;
}
int Equilibrium::ReadNovaqpg(int iPsi,int & nPsi, double *pPsi, double *pq, double *pP, double * pg)
{
	using namespace std;
	char q_p_g[]="q_p_g.dat";
        ifstream        qpg(q_p_g,ios::in);
        if(qpg.fail())
        {
                cerr<<"can not find file:"<<q_p_g<<endl;
                return 1;
        }
	//Psi: poloidal flux, q: saftly factor, P: pressure, g: R*B_phi/R0, f:??, fb:??
	double Psi, q, dqdPsi, P, dPdPsi, g, dgdPsi, f, dfdPsi, fb, fbdPsi;
	
	while(!qpg.eof())
	{
		qpg>>nPsi>>Psi>>q>>dqdPsi>>P>>dPdPsi>>g>>dgdPsi>>f>>dfdPsi>>fb>>fbdPsi;		
	}
	if(iPsi!=nPsi)
	{
		cerr<<"iPsi:"<<iPsi<<" nPsi:"<<nPsi<<endl;
		cerr<<q_p_g<<" data do not correspond to the psi_xz file"<<endl;
		return 1;
	}
	qpg.clear();
	qpg.seekg(0,qpg.beg);
	
	for(int i=0;i<iPsi;i++)	
        {
                qpg>>nPsi>>pPsi[i]>>pq[i]>>dqdPsi>>pP[i]>>dPdPsi>>pg[i]>>dgdPsi>>f>>dfdPsi>>fb>>fbdPsi;
        }
	return 0;
}
void Equilibrium::WriteEqu()
{
	using namespace std;
	ofstream fout("R_Z_BR_Psi.dat");
	
	MyVector2D<double> BPhi(m_mR,m_mZ,m_MHDequ.BPhi);
	
	fout.setf(ios::fixed);
	fout.precision(6);	
	for(int i=0;i<m_mR;i++)
		for(int j=0;j<m_mZ;j++)
		{	
			fout<<m_mesh.m_R[i]<<'\t'<<m_mesh.m_Z[j]<<'\t'<<
				m_MHDequ.BR[i*m_mZ+j]<<'\t'<<m_MHDequ.BZ[i*m_mZ+j]<<'\t'<<
				m_MHDequ.Psi[i*m_mZ+j]<<'\t'<<m_MHDequ.p[i*m_mZ+j]<<'\t'<<m_MHDequ.BPhi[i*m_mZ+j]<<
				'\t'<<BPhi[i][j]<<endl;
		}
	fout.clear();
	
}	

void Equilibrium::WriteEquHdf5(H5::H5File *pfile)
{	
	m_mesh.WriteMeshHdf5(pfile);
	using namespace H5;
	
	hsize_t dimsf[2];
	MyVector2D<double> Psi(m_mR,m_mZ,m_MHDequ.Psi);

	dimsf[0]=m_mR;
	dimsf[1]=m_mZ;
	DataSpace dataspace(2,dimsf);
	IntType datatype(PredType::NATIVE_DOUBLE);

	DataSet drho = pfile->createDataSet("rho",datatype,dataspace);
	DataSet dp   = pfile->createDataSet("p",datatype,dataspace);
	DataSet dBR  = pfile->createDataSet("BR",datatype,dataspace);
	DataSet dBZ  = pfile->createDataSet("BZ",datatype,dataspace);
	DataSet dBPhi= pfile->createDataSet("BPhi",datatype,dataspace);
	DataSet dJR  = pfile->createDataSet("JR",datatype,dataspace);
	DataSet dJZ  = pfile->createDataSet("JZ",datatype,dataspace);
	DataSet dJPhi= pfile->createDataSet("JPhi",datatype,dataspace);
	DataSet dpsi = pfile->createDataSet("Psi",datatype,dataspace);

	drho.write(m_MHDequ.rho,PredType::NATIVE_DOUBLE);
	dp.write(m_MHDequ.p,PredType::NATIVE_DOUBLE);
	dBR.write(m_MHDequ.BR,PredType::NATIVE_DOUBLE);
	dBZ.write(m_MHDequ.BZ,PredType::NATIVE_DOUBLE);
	dBPhi.write(m_MHDequ.BPhi,PredType::NATIVE_DOUBLE);
	dJR.write(m_MHDequ.JR,PredType::NATIVE_DOUBLE);
	dJZ.write(m_MHDequ.JZ,PredType::NATIVE_DOUBLE);
	dJPhi.write(m_MHDequ.JPhi,PredType::NATIVE_DOUBLE);
	dpsi.write(m_MHDequ.Psi,PredType::NATIVE_DOUBLE);		
		
}


