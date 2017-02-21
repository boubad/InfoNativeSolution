#include "stdafx.h"
#include "CppUnitTest.h"
////////////////////////
#include "test_serverdata.h"
////////////////////////////////////
#include <couchdbpersistmanager.h>
#include <blob.h>
//////////////////////////
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestGenInfo
{
	///////////////////////////////
	using namespace info;
	using namespace info::persist;
	using namespace std;
	////////////////////////////
	static const string_type STRING_VAL{U("Hello World")};
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

	TEST_CLASS(PersistManagerTest)
	{
	public:
		std::shared_ptr<CouchDBPersistManager> m_man;
		anymap_type m_map;
		bool boolval;
		int ival;
		double doubleval;
		string_type stringval;
		string_type testType;
		//
		IPersistManager *get_manager(void) {
			return m_man.get();
		}// get_manager
	public:
		TEST_METHOD_INITIALIZE(SetUp) {
			m_man.reset(new CouchDBPersistManager{TestServerData::serverUrl(),TestServerData::databaseName()});
			Assert::IsNotNull(m_man.get());
			//
			boolval = true;
			ival = 1234;
			doubleval = 5678.1234;
			stringval = STRING_VAL;
			testType = STRING_TESTTYPE;
			//
			m_map.clear();
			m_map[TESTKEY_TYPE] = any{STRING_TESTTYPE};
			m_map[TESTKEY_BOOLVAL] = any{boolval};
			m_map[TESTKEY_INTVAL] = any{ ival };
			m_map[TESTKEY_DOUBLEVAL] = any{ doubleval };
			m_map[TESTKEY_STRINGVAL] = any{ STRING_VAL };
		}//SetUp
		TEST_METHOD_CLEANUP(TearDown) {
			m_man.reset();
			m_map.clear();
		}// TearDown
	public:
		TEST_METHOD(test_get_persister_id) {
			IPersistManager *pMan = get_manager();
			string_type id = pMan->get_persister_id();
			Assert::IsFalse(id.empty());
		}//test_get_persister_id
		TEST_METHOD(test_check_index) {
			string_type field{STRING_FIELDNAME};
			string_type name{STRING_INDEXNAME};
			index_vector oinds =  {std::make_pair(field,false) };
			IPersistManager *pMan = get_manager();
			bool bRet = pMan->chek_index_async(oinds, name).get();
			Assert::IsTrue(bRet);
		}//test_check_index
		TEST_METHOD(test_crud) {
			IPersistManager *pMan = get_manager();
			anymap_ptr pMap = pMan->maintains_element_async(m_map).get();
			anymap_type *p = pMap.get();
			string_type id{};
			Assert::IsNotNull(p);
			{
				auto it = p->find(TESTKEY_ID);
				Assert::IsTrue(it != p->end());
				info_any_to_string((*it).second, id);
				Assert::IsFalse(id.empty());
			}
			bool bRet = p->find(TESTKEY_REV) != p->end();
			Assert::IsTrue(bRet);
			{
				auto it = p->find(TESTKEY_TYPE);
				Assert::IsTrue(it != p->end());
				string_type s{};
				info_any_to_string((*it).second, s);
				Assert::AreEqual(testType, s);
			}
			{
				auto it = p->find(TESTKEY_BOOLVAL);
				Assert::IsTrue(it != p->end());
				bool s{false};
				info_any_to_bool((*it).second, s);
				Assert::AreEqual(boolval, s);
			}
			{
				auto it = p->find(TESTKEY_INTVAL);
				Assert::IsTrue(it != p->end());
				int s{ -1 };
				info_any_to_int((*it).second, s);
				Assert::AreEqual(ival, s);
			}
			{
				auto it = p->find(TESTKEY_DOUBLEVAL);
				Assert::IsTrue(it != p->end());
				double s{ 0};
				info_any_to_double((*it).second, s);
				Assert::AreEqual(doubleval, s);
			}
			{
				auto it = p->find(TESTKEY_STRINGVAL);
				Assert::IsTrue(it != p->end());
				string_type s{};
				info_any_to_string((*it).second, s);
				Assert::AreEqual(stringval, s);
			}
			//
			{
				anymap_type o{};
				o[TESTKEY_ID] = any{id};
				anymap_ptr xMap = pMan->find_element_async(o).get();
				anymap_type *x = xMap.get();
				Assert::IsNotNull(x);
			}
			{
				size_t n{512};
				std::unique_ptr<byte> data{ new byte[n] };
				byte *p = data.get();
				Assert::IsNotNull(p);
				for (size_t i = 0; i < n; ++i) {
					p[i] = (byte)i;
				}// i
				string_type mime{STRING_MIMETYPE};
				string_type name{STRING_ATTACHMENTNAME};
				Blob blob{p,n,mime};
				blob.name(name);
				blob.id(id);
				Assert::IsTrue(blob.is_valid());
				bool b = pMan->maintains_blob_async(blob).get();
				Assert::IsTrue(b);
				blob_ptr pb = pMan->read_blob_async(blob).get();
				Blob *x = pb.get();
				Assert::IsNotNull(x);
				Assert::IsTrue(x->is_valid());
				stringsvector_ptr osv = pMan->read_all_blobs_async(id).get();
				std::vector<string_type> *pvec = osv.get();
				Assert::IsNotNull(pvec);
				Assert::IsFalse(pvec->empty());
				bool bx = pMan->remove_blob_async(blob).get();
				Assert::IsTrue(bx);
			}// attachments
			{
				anymap_type o{};
				o[TESTKEY_TYPE] = any{ testType };
				int nCount = pMan->get_count_async(o).get();
				Assert::IsTrue(nCount > 0);
				anymapvector_ptr pv = pMan->get_elements_async(o, 0, nCount).get();
				anymap_vector *vv = pv.get();
				Assert::IsNotNull(vv);
				Assert::IsFalse(vv->empty());
				bool b = pMan->remove_elements_async(pv).get();
				Assert::IsTrue(b);
			}
		}// test_crud
	};// class PersistManagerTest

	///////////////////////////
}// namespace UnitTest_GenData
