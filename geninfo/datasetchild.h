#pragma once
#ifndef DATASETCHILD_H
#define DATASETCHILD_H
///////////////////////////
#include "siglenameditem.h"
//////////////////////////////
namespace info {
namespace domain {
///////////////////////
class Dataset;
////////////////////////////////////
class DatasetChild: public SigleNamedItem {
private:
	string_type m_setsigle;
protected:
	DatasetChild();
	DatasetChild(const Dataset &oSet, const string_type &ssigle);
	DatasetChild(const string_type &setsigle, const string_type &ssigle);
	DatasetChild(const anymap_type &oMap);
	DatasetChild(const DatasetChild &other);
	DatasetChild & operator=(const DatasetChild &other);
public:
	virtual ~DatasetChild();
public:
	const string_type &dataset_sigle(void) const {
		return (this->m_setsigle);
	}
	void dataset_sigle(const string_type &s) {
		this->m_setsigle = to_upper(trim(s));
	}
public:
	virtual void clear(void);
	virtual int compare_to(const InfoItem &other) const;
	virtual bool is_storeable(void) const;
	virtual void to_map(anymap_type &oMap) const;
};
// class DatasetChild
/////////////////////////////////////
}// namespace domain
}		// namespace info
//////////////////////////
#endif // DATASETCHILD_H
