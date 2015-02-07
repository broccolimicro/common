/*
 * text.h
 *
 *  Created on: Jan 31, 2015
 *      Author: nbingham
 */

#include "standard.h"

#ifndef common_text_h
#define common_text_h

bool ac(char c);
bool nc(char c);
bool oc(char c);
bool sc(char c);

template <class type>
string to_string(type value)
{
	ostringstream os;
	os << value;
	return os.str();
}

int edit_distance(const string &s1, const string &s2);

#endif
