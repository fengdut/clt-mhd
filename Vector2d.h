
#ifndef _VECTOR2D_H
#define _VECTOR2D_H

#include"assert.h"
#include<iostream>

//#include"MatrixCompute.h"
#define INIT_MEMORY 129


template <typename _F>
void _MatrixMultiple(_F** C,_F **A,_F** B,int n)
{
	int i,j;
	_F x=0;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			x	=	0;
			for(int k=0;k<n;k++)
			{
				x	+=	A[i][k]*B[k][j];
			}
			C[i][j]	=	x;
		}
	}
}

template <typename T>
void _MatrixMultipleVector(T *v,T **A, T *h,int n)
{
	int i,j;
	T s	=0	;
	for(i=0;i<n;i++)
	{
		s	=	0;
		for(j=0;j<n;j++)
		{
			s	+=	A[i][j]*h[j];
		}
		v[i]	=	s;
	}
}

template <typename T>
class MyVector2D;

template <typename T>
class MyVector1D
{
public:
	MyVector1D():
		m_Length(0),
		m_Vector(0)
	{
		
	}
		MyVector1D(int n):
	m_Length(n)
		{
			m_Vector	=	new T[m_Length];
			//memset(m_Vector,0,sizeof(T)*m_Length);
		}
	~MyVector1D()
	{
		if(m_Vector!=0)
			delete[] m_Vector;
	}
	 T & operator[](int x)
	 {
		 return m_Vector[x];
	 }
	 MyVector1D<T>&	operator =(const MyVector1D<T> &a)
	 {
		 if(m_Length!=a.m_Length)
		 {
			 m_Length	=	a.m_Length;
			 GrowLength(a.m_Length);
		 }
		 memcpy(m_Vector,a.m_Vector,sizeof(T)*m_Length);
		 return *this;
	 }
	 MyVector1D<T> & operator +=(MyVector1D<T>& a)
	 {
		 assert(m_Length==a.m_Length);
		 T* p	=	a.m_Vector;
		 for(int i=0;i<m_Length;i++)
			 m_Vector[i]	+=	p[i];
		 return *this;
	 }
	 MyVector1D<T> & operator +=(T & a)
	 {
		 for(int i=0;i<m_Length;i++)
			 m_Vector[i]	+=a;
		 return *this;
	 }
	 MyVector1D<T> & operator -=(T &a)
	 {
		 for(int i=0;i<m_Length;i++)
			 m_Vector[i]	-=a;
		 return *this;
	 }
	  MyVector1D<T> & operator =(T & a)
	 {
		 for(int i=0;i<m_Length;i++)
			 m_Vector[i]	=a;
		 return *this;
	 }
	  MyVector1D<T> & operator -=(MyVector1D<T>& a)
	 {
		 T * p	=	a.m_Vector;
		 assert(m_Length==a.m_Length);
		 for(int i=0;i<m_Length;i++)
			 m_Vector[i]	-=	p[i];
		 return *this;
	 }

	 MyVector1D<T> & operator *=(MyVector2D<T>& a)
	 {
		 static MyVector1D<T> tv;
		 assert(m_Length==a.m_dimRow);
		 tv	=	*this;
		_MatrixMultipleVector(m_Vector,a.m_2DVector,tv.m_Vector,m_Length);
		return * this;
	 }
	 MyVector1D<T> & operator *(MyVector2D<T>& a)
	 {
		 static MyVector1D<T> tv;
		 assert(m_Length==a.m_dimRow);
		 tv	=	*this;
			_MatrixMultipleVector(m_Vector,a.m_2DVector,tv.m_Vector,m_Length);
			return * this;
	 }
	 MyVector1D<T> & operator *=(T a)
	 {
			for(int i=0;i<m_Length;i++)
			 m_Vector[i]	*=	a;

			return * this;
	 }

	 void GrowLength(int n)
	 {
		 if(m_Length!=0)
			 delete m_Vector;
		 m_Vector	=	new T[n];
		 m_Length	=	n;
	 }
	 int GetLength()
	 {
		 return m_Length;
	 }
	 int size()
	 {
		 return m_Length;
	 }
	 T* GetVector()
	 {
		 return m_Vector;
	 }
	 void DataOut(std::ostream & out)
	{
		for(int i=0;i<m_Length;i++)
		{
			out<<m_Vector[i]<<"\t";
		}
		out<<std::endl;
	}
private:
	int m_Length;
	T* m_Vector;
};


template <class T>
class MyVector2D
{

public:
   MyVector2D()
   :m_dimRow(0),
    m_dimCol(0),
	m_2DVector(NULL)
    {
		
	}
   friend class MyVector1D<T>;
   MyVector2D(int nRow, int nCol):
	  m_dimRow(nRow),
	  m_dimCol(nCol)
   {
		m_2DVector =	 new (T*[nRow]);
		m_2DVector[0]	=	new T[nRow*nCol];
		for (int i=1; i < nRow; i++)
		{
			m_2DVector[i] =	 m_2DVector[i-1]+nCol;
		}
   }
   
   MyVector2D(int nRow, int nCol, T * data):
	m_dimRow(nRow),m_dimCol(nCol)
   {
	 m_2DVector =     new (T*[nRow]);
                m_2DVector[0]   =       new T[nRow*nCol];
                for (int i=1; i < nRow; i++)
                {
                        m_2DVector[i] =  m_2DVector[i-1]+nCol;
                }
	memcpy(m_2DVector[0],data,sizeof(T)*m_dimCol*m_dimRow);

   }
   ~MyVector2D()
	{
		  FreeM();
	}
	MyVector2D(MyVector2D& v)
	{
		m_2DVector=NULL;
		m_dimRow	=	v.GetRowNo();
		m_dimCol	=	v.GetColNo();
		GrowRC(m_dimRow,m_dimCol);
		for(int i=0;i<m_dimRow;i++)
		{
			for(int j=0;j<m_dimCol;j++)
				m_2DVector[i][j]	=v[i][j];
		}
	
	}
   void SetAt(int nRow, int nCol, const T& value) //throw(std::out_of_range)
   {
     // if(nRow >= m_dimRow || nCol >= m_dimCol)
       //  throw out_of_range("Array out of bound");
      //else
         m_2DVector[nRow][nCol] = value;
   }

