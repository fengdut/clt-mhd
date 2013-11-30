#ifndef _MESH_H
#define _MESH_H

class Mesh
{
public:
        Mesh();
        ~Mesh();
        int     CreateMesh(double R0, double R1, double Z0, double Z1);
        void Meshinfo();
        void GetmRmZ(int &mR, int &mZ);
        void WriteMesh();
	void FindNeigh(double R, double Z, double *gridR, double *gridZ, int totalgrid, int *igrid);

public:
        double m_R0;
        double m_R1;
        double m_Z0;
        double m_Z1;
        double m_Phi0;
        double m_Phi1;
        int m_mR;
        int m_mZ;
        int m_mPhi;
        bool m_uniform;
//for non-uniform mesh
        double * m_R;
        double * m_dR;
        double * m_Z;
        double * m_dZ;
        double * m_Phi;
        double * m_dPhi;



};


#endif
