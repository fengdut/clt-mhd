
#include "ap.h"
#include "alglibinternal.h"
#include "alglibmisc.h"
#include "linalg.h"
#include "statistics.h"
#include "dataanalysis.h"
#include "specialfunctions.h"
#include "solvers.h"
#include "optimization.h"
#include "diffequations.h"
#include "fasttransforms.h"
#include "integration.h"
#include "interpolation.h"

#include<iostream>
void InterData2D(const double *R, const double *Z, const double *V, const int N,
        const double *iR, const double *iZ, const int mR, const int mZ, double *iV)
{
	using namespace std;
	using namespace alglib;
	real_2d_array xy;
	idwinterpolant idw;
	xy.setlength(N,3);	
	double r=0.01;
	for(int i=0;i<N;i++)
	{
		xy(i,0)	=R[i];
		xy(i,1)	=Z[i];
		xy(i,2)	=V[i];
	}
	idwbuildmodifiedshepardr(xy,N,2,r,idw);
	
	real_1d_array tx;
	double vx[2]={0};
	for(int i=0;i<mR;i++)
	{	
		for(int j=0;j<mZ;j++)
		{
			vx[0]=iR[i];	
			vx[1]=fabs(iZ[j]);
			tx.setcontent(2,vx);
			iV[i*mZ+j]=idwcalc(idw,tx);
		}
	}
}




void InterData2DPsi(const double *psi1d,const double *v1d, const int N,
  const int mR, const int mZ, const double *psi2d,double *iV)
{
	using namespace alglib;
	real_2d_array xy;
	idwinterpolant idw;
	xy.setlength(N,2);
	double r=0.01;
	for(int i=0;i<N;i++)
	{
		xy(i,0)=psi1d[i];
		xy(i,1)=v1d[i];
	}
	idwbuildmodifiedshepardr(xy,N,1,r,idw);
	
	real_1d_array	tx;
	for(int i=0;i<mR;i++)
		for(int j=0;j<mZ;j++)
		{
			tx.setcontent(1,&psi2d[i*mZ+j]);
			iV[i*mZ+j]	=	idwcalc(idw,tx);
		}	
}


