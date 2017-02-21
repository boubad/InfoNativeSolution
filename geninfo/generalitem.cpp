//generalitem.cpp
//////////////////////
#include "stdafx.h"
#include "generalitem.h"
#include "infostrings.h"
////////////////////////////
namespace info {
	namespace domain {
		////////////////////////////
		GeneralItem::GeneralItem() {

		}
		GeneralItem::GeneralItem(const anymap_type &oMap) : InfoItem(oMap) {
			for (auto it = oMap.begin(); it != oMap.end(); ++it) {
				string_type s = to_lower(trim((*it).first));
				if (s.empty()) {
					continue;
				}
				any v = (*it).second;
				if (INFO_ANY_EMPTY(v)){
					m_map[s] = v;
										continue;
				}

				if ((s != KEY_ID) && (s != KEY_REV) && (s != KEY_TYPE)) {
					m_map[s] = v;
				}
			}// it
		}
		GeneralItem::GeneralItem(const GeneralItem &other) : InfoItem(other), m_map(other.m_map) {

		}
		GeneralItem & GeneralItem::operator=(const GeneralItem &other) {
			if (this != &other) {
				InfoItem::operator=(other);
				this->m_map = other.m_map;
			}
			return (*this);
		}
		GeneralItem::~GeneralItem() {

		}
		string_type GeneralItem::type(void) const {
			return (TYPE_GENERAL);
		}
		bool GeneralItem::is_storeable(void) const {
			return (!this->m_map.empty());
		}
		item_ptr GeneralItem::clone(void) const {
			item_ptr oRet{ new GeneralItem{*this} };
			return (oRet);
		}
		void GeneralItem::to_map(anymap_type &oMap) const {
			InfoItem::to_map(oMap);
			for (auto it = m_map.begin(); it != m_map.end(); ++it) {
				any v = (*it).second;
				if (INFO_ANY_EMPTY(v)){
					continue;
				}
				string_type s = to_lower(trim((*it).first));
				if ((s != KEY_ID) && (s != KEY_REV) && (s != KEY_TYPE)) {
					oMap[s] = v;
				}
			}// it
		}
		void GeneralItem::clear(void) {
			InfoItem::clear();
			m_map.clear();
		}// clear
		/////////////////////////////
	}// domain
}// namespace ino
