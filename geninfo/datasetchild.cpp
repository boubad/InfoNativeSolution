//datasetchild.cpp
///////////////////////
#include "stdafx.h"
#include "datasetchild.h"
#include "dataset.h"
#include "infostrings.h"
/////////////////////////////
namespace info {
namespace domain {
/////////////////////////////////////////
DatasetChild::DatasetChild() {

}
DatasetChild::DatasetChild(const Dataset &oSet, const string_type &ssigle) :
		SigleNamedItem(ssigle), m_setsigle(oSet.sigle()) {

}
DatasetChild::DatasetChild(const string_type &setsigle,
		const string_type &ssigle) :
		SigleNamedItem(ssigle), m_setsigle(setsigle) {

}
DatasetChild::DatasetChild(const anymap_type &oMap) :
		SigleNamedItem(oMap) {
	{
		auto it = oMap.find(KEY_DATASETSIGLE);
		if (it != oMap.end()) {
			any src { (*it).second };
			string_type s { };
			if (info_any_to_string(src, s)) {
				this->dataset_sigle(s);
			}
		}
	} // dataset_sigle
}
DatasetChild::DatasetChild(const DatasetChild &other) :
		SigleNamedItem(other), m_setsigle(other.m_setsigle) {

}
DatasetChild & DatasetChild::operator=(const DatasetChild &other) {
	if (this != &other) {
		SigleNamedItem::operator=(other);
		this->m_setsigle = other.m_setsigle;
	}
	return (*this);
}
DatasetChild::~DatasetChild() {

}
void DatasetChild::clear(void) {
	SigleNamedItem::clear();
	this->m_setsigle.clear();
}
int DatasetChild::compare_to(const InfoItem &other) const {
	const DatasetChild *p = dynamic_cast<const DatasetChild *>(&other);
	if (p == nullptr) {
		return (-1);
	}
	if (this->m_setsigle < p->m_setsigle) {
		return (-1);
	} else if (this->m_setsigle > p->m_setsigle) {
		return (1);
	}
	return SigleNamedItem::compare_to(other);
}
bool DatasetChild::is_storeable(void) const {
	return ((!this->m_setsigle.empty()) && SigleNamedItem::is_storeable());
}
void DatasetChild::to_map(anymap_type &oMap) const {
	SigleNamedItem::to_map(oMap);
	string_type s = this->dataset_sigle();
	if (!s.empty()) {
		oMap[KEY_DATASETSIGLE] = any { s };
	}
}
////////////////////////////////////////
}// namespace domain
} // namespace info
