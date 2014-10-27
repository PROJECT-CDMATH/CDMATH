/*
 * IntTab.cxx
 *
 *  Created on: 22 févr. 2013
 *      Author: mekkas
 */



#include "IntTab.hxx"

#include <string.h>

using namespace std;

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

void
IntTab::resize(const int size)
{
	int* oldvalues=new int [_numberOfElements] ;
	int oldsize=_numberOfElements;
	copy(_values,_values+oldsize,oldvalues);
	delete [] _values;

	_numberOfElements+=size;
	_values = new int [_numberOfElements] ;
	for (int i=0;i<oldsize;i++)
		_values[i] = oldvalues[i] ;
	for (int i=oldsize;i<_numberOfElements;i++)
		_values[i] = 0 ;
	delete [] oldvalues;
}

IntTab&
IntTab::operator=(const IntTab & dt)
{
	_numberOfElements=dt.size();
    if (_values)
        delete [] _values ;
	_values = new int [_numberOfElements];
	memcpy(_values,dt.getValues(),_numberOfElements*sizeof(int)) ;
	return *this;
}

IntTab&
IntTab::operator=(const int value)
{
	for (int i=0;i<_numberOfElements;i++)
		_values[i] = value ;
	return *this;
}

int&
IntTab::operator()(const int i)
{
	return _values[i];
}
const int&
IntTab::operator()(const int i) const
{
	return _values[i];
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
IntTab::operator+=(const IntTab& inttab)
{
	for (int i=0;i<inttab.size();i++)
		_values[i] += inttab(i) ;
	return *this;
}

IntTab&
IntTab::operator+=(const int value)
{
	for (int i=0;i<_numberOfElements;i++)
		_values[i] += value ;
	return *this;
}

IntTab&
IntTab::operator*=(const int value)
{
	for (int i=0;i<_numberOfElements;i++)
		_values[i] *= value ;
	return *this;
}

IntTab&
IntTab::operator/=(const int value)
{
	for (int i=0;i<_numberOfElements;i++)
		_values[i] /= value ;
	return *this;
}

IntTab&
IntTab::operator-=(const IntTab& inttab)
{
	for (int i=0;i<inttab.size();i++)
		_values[i] -= inttab(i) ;
	return *this;
}
IntTab&
IntTab::operator-=(const int value)
{
	for (int i=0;i<_numberOfElements;i++)
		_values[i] -= value ;
	return *this;
}

int*
IntTab::getPointer(void)
{
	return _values;
}

ostream&
operator<<(ostream& out, const IntTab& U)
{
	for (int i=0; i<U.size();i++)
	{
		out.width(6);
		out.precision(6);
		out<<U[i];
		out<<endl;
	}
	return out;
}
