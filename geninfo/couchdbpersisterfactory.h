#pragma once
#ifndef COUCHDBPERSISTERFACTORY_H
#define COUCHDBPERSISTERFACTORY_H
///////////////////////////////////
#include "persist_manager.h"
/////////////////////////////////
namespace info {
namespace persist {
///////////////////////////////////
class CouchDBPersisterFactory {
public:
	static IPersistManager *create(const string_type &url,
			const string_type &base);
};
// class CouchDBPersisterFactory
///////////////////////////////
}// namespace persist
}		// namespace info
////////////////////////
#endif // COUCHDBPERSISTERFACTORY_H
