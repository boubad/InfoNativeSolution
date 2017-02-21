#pragma once
#ifndef SIGLENAMEDITEM_H
#define SIGLENAMEDITEM_H
////////////////////////////
#include "baseitem.h"
//////////////////////////////////
namespace info {
namespace domain {
///////////////////////////////////
class SigleNamedItem: public BaseItem {
private:
	string_type m_sigle;
	string_type m_name;
protected:
	SigleNamedItem();
	SigleNamedItem(const string_type &ssigle);
	SigleNamedItem(const anymap_type &oMap);
	SigleNamedItem(const SigleNamedItem &other);
	SigleNamedItem & operator=(const SigleNamedItem &other);
public:
	virtual ~SigleNamedItem();
public:
	const string_type &sigle(void) const {
		return (this->m_sigle);
	}
	void sigle(const string_type &s) {
		this->m_sigle = to_upper(trim(s));
	}
	const string_type &name(void) const {
		return (this->m_name);
	}
	void name(const string_type &s) {
		this->m_name = trim(s);
	}
public:
	virtual bool is_storeable(void) const;
	virtual void to_map(anymap_type &oMap) const;
	virtual int compare_to(const InfoItem &other) const;
	virtual string_type toString(void) const;
	virtual void clear(void);
};
// class SigleNamedItem
////////////////////////////////////
}// namespace domain
}		// namespace info
/////////////////////////////
#endif // SIGLENAMEDITEM_H
