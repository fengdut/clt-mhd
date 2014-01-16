#include<iostream>
#include"MHDType.h"
#include"Equilibrium.h"
#include"Output.h"
#include"MHDEvolution.h"


int main()
{
	using namespace std;
	cout<<"Test of China Large Scale Toroidal Code"<<endl;
	cout<<"Version 0.01"<<endl;
	
	Output output;
	Equilibrium equ;
	equ.ReadNova();
	equ.WriteEquHdf5(output.GetH5());
	equ.WriteEqu();

	double dt=0.01;
	double gamma=1.5;
	MHDEvolution mhd(&equ,dt,gamma);
	mhd.GetRHS();
	mhd.WriteMHDHdf5(output.GetH5());
	return 0;
}



