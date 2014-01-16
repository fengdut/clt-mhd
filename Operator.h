#ifndef _OPERATOR_H
#define _OPERATOR_h

#include"Mesh.h"
class Operator
{
public:
	Operator(Mesh * pmesh);
	~Operator();

//C=A dot B, piA[0]=iA_R, piA[1]=iA_Z, piA[2]=iA_Phi, same for B.
	void ADotsB(double ** piA, double **piB, double *oC);
//C = A x B, piA[0]=iA_R, piA[1]=iA_Z, piA[2]=iA_Phi, same for B, C
	void ACrossB(double **piA, double **piB, double **piC);
//
	void STimesV(double *iS,double **ipV, double **opV);
	void aTimesS(double a, double *iS, double *oS);
	
	void CurlEqu(double *iVR,double * iVZ, double * iVPhi, double *oVR, double * oVZ, double *oVPhi);
	void Divergence(double **ipV, double *oV);
	void Curl();
//B=grad(A)
	void Grad(double *iA, double **poB);

	inline int I3D(int i,int j, int k)
	{
		return k*m_pmesh->m_mZ*m_pmesh->m_mR+i*m_pmesh->m_mZ+j;
	};
	
	inline void IaIb(int i,int m, int & ia, int & ib)
	{
		ia=i+1;ib=i-1;
		if(i==0)
		{
			ia=1;ib=0;
		}
		else if(i==m-1)
		{
			ia=i;
			ib=i-1;
		}
	}
private:
	Operator();
	Mesh * m_pmesh;
	int mR;
        int mZ;
        int mPhi;
	int NT;
        double *R;
        double *Z;
        double *Phi;

};


#endif


