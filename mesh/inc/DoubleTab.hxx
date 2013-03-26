/*
 * DoubleTab.hxx
 *
 *  Created on: 22 févr. 2013
 *      Author: mekkas
 */

#ifndef DOUBLETAB_HXX_
#define DOUBLETAB_HXX_

class DoubleTab
{
	public://----------------------------------------------------------------

	DoubleTab();
	DoubleTab(const int size);
	DoubleTab(const int size, const double initialValue);
	DoubleTab(const DoubleTab& dt);
	~DoubleTab();

	int size() const;
	double* getPointer(void) ;
	double* getValues(void) const ;

	DoubleTab & operator=(const DoubleTab & dt);
	DoubleTab & operator=(const double value);
	double & operator[](const int i);
	const double & operator[](const int i) const;
	DoubleTab& operator+=(const DoubleTab& dt);
	DoubleTab& operator+=(const double value);
	DoubleTab& operator-=(const DoubleTab& dt);
	DoubleTab& operator-=(const double value);
	DoubleTab& operator*= (const double value) ;
	DoubleTab& operator/= (const double value) ;

	private://----------------------------------------------------------------
	/*
	 * Values.
	 */
	double* _values;
	/*
	 * number of elements.
	 */
	int _numberOfElements;
};


#endif /* DOUBLETAB_HXX_ */
