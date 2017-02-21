//couchdbpersisterfactory.h
/////////////////////////////////
#include "stdafx.h"
#include "couchdbpersisterfactory.h"
#include "couchdbpersistmanager.h"
///////////////////////////////////////////
namespace info {
namespace persist {
/////////////////////////////
IPersistManager *CouchDBPersisterFactory::create(const string_type &url,
		const string_type &base) {
	return (new CouchDBPersistManager { url, base });
} // create
///////////////////////////////
}		// namespace persist
}		// namespace info
