#include"Operator.h"

Operator::Operator()
{
	m_pmesh=0;
}

Operator::~Operator()
{}

Operator::Operator(Mesh * pmesh):m_pmesh(pmesh)
{
	mR          =       m_pmesh->m_mR;
        mZ          =       m_pmesh->m_mZ;
        mPhi        =       m_pmesh->m_mPhi;
        R       =       m_pmesh->m_R;
        Z       =       m_pmesh->m_Z;
        Phi     =       m_pmesh->m_Phi;
	NT=m_pmesh->m_mR*m_pmesh->m_mZ*m_pmesh->m_mPhi;
}

//
void Operator::STimesV(double *iS,double **ipV, double **opV)
{
	for(int i=0;i<NT;i++)
	{
		opV[0][i]	=	iS[i]*ipV[0][i];
		opV[1][i]	=	iS[i]*ipV[1][i];
		opV[2][i] 	 =	iS[i]*ipV[2][i];
	}
}
//Curl equ: oV = grad cross iV
//for equilibrium case, with dVdPhi=0.
void Operator::CurlEqu(double *iVR,double * iVZ, double * iVPhi, double *oVR, double * oVZ, double *oVPhi)
{
	
	for(int i=1;i<mR-1;i++)
	{
		int ia	=	i+1;
		int ib	=	i-1;
		for(int j=1;j<mZ-1;j++)
		{
			int ja	=	j+1;
			int jb	=	j-1;
			double ddz	=	Z[ja]-Z[jb];
			double ddr	=	R[ia]-R[ib];
			oVR[i*mZ+j]	=	0 -	(iVPhi[i*mZ+ja]-iVPhi[i*mZ+jb])/ddz;
			oVPhi[i*mZ+j]	=	(iVR[i*mZ+ja]-iVR[i*mZ+jb]) /ddz - (iVZ[ia*mZ+j]-iVZ[ib*mZ+j])/ddr;
			oVZ[i*mZ+j]	=	1/R[i] * (R[ia]*iVPhi[ia*mZ+j]-R[ib]*iVPhi[ib*mZ+j])/ddr	-	0;
		}
	}
	
	for(int i=1;i<mR-1;i++)
	{
		int ia	=i+1;
		int ib	=i-1;
		int j=0;
		int ja=1;
		double ddz      =       Z[ja]-Z[j];
                double ddr      =       R[ia]-R[ib];	
		oVR[i*mZ+j]     =       0 -     (iVPhi[i*mZ+ja]-iVPhi[i*mZ+j])/ddz;
                oVPhi[i*mZ+j]   =       (iVR[i*mZ+ja]-iVR[i*mZ+j])/ddz - (iVZ[ia*mZ+j]-iVZ[ib*mZ+j])/ddr;
                oVZ[i*mZ+j]     =       1/R[i]  * (R[ia]*iVPhi[ia*mZ+j]-R[ib]*iVPhi[ib*mZ+j])/ddr -       0;

		j=mZ-1;	
		int jb=j-1;
		ddz	=	Z[j]-Z[jb];
		oVR[i*mZ+j]     =       0 -     (iVPhi[i*mZ+j]-iVPhi[i*mZ+jb])/ddz;
                oVPhi[i*mZ+j]   =       (iVR[i*mZ+j]-iVR[i*mZ+jb])/ddz - (iVZ[ia*mZ+j]-iVZ[ib*mZ+j])/ddr;
                oVZ[i*mZ+j]     =       1/R[i]  * (R[ia]*iVPhi[ia*mZ+j]-R[ib]*iVPhi[ib*mZ+j])/ddr -       0;

	}
		
	for(int j=1;j<mZ-1;j++)
	{
		int ja=j+1;
		int jb=j-1;
		int i=0;
		int ia=1;
		double ddz      =       Z[ja]-Z[jb];
                double ddr      =       R[ia]-R[i];
                oVR[i*mZ+j]     =       0 -     (iVPhi[i*mZ+ja]-iVPhi[i*mZ+jb])/ddz;
                oVPhi[i*mZ+j]   =       (iVR[i*mZ+ja]-iVR[i*mZ+jb])/ddz - (iVZ[ia*mZ+j]-iVZ[i*mZ+j])/ddr;
                oVZ[i*mZ+j]     =       1/R[i]  * (R[ia]*iVPhi[ia*mZ+j]-R[i]*iVPhi[i*mZ+j])/ddr -       0;
		
		i = mR-1;
		int ib=i-1;
		ddr	=R[i]-R[ib];
		oVR[i*mZ+j]     =       0 -     (iVPhi[i*mZ+ja]-iVPhi[i*mZ+jb])/ddz;
                oVPhi[i*mZ+j]   =       (iVR[i*mZ+ja]-iVR[i*mZ+jb])/ddz - (iVZ[i*mZ+j]-iVZ[ib*mZ+j])/ddr;
                oVZ[i*mZ+j]     =       1/R[i]  * (R[i]*iVPhi[i*mZ+j]-R[ib]*iVPhi[ib*mZ+j])/ddr -       0;
	

	}	

	int i=0;
	int j=0;
	int ia=1;
	int ja=1;
	double ddz      =       Z[ja]-Z[j];
        double ddr      =       R[ia]-R[i];
	oVR[i*mZ+j]     =       0 -     (iVPhi[i*mZ+j]-iVPhi[i*mZ+ja])/ddz;
        oVPhi[i*mZ+j]   =       (iVR[i*mZ+ja]-iVR[i*mZ+j])/ddz - (iVZ[ia*mZ+j]-iVZ[i*mZ+j])/ddr;
        oVZ[i*mZ+j]     =       1/R[i]  * (R[ia]*iVPhi[ia*mZ+j]-R[i]*iVPhi[i*mZ+j])/ddr -       0;
	
	i=mR-1;
	j=0;
	int ib=i-1;
	ja=1;
	ddz      =       Z[ja]-Z[j];
        ddr      =       R[i]-R[ib];
	oVR[i*mZ+j]     =       0 -     (iVPhi[i*mZ+ja]-iVPhi[i*mZ+j])/ddz;
        oVPhi[i*mZ+j]   =       (iVR[i*mZ+ja]-iVR[i*mZ+j])/ddz - (iVZ[i*mZ+j]-iVZ[ib*mZ+j])/ddr;
        oVZ[i*mZ+j]     =       1/R[i]  * (R[i]*iVPhi[i*mZ+j]-R[ib]*iVPhi[ib*mZ+j])/ddr -       0;
	
	i=0;
	j=mZ-1;
	ia=1;
	int jb=j-1;
	ddz	=	Z[j]-Z[jb];
	ddr	=	R[ia]-R[i];
        oVR[i*mZ+j]     =       0 -     (iVPhi[i*mZ+j]-iVPhi[i*mZ+jb])/ddz;
        oVPhi[i*mZ+j]   =       (iVR[i*mZ+j]-iVR[i*mZ+jb])/ddz - (iVZ[ia*mZ+j]-iVZ[i*mZ+j])/ddr;
        oVZ[i*mZ+j]     =       1/R[i]  * (R[ia]*iVPhi[ia*mZ+j]-R[i]*iVPhi[i*mZ+j])/ddr -       0;

	i=mR-1;
	j=mZ-1;
	ib=i-1;
	jb=j-1;
        ddz     =       Z[j]-Z[jb];
        ddr     =       R[i]-R[ib];
 	oVR[i*mZ+j]     =       0 -     (iVPhi[i*mZ+j]-iVPhi[i*mZ+jb])/ddz;
        oVPhi[i*mZ+j]   =       (iVR[i*mZ+j]-iVR[i*mZ+jb])/ddz - (iVZ[i*mZ+j]-iVZ[ib*mZ+j])/ddr;
        oVZ[i*mZ+j]     =       1/R[i]  * (R[i]*iVPhi[i*mZ+j]-R[ib]*iVPhi[ib*mZ+j])/ddr -       0;
	
}

