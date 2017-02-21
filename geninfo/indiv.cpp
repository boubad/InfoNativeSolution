//indiv.cpp
////////////////////////
#include "stdafx.h"
#include "indiv.h"
#include "infostrings.h"
///////////////////////////
namespace info {
namespace domain {
///////////////////////////////
Indiv::Indiv() {

}
Indiv::Indiv(const anymap_type &oMap) :
		DatasetChild(oMap) {

}
Indiv::Indiv(const Dataset &oSet, const string_type &ssigle) :
		DatasetChild(oSet, ssigle) {

}
Indiv::Indiv(const string_type &setsigle, const string_type &ssigle) :
		DatasetChild(setsigle, ssigle) {

}
Indiv::Indiv(const Indiv &other) :
		DatasetChild(other) {

}
Indiv & Indiv::operator=(const Indiv &other) {
	if (this != &other) {
		DatasetChild::operator=(other);
	}
	return (*this);
}
Indiv::~Indiv() {

}
item_ptr Indiv::clone(void) const {
	item_ptr oRet { new Indiv { *this } };
	return oRet;
}
string_type Indiv::type(void) const {
	return (TYPE_INDIV);
}
//////////////////////////////////
}// namespace domain
} // namespace info
