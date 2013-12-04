#include<iostream>
#include"MHDType.h"
#include"Equilibrium.h"
#include"Output.h"

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
	return 0;
}



