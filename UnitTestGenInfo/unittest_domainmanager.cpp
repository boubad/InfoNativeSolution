#include "stdafx.h"
#include "CppUnitTest.h"
////////////////////////
#include "test_serverdata.h"
////////////////////////////////////
#include <couchdbpersistmanager.h>
#include <domainpersistmanager.h>
#include <generalitem.h>
#include <blob.h>
//////////////////////////
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestGenInfo
{
	///////////////////////////////
	using namespace info;
	using namespace info::persist;
	using namespace info::domain;
	using namespace std;
	/////////////////////////////
	static const string_type STRING_VAL{ U("Hello World") };
	static const string_type STRING_TESTTYPE{ U("testtype") };
	static const string_type TESTKEY_TYPE{ U("type") };
	static const string_type TESTKEY_BOOLVAL{ U("boolval") };
	static const string_type TESTKEY_INTVAL{ U("intval") };
	static const string_type TESTKEY_DOUBLEVAL{ U("doubleval") };
	static const string_type TESTKEY_STRINGVAL{ U("stringval") };
	static const string_type STRING_FIELDNAME{ U("testfield") };
	static const string_type STRING_INDEXNAME{ U("testname") };
	static const string_type TESTKEY_ID{ U("_id") };
	static const string_type TESTKEY_REV{ U("_rev") };
	static const string_type STRING_MIMETYPE{ U("application/octet-stream") };
	static const string_type STRING_ATTACHMENTNAME{ U("attachname") };
	///////////////////////////

	TEST_CLASS(DomainManagerTest)
	{
	public:
		std::shared_ptr<CouchDBPersistManager> m_persist;
		std::shared_ptr<DomainPersistManager> m_man;
		anymap_type m_map;
		GeneralItem m_item;
		bool boolval{ true };
		int ival{ 1234 };
		double doubleval{ 5678.1234 };
		string_type stringval{ STRING_VAL};
		//
		DomainPersistManager *get_manager(void) {
			return m_man.get();
		}// get_manager
	public:
		TEST_METHOD_INITIALIZE(SetUp) {
			m_persist.reset(new CouchDBPersistManager{ TestServerData::serverUrl(),TestServerData::databaseName() });
			IPersistManager *pPersist = m_persist.get();
			Assert::IsNotNull(pPersist);
			m_man.reset(new DomainPersistManager{ pPersist });
			Assert::IsNotNull(m_man.get());
			//
			m_map.clear();
			m_map[TESTKEY_BOOLVAL] = any{ boolval };
			m_map[TESTKEY_INTVAL] = any{ ival };
			m_map[TESTKEY_DOUBLEVAL] = any{ doubleval };
			m_map[TESTKEY_STRINGVAL] = any{ STRING_VAL };
			m_item = GeneralItem{ m_map };
		}//SetUp
		TEST_METHOD_CLEANUP(TearDown) {
			m_map.clear();
			m_man.reset();
			m_persist.reset();
		}// TearDown
	public:
		
		TEST_METHOD(test_item_crud) {
			DomainPersistManager *pMan = m_man.get();
			item_ptr pMap = pMan->maintains_element_async(m_item).get();
			item_type *p0 = pMap.get();
			GeneralItem *p = dynamic_cast<GeneralItem *>(p0);
			Assert::IsNotNull(p);
			Assert::IsTrue(p->is_persisted());
			Assert::IsTrue(p->is_removeable());
			Assert::IsTrue(p->is_removeable());
			string_type id = p->id();
			{
				bool b = p->has_key(TESTKEY_BOOLVAL);
				any v{};
				Assert::IsTrue(p->get_value(TESTKEY_BOOLVAL, v));
				bool s{ false };
				info_any_to_bool(v, s);
				Assert::AreEqual(boolval, s);
			}
			{
				bool b = p->has_key(TESTKEY_INTVAL);
				any v{};
				Assert::IsTrue(p->get_value(TESTKEY_INTVAL, v));
				int s{ -1 };
				info_any_to_int(v, s);
				Assert::AreEqual(ival, s);
			}
			{
				bool b = p->has_key(TESTKEY_DOUBLEVAL);
				any v{};
				Assert::IsTrue(p->get_value(TESTKEY_DOUBLEVAL, v));
				double s{ -1.0 };
				info_any_to_double(v, s);
				Assert::AreEqual(doubleval, s);
			}
			{
				bool b = p->has_key(TESTKEY_STRINGVAL);
				any v{};
				Assert::IsTrue(p->get_value(TESTKEY_STRINGVAL, v));
				string_type s{ };
				info_any_to_string(v, s);
				Assert::AreEqual(stringval, s);
			}
			//
			{
				GeneralItem o{};
				o.id(id);
				item_ptr x = pMan->find_element_async(o).get();
				GeneralItem *px = dynamic_cast<GeneralItem *>(x.get());
				Assert::IsNotNull(px);
			}
			{
				size_t n{ 512 };
				std::unique_ptr<byte> data{ new byte[n] };
				byte *pd = data.get();
				Assert::IsNotNull(pd);
				for (size_t i = 0; i < n; ++i) {
					pd[i] = (byte)i;
				}// i
				string_type mime{ STRING_MIMETYPE};
				string_type name{ STRING_ATTACHMENTNAME };
				Blob blob{ pd,n,mime };
				blob.name(name);
				blob.id(id);
				Assert::IsTrue(blob.is_valid());
				bool b = pMan->maintains_blob_async(blob).get();
				stringsvector_ptr osv = pMan->read_all_blobs_async(*p).get();
				std::vector<string_type> *pvec = osv.get();
				Assert::IsNotNull(pvec);
				Assert::IsFalse(pvec->empty());
				bool bx = pMan->remove_blob_async(blob).get();
				Assert::IsTrue(bx);
			}// attachments
			{
				GeneralItem o{};
				int nCount = pMan->get_count_async(o).get();
				Assert::IsTrue(nCount > 0);
				itemptrvector_ptr pv = pMan->get_elements_async(o, 0, nCount).get();
				itemptr_vector *vv = pv.get();
				Assert::IsNotNull(vv);
				Assert::IsFalse(vv->empty());
				bool b = pMan->remove_elements_async(pv).get();
				Assert::IsTrue(b);
			}
		}// test_crud
	};// class PersistManagerTest

	  ///////////////////////////
}// namespace UnitTest_GenData