void Operator::Divergence(double **ipV, double *oV)
//div V = 1/r * d(r * Vr) / dr + 1/r *d(Vphi)/dphi + d(Vz)/dz 
{
	double * iVR=ipV[0];
	double * iVZ=ipV[1];
	double * iVPhi=ipV[2];
	int ia,ib,ja,jb,ka,kb;
	for(int i=0;i<mR;i++)
	{
		IaIb(i,mR,ia,ib);	
	for(int j=0;j<mZ;j++)
	{
		IaIb(j,mZ,ja,jb);
	for(int k=0;k<mPhi;k++)
	{
		IaIb(k,mPhi,ka,kb);
		oV[I3D(i,j,k)]	=	 1/R[i]*(R[ia]*iVR[I3D(ia,j,k)] -R[ib]*iVR[I3D(ib,j,k)])/(R[ia]-R[ib]) 
					+1/R[i]*(iVPhi[I3D(i,j,ka)]-iVPhi[I3D(i,j,kb)])/(Phi[ka]-Phi[kb])
					+(iVZ[I3D(i,ja,k)]-iVZ[I3D(i,jb,k)])/(Z[ja]-Z[jb]);	
	}
	}
	}
		
}

void Operator::ADotsB(double **piA, double **piB, double * oC)
{
	for(int i=0;i<NT;i++)
	{
		oC[i]	=	piA[0][i]*piB[0][i]	
			+	piA[1][i]*piB[1][i]
			+	piA[2][i]*piB[2][i];
	}
}

void Operator::ACrossB(double **piA, double **piB, double **piC)
{
	for(int i=0;i<NT;i++)
	{
		piC[0][i]=piA[2][i]*piB[1][i]-piA[1][i]*piB[2][i]; //C_R
		piC[1][i]=piA[0][i]*piB[2][i]-piA[2][i]*piB[0][i]; //C_Z
		piC[2][i]=piA[1][i]*piB[0][i]-piA[0][i]*piB[1][i]; //C_phi		
	}
}


void Operator::Grad(double *iA, double **poB)
{
	int ia,ib,ja,jb,ka,kb;	
	for(int i=0;i<mR;i++)
	{
	IaIb(i,mR,ia,ib);
	for(int j=0;j<mZ;j++)
	{
	IaIb(j,mZ,ja,jb);	
	for(int k=0;k<mPhi;k++)
	{
		IaIb(k,mPhi,ka,kb);	
		poB[0][I3D(i,j,k)]	=	(iA[I3D(ia,j,k)]-iA[I3D(ib,j,k)])/(R[ia]-R[ib]);
		poB[1][I3D(i,j,k)]	=	(iA[I3D(i,ja,k)]-iA[I3D(i,jb,k)])/(Z[ja]-Z[jb]);
		poB[2][I3D(i,j,k)]	=	1/R[i]*(iA[I3D(i,j,ka)]-iA[I3D(i,j,kb)])/(Phi[ka]-Phi[kb]);
	}
	}	
	}
}

void Operator::aTimesS(double a, double *iS, double *oS)
{
	for(int i=0;i<NT;i++)
	{
		oS[i]	=	a*iS[i];
	}	
}




