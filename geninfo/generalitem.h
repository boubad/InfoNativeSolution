#pragma once
#ifndef GENERALITEM_H
#define GENERALITEM_H
///////////////////////
#include "infoitem.h"
////////////////////////////
namespace info {
namespace domain {
////////////////////////
class GeneralItem: public InfoItem {
public:
	using iterator = anymap_type::iterator;
	using const_iterator = anymap_type::const_iterator;
private:
	anymap_type m_map;
public:
	GeneralItem();
	GeneralItem(const anymap_type &oMap);
	GeneralItem(const GeneralItem &other);
	GeneralItem & operator=(const GeneralItem &other);
	virtual ~GeneralItem();
public:
	virtual string_type type(void) const;
	virtual bool is_storeable(void) const;
	virtual item_ptr clone(void) const;
	virtual void to_map(anymap_type &oMap) const;
	virtual void clear(void);
public:
	const_iterator begin(void) const {
		return (m_map.begin());
	}
	const_iterator end(void) const {
		return (m_map.end());
	}
	iterator begin(void) {
		return (m_map.begin());
	}
	iterator end(void) {
		return (m_map.end());
	}
	const anymap_type get_map(void) const {
		return (m_map);
	}
	anymap_type get_map(void) {
		return (m_map);
	}
	bool has_key(const string_type &s) const {
		string_type key = to_lower(trim(s));
		return (m_map.find(key) != m_map.end());
	}
	bool get_value(const string_type &s, any &v) const {
		string_type key = to_lower(trim(s));
		auto it = m_map.find(key);
		if (it != m_map.end()) {
			v = (*it).second;
			return (true);
		} else {
			v = any { };
			return (false);
		}
	} // get_value
};
// class InfoItem
////////////////////////
}// namespace domain
} // namespace info
///////////////////////
#endif // INFOITEM_H
