//variable.cpp
/////////////////////////
#include "stdafx.h"
#include "variable.h"
#include "infostrings.h"
//////////////////////////
namespace info {
namespace domain {
////////////////////////////
Variable::Variable() :
		m_type(variable_datatype::unknown), m_genre(variable_kindtype::unknown) {
}
Variable::Variable(const anymap_type &oMap) :
		DatasetChild(oMap), m_type(variable_datatype::unknown), m_genre(
				variable_kindtype::unknown) {
	{
		auto it = oMap.find(KEY_VARTYPE);
		if (it != oMap.end()) {
			any src { (*it).second };
			int s { };
			if (info_any_to_int(src, s)) {
				variable_datatype st = static_cast<variable_datatype>(s);
				if (st != variable_datatype::unknown) {
					this->m_type = st;
				}
			}
		}
	} // type
	{
		auto it = oMap.find(KEY_VARGENRE);
		if (it != oMap.end()) {
			any src { (*it).second };
			int s { };
			if (info_any_to_int(src, s)) {
				variable_kindtype st = static_cast<variable_kindtype>(s);
				if (st != variable_kindtype::unknown) {
					this->m_genre = st;
				}
			}
		}
	} // genre
	{
		auto it = oMap.find(KEY_MODALITES);
		if (it != oMap.end()) {
			any src { (*it).second };
			info_any_to_stringset(src, this->m_modalites);
		}
	} // modalites
}
Variable::Variable(const Dataset &oSet, const string_type &ssigle) :
		DatasetChild(oSet, ssigle), m_type(variable_datatype::unknown), m_genre(
				variable_kindtype::unknown) {

}
Variable::Variable(const string_type &setsigle, const string_type &ssigle) :
		DatasetChild(setsigle, ssigle), m_type(variable_datatype::unknown), m_genre(
				variable_kindtype::unknown) {

}
Variable::Variable(const Variable &other) :
		DatasetChild(other), m_type(other.m_type), m_genre(other.m_genre), m_modalites(
				other.m_modalites) {

}
Variable & Variable::operator=(const Variable &other) {
	if (this != &other) {
		DatasetChild::operator=(other);
		this->m_type = other.m_type;
		this->m_genre = other.m_genre;
		this->m_modalites = other.m_modalites;
	}
	return (*this);
}
Variable::~Variable() {

}
item_ptr Variable::clone(void) const {
	item_ptr oRet { new Variable { *this } };
	return (oRet);
}
string_type Variable::type(void) const {
	return (TYPE_VARIABLE);
}
bool Variable::is_storeable(void) const {
	return ((this->m_type != variable_datatype::unknown)
			&& DatasetChild::is_storeable());
}
void Variable::to_map(anymap_type &oMap) const {
	DatasetChild::to_map(oMap);
	if (this->m_type != variable_datatype::unknown) {
		int n { static_cast<int>(this->m_type) };
		oMap[KEY_VARTYPE] = any { n };
	}
	if (this->m_genre != variable_kindtype::unknown) {
		int n { static_cast<int>(this->m_genre) };
		oMap[KEY_VARGENRE] = any { n };
	}
	if (!this->m_modalites.empty()) {
		string_type v { };
		for (auto it = this->m_modalites.begin(); it != this->m_modalites.end();
				++it) {
			if (it != this->m_modalites.begin()) {
#if defined(_MSC_VER)
				v += string_type(L",");
#else
				v += string_type(",");
#endif
			}
			string_type s = *it;
			v += s;
		}
		oMap[KEY_MODALITES] = v;
	}
} // to_map
////////////////////////////
}		// namespace domain
}		// namespace info
