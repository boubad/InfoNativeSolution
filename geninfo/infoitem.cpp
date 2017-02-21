//infoitem.cpp
/////////////////////
#include "stdafx.h"
#include "infoitem.h"
#include "infostrings.h"
/////////////////////////
namespace info {
namespace domain {
///////////////////////////
InfoItem::InfoItem() {
}
InfoItem::InfoItem(const anymap_type &oMap) {
	{
		auto it = oMap.find(KEY_ID);
		if (it != oMap.end()) {
			info_any_to_string((*it).second, m_id);
		}
	}
	{
		auto it = oMap.find(KEY_REV);
		if (it != oMap.end()) {
			info_any_to_string((*it).second, m_rev);
		}
	}
}
InfoItem::InfoItem(const InfoItem &other) :
		m_id(other.m_id), m_rev(other.m_rev) {
}
InfoItem & InfoItem::operator=(const InfoItem &other) {
	if (this != &other) {
		m_id = other.m_id;
		m_rev = other.m_rev;
	}
	return (*this);
}
InfoItem::~InfoItem() {
}
void InfoItem::to_map(anymap_type &oMap) const {
	string_type t = this->type();
	oMap[KEY_TYPE] = t;
	if (!m_id.empty()) {
		oMap[KEY_ID] = m_id;
	}
	if (!m_rev.empty()) {
		oMap[KEY_REV] = m_rev;
	}
}
void InfoItem::id(const string_type &s) {
	m_id = trim(s);
}
int InfoItem::compare_to(const InfoItem &other) const {
	if (m_id < other.m_id) {
		return -1;
	} else if (m_id > other.m_id) {
		return 1;
	} else {
		return 0;
	}
}
string_type InfoItem::toString(void) const {
	return m_id;
}
void InfoItem::clear(void) {
	m_id.clear();
	m_rev.clear();
}
/////////////////////////////
}// namespace domain
} // namespace info
