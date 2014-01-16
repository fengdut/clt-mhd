#include"MHDType.h"
#include<iostream>
#include<fstream>
#include"assert.h"
#include"math.h"
#define _USE_MATH_DEFINES
#include"Vector2d.h"
MHDType::MHDType()
{
	m_NT	=0;
	rho	=0;
	p	=0;
	vR	=0;
	vZ	=0;
	vPhi	=0;
	BR	=0;
	BZ	=0;
	BPhi	=0;
	JR	=0;
	JZ	=0;
	JPhi	=0;
	Psi	=0;
}

MHDType::MHDType(int NT)
{
	SetNT(NT);
}

MHDType::~MHDType()
{
	if(m_NT>0)
	{
		delete[] rho;
		delete[] p;
		delete[] vR;
		delete[] vZ;
		delete[] vPhi;
		delete[] BR;
                delete[] BZ;
                delete[] BPhi;
                delete[] JZ;
                delete[] JR;
                delete[] JPhi;
		delete[] Psi;
	}	
}

void MHDType::SetNT(int NT)
{
	 assert(NT>0&&NT<1000000);

	if(m_NT>0)
	{
		delete[] rho;
                delete[] p;
                delete[] vR;
                delete[] vZ;
                delete[] vPhi;
                delete[] BR;
                delete[] BZ;
                delete[] BPhi;
                delete[] JZ;
                delete[] JR;
                delete[] JPhi;
		delete[] Psi;
	}
	using namespace std;
	cout<<"Total grid: "<<NT<<endl;
        m_NT    =       NT;
        rho     =       new double[NT];
	
        p       =       new double[NT];
		
        vR      =       new double[NT];
        vZ      =       new double[NT];
        vPhi    =       new double[NT];
			
        BR      =       new double[NT];
        BZ      =       new double[NT];
        BPhi    =       new double[NT];
        JR      =       new double[NT];
        JZ      =       new double[NT];
        JPhi    =       new double[NT];
	Psi	=	new double[NT];	
	memset(rho,0,sizeof(double)*NT);
	memset(p,0,sizeof(double)*NT);
	memset(vR,0,sizeof(double)*NT);
	memset(vZ,0,sizeof(double)*NT);
	memset(vPhi,0,sizeof(double)*NT);
	memset(JR,0,sizeof(double)*NT);
	memset(JZ,0,sizeof(double)*NT);
	memset(JPhi,0,sizeof(double)*NT);
	memset(Psi,0,sizeof(double)*NT);
}


