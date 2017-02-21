//siglenameditem.cpp
////////////////////////
#include "stdafx.h"
#include "siglenameditem.h"
#include "infostrings.h"
/////////////////////////////
namespace info {
namespace domain {
////////////////////////////////////
SigleNamedItem::SigleNamedItem() {
}
SigleNamedItem::SigleNamedItem(const string_type &ssigle) {
	this->sigle(ssigle);
}
SigleNamedItem::SigleNamedItem(const anymap_type &oMap) :
		BaseItem(oMap) {
	{
		auto it = oMap.find(KEY_SIGLE);
		if (it != oMap.end()) {
			any src { (*it).second };
			string_type s { };
			if (info_any_to_string(src, s)) {
				this->sigle(s);
			}
		}
	} // sigle
	{
		auto it = oMap.find(KEY_NAME);
		if (it != oMap.end()) {
			any src { (*it).second };
			string_type s { };
			if (info_any_to_string(src, s)) {
				this->name(s);
			}
		}
	} // name

}
SigleNamedItem::SigleNamedItem(const SigleNamedItem &other) :
		BaseItem(other), m_sigle(other.m_sigle), m_name(other.m_name) {

}
SigleNamedItem & SigleNamedItem::operator=(const SigleNamedItem &other) {
	if (this != &other) {
		BaseItem::operator=(other);
		this->m_sigle = other.m_sigle;
		this->m_name = other.m_name;
	}
	return (*this);
}
SigleNamedItem::~SigleNamedItem() {

}
string_type SigleNamedItem::toString(void) const {
	string_type s { this->sigle() };
	return (s);
}
void SigleNamedItem::clear(void) {
	InfoItem::clear();
	this->m_sigle.clear();
	this->m_name.clear();
}

bool SigleNamedItem::is_storeable(void) const {
	return ((!m_sigle.empty()) && (!m_name.empty()));
}
int SigleNamedItem::compare_to(const InfoItem &other) const {
	const SigleNamedItem *p = dynamic_cast<const SigleNamedItem *>(&other);
	if (p == nullptr) {
		return (-1);
	}
	if (this->m_sigle < p->m_sigle) {
		return (-1);
	} else if (this->m_sigle > p->m_sigle) {
		return (1);
	} else {
		return (0);
	}
}
void SigleNamedItem::to_map(anymap_type &oMap) const {
	InfoItem::to_map(oMap);
	string_type s1 = this->sigle();
	if (!s1.empty()) {
		oMap[KEY_SIGLE] = any { s1 };
	}
	string_type s2 = this->name();
	if (!s2.empty()) {
		oMap[KEY_NAME] = any { s2 };
	}
}
////////////////////////////////////
}// namespace domain
} // namespace info
