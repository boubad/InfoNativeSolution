#pragma once
#ifndef DATASET_H
#define DATASET_H
////////////////////////////
#include "siglenameditem.h"
/////////////////////////////
namespace info {
namespace domain {
//////////////////////////////
class Dataset: public SigleNamedItem {
private:
	string_type m_annee;
public:
	Dataset();
	Dataset(const string_type &setsigle);
	Dataset(const anymap_type &oMap);
	Dataset(const Dataset &other);
	Dataset & operator=(const Dataset &other);
	virtual ~Dataset();
public:
	const string_type &annee(void) const {
		return (this->m_annee);
	}
	void annee(const string_type &s) {
		this->m_annee = to_upper(trim(s));
	}
public:
	virtual string_type type(void) const;
	virtual item_ptr clone(void) const;
	virtual void to_map(anymap_type &oMap) const;
};
// class Dataset
////////////////////////////////
}// namespace domain
}		// namespace info
/////////////////////////
#endif // DATASET_H
