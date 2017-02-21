//dataset.cpp
#include "stdafx.h"
#include "dataset.h"
#include "infostrings.h"
/////////////////////////
namespace info {
	namespace domain {
		////////////////////////////////////
		Dataset::Dataset() {

		}
		Dataset::Dataset(const anymap_type &oMap) :
			SigleNamedItem(oMap) {
				{
					auto it = oMap.find(KEY_ANNEE);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{};
						if (info_any_to_string(src, s)) {
							this->annee(s);
						}
					}
				} // sigle
		}
		Dataset::Dataset(const string_type &setsigle) :
			SigleNamedItem(setsigle) {
		}
		Dataset::Dataset(const Dataset &other) :
			SigleNamedItem(other),m_annee(other.m_annee) {

		}
		Dataset & Dataset::operator=(const Dataset &other) {
			if (this != &other) {
				SigleNamedItem::operator=(other);
				this->m_annee = other.m_annee;
			}
			return (*this);
		}
		Dataset::~Dataset() {

		}
		item_ptr Dataset::clone(void) const {
			item_ptr oRet{ new Dataset { *this } };
			return oRet;
		}
		string_type Dataset::type(void) const {
			return TYPE_DATASET;
		}
		void Dataset::to_map(anymap_type &oMap) const {
			SigleNamedItem::to_map(oMap);
			string_type s1 = this->annee();
			if (!s1.empty()) {
				oMap[KEY_ANNEE] = any{ s1 };
			}
		}
		///////////////////////////////////////
	}// namespace domain
} // namespace info
