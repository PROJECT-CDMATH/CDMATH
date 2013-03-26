/*
 * DoubleTab.cxx
 *
 *  Created on: 22 févr. 2013
 *      Author: mekkas
 */



#include "DoubleTab.hxx"
#include <string.h>

DoubleTab::~DoubleTab()
{
	delete [] _values;

}

DoubleTab::DoubleTab()
{
	_numberOfElements=0;
	_values=NULL;
}
DoubleTab::DoubleTab(const int size)
{
	_numberOfElements=size;
	_values = new double [size];
}

DoubleTab::DoubleTab(const int size, const double initialValue)
{
	_values = new double [size];
	_numberOfElements=size;
	for (int i=0;i<size;i++)
		_values[i] = initialValue ;
}

DoubleTab::DoubleTab(const DoubleTab& dt)
{
	_numberOfElements=dt.size();
	_values = new double [_numberOfElements];
	memcpy(_values,dt.getValues(),_numberOfElements*sizeof(double)) ;
}

DoubleTab&
DoubleTab::operator=(const DoubleTab & dt)
{
	_numberOfElements=dt.size();
	_values = new double [_numberOfElements];
	memcpy(_values,dt.getValues(),_numberOfElements*sizeof(double)) ;
	return *this;
}

DoubleTab&
DoubleTab::operator=(const double value)
{
	for (int i=0;i<size();i++)
		_values[i] = value ;
	return *this;
}

double&
DoubleTab::operator[](const int i)
{
	return _values[i];
}
const double&
DoubleTab::operator[](const int i) const
{
	return _values[i];
}
int
DoubleTab::size() const
{
	return _numberOfElements;
}

double*
DoubleTab::getValues(void) const
{
	return _values;
}

double*
DoubleTab::getPointer(void)
{
	return _values;
}

DoubleTab&
DoubleTab::operator+=(const DoubleTab& dt)
{
	for (int i=0;i<dt.size();i++)
		_values[i] += dt[i] ;
	return *this;
}

DoubleTab&
DoubleTab::operator+=(const double value)
{
	for (int i=0;i<size();i++)
		_values[i] += value ;
	return *this;
}

DoubleTab&
DoubleTab::operator*=(const double value)
{
	for (int i=0;i<size();i++)
		_values[i] *= value ;
	return *this;
}

DoubleTab&
DoubleTab::operator/=(const double value)
{
	for (int i=0;i<size();i++)
		_values[i] /= value ;
	return *this;
}

DoubleTab&
DoubleTab::operator-=(const DoubleTab& dt)
{
	for (int i=0;i<dt.size();i++)
		_values[i] -= dt[i] ;
	return *this;
}

DoubleTab&
DoubleTab::operator-=(const double value)
{
	for (int i=0;i<size();i++)
		_values[i] -= value ;
	return *this;
}
