#ifndef _INTERPOLATION
#define _INTERPOLATION



void InterData2D(const double *R, const double *Z, const double *V, const int N, 
	const double *iR, const double *iZ, const int mR, const int mZ, double *iV);


void InterData2DPsi(const double *psi1d,const double *v1d, const int N,
 const int mR, const int mZ, const double *psi2d,double *iV);

#endif
