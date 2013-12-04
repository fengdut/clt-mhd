#include"Operator.h"

Operator::Operator()
{
	m_pmesh=0;
}

Operator::~Operator()
{}

Operator::Operator(Mesh * pmesh):m_pmesh(pmesh)
{
}

//for equilibrium case, with dVdPhi=0.
void Operator::CurlEqu(double *iVR,double * iVZ, double * iVPhi, double *oVR, double * oVZ, double *oVPhi)
{
	double *R	=	m_pmesh->m_R;
	double *Z	=	m_pmesh->m_Z;
//	double *dR	=	m_pmesh->m_dR;
//	double *dZ	=	m_pmesh->m_dZ;
	int mR		=	m_pmesh->m_mR;
	int mZ		=	m_pmesh->m_mZ;
	
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

