//baseitem.cpp
////////////////////////
#include "stdafx.h"
#include "baseitem.h"
#include "infostrings.h"
//////////////////////////
namespace info {
	namespace domain {
		//////////////////////////
		BaseItem::BaseItem() :m_status(status_type::unknown) {
		}
		BaseItem::BaseItem(const anymap_type &oMap) : m_status(status_type::unknown) {

			{
				auto it = oMap.find(KEY_DESC);
				if (it != oMap.end()) {
					any src{ (*it).second };
					string_type s{};
					if (info_any_to_string(src, s)) {
						this->description(s);
					}
				}
			}// desc
			{
				auto it = oMap.find(KEY_STATUS);
				if (it != oMap.end()) {
					any src{ (*it).second };
					int s{ 0 };
					if (info_any_to_int(src, s)) {
						status_type st = static_cast<status_type>(s);
						if (st != status_type::unknown) {
							this->status(st);
						}
					}
				}
			}// status
		}
		BaseItem::BaseItem(const BaseItem &other) : InfoItem(other), m_status(other.m_status), m_desc(other.m_desc) {

		}
		BaseItem & BaseItem::operator=(const BaseItem &other) {
			if (this != &other) {
				InfoItem::operator=(other);
				this->m_status = other.m_status;
				this->m_desc = other.m_desc;
			}
			return (*this);
		}
		BaseItem::~BaseItem() {

		}
		void BaseItem::clear(void) {
			InfoItem::clear();
			this->m_status = status_type::unknown;
			this->m_desc.clear();
		}
		void BaseItem::to_map(anymap_type &oMap) const {
			InfoItem::to_map(oMap);
			string_type s4 = this->description();
			if (!s4.empty()) {
				oMap[KEY_DESC] = any{ s4 };
			}
			status_type s = this->status();
			if (s != status_type::unknown) {
				int n = static_cast<int>(s);
				oMap[KEY_STATUS] = any{ n };
			}
		}// to_map
		///////////////////////////
	}// namespace domain
}// namespace info
