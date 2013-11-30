#include<iostream>
#include<ap.h>
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


int main()
{
	using namespace std;
	using namespace alglib;
	cout<<"Testing interpolation"<<endl;
	real_2d_array  xy;	
	idwinterpolant z;
	int n,nx;
	double r;
		
	double bb[]	=	{1,1,0.1,1,2,0.2,2,1,0.3,2,2,0.4};
	
	xy.setcontent(4,3,bb);
	n=4;
	nx=2;
	r=1;
	
	idwbuildmodifiedshepardr(xy,n,nx,r,z);	
	
	real_1d_array tx;
	double x[]={1.5,1.5};
	tx.setcontent(2,x);
	double tv=0;
	tv=idwcalc(z,tx);
	cout<<tv<<endl;	
	return 0;
}

