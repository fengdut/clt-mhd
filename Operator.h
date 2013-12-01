#ifndef _OPERATOR_H
#define _OPERATOR_h

#include"Mesh.h"
class Operator
{
public:
Operator(Mesh * pmesh);
~Operator();

void CurlEqu(double *iVR,double * iVZ, double * iVPhi, double *oVR, double * oVZ, double *oVPhi);
private:
Operator();
Mesh * m_pmesh;
};


#endif


