#pragma once
#ifndef INFOITEM_H
#define INFOITEM_H
///////////////////////
#include "infocommon.h"
////////////////////////////
namespace info {
namespace domain {
////////////////////////
class InfoItem;
/////////////////////////////
using item_type = InfoItem;
using item_ptr = std::shared_ptr<item_type>;
using itemptr_vector = std::vector<item_ptr>;
using itemptrvector_ptr = std::shared_ptr<itemptr_vector>;
///////////////////////
class InfoItem {
private:
	string_type m_id;
	string_type m_rev;
protected:
	InfoItem();
	InfoItem(const anymap_type &oMap);
	InfoItem(const InfoItem &other);
	InfoItem & operator=(const InfoItem &other);
public:
	virtual ~InfoItem();
	bool operator==(const InfoItem &other) const {
		return (this->compare_to(other) == 0);
	}
	bool operator<(const InfoItem &other) const {
		return (this->compare_to(other) < 0);
	}
public:
	virtual string_type type(void) const = 0;
	virtual bool is_storeable(void) const = 0;
	virtual item_ptr clone(void) const = 0;
public:
	virtual void to_map(anymap_type &oMap) const;
	virtual int compare_to(const InfoItem &other) const;
	virtual string_type toString(void) const;
	virtual void clear(void);
public:
	bool has_id(void) const {
		return (!m_id.empty());
	}
	bool is_removeable(void) const {
		return this->is_persisted();
	}
	bool is_persisted(void) const {
		return ((!m_id.empty()) && (!m_rev.empty()));
	}
	const string_type &id(void) const {
		return m_id;
	}
	void id(const string_type &s);
	const string_type &version(void) const {
		return m_rev;
	}
	string_type to_json(void) const;
};
// class InfoItem
////////////////////////
}// namespace domain
} // namespace info
///////////////////////
#endif // INFOITEM_H
