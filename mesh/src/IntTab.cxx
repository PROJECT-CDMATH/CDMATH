/*
 * IntTab.cxx
 *
 *  Created on: 22 févr. 2013
 *      Author: mekkas
 */



#include "IntTab.hxx"
#include <string.h>

IntTab::~IntTab()
{
	delete [] _values;
}

IntTab::IntTab()
{
	_numberOfElements=0;
	_values=NULL;
}

IntTab::IntTab(const int size)
{
	_numberOfElements=size;
	_values = new int [size];
}

IntTab::IntTab(const int size, const int initialValue)
{
	_values = new int [size];
	_numberOfElements=size;
	for (int i=0;i<size;i++)
		_values[i] = initialValue ;
}

IntTab::IntTab(const IntTab& dt)
{
	_numberOfElements=dt.size();
	_values = new int [_numberOfElements];
	memcpy(_values,dt.getValues(),_numberOfElements*sizeof(int)) ;
}

IntTab&
IntTab::operator=(const IntTab & dt)
{
	_numberOfElements=dt.size();
	_values = new int [_numberOfElements];
	memcpy(_values,dt.getValues(),_numberOfElements*sizeof(int)) ;
	return *this;
}

IntTab&
IntTab::operator=(const int value)
{
	for (int i=0;i<size();i++)
		_values[i] = value ;
	return *this;
}

int&
IntTab::operator[](const int i)
{
	return _values[i];
}
const int&
IntTab::operator[](const int i) const
{
	return _values[i];
}
int
IntTab::size() const
{
	return _numberOfElements;
}

int*
IntTab::getValues(void) const
{
	return _values;
}

IntTab&
IntTab::operator+=(const IntTab& dt)
{
	for (int i=0;i<dt.size();i++)
		_values[i] += dt[i] ;
	return *this;
}

IntTab&
IntTab::operator+=(const int value)
{
	for (int i=0;i<size();i++)
		_values[i] += value ;
	return *this;
}

IntTab&
IntTab::operator*=(const int value)
{
	for (int i=0;i<size();i++)
		_values[i] *= value ;
	return *this;
}

IntTab&
IntTab::operator/=(const int value)
{
	for (int i=0;i<size();i++)
		_values[i] /= value ;
	return *this;
}

IntTab&
IntTab::operator-=(const IntTab& dt)
{
	for (int i=0;i<dt.size();i++)
		_values[i] -= dt[i] ;
	return *this;
}
IntTab&
IntTab::operator-=(const int value)
{
	for (int i=0;i<size();i++)
		_values[i] -= value ;
	return *this;
}

int*
IntTab::getPointer(void)
{
	return _values;
}
