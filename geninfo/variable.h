#pragma once
#ifndef VARIABLE_H
#define VARIABLE_H
//////////////////////////
#include "datasetchild.h"
/////////////////////////////////
namespace info {
namespace domain {
//////////////////////////////
enum class variable_datatype
	: int {
		unknown = 0,
	real = 1,
	integer = 2,
	text = 4,
	boolean = 8,
	date = 16,
	datetime = 32,
	enumtype = 64,
	other = 128
};
enum class variable_kindtype
	: int {
		unknown = 0, normal = 1, info = 2, modal = 4, ordinal = 8
};
///////////////////////////////
class Variable: public DatasetChild {
private:
	variable_datatype m_type;
	variable_kindtype m_genre;
	std::set<string_type> m_modalites;
public:
	Variable();
	Variable(const anymap_type &oMap);
	Variable(const Dataset &oSet, const string_type &ssigle);
	Variable(const string_type &setsigle, const string_type &ssigle);
	Variable(const Variable &other);
	Variable & operator=(const Variable &other);
	virtual ~Variable();
public:
	variable_datatype variable_type(void) const {
		return (this->m_type);
	}
	void variable_type(variable_datatype t) {
		this->m_type = t;
	}
	variable_kindtype genre(void) const {
		return (this->m_genre);
	}
	void genre(variable_kindtype g) {
		this->m_genre = g;
	}
	const std::set<string_type> & modalites(void) const {
		return (this->m_modalites);
	}
	std::set<string_type> & modalites(void) {
		return (this->m_modalites);
	}
	void add_modalite(const string_type &s) {
		string_type ss { to_upper(trim(s)) };
		if (!ss.empty()) {
			this->m_modalites.insert(ss);
		}
	}
	void remove_modalite(const string_type &s) {
		string_type ss { to_upper(trim(s)) };
		if (!ss.empty()) {
			auto it = this->m_modalites.find(ss);
			if (it != this->m_modalites.end()) {
				this->m_modalites.erase(it);
			}
		}
	}
public:
	virtual item_ptr clone(void) const;
	virtual string_type type(void) const;
	virtual bool is_storeable(void) const;
	virtual void to_map(anymap_type &oMap) const;
};
// class Variable
//////////////////////////////////
}// namespace data
}		// namespace info
///////////////////////////
#endif // VARIABLE_H
