#pragma once
#ifndef BASEITEM_H
#define BASEITEM_H
/////////////////////////////
#include "infoitem.h"
////////////////////////////////
namespace info {
namespace domain {
////////////////////////////
enum class status_type
	: int {
		unknown = 0,
	persisted = 1,
	tail = 2,
	disabled = 4,
	inserted = 8,
	updated = 16,
	deleted = 32,
	info = 64,
	normal = 128
};
///////////////////////////
class BaseItem: public InfoItem {
private:
	status_type m_status;
	string_type m_desc;
protected:
	BaseItem();
	BaseItem(const anymap_type &oMap);
	BaseItem(const BaseItem &other);
	BaseItem & operator=(const BaseItem &other);
public:
	virtual ~BaseItem();
public:
	status_type status(void) const {
		return (m_status);
	}
	void status(status_type t) {
		m_status = t;
	}
	const string_type &description(void) const {
		return (m_desc);
	}
	void description(const string_type &s) {
		m_desc = trim(s);
	}
	//
public:
	virtual void clear(void);
	virtual void to_map(anymap_type &oMap) const;
};
// class BaseItem
/////////////////////////////
}// domain
}		// namespace info
/////////////////////////////
#endif // BASEITEM_H
