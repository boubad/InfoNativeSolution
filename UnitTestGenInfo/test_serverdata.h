#pragma once
#ifndef TEST_SERVERDATA_H
#define TEST_SERVERDATA_H
/////////////////////////
#include <infocommon.h>
//////////////////////////////
namespace UnitTestGenInfo {
	//////////////////////////
	class TestServerData {
	private:
		static info::string_type m_url;
		static info::string_type m_database;
	public:
		static const info::string_type & serverUrl(void) {
			return m_url;
		}
		static const info::string_type & databaseName(void) {
			return m_database;
		}
	};// class TestServerData
	///////////////////////////
}// namespace UnitTestGenInfo
///////////////////////////
#endif // TEST_SERVERDATA_H
