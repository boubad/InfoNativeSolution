#pragma once
#ifndef INDIV_H
#define INDIV_H
///////////////////////////
#include "datasetchild.h"
/////////////////////////////////
namespace info {
namespace domain {
////////////////////////////////
class Indiv: public DatasetChild {
public:
	Indiv();
	Indiv(const anymap_type &oMap);
	Indiv(const Dataset &oSet, const string_type &ssigle);
	Indiv(const string_type &setsigle, const string_type &ssigle);
	Indiv(const Indiv &other);
	Indiv & operator=(const Indiv &other);
	virtual ~Indiv();
public:
	virtual string_type type(void) const;
	virtual item_ptr clone(void) const;
};
// class Indiv
///////////////////////////////////
}// namespace domain
}		// namespace info
////////////////////////////
#endif // INDIV_H
