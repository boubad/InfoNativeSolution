//dbmanager1.cpp
//////////////////////////
#include "stdafx.h"
///////////////////
#include "couchdbmanager.h"
#include "couchdbdata.h"
/////////////////////////////////
namespace info {
	namespace persist {
		//////////////////////////////////////
		using namespace info::persist;
		using namespace utility;                    // Common utilities like string conversions
		using namespace web;                        // Common features like URIs.
		using namespace web::http;                  // Common HTTP functionality
		using namespace web::json;
		using namespace web::http::client;
		using namespace concurrency::streams;
		using namespace pplx;
		using namespace std;
		//////////////////////////////////////
		static const string_t DEFAULT_BASEURL = COUCHDB_DEFAULT_URL;
		static const string_t DEFAULT_DATABASE = COUCHDB_DEFAULT_DATABASE;
		///////////////////////////////////////////
		static const string_t STRING_QUOTE = U("\"");
		static const string_t ACCEPT_STRING = U("accept");
		static const string_t APPLICATION_JSON_STRING = U("application/json");
		static const string_t HEADER_IF_MATCH = U("If-Match");
		static const string_t HEADER_ETAG = U("ETag");
		static const string_t HEADER_COMMIT = U("X-Couch-Full-Commit");
		static const string_t STRING_TRUE = U("true");
		static const string_t STRING_ID = U("_id");
		static const string_t STRING_REV = U("_rev");
		static const string_t STRING_ATTACHMENTS = U("_attachments");
		static const string_t STRING_SLASH = U("/");
		///////////////////////////////////////////////
		CouchDBManager::CouchDBManager(const string_t &Url, const string_t &data) : _baseUrl(Url), _database(data) {
			if (this->_baseUrl.empty()) {
				this->_baseUrl = DEFAULT_BASEURL;
			}
			if (this->_database.empty()) {
				this->_baseUrl = DEFAULT_DATABASE;
			}
		}
		CouchDBManager::CouchDBManager(const string_t &Url) : CouchDBManager(Url, DEFAULT_DATABASE) {
		}
		CouchDBManager::CouchDBManager() : CouchDBManager(DEFAULT_BASEURL) {
		}
		CouchDBManager::CouchDBManager(const CouchDBManager &other) : _baseUrl(other._baseUrl), _database(other._database) {

		}
		CouchDBManager & CouchDBManager::operator=(const CouchDBManager &other) {
			if (this != &other) {
				this->_baseUrl = other._baseUrl;
				this->_database = other._database;
			}
			return *this;
		}
		CouchDBManager::~CouchDBManager() {

		}
		const string_t & CouchDBManager::baseUrl(void) const {
			return this->_baseUrl;
		}
		void CouchDBManager::baseUrl(const string_t &s) {
			this->_baseUrl = s;
		}
		const string_t & CouchDBManager::database(void) const {
			return this->_database;
		}
		void CouchDBManager::database(const string_t &s) {
			this->_database = s;
		}
		bool CouchDBManager::is_valid(void) const {
			return (!this->_baseUrl.empty());
		}
		void CouchDBManager::check_doc(const value &doc) const {
			if (doc.is_null()) {
				throw std::invalid_argument("doc is null.");
			}
			if (!doc.is_object()) {
				throw std::invalid_argument("doc is not a json object.");
			}
			const object &o = doc.as_object();
			if (o.empty()) {
				throw std::invalid_argument("doc is empty.");
			}
		}// check_doc
		CouchDBManager::manager_ptr CouchDBManager::get_data(void) const {
	manager_ptr oRet { new ManagerData { } };
			assert(oRet.get() != nullptr);
			oRet->client.reset(new http_client{ this->baseUrl() });
			assert(oRet->client.get() != nullptr);
			oRet->database = this->database();
			return (oRet);
		}// get_data
		///////////////////////////////////////////////
		task_updateresponse  CouchDBManager::create_document(const value &doc, const string_t &db /*= string_t()*/) const {
			assert(this->is_valid());
			this->check_doc(doc);
			string_t uri = (db.empty()) ? this->database() : db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			http_client client(this->baseUrl());
			http_request request(methods::POST);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			request.headers().add(HEADER_COMMIT, STRING_TRUE);
			request.set_request_uri(uri);
			request.set_body(doc);
			return client.request(request).then([](http_response response) {
				return response.extract_json();
			}).then([](task<value> previousTask) {
				UpdateResponse oRet{ previousTask.get() };
				return task_from_result(oRet);
			});
		}// create_document
		task_string CouchDBManager::exists_document(const string_t &id, const string_t &db /*= string_t()*/) const {
			assert(this->is_valid());
			if (id.empty()) {
		throw new std::invalid_argument { "invalid id value." };
			}
			string_t uri = (db.empty()) ? this->database() : db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			uri = uri + STRING_SLASH + url_encode(id);
			http_client client(this->baseUrl());
			http_request request(methods::HEAD);
			request.set_request_uri(uri);
			return client.request(request).then([](http_response response) {
				string_t sRet{};
				bool bRet {(response.status_code() == status_codes::OK) || (response.status_code() == status_codes::NotModified)};
				if (bRet) {
					auto headers = response.headers();
					if (headers.has(HEADER_ETAG)) {
						string_t sx = headers[HEADER_ETAG];
						size_t n = sx.length();
						if (n > 2) {
							sRet = sx.substr(1, n - 2);
						}
					}
				}
				else if (response.status_code() != status_codes::NotFound) {
					throw http_exception(response.status_code());
				}
				return task_from_result(sRet);
			});
		}//exists_document
		task_value  CouchDBManager::read_document(const string_t &id, const ReadDocOptions &options /*= ReadDocOptions()*/,
			const string_t &db /* = string_t()*/) const {
			assert(this->is_valid());
			if (id.empty()) {
		throw new std::invalid_argument { "invalid id value." };
			}
			string_t uri = (db.empty()) ? this->database() : db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			uri = uri + STRING_SLASH + url_encode(id);
			http_client client(this->baseUrl());
			http_request request(methods::GET);
			string_t sreq = options.toString();
			if (!sreq.empty()) {
				uri = uri + sreq;
			}
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			string_t rev = options.rev;
			if (!rev.empty()) {
				rev = STRING_QUOTE + rev + STRING_QUOTE;
				request.headers().add(HEADER_IF_MATCH, rev);
			}
			request.set_request_uri(uri);
			return client.request(request).then([](http_response response) {
				value oRet{};
				auto code = response.status_code();
				if ((code == status_codes::OK) || (code == status_codes::NotModified)) {
					return response.extract_json();
				}
				return task_from_result(oRet);
			});
		}// read_document
		task_updateresponse CouchDBManager::delete_document(const string_t &docid, const string_t &db /*= string_t()*/) const {
			assert(this->is_valid());
			if (docid.empty()) {
		throw new std::invalid_argument { "invalid id value." };
			}
			string_t uri = (db.empty()) ? this->database() : db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			uri = uri + STRING_SLASH + url_encode(docid);
			manager_ptr odata = this->get_data();
			odata->uri = uri;
			http_request request(methods::HEAD);
			request.set_request_uri(uri);
			return odata->client->request(request).then([odata](http_response response) {
				string_t rev{};
				bool bRet {(response.status_code() == status_codes::OK) || (response.status_code() == status_codes::NotModified)};
				if (bRet) {
					auto headers = response.headers();
					if (headers.has(HEADER_ETAG)) {
						rev = headers[HEADER_ETAG];
					}
				}
				else {
					throw http_exception(response.status_code());
				}
				http_request req(methods::DEL);
				req.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
				req.headers().add(HEADER_IF_MATCH, rev);
				req.headers().add(HEADER_COMMIT, STRING_TRUE);
				req.set_request_uri(odata->uri);
				return odata->client->request(req);
			}).then([](http_response response) {
				return response.extract_json();
			}).then([](task<value> previousTask) {
				UpdateResponse oRet{ previousTask.get() };
				return task_from_result(oRet);
			});
		}// delete_document
		task_updateresponse  CouchDBManager::maintains_document(value &doc, const string_t &db /*= string_t()*/) const {
			assert(this->is_valid());
			this->check_doc(doc);
			string_t uri = (db.empty()) ? this->database() : db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			if (!doc.has_field(STRING_ID)) {
				return this->create_document(doc, db);
			}
			string_t id = (doc.at(STRING_ID)).as_string();
			ReadDocOptions options{};
			options.attachments = true;
			manager_ptr odata = this->get_data();
			odata->uri = uri;
			odata->doc = doc;
			return this->read_document(id, options, db).then([odata](task<value> tval) {
				value val = tval.get();
				if (val.is_null()) {
					value vo = value::object();
					object &o = vo.as_object();
					const object &old = odata->doc.as_object();
					for (auto it = old.begin(); it != old.end(); ++it) {
						string_t key = (*it).first;
						auto c = *(key.begin());
						if (c != U('_')) {
							o[key] = (*it).second;
						}
					}// id
					http_request request(methods::POST);
					request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
					request.headers().add(HEADER_COMMIT, STRING_TRUE);
					request.set_request_uri(odata->uri);
					request.set_body(vo);
					return odata->client->request(request);
				}
				value docx{ odata->doc };
				string_t sid = (val.at(STRING_ID)).as_string();
				string_t rev = (val.at(STRING_REV)).as_string();
				docx[STRING_REV] = val.at(STRING_REV);
				docx[STRING_ID] = value{ sid };
				if (val.has_field(STRING_ATTACHMENTS)) {
					docx[STRING_ATTACHMENTS] = val.at(STRING_ATTACHMENTS);
				}
				string_t suri = odata->uri + STRING_SLASH + url_encode(sid) + U("?rev=") + rev;
				http_request request(methods::PUT);
				string_t srev = STRING_QUOTE + rev + STRING_QUOTE;
				request.headers().add(HEADER_IF_MATCH, srev);
				request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
				request.headers().add(HEADER_COMMIT, STRING_TRUE);
				request.set_request_uri(suri);
				request.set_body(docx);
				return odata->client->request(request);
			}).then([](http_response response) {
				return response.extract_json();
			}).then([](task<value> previousTask) {
				UpdateResponse oRet{ previousTask.get() };
				return task_from_result(oRet);
			});
		}// maintains_document
		////////////////////////////////////////////
		task_updatevector CouchDBManager::maintains_documents(value_vector &oVal, bool bDelete /*= false */,
			const string_t &db /*= string_t()*/) const {
			std::vector<task_updateresponse> oTasks{};
			if (!bDelete) {
				for (auto it = oVal.begin(); it != oVal.end(); ++it) {
					oTasks.push_back(this->maintains_document(*it, db));
				}// it
			}
			else {
				for (auto it = oVal.begin(); it != oVal.end(); ++it) {
					auto v = *it;
					if (v.has_field(STRING_ID)) {
						string_t id = (v.at(STRING_ID)).as_string();
						if (!id.empty()) {
							oTasks.push_back(this->delete_document(id, db));
						}
					}
				}// it
			}
			if (oTasks.empty()) {
				std::vector < UpdateResponse> ox{};
				return pplx::task_from_result(ox);
			}
			return pplx::when_all(oTasks.begin(), oTasks.end());
		}//maintains_docs
		//////////////////////////////
	}// namespace persist
}// namespace info
