//value.cpp
/////////////////////////
#include "stdafx.h"
#include "value.h"
#include "variable.h"
#include "indiv.h"
#include "infostrings.h"
/////////////////////////////
namespace info {
namespace domain {
//////////////////////////////////
Value::Value() {
}
Value::Value(const Indiv &oInd, const Variable &oVar, const any &v) :
		m_indsigle(oInd.sigle()), m_varsigle(oVar.sigle()), m_setsigle(
				oInd.dataset_sigle()), m_val(v) {
}
Value::Value(const string_type &setsigle, const string_type &indsigle,
		const string_type &varsigle, const any &v) :
		m_indsigle(indsigle), m_varsigle(varsigle), m_setsigle(setsigle), m_val(
				v) {

}
Value::Value(const anymap_type &oMap) :
		BaseItem(oMap) {
	{
		auto it = oMap.find(KEY_INDIVSIGLE);
		if (it != oMap.end()) {
			any src { (*it).second };
			string_type s { };
			if (info_any_to_string(src, s)) {
				this->indiv_sigle(s);
			}
		}
	} // indsigle
	{
		auto it = oMap.find(KEY_VARIABLESIGLE);
		if (it != oMap.end()) {
			any src { (*it).second };
			string_type s { };
			if (info_any_to_string(src, s)) {
				this->variable_sigle(s);
			}
		}
	} // varsigle
	{
		auto it = oMap.find(KEY_DATASETSIGLE);
		if (it != oMap.end()) {
			any src { (*it).second };
			string_type s { };
			if (info_any_to_string(src, s)) {
				this->dataset_sigle(s);
			}
		}
	} // datasetsigle
	{
		auto it = oMap.find(KEY_VALUE);
		if (it != oMap.end()) {
			this->m_val = (*it).second;
		}
	} // value
}
Value::Value(const Value &other) :
		BaseItem(other), m_indsigle(other.m_indsigle), m_varsigle(
				other.m_varsigle), m_setsigle(other.m_setsigle), m_val(
				other.m_val) {

}
Value & Value::operator=(const Value &other) {
	if (this != &other) {
		BaseItem::operator=(other);
		this->m_indsigle = other.m_indsigle;
		this->m_varsigle = other.m_varsigle;
		this->m_setsigle = other.m_setsigle;
		this->m_val = other.m_val;
	}
	return (*this);
}
Value::~Value() {

}
void Value::value(const any &v) {
	if (INFO_ANY_EMPTY(v)) {
		this->m_val = any { };
		return;
	}
	any s { v };
	this->m_val = s;
}
string_type Value::toString(void) const {
	string_type s { };
	if (!INFO_ANY_EMPTY(this->m_val)) {
		info_any_to_string(this->m_val, s);
	}
	return (s);
}
item_ptr Value::clone(void) const {
	item_ptr oRet { new Value { *this } };
	return oRet;
}

void Value::clear(void) {
	BaseItem::clear();
	this->m_indsigle.clear();
	this->m_varsigle.clear();
	this->m_setsigle.clear();
	this->m_val = any { };
}
int Value::compare_to(const InfoItem &other) const {
	const Value *p = dynamic_cast<const Value *>(&other);
	if (p == nullptr) {
		return (-1);
	}
	if (this->m_setsigle < p->m_setsigle) {
		return (-1);
	} else if (this->m_setsigle > p->m_setsigle) {
		return (1);
	}
	if (this->m_indsigle < p->m_indsigle) {
		return (-1);
	} else if (this->m_indsigle > p->m_indsigle) {
		return (1);
	}
	if (this->m_varsigle < p->m_varsigle) {
		return (-1);
	} else if (this->m_varsigle > p->m_varsigle) {
		return (1);
	}
	return (0);
}
string_type Value::type(void) const {
	return (TYPE_VALUE);
}
bool Value::is_storeable(void) const {
	return ((!this->m_setsigle.empty()) && (!this->m_indsigle.empty())
			&& (!this->m_varsigle.empty()));
}
void Value::to_map(anymap_type &oMap) const {
	BaseItem::to_map(oMap);
	if (!this->m_setsigle.empty()) {
		oMap[KEY_DATASETSIGLE] = any { this->m_setsigle };
	}
	if (!this->m_indsigle.empty()) {
		oMap[KEY_INDIVSIGLE] = any { this->m_indsigle };
	}
	if (!this->m_varsigle.empty()) {
		oMap[KEY_VARIABLESIGLE] = any { this->m_varsigle };
	}
	if (!INFO_ANY_EMPTY(this->m_val)) {
		oMap[KEY_VALUE] = this->m_val;
	}
}
////////////////////////////////////////
}// namespace data
} // namespace info
