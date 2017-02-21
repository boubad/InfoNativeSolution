#include "stdafx.h"
#include "info_docutils.h"
///////////////////////
namespace info {
	namespace persist {
		////////////////////////////////
		static const string_t STRING_ID = U("_id");
		static const string_t STRING_REV = U("_rev");
		static const string_t STRING_REV2 = U("rev");
		static const string_t STRING_STATUS = U("status");
		static const string_t STRING_START = U("start");
		static const string_t STRING_IDS = U("ids");
		static const string_t STUB = U("stub");
		static const string_t REVPOS = U("revpos");
		static const string_t LENGTH = U("length");
		static const string_t ENCODED_LENGTH = U("encoded_length");
		static const string_t ENCODING = U("encoding");
		static const string_t CONTENT_TYPE = U("content_type");
		static const string_t DATA = U("data");
		static const string_t DIGEST = U("digest");
		static const string_t STRING_DELETED2 = U("deleted");
		static const string_t STRING_AVAILABLE = U("available");
		////////////////////////////////////////////
		AttachmentDesc::AttachmentDesc() :stub(false), revpos(0), length(0), encoded_length(0)
		{
		}
		AttachmentDesc::AttachmentDesc(const value &oMap) : stub(false), revpos(0), length(0), encoded_length(0) {
			if ((!oMap.is_null()) && oMap.is_object()) {
				if (oMap.has_field(STUB)) {
					const value &v = oMap.at(STUB);
					if ((!v.is_null()) && v.is_boolean()) {
						this->stub = v.as_bool();
					}
				}
				if (oMap.has_field(REVPOS)) {
					const value &v = oMap.at(REVPOS);
					if ((!v.is_null()) && v.is_integer()) {
						this->revpos = static_cast<size_t>(v.as_integer());
					}
				}
				if (oMap.has_field(LENGTH)) {
					const value &v = oMap.at(LENGTH);
					if ((!v.is_null()) && v.is_integer()) {
						this->length = static_cast<size_t>(v.as_integer());
					}
				}
				if (oMap.has_field(ENCODED_LENGTH)) {
					const value &v = oMap.at(ENCODED_LENGTH);
					if ((!v.is_null()) && v.is_integer()) {
						this->encoded_length = static_cast<size_t>(v.as_integer());
					}
				}
				if (oMap.has_field(ENCODING)) {
					const value &v = oMap.at(ENCODING);
					if ((!v.is_null()) && v.is_string()) {
						this->encoding = v.as_string();
					}
				}
				if (oMap.has_field(CONTENT_TYPE)) {
					const value &v = oMap.at(CONTENT_TYPE);
					if ((!v.is_null()) && v.is_string()) {
						this->content_type = v.as_string();
					}
				}
				if (oMap.has_field(DATA)) {
					const value &v = oMap.at(DATA);
					if ((!v.is_null()) && v.is_string()) {
						this->data = v.as_string();
					}
				}
				if (oMap.has_field(DIGEST)) {
					const value &v = oMap.at(DIGEST);
					if ((!v.is_null()) && v.is_string()) {
						this->digest = v.as_string();
					};
				}
			}// oMap
		}// AttachmentDesc
		AttachmentDesc::AttachmentDesc(const AttachmentDesc &other) :stub(other.stub), revpos(other.revpos), length(other.length), encoded_length(other.encoded_length),
			encoding(other.encoding), content_type(other.content_type), data(other.data),
			digest(other.digest), name(other.name), url(other.url) {
		}
		AttachmentDesc & AttachmentDesc::operator=(const AttachmentDesc &other) {
			if (this != &other) {
				this->stub = other.stub;
				this->revpos = other.revpos;
				this->length = other.length;
				this->encoded_length = other.encoded_length;
				this->encoding = other.encoding;
				this->content_type = other.content_type;
				this->data = other.data;
				this->digest = other.digest;
				this->name = other.name;
				this->url = other.url;
			}
			return *this;
		}
		AttachmentDesc::~AttachmentDesc()
		{
		}
		///////////////////////////////////////
		RevisionsData::RevisionsData() :_start(0) {
		}
		RevisionsData::RevisionsData(const value &oMap) : _start(0) {
			if ((!oMap.is_null()) && oMap.is_object()) {
				if (oMap.has_field(STRING_START)) {
					const value &v = oMap.at(STRING_START);
					if ((!v.is_null()) && v.is_integer()) {
						this->_start = static_cast<size_t>(v.as_integer());
					}
				}
				if (oMap.has_field(STRING_IDS)) {
					const value &v = oMap.at(STRING_IDS);
					if ((!v.is_null()) && v.is_array()) {
						const array &oAr = v.as_array();
						std::vector<string_t> &vec = this->_ids;
						std::for_each(oAr.begin(), oAr.end(), [&vec](const value &s) {
							vec.push_back(s.as_string());
						});
					}
				}
			}// oMap
		}
		RevisionsData::RevisionsData(const RevisionsData &other) :_start(other._start),
			_ids(other._ids) {

		}
		RevisionsData & RevisionsData::operator=(const RevisionsData &other) {
			if (this != &other) {
				this->_start = other._start;
				this->_ids = other._ids;
			}
			return *this;
		}
		RevisionsData::~RevisionsData() {

		}
		size_t  RevisionsData::start(void) const {
			return this->_start;
		}
		const std::vector<string_t> & RevisionsData::ids(void) const {
			return this->_ids;
		}
		/////////////////////////////////////
		RevisionInfo::RevisionInfo() {

		}
		RevisionInfo::RevisionInfo(const value &oMap) {
			if ((!oMap.is_null()) && oMap.is_object()) {
				if (oMap.has_field(STRING_REV2)) {
					const value &v = oMap.at(STRING_REV2);
					if ((!v.is_null()) && v.is_string()) {
						this->_rev = v.as_string();
					}
				}
				if (oMap.has_field(STRING_STATUS)) {
					const value &v = oMap.at(STRING_STATUS);
					if ((!v.is_null()) && v.is_string()) {
						this->_status = v.as_string();
					}
				}
			}// oMap
		}
		RevisionInfo::RevisionInfo(const RevisionInfo &other) :_rev(other._rev),
			_status(other._status) {

		}
		RevisionInfo & RevisionInfo::operator=(const RevisionInfo &other) {
			if (this != &other) {
				this->_rev = other._rev;
				this->_status = other._status;
			}
			return *this;
		}
		RevisionInfo::~RevisionInfo() {

		}
		const string_t & RevisionInfo::rev(void) const {
			return this->_rev;
		}
		const string_t & RevisionInfo::status(void) const {
			return this->_status;
		}
		bool RevisionInfo::is_available(void) const {
			return (!this->_status.empty()) && (this->_status == STRING_AVAILABLE);
		}
		bool RevisionInfo::is_deleted(void) const {
			return (!this->_status.empty()) && (this->_status == STRING_DELETED2);
		}
		////////////////////////////////
	}// namespace persist
}// namespace info
