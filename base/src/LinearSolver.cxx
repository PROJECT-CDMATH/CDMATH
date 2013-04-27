/*
 * LinearSolver.cxx
 *
 *  Created on: 15 avr. 2013
 *      Author: mekkas
 */

#include "LinearSolver.hxx"

#include <string>
#include <cmath>

using namespace std;

LinearSolver::LinearSolver ( void )
{
	_tol=1.E-15;
	_numberMaxOfIter=0;
	_name="";
	_residu=1.E30;
	_convergence=false;
	_numberOfIter=0;
}

LinearSolver::LinearSolver( const Matrix& matrix, const Vector& vector, int numberMaxOfIter, double tol, std::string method  )
{
	double det=matrix.determinant();
	if (abs(det)<1.E-8)
		throw "Matrix is not invertible!!!";

	_tol=tol;
	_name=method;
	_numberMaxOfIter=numberMaxOfIter;
	_matrix=matrix;
	_vector=vector;
	_residu=1.E30;
	_convergence=false;
	_numberOfIter=0;
}

int
LinearSolver::getNumberOfIter( void ) const
{
	return _numberOfIter;
}

bool
LinearSolver::getStatus( void ) const
{
	return _convergence;
}

double
LinearSolver::getResidu( void ) const
{
	return _residu;
}

double
LinearSolver::getTolerance(void) const
{
	return _tol;
}

int
LinearSolver::getNumberMaxOfIter(void) const
{
	return _numberMaxOfIter;
}

Matrix
LinearSolver::getMatrix(void) const
{
	return _matrix;
}

Vector
LinearSolver::getSndMember(void) const
{
	return _vector;
}

string
LinearSolver::getName(void) const
{
	return _name;
}

LinearSolver::LinearSolver ( const LinearSolver& LS )
{
	_tol=LS.getTolerance();
	_name=LS.getName();
	_numberMaxOfIter=LS.getNumberMaxOfIter();
	_matrix=LS.getMatrix();
	_vector=LS.getSndMember();
	_residu=LS.getResidu();
	_convergence=LS.getStatus();
	_numberOfIter=LS.getNumberOfIter();
}

Vector
LinearSolver::solve( void )
{
	Vector X;
	if (_name.compare("CG")==0)
		X=conjugateGradient();
	else if (_name.compare("BCG")==0)
		X=Bicgstab();
	else
		throw "Method is not implemented: "+_name;

	return X;
}

Vector
LinearSolver::conjugateGradient( void )
{
	_numberOfIter = 0;
	_convergence = false;
	Vector b=getSndMember();
	Matrix A=getMatrix();
	Vector X(b.getNumberOfRows());
	Vector r(b.getNumberOfRows());
	r = b - A*X;
	Vector p = r;
	double rsold=r*r;

	while (_numberOfIter < _numberMaxOfIter)
	{
		Vector AP = A*p;
		double alpha = rsold / (p*AP);
		X = X + alpha*p;
		r = r - alpha*AP;
		double rsnew=r*r;
	 	_residu=r.norm();
	   	if ( _residu <= _tol )
		{
	   		_convergence=true;
			return X;
		}
		p = r + (rsnew/rsold)*p;
		rsold=rsnew;
		_numberOfIter++;
	}
	return X;
}


Vector
LinearSolver::Bicgstab( void )
{
	_numberOfIter = 0;
	_convergence = false;
	Vector b=getSndMember();
	Matrix A=getMatrix();
	double Normb = b.norm();

   	if( Normb == 0.0 )
		Normb = 1.0;
   	Vector X(b.getNumberOfRows());
	Vector P(b.getNumberOfRows());
	Vector V(b.getNumberOfRows());
	Vector S(b.getNumberOfRows());
	Vector T(b.getNumberOfRows());
	Vector VecResidu(b.getNumberOfRows());

   	Vector P_AX = A*X;
   	VecResidu = b - P_AX;
   	Vector R_Tld = VecResidu;

	double NormR = VecResidu.norm();

	_residu = NormR / Normb;
   	if ( _residu <= _tol )
	{
   		_convergence=true;
		return X;
	}

	double Omega = 1.0;
	double Alpha;
	for(_numberOfIter=1;_numberOfIter<_numberMaxOfIter;_numberOfIter++)
	{
		double Rho = R_Tld*VecResidu;
		double Rho_1;
		if(Rho == 0)
			break;
		if(_numberOfIter>1)
		{
			double Beta  = (Rho/Rho_1)*(Alpha/Omega);
			P = VecResidu + Beta*(P-Omega*V);
		}else
			P=VecResidu;

		V=A*P;
		Alpha = Rho/(R_Tld*V);
		S = VecResidu-Alpha*V;

		double NormS = S.norm();

		if(NormS < _tol)
		{
			X = X + Alpha*P;
			_residu = NormS/Normb;
			break;
		}

		T=A*S;
		Omega = T*S/(T*T);
		X = X + Alpha*P + Omega*S;
		VecResidu = S - Omega*T;

		NormR = VecResidu.norm();

		_residu = NormR/Normb;

		if(_residu <= _tol)
			break;
	    if(Omega == 0.0)
			 break;
		Rho_1 = Rho;
	}
	if (_residu <= _tol)
		_convergence = 0;
	return X;
}
