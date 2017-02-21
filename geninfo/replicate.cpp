#include "stdafx.h"
#include "replicate.h"
///////////////////////////////
namespace info {
	namespace persist {
		//////////////////////////////////////////
		static const string_t KEY_HISTORY = U("history");
		static const string_t KEY_OK = U("ok");
		static const string_t KEY_REPLICATION_ID_VERSION = U("replication_id_version");
		static const string_t KEY_SESSION_ID = U("session_id");
		static const string_t KEY_SOURCE_LAST_SEQ = U("source_last_seq");
		static const string_t KEY_DOC_WRITE_FAILURES = U("doc_write_failures");
		static const string_t KEY_DOCS_READ = U("docs_read");
		static const string_t KEY_DOCS_WRITTEN = U("docs_written");
		static const string_t KEY_END_LAST_SEQ = U("end_last_seq");
		static const string_t KEY_END_TIME = U("end_time");
		static const string_t KEY_MISSING_CHECKED = U("missing_checked");
		static const string_t KEY_MISSING_FOUND = U("missing_found");
		static const string_t KEY_RECORDED_SEQ = U("recorded_seq");
		static const string_t KEY_START_LAST_SEQ = U("start_last_seq");
		static const string_t KEY_START_TIME = U("start_time");
		static const string_t KEY_CANCEL = U("cancel");
		static const string_t KEY_CONTINIOUS = U("continuous");
		static const string_t KEY_CREATE_TARGET = U("create_target");
		static const string_t KEY_PROXY = U("proxy");
		static const string_t KEY_SOURCE = U("source");
		static const string_t KEY_TARGET = U("target");
		static const string_t KEY_DOCS_IDS = U("doc_ids");
		////////////////////////////////////////
		ReplicateResponse::ReplicateResponse() {

		}
		ReplicateResponse::ReplicateResponse(const value &v) :m_val(v) {

		}
		ReplicateResponse::ReplicateResponse(const ReplicateResponse &other) : m_val(other.m_val) {

		}
		ReplicateResponse & ReplicateResponse::operator=(const ReplicateResponse &other) {
			if (this != &other) {
				this->m_val = other.m_val;
			}
			return *this;
		}
		ReplicateResponse::~ReplicateResponse() {

		}
		std::vector<ReplicationItem> ReplicateResponse::history(void) const {
	std::vector<ReplicationItem> oRet { };
			if (this->m_val.has_field(KEY_HISTORY)) {
				const value & v = this->m_val.at(KEY_HISTORY);
				if ((!v.is_null()) && v.is_array()) {
					web::json::array vv = v.as_array();
					for (auto it = vv.begin(); it != vv.end(); ++it) {
						ReplicationItem item(*it);
						oRet.push_back(item);
					}// it
				}// array
			}// field
			return oRet;
		}
		bool ReplicateResponse::ok(void) const {
	bool bRet { false };
			if (this->m_val.has_field(KEY_OK)) {
				const value &v = this->m_val.at(KEY_OK);
				if ((!v.is_null()) && v.is_boolean()) {
					bRet = v.as_bool();
				}
			}// field
			return bRet;
		}
		size_t ReplicateResponse::replication_id_version(void) const {
			size_t nRet = 0;
			if (this->m_val.has_field(KEY_REPLICATION_ID_VERSION)) {
				const value &v = this->m_val.at(KEY_REPLICATION_ID_VERSION);
				if ((!v.is_null()) && v.is_integer()) {
					nRet = v.as_integer();
				}
			}
			return nRet;
		}
		string_t ReplicateResponse::session_id(void) const {
	string_t sRet { };
			if (this->m_val.has_field(KEY_SESSION_ID)) {
				const value &v = this->m_val.at(KEY_SESSION_ID);
				if ((!v.is_null()) && v.is_string()) {
					sRet = v.as_string();
				}
			}
			return sRet;
		}
		size_t ReplicateResponse::source_last_seq(void) const {
			size_t nRet = 0;
			if (this->m_val.has_field(KEY_SOURCE_LAST_SEQ)) {
				const value &v = this->m_val.at(KEY_SOURCE_LAST_SEQ);
				if ((!v.is_null()) && v.is_integer()) {
					nRet = v.as_integer();
				}
			}
			return nRet;
		}
		//////////////////////////////////////////
		ReplicationItem::ReplicationItem() {

		}
		ReplicationItem::ReplicationItem(const value &v) :m_val(v) {

		}
		ReplicationItem::ReplicationItem(const ReplicationItem &other) : m_val(other.m_val) {

		}
		ReplicationItem & ReplicationItem::operator=(const ReplicationItem &other) {
			if (this != &other) {
				this->m_val = other.m_val;
			}
			return *this;
		}
		ReplicationItem::~ReplicationItem() {

		}
		size_t ReplicationItem::doc_write_failures(void) const {
			size_t nRet = 0;
			if (this->m_val.has_field(KEY_DOC_WRITE_FAILURES)) {
				const value &v = this->m_val.at(KEY_DOC_WRITE_FAILURES);
				if ((!v.is_null()) && v.is_integer()) {
					nRet = v.as_integer();
				}
			}
			return nRet;
		}
		size_t ReplicationItem::docs_read(void) const {
			size_t nRet = 0;
			if (this->m_val.has_field(KEY_DOCS_READ)) {
				const value &v = this->m_val.at(KEY_DOCS_READ);
				if ((!v.is_null()) && v.is_integer()) {
					nRet = v.as_integer();
				}
			}
			return nRet;
		}
		size_t ReplicationItem::docs_written(void) const {
			size_t nRet = 0;
			if (this->m_val.has_field(KEY_DOCS_WRITTEN)) {
				const value &v = this->m_val.at(KEY_DOCS_WRITTEN);
				if ((!v.is_null()) && v.is_integer()) {
					nRet = v.as_integer();
				}
			}
			return nRet;
		}
		size_t ReplicationItem::end_last_seq(void) const {
			size_t nRet = 0;
			if (this->m_val.has_field(KEY_END_LAST_SEQ)) {
				const value &v = this->m_val.at(KEY_END_LAST_SEQ);
				if ((!v.is_null()) && v.is_integer()) {
					nRet = v.as_integer();
				}
			}
			return nRet;
		}
		string_t ReplicationItem::end_time(void) const {
	string_t sRet { };
			if (this->m_val.has_field(KEY_END_TIME)) {
				const value &v = this->m_val.at(KEY_END_TIME);
				if ((!v.is_null()) && v.is_string()) {
					sRet = v.as_string();
				}
			}
			return sRet;
		}
		size_t ReplicationItem::missing_checked(void) const {
			size_t nRet = 0;
			if (this->m_val.has_field(KEY_MISSING_CHECKED)) {
				const value &v = this->m_val.at(KEY_MISSING_CHECKED);
				if ((!v.is_null()) && v.is_integer()) {
					nRet = v.as_integer();
				}
			}
			return nRet;
		}
		size_t ReplicationItem::missing_found(void) const {
			size_t nRet = 0;
			if (this->m_val.has_field(KEY_MISSING_FOUND)) {
				const value &v = this->m_val.at(KEY_MISSING_FOUND);
				if ((!v.is_null()) && v.is_integer()) {
					nRet = v.as_integer();
				}
			}
			return nRet;
		}
		size_t ReplicationItem::recorded_seq(void) const {
			size_t nRet = 0;
			if (this->m_val.has_field(KEY_RECORDED_SEQ)) {
				const value &v = this->m_val.at(KEY_RECORDED_SEQ);
				if ((!v.is_null()) && v.is_integer()) {
					nRet = v.as_integer();
				}
			}
			return nRet;
		}
		string_t ReplicationItem::session_id(void) const {
	string_t sRet { };
			if (this->m_val.has_field(KEY_SESSION_ID)) {
				const value &v = this->m_val.at(KEY_SESSION_ID);
				if ((!v.is_null()) && v.is_string()) {
					sRet = v.as_string();
				}
			}
			return sRet;
		}
		size_t ReplicationItem::start_last_seq(void) const {
			size_t nRet = 0;
			if (this->m_val.has_field(KEY_START_LAST_SEQ)) {
				const value &v = this->m_val.at(KEY_START_LAST_SEQ);
				if ((!v.is_null()) && v.is_integer()) {
					nRet = v.as_integer();
				}
			}
			return nRet;
		}
		string_t ReplicationItem::start_time(void) const {
	string_t sRet { };
			if (this->m_val.has_field(KEY_START_TIME)) {
				const value &v = this->m_val.at(KEY_START_TIME);
				if ((!v.is_null()) && v.is_string()) {
					sRet = v.as_string();
				}
			}
			return sRet;
		}
		//////////////////////////////////////
		ReplicateOptions::ReplicateOptions() :cancel(false), continuous(false), create_target(false) {
		}
		ReplicateOptions::ReplicateOptions(const string_t &sSource, const string_t & sDest) : cancel(false), continuous(false), create_target(false) {
			this->source = sSource;
			this->target = sDest;
		}
		ReplicateOptions::ReplicateOptions(const ReplicateOptions &other) : DocBase(other), cancel(other.cancel), continuous(other.continuous), create_target(other.create_target),
			proxy(other.proxy), source(other.source), target(other.target), doc_ids(other.doc_ids) {

		}
		ReplicateOptions & ReplicateOptions::operator=(const ReplicateOptions &other) {
			if (this != &other) {
				DocBase::operator=(other);
				this->cancel = other.cancel;
				this->continuous = other.continuous;
				this->create_target = other.create_target;
				this->proxy = other.proxy;
				this->source = other.source;
				this->target = other.target;
				this->doc_ids = other.doc_ids;
			}
			return *this;
		}
		ReplicateOptions::~ReplicateOptions() {

		}
		void ReplicateOptions::add_docid(const string_t &id) {
			if (!id.empty()) {
				this->doc_ids.insert(id);
			}
		}
		bool ReplicateOptions::is_valid(void) const {
			return (!this->source.empty()) && (!this->target.empty());
		}
		value ReplicateOptions::to_value(void) const {
			value val = value::object();
			val[KEY_CANCEL] = value::boolean(this->cancel);
			val[KEY_CONTINIOUS] = value::boolean(this->continuous);
			val[KEY_CREATE_TARGET] = value::boolean(this->create_target);
			if (!this->proxy.empty()) {
				val[KEY_PROXY] = value::string(this->proxy);
			}
			if (!this->source.empty()) {
				val[KEY_SOURCE] = value::string(this->source);
			}
			if (!this->target.empty()) {
				val[KEY_TARGET] = value::string(this->target);
			}
			if (!this->doc_ids.empty()) {
		std::vector<value> vv { };
				for (auto it = this->doc_ids.begin(); it != this->doc_ids.end(); ++it) {
					string_t s = *it;
					if (!s.empty()) {
						vv.push_back(value::string(s));
					}
				}// it
				if (!vv.empty()) {
					val[KEY_DOCS_IDS] = value::array(vv);
				}
			}
			return val;
		}// to_value
		 ///////////////////////
	}// namespace persist
}// namespace geninfo
