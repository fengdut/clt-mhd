#include<iostream>
#include"MHDType.h"
#include"equilibrium.h"

int main()
{
	using namespace std;
	cout<<"Test of China Large Scale Toroidal Code"<<endl;
	cout<<"Version 0.01"<<endl;
	

	Equilibrium equ;
	equ.ReadNova();
	equ.WriteEqu();
	return 0;
}



