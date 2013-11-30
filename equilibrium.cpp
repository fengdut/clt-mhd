#include"equilibrium.h"
#include<iostream>
#include<fstream>
#include"interp.h"

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
	dBRdR	=	new double[totalgrid];
	dBRdZ	=	new double[totalgrid];
	dBZdR	=	new double[totalgrid];
	dBZdZ	=	new double[totalgrid];
	
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
	InterData2D(R,Z,BZ,totalgrid+1,m_mesh.m_R,m_mesh.m_Z,m_mR,m_mZ,m_MHDequ.BZ);	
	
	double *R1D	=	new double[nPsi];
	R1D[0]	=	xmagax;
	for(int i=1;i<nPsi;i++)
	{
		R1D[i]=R[(i-1)*ntheta];
	}
	InterData2DPsi(pPsi,pP,nPsi,m_mR,m_mZ,m_MHDequ.Psi,m_MHDequ.p);
	InterData2DPsi(pPsi,pg,nPsi,m_mR,m_mZ,m_MHDequ.Psi,m_MHDequ.BPhi);
			
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
	
	fout.setf(ios::fixed);
	fout.precision(6);	
	for(int i=0;i<m_mR;i++)
		for(int j=0;j<m_mZ;j++)
		{	
			fout<<m_mesh.m_R[i]<<'\t'<<m_mesh.m_Z[j]<<'\t'<<
				m_MHDequ.BR[i*m_mZ+j]<<'\t'<<m_MHDequ.BZ[i*m_mZ+j]<<'\t'<<
				m_MHDequ.Psi[i*m_mZ+j]<<'\t'<<m_MHDequ.p[i*m_mZ+j]<<'\t'<<m_MHDequ.BPhi[i*m_mZ+j]<<endl;
		}
	fout.clear();
}	


