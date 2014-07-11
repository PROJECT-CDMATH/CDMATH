/*
 * IntTab.hxx
 *
 *  Created on: 22 févr. 2013
 *      Author: mekkas
 */

#ifndef INTTAB_HXX_
#define INTTAB_HXX_

#include <iostream>

class IntTab
{
	public://----------------------------------------------------------------
	IntTab();

	IntTab(const int size);

	IntTab(const int size, const int initialValue);

	IntTab(const IntTab& dt);

	~IntTab();

	int size() const;

	int* getPointer(void) ;

	int* getValues(void) const ;

	IntTab & operator=(const IntTab & dt);

	IntTab & operator=(const int value);

	int & operator()(const int i);

	const int & operator()(const int i) const;

	int & operator[](const int i);

	const int & operator[](const int i) const;

	IntTab& operator+=(const IntTab& dt);

	IntTab& operator+=(const int value);

	IntTab& operator-=(const IntTab& dt);

	IntTab& operator-=(const int value);

	IntTab& operator*= (const int value) ;

	IntTab& operator/= (const int value) ;

	friend std::ostream& operator<<(std::ostream& out, const IntTab& U ) ;


	private://----------------------------------------------------------------
	/*
	 * Values.
	 */
	int* _values;
	/*
	 * number of elements.
	 */
	int _numberOfElements;
};


#endif /* INTTAB_HXX_ */
