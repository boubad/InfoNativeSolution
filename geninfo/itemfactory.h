#pragma once
#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H
////////////////////////////
#include "infoitem.h"
///////////////////////////
namespace info {
namespace domain {
////////////////////////////////
class ItemFactory {
public:
	static item_ptr create(const string_type &stype);
	static item_ptr create(const anymap_type &oMap);
};
// class ItemFactory
////////////////////////////////////
}// namespace domain
}		// namespace info
/////////////////////////
#endif // ITEMFACTORY_H

