#include "stdafx.h"
#include "CppUnitTest.h"
////////////////////////
#include "test_serverdata.h"
////////////////////////////////////
#include <couchdbmanager.h>
#include <findselector.h>
#include <blob.h>
//////////////////////////
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestGenInfo
{
	///////////////////////////////
	using namespace info;
	using namespace info::persist;
	using namespace std;
	///////////////////////////
	static const string_t KEY_ID{ U("_id") };
	static const string_t KEY_REV{ U("_rev") };
	static const string_t KEY_TYPE{ U("type") };
	static const string_t KEY_BVAL{ U("bval") };
	static const string_t KEY_IVAL{ U("ival") };
	static const string_t KEY_FVAL{ U("fval") };
	static const string_t KEY_SVAL{ U("sval") };
	static const string_t KEY_VEC{ U("vec") };
	////////////////////////////////////////
	static const string_t VAL_ID{ U("testId") };
	static const string_t VAL_TYPE{ U("testType") };
	static const bool VAL_BOOL{ true };
	static const int VAL_INT{ 1234 };
	static const double VAL_DOUBLE{ 6578.59 };
	static const string_t VAL_STRING{ U("teststringval") };
	static const string_t STRING_CREATED{ U("created") };
	static const string_t STRING_EXISTS{ U("exists") };
	static const string_t ATTACHMENT_NAME{ U("testattach") };
	static const string_t ATTACHMENT_TYPE{ U("application/octet-stream") };
	static const size_t ATTACHMENT_SIZE{ 512 };
	static const string_t INDEX_NAME{ U("i_my_test_index") };
	static const size_t DOCS_COUNT{ 10 };
	///////////////////////////////////////
	TEST_CLASS(CouchDBTest)
	{
	public:
		value testDoc;
		std::shared_ptr<CouchDBManager> m_man;
		//
		static string_t st_url;
		static string_t st_base;
	public:
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			st_url = TestServerData::serverUrl();
			st_base = TestServerData::databaseName();
			Assert::IsFalse(st_url.empty());
			Assert::IsFalse(st_base.empty());
		}//ClassInitialize
		TEST_CLASS_CLEANUP(ClassCleanup)
		{
			st_url.clear();
			st_base.clear();
		} //ClassCleanup
		TEST_METHOD_INITIALIZE(setUp)
		{
			//
			testDoc = value::object();
			object &obj = testDoc.as_object();
			obj[KEY_ID] = value{ VAL_ID };
			obj[KEY_TYPE] = value{ VAL_TYPE };
			obj[KEY_BVAL] = value{ VAL_BOOL };
			obj[KEY_IVAL] = value{ VAL_INT };
			obj[KEY_FVAL] = value{ VAL_DOUBLE };
			obj[KEY_SVAL] = value{ VAL_STRING };
			//
			std::vector<value> vec{};
			for (int i = 0; i < 5; ++i) {
				stringstream_t os{};
				os << U("modal") << (i + 1);
				value vx{ os.str() };
				vec.push_back(vx);
			}// i
			value vz = value::array(vec);
			obj[KEY_VEC] = vz;
			m_man.reset(new CouchDBManager{st_url,st_base});
			Assert::IsNotNull(m_man.get());
		}// setUp
		TEST_METHOD_CLEANUP(tearDown)
		{
			m_man.reset();
		}//tearDown
		TEST_METHOD(test_server_base)
		{
			CouchDBManager *pMan = m_man.get();
			ServerInfo rsp = pMan->get_server_info().get();
			string_t ss = rsp.couchdb();
			string_t expected{ U("Welcome") };
			Assert::AreEqual(expected, ss);
			{
				Logger::WriteMessage(L"SERVER INFO");
				stringstream_t os{};
				rsp.write_to(os);
				string_t s = os.str();
				Logger::WriteMessage(s.c_str());
			}
			DatabaseInfo oInfo = pMan->get_database_info(pMan->database()).get();
			{
				Logger::WriteMessage(L"DATABASE INFO");
				stringstream_t os{};
				oInfo.write_to(os);
				string_t s = os.str();
				Logger::WriteMessage(s.c_str());
			}
		}// test_server_base
		TEST_METHOD(test_server_misc)
		{
			CouchDBManager *pMan = m_man.get();
			size_t count{ 5 };
			std::vector<string_t> v = pMan->get_uuids(count).get();
			Assert::AreEqual(count, v.size());
			Logger::WriteMessage(L"UUIDS\n");
			for (auto it = v.begin(); it != v.end(); ++it) {
				string_t s = *it;
				Logger::WriteMessage(s.c_str());
			}// it
			std::vector<string_t> vv = pMan->get_all_dbs().get();
			Assert::IsTrue(v.size() > 0);
			Logger::WriteMessage(L"ALL DATABASES\n");
			for (auto it = vv.begin(); it != vv.end(); ++it) {
				string_t s = *it;
				Logger::WriteMessage(s.c_str());
			}// it
			string_t testBase{ U("xxtestbaase") };
			bool bx = pMan->exists_database(testBase).get();
			if (!bx) {
				UpdateResponse r = pMan->create_database(testBase).get();
				Assert::IsTrue(r.ok());
				bx = pMan->exists_database(testBase).get();
				Assert::IsTrue(bx);
			}
			UpdateResponse rr = pMan->delete_database(testBase).get();
			Assert::IsTrue(rr.ok());
			bx = pMan->exists_database(testBase).get();
			Assert::IsFalse(bx);
		}//TestServerMisc 
		TEST_METHOD(test_create_read_delete_document)
		{
			CouchDBManager *pMan = m_man.get();
			string_t id{ VAL_ID };
			string_t rev = pMan->exists_document(id).get();
			if (rev.empty()) {
				UpdateResponse r = pMan->create_document(testDoc).get();
				Assert::IsTrue(r.ok());
				rev = pMan->exists_document(id).get();
				Assert::IsFalse(rev.empty());
			}// empty
			value doc = pMan->read_document(id).get();
			{
				Logger::WriteMessage(L"\n");
				string_t sx = doc.serialize();
				Logger::WriteMessage(sx.c_str());
			}
			Assert::IsFalse(doc.is_null());
			Assert::IsTrue(doc.is_object());
			//
			Assert::IsTrue(doc.has_field(KEY_ID));
			Assert::IsTrue(doc.at(KEY_ID).is_string());
			string_t s = doc.at(KEY_ID).as_string();
			Assert::AreEqual(id, s);
			//
			Assert::IsTrue(doc.has_field(KEY_REV));
			Assert::IsTrue(doc.at(KEY_REV).is_string());
			string_t sv = doc.at(KEY_REV).as_string();
			Assert::AreEqual(rev, sv);
			//
			Assert::IsTrue(doc.has_field(KEY_TYPE));
			Assert::IsTrue(doc.at(KEY_TYPE).is_string());
			string_t st = doc.at(KEY_TYPE).as_string();
			Assert::AreEqual(VAL_TYPE, st);
			//
			Assert::IsTrue(doc.has_field(KEY_BVAL));
			Assert::IsTrue(doc.at(KEY_BVAL).is_boolean());
			bool sb = doc.at(KEY_BVAL).as_bool();
			Assert::AreEqual(VAL_BOOL, sb);
			//
			Assert::IsTrue(doc.has_field(KEY_IVAL));
			Assert::IsTrue(doc.at(KEY_IVAL).is_integer());
			int si = doc.at(KEY_IVAL).as_integer();
			Assert::AreEqual(VAL_INT, si);
			//
			Assert::IsTrue(doc.has_field(KEY_FVAL));
			Assert::IsTrue(doc.at(KEY_FVAL).is_double());
			double sd = doc.at(KEY_FVAL).as_double();
			Assert::AreEqual(VAL_DOUBLE, sd);
			//
			Assert::IsTrue(doc.has_field(KEY_SVAL));
			Assert::IsTrue(doc.at(KEY_SVAL).is_string());
			string_t ss = doc.at(KEY_SVAL).as_string();
			Assert::AreEqual(VAL_STRING, ss);
			//
			Assert::IsTrue(doc.has_field(KEY_VEC));
			Assert::IsTrue(doc.at(KEY_VEC).is_array());
			const web::json::array &oAr = doc.at(KEY_VEC).as_array();
			for (auto it = oAr.begin(); it != oAr.end(); ++it) {
				const value &x = *it;
				Assert::IsFalse(x.is_null());
				Assert::IsTrue(x.is_string());
			}// it
			 //
			UpdateResponse r1 = pMan->maintains_document(doc).get();
			Assert::IsTrue(r1.ok());
			UpdateResponse r2 = pMan->delete_document(id).get();
			Assert::IsTrue(r2.ok());
			string_t xrev = pMan->exists_document(id).get();
			Assert::IsTrue(xrev.empty());
		}//test_create_read_delete_document
		TEST_METHOD(test_create_index) {
			CouchDBManager *pMan = m_man.get();
			CreateIndexData data{};
			data.set_name(U("i_testindex"));
			data.add_index_field(U("field2"),true);
			//data.add_index_field(U("field1"), false);
			//data.add_index_field(U("field3"), true);
			CreateIndexResponse rsp = pMan->create_index(data).get();
			string_t sx = rsp.result;
			Assert::IsTrue((sx == STRING_EXISTS) || (sx == STRING_CREATED));
		}// test_create_index
		TEST_METHOD(test_attachements)
		{
			CouchDBManager *pMan = m_man.get();
			string_t id{ VAL_ID };
			string_t rev = pMan->exists_document(id).get();
			if (rev.empty()) {
				UpdateResponse r = pMan->create_document(testDoc).get();
				Assert::IsTrue(r.ok());
				rev = pMan->exists_document(id).get();
				Assert::IsFalse(rev.empty());
			}// empty
			value doc = pMan->read_document(id).get();
			id = doc.at(KEY_ID).as_string();
			rev = doc.at(KEY_REV).as_string();
			string_t name{ ATTACHMENT_NAME };
			string_t stype{ ATTACHMENT_TYPE };
			const size_t n{ ATTACHMENT_SIZE };
			std::vector<unsigned char> vec{};
			vec.resize(n);
			for (size_t i = 0; i < n; ++i) {
				vec[i] = static_cast<unsigned char>(i);
			}// i
			Blob blob(vec, stype);
			blob.name(name);
			blob.id(id);
			Assert::IsTrue(blob.is_valid());
			UpdateResponse r = pMan->maintains_attachment(id, name, stype, vec).get();
			Assert::IsTrue(r.ok());
			rev = r.rev();
			//
			AttachmentData ad = pMan->read_attachment(id, name).get();
			Assert::IsTrue(ad.length == n);
			Assert::IsNotNull(ad.dataPtr.get());
			//
			UpdateResponse rr = pMan->remove_attachment(id, name).get();
			Assert::IsTrue(rr.ok());
			rev = rr.rev();
			//
			AttachmentData add = pMan->read_attachment(id, name).get();
			Assert::IsTrue(add.length < 1);
			Assert::IsTrue(add.content_type.empty());
			Assert::IsNull(add.dataPtr.get());
			//
			UpdateResponse r2 = pMan->delete_document(id).get();
			Assert::IsTrue(r2.ok());
		}//test_attachements
		TEST_METHOD(test_find_options)
		{
			CouchDBManager *pMan = m_man.get();
			std::vector<value> vec{};
			for (size_t i = 0; i < DOCS_COUNT; ++i) {
				value v = value::object();
				object &obj = v.as_object();
				stringstream_t os{};
				os << U("testobj") << (i + 1);
				obj[KEY_ID] = value{ os.str() };
				obj[KEY_TYPE] = value{ VAL_TYPE };
				obj[KEY_IVAL] = value{ static_cast<int>(i * i + 1) };
				vec.push_back(v);
			}
			std::vector<UpdateResponse> vv = pMan->maintains_documents(vec).get();
			Assert::AreEqual(vec.size(), vv.size());
			for (auto it = vv.begin(); it != vv.end(); ++it) {
				const UpdateResponse &r = *it;
				Assert::IsTrue(r.ok());
			}// it
			std::map<string_t, value> opts{};
			opts[KEY_TYPE] = value{ VAL_TYPE };
			int n = pMan->get_docs_count(opts).get();
			Assert::IsTrue(n >= DOCS_COUNT);
			vec.clear();
			int offset{ 0 }, count{ 3 };
			while (offset < n) {
				std::vector<value> vx = pMan->read_docs(opts, offset, count).get();
				size_t nz = vx.size();
				Assert::IsTrue(nz <= static_cast<size_t>(count));
				for (auto it = vx.begin(); it != vx.end(); ++it) {
					vec.push_back(*it);
				}
				if (nz < static_cast<size_t>(count)) {
					break;
				}
				offset += count;
			}// n
			vv = pMan->maintains_documents(vec, true).get();
			Assert::AreEqual(vec.size(), vv.size());
			for (auto it = vv.begin(); it != vv.end(); ++it) {
				const UpdateResponse &r = *it;
				Assert::IsTrue(r.ok());
			}// it
		}// test_find_options

	};// class CouchDBTest
	////////////////////////
	string_t CouchDBTest::st_url{};
	string_t CouchDBTest::st_base{};
	///////////////////////////
}// namespace UnitTest_InfoData
