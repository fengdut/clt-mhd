#ifndef _EQUILIBRIUM_
#define _EQUILIBRIUM_

#include"MHDType.h"

class Equilibrium
{
public:
Equilibrium();
~Equilibrium();

private:
double m_a;
double m_R0;
double m_Rx;

double m_Zmax;
double m_Rmax;
double m_Rmin;

MHDType m_MHDequ;

};



#endif


