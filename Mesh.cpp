#include"Mesh.h"
#include<iostream>

#include"Meshconfig.h"
#include<iostream>
#include<fstream>
#include"assert.h"
#include"math.h"
#include"H5Cpp.h"

#define _USE_MATH_DEFINES

Mesh::Mesh()
{
        m_mR=1;
        m_mZ=1;
        m_mPhi=1;
        m_R=0;
        m_dR=0;
        m_Z=0;
        m_dZ=0;
        m_Phi=0;
        m_dPhi=0;
        m_uniform= true;

        int tac=0;
        char **av=0;

        getmeshconfig(tac, av,m_mR,m_mZ,m_mPhi);
        assert(m_mR>1&&m_mR<500);
        assert(m_mZ>1&&m_mZ<500);
        assert(m_mPhi>1&& m_mPhi<100);

        m_R = new double[m_mR];
        m_dR = new double[m_mR-1];

        m_Z = new double[m_mZ];
        m_dZ = new double[m_mZ-1];

        m_Phi = new double[m_mPhi];
        m_dPhi = new double[m_mPhi];

 }

int Mesh::CreateMesh(double R0, double R1, double Z0, double Z1)
{
        assert(R1>R0&&Z1>Z0);
        assert(m_uniform);

        m_R0    =       R0;
        m_R1    =       R1;
        m_Z0    =       Z0;
        m_Z1    =       Z1;
        m_Phi0  =       0;
        m_Phi1 =        2*M_PI*(m_mPhi-1)/m_mPhi;

        m_R[0]  =       R0;
        double dR       =       (R1-R0)/(m_mR-1);
        for(int i=1;i<m_mR;i++)
        {
                m_R[i]  =       R0+i*dR;
                m_dR[i-1]=      dR;
        }

        m_Z[0]  =       Z0;
        double dZ       =       (Z1-Z0)/(m_mZ-1);
        for(int i=1;i<m_mZ;i++)
        {
                m_Z[i]  =       Z0      +i*dZ;
                m_dZ[i-1]=      dZ;
        }

        double dPhi     =       2*M_PI /m_mPhi;
        m_Phi[0]=       0;
        for(int i=1;i<m_mPhi;i++)
        {
                m_Phi[i]        =       dPhi*i;
                m_dPhi[i-1]     =       dPhi;
        }
        WriteMesh();
        return 0;
}
Mesh::~Mesh()
{
        delete[] m_R;
        delete[] m_dR;
        delete[] m_Z;
        delete[] m_dZ;
        delete[] m_Phi;
        delete[] m_dPhi;
}

void Mesh::Meshinfo()
{
using namespace std;
cout<<"R grid: "<<m_mR<<", Z grid: "<<m_mZ<<", Phi grid: "<<m_mPhi<<endl;


}
void Mesh::WriteMesh()
{
        using namespace std;
        ofstream omesh;
        omesh.open("clt_mesh.dat");
        for(int i=0;i<m_mR;i++)
                for(int j=0;j<m_mZ;j++)
                {
                        omesh<<m_R[i]<<'\t'<<m_Z[j]<<endl;
                }
        omesh.close();
}


void Mesh::GetmRmZ(int &mR, int &mZ)
{
        mR=m_mR;
        mZ=m_mZ;
}

inline double DIST2(double R0, double Z0, double R1, double Z1)
{
        return (R0-R1)*(R0-R1)+(Z0-Z1)*(Z0-Z1);
}

inline int getmaxi(double *dist)
{
        int maxi=0;
        double maxd=dist[0];
        for (int i=1;i<4;i++)
        {
                maxi    =       maxd>dist[i] ? maxi:i;
                maxd    =       maxd>dist[i] ? maxd:dist[i];
        }
        return maxi;
}
void Mesh::FindNeigh(double R, double Z,double *gridR, double *gridZ, int totalgrid, int * igrid)
{
        int a[4]={0};
        double dist[4];

        dist[0]=dist[1]=dist[2]=dist[3]=DIST2(R,Z,gridR[0],gridZ[0]);
        int maxi = 0;
        double tdist;
        for(int i=1;i<totalgrid;i++)
        {
                maxi=getmaxi(dist);
                tdist=DIST2(R,Z,gridR[i],gridZ[i]);
                a[maxi] = dist[maxi]<tdist ? a[maxi] : i;
                dist[maxi]=dist[maxi]<tdist ? dist[maxi]:tdist;

        }
        for(int i=0;i<4;i++)
        {
                igrid[i]=a[i];
        }

}
void Mesh::WriteMeshHdf5(H5::H5File * pfile)
{
	using namespace H5;
	
	hsize_t	dimsf[1];
	dimsf[0]=m_mR;
	DataSpace dataspace(1,dimsf);
	IntType datatype(PredType::NATIVE_DOUBLE);

	DataSet datasetR	=	pfile->createDataSet("R",datatype,dataspace);
	
	dimsf[0]=m_mZ;
	DataSpace dataspaceZ(1,dimsf);
	DataSet datasetZ	=	pfile->createDataSet("Z",datatype,dataspaceZ);
	datasetR.write(m_R,PredType::NATIVE_DOUBLE);
	datasetZ.write(m_Z,PredType::NATIVE_DOUBLE);
	
}


