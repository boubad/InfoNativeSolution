#pragma once
#ifndef VALUE_H
#define VALUE_H
////////////////////////
#include "baseitem.h"
/////////////////////////
namespace info {
namespace domain {
//////////////////////////////
class Variable;
class Indiv;
////////////////////////////////
class Value: public BaseItem {
private:
	string_type m_indsigle;
	string_type m_varsigle;
	string_type m_setsigle;
	any m_val;
public:
	Value();
	Value(const Indiv &oInd, const Variable &oVar, const any &v = any { });
	Value(const string_type &setsigle, const string_type &indsigle,
			const string_type &varsigle, const any &v = any { });
	Value(const anymap_type &oMap);
	Value(const Value &other);
	Value & operator=(const Value &other);
	virtual ~Value();
public:
	const string_type &indiv_sigle(void) const {
		return (this->m_indsigle);
	}
	void indiv_sigle(const string_type &s) {
		this->m_indsigle = to_upper(trim(s));
	}
	const string_type &variable_sigle(void) const {
		return (this->m_varsigle);
	}
	void variable_sigle(const string_type &s) {
		this->m_varsigle = to_upper(trim(s));
	}
	const string_type &dataset_sigle(void) const {
		return (this->m_setsigle);
	}
	void dataset_sigle(const string_type &s) {
		this->m_setsigle = to_upper(trim(s));
	}
	const any & value(void) const {
		return (this->m_val);
	}
	void value(const any &v);
	bool is_empty(void) const {
#if defined(_MSC_VER)
		return (!this->m_val.has_value());
#else
		return (this->m_val.empty());
#endif
	}
public:
	virtual item_ptr clone(void) const;
	virtual string_type toString(void) const;
	virtual void clear(void);
	virtual int compare_to(const InfoItem &other) const;
	virtual string_type type(void) const;
	virtual bool is_storeable(void) const;
	virtual void to_map(anymap_type &oMap) const;
};
// class Value;
/////////////////////////////////
}// namespace domain
}		// namespace info
//////////////////////////
#endif // VALUE_H
