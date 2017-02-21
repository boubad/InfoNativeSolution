#include "stdafx.h"
#include "findselector.h"
////////////////////////////////
namespace info {
	namespace persist {
		///////////////////
		using namespace web::json;
		//////////////////////////////////////////
		//
		static const string_t STRING_ID = U("_id");
		static const string_t STRING_SELECTOR = U("selector");
		static const string_t STRING_FIELDS = U("fields");
		static const string_t STRING_SKIP = U("skip");
		static const string_t STRING_LIMIT = U("limit");
		static const string_t STRING_SORT = U("sort");
		static const string_t STRING_ASC = U("asc");
		static const string_t STRING_DESC = U("desc");
		static const string_t STRING_USEINDEX = U("use_index");
		static const string_t STRING_INDEX = U("index");
		static const string_t STRING_NAME = U("name");
		///////////////////////////////////////////
		CreateIndexData::CreateIndexData() {

		}
		CreateIndexData::CreateIndexData(const CreateIndexData &other) : DocBase(other),
			name(other.name), fields(other.fields) {

		}
		CreateIndexData & CreateIndexData::operator=(const CreateIndexData &other) {
			if (this != &other) {
				DocBase::operator=(other);
				this->name = other.name;
				this->fields = other.fields;
			}
			return (*this);
		}
		CreateIndexData::~CreateIndexData() {

		}
		void CreateIndexData::set_name(const string_t &s) {
			this->name = s;
		}
		void CreateIndexData::add_index_field(const string_t &field, bool bAsc/*=true*/) {
			if (this->fields.is_null()) {
				this->fields = value::object();
				value va = value::array();
				object &obj = this->fields.as_object();
				obj[STRING_FIELDS] = va;
			}
			value &va = this->fields.at(STRING_FIELDS);
			array &oAr = va.as_array();
			value vo = value::object();
			object &o = vo.as_object();
			if (bAsc) {
				o[field] = value(STRING_ASC);
			}
			else {
				o[field] = value(STRING_DESC);
			}
			size_t n = oAr.size();
			oAr[n] = value(vo);
		}
		value CreateIndexData::to_value(void) const {
			value vRet;
			if (!this->fields.is_null()) {
				vRet = value::object();
				object &obj = vRet.as_object();
				obj[STRING_INDEX] = this->fields;
				if (!this->name.empty()) {
					obj[STRING_NAME] = value(this->name);
				}
			}
			return (vRet);
		}// to_value
		 //////////////////////////////////////////
		FindOptions::FindOptions() :skip(0), limit(0) {

		}
		FindOptions::FindOptions(const FindOptions &other) : DocBase(other),skip(other.skip), limit(other.limit),
			selector(other.selector), sort(other.sort), fields(other.fields), use_index(other.use_index) {

		}
		FindOptions & FindOptions::operator=(const FindOptions &other) {
			if (this != &other) {
				DocBase::operator=(other);
				this->skip = other.skip;
				this->limit = other.limit;
				this->selector = other.selector;
				this->sort = other.sort;
				this->fields = other.fields;
				this->use_index = other.use_index;
			}
			return (*this);
		}
		FindOptions::~FindOptions() {

		}
		void FindOptions::set_skip(int n) {
			if (n >= 0) {
				this->skip = n;
			}
		}
		void FindOptions::set_limit(int n) {
			this->limit = n;
		}
		void FindOptions::add_fetch_item(const string_t &field, const value &v) {
			if (this->selector.is_null()) {
				this->selector = value::object();
			}
			object &obj = this->selector.as_object();
			obj[field] = v;
		}
		void FindOptions::add_return_field(const string_t &field) {
			if (this->fields.is_null()) {
				this->fields = value::array();
			}
			web::json::array &oAr = this->fields.as_array();
			size_t n = oAr.size();
			oAr[n] = value(field);
		}
		void FindOptions::add_sort_field(const string_t &field, bool bAsc /* = true */) {
			if (this->sort.is_null()) {
				this->sort = value::array();
			}
			web::json::array &oAr = this->sort.as_array();
			value vo = value::object();
			object &o = vo.as_object();
			if (bAsc) {
				o[field] = value(STRING_ASC);
			}
			else {
				o[field] = value(STRING_DESC);
			}
			size_t n = oAr.size();
			oAr[n] = value(vo);
		}
		void FindOptions::set_use_index(const value &v) {
			this->use_index = v;
		}
		value FindOptions::to_value(void) const {
			value vRet{};
			if (!this->selector.is_null()) {
				vRet = value::object();
				object &obj = vRet.as_object();
				vRet[STRING_SELECTOR] = this->selector;
				if (this->limit > 0) {
					obj[STRING_LIMIT] = value(this->limit);
				}
				if (this->skip > 0) {
					obj[STRING_SKIP] = value(this->skip);
				}
				if (!this->sort.is_null()) {
					obj[STRING_SORT] = this->sort;
				}
				if (!this->fields.is_null()) {
					obj[STRING_FIELDS] = this->fields;
				}
				if (!this->use_index.is_null()) {
					obj[STRING_USEINDEX] = this->use_index;
				}
			}
			return (vRet);
		}// to_value
		 /////////////////////////////////////////
	}
}