   void FreeM()
   {
		if(m_2DVector!=NULL)
		{
			delete[] m_2DVector[0];
			delete[] m_2DVector;
			m_2DVector	=	NULL;
		}
		m_dimRow	=	0;
		m_dimCol	=	0;

   }
   void GrowRC(int NoR,int NoC)
   {
		FreeM();
		m_dimRow	=	NoR;
		m_dimCol	=	NoC;

		m_2DVector	=	new(T*[m_dimRow]);
		m_2DVector[0]	=	new T[m_dimRow*m_dimCol];
		for(int i = 1; i <   m_dimRow; i++)
		{ 
			m_2DVector[i]	=	m_2DVector[i-1]+m_dimCol;
		}
	
   }
   T* & operator[](int x)
   {
      return m_2DVector[x];
   }

 MyVector2D<T>&	operator =(const MyVector2D<T> &a)
	{
		if(m_dimRow!=a.m_dimRow||m_dimCol!=a.m_dimCol)
		{
			GrowRC(a.m_dimRow,a.m_dimCol);
		}

		memcpy(m_2DVector[0],a.m_2DVector[0],sizeof(T)*m_dimCol*m_dimRow);
		
		return *this;
	}
 MyVector2D<T> & operator +=(MyVector2D<T>& a)
 {
	assert(a.m_dimCol==m_dimCol&&a.m_dimRow==m_dimCol);
	T *pa,*pb;
	for(int i=0;i<m_dimRow;i++)
	{
		pa	=	m_2DVector[i];
		pb	=	a.m_2DVector[i];
		for(int j=0;j<m_dimCol;j++)
			pa[j]	+=	pb[j];
	}
	return *this;
 }

 void DiagonalAdd(T &a)
 {
	 assert(m_dimCol==m_dimRow);
	for(int i=0;i<m_dimRow;i++)
			m_2DVector[i][i]	+=a;
 }
 void SetI()
 {	
	assert(m_dimCol==m_dimRow);
	for(int i=0;i<m_dimRow;i++)
			m_2DVector[i][i]	=	1;
 }
 MyVector2D<T> & operator -=(MyVector2D<T>& a)
 {
	assert(a.m_dimCol==m_dimCol&&a.m_dimRow==m_dimCol);
	for(int i=0;i<m_dimRow;i++)
		for(int j=0;j<m_dimCol;j++)
			m_2DVector[i][j]	-=	a[i][j];
	return *this;
 }
  MyVector2D<T> & operator *=(T a)
 {
	for(int i=0;i<m_dimRow;i++)
		for(int j=0;j<m_dimCol;j++)
			m_2DVector[i][j]	*=	a;
	return *this;
 }

 MyVector2D<T> & operator *=(const MyVector2D<T>& a)
 {
	assert(a.m_dimCol==m_dimCol&&a.m_dimRow==m_dimCol);
	static MyVector2D<T>	ta;
	static MyVector2D<T>	tb;
	ta	=	*this;
	tb	=	a;
	_MatrixMultiple<T>(this->m_2DVector,ta.m_2DVector,tb.m_2DVector,m_dimCol);
	return *this;
 }
  void Plus(MyVector2D& a,MyVector2D &b)
  {
	  for(int i=0;i<m_dimRow;i++)
		{
			for(int j=0;j<m_dimCol;j++)
			m_2DVector[i][j]	=a[i][j]+b[i][j];
		}
  }
  void Minus(MyVector2D& a,MyVector2D &b)
  {
	for(int i=0;i<m_dimRow;i++)
		{
			for(int j=0;j<m_dimCol;j++)
			m_2DVector[i][j]	=a[i][j]-b[i][j];
		}
  }
   bool CopyToArray(T** u,int n,int m)
   {
	   if(n!=m_dimRow||m!=m_dimCol)
		   return false;
	   else
	   {
			for(int i=0;i<m_dimRow;i++)
				for(int j=0;j<m_dimCol;j++)	
					u[i][j]	=	m_2DVector[i][j];
			return true;
	   }
   }
   bool CopyFromArray(T ** u, int n,int m)
   {
		if(n!=m_dimRow||m!=m_dimCol)
		   return false;
	   else
	   {
			for(int i=0;i<m_dimRow;i++)
				for(int j=0;j<m_dimCol;j++)	
					m_2DVector[i][j]	=u[i][j];
			return true;
	   }
   }
   void SetZero()
   {
		memset(m_2DVector[0],0,sizeof(T)*m_dimCol*m_dimRow);
   }
	int GetRowNo()
	{
		return m_dimRow;
	}
	int GetColNo()
	{
		return m_dimCol;
	}

	void DataOut(std::ostream & out)
	{

		out.precision(8);
		for(int i=0;i<m_dimRow;i++)
		{
			
			for(int j=0;j<m_dimCol;j++)
				out<<std::left<<m_2DVector[i][j]<<",\t";
			out<<std::endl;
			
		}

	
	}

	T** &Get2DVector()
	{
		return m_2DVector;
	}

	int m_dimRow;
	int m_dimCol;
	T** m_2DVector;


};


#endif

