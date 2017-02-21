#include "stdafx.h"
#include "CppUnitTest.h"
#include <couchdbmanager.h>
//////////////////////////
#include "test_serverdata.h"
//////////////////////////
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestGenInfo
{		

	using namespace info;
	using namespace info::persist;
	///////////////////////////
	TEST_MODULE_INITIALIZE(ModuleInitialize)
	{
		string_type sUrl = TestServerData::serverUrl();
		string_t sBase = TestServerData::databaseName();
		CouchDBManager oMan{ sUrl,sBase };
		bool b = oMan.exists_database(sBase).get();
		if (!b) {
			UpdateResponse r = oMan.create_database(sBase).get();
			Assert::IsTrue(r.ok());
		}// create_database
	}//ModuleInitialize
	 ///////////////////////////////
	TEST_MODULE_CLEANUP(ModuleCleanup)
	{
	}
}