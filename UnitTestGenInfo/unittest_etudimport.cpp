#include "stdafx.h"
#include "CppUnitTest.h"
////////////////////////
#include "test_serverdata.h"
#include "infotestdata.h"
////////////////////////////////////
#include <couchdbpersistmanager.h>
#include <domainpersistmanager.h>
#include <etudiant.h>
////////////////////////////
#include <fstream>
//////////////////////////
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestGenInfo
{
	///////////////////////////////
	using namespace info;
	using namespace info::persist;
	using namespace info::domain;
	using namespace std;
	////////////////////////////////
#if defined(_WIN64)
	static const string_type TestFileName{U("..\\..\\testdata\\Etuds_s4.txt")};
#else
	static const string_type TestFileName{ U("..\\testdata\\Etuds_s4.txt") };
#endif // _WIN64
	////////////////////////////
	TEST_CLASS(ImportEtudTest)
	{
	public:
		std::shared_ptr<CouchDBPersistManager> m_persist;
		std::shared_ptr<DomainPersistManager> m_man;
		//
		DomainPersistManager *get_manager(void) {
			return m_man.get();
		}// get_manager
	public:
		TEST_METHOD_INITIALIZE(SetUp) {
			m_persist.reset(new CouchDBPersistManager{ TestServerData::serverUrl(),TestServerData::databaseName() });
			IPersistManager *p = m_persist.get();
			Assert::IsNotNull(p);
			m_man.reset(new DomainPersistManager{ p });
			Assert::IsNotNull(m_man.get());
			//
		}//SetUp
		TEST_METHOD_CLEANUP(TearDown) {
			m_man.reset();
			m_persist.reset();
		}// TearDown
	public:
		TEST_METHOD(test_check_etuds_file)
		{
			ifstream_t in(TestFileName.c_str());
			if (in.is_open()) {
				Logger::WriteMessage(L"FileOpen");
				Assert::IsTrue(true);
			}
		}//test_check_etuds_file
		TEST_METHOD(test_import_etud)
		{
			DomainPersistManager *pMan = get_manager();
			Assert::IsNotNull(pMan);
			//
			bool b = info_import_etudiants_file(TestFileName, pMan).get();
			Assert::IsTrue(b);
		}// test_import_etud
		
	};// class PersistManagerTest

	  ///////////////////////////
}// namespace UnitTest_GenData
