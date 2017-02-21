//test_serverdata.cpp
/////////////////////
#include "stdafx.h"
////////////////////
#include "test_serverdata.h"
//////////////////////////////
namespace UnitTestGenInfo{
	using namespace info;
	///////////////////////////
	//string_type TestServerData::m_url{U("http://192.168.0.26:5984")};
	string_type TestServerData::m_url{ U("http://localhost:5984") };
	string_type TestServerData::m_database{U("test")};
	/////////////////////////////
}//namespace UnitTest_Gendata
