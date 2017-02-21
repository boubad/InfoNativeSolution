//couchdbmanager.h
/////////////////////////
#include "stdafx.h"
#include "couchdbmanager.h"
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
		static const string_t ACCEPT_STRING = U("accept");
		static const string_t APPLICATION_JSON_STRING = U("application/json");
		static const string_t HEADER_COMMIT = U("X-Couch-Full-Commit");
		static const string_t STRING_TRUE = U("true");
		static const string_t STRING_ID = U("_id");
		static const string_t STRING_REV = U("_rev");
		static const string_t STRING_FIND = U("/_find");
		static const string_t STRING_DELETED = U("_deleted");
		///////////////////////////////////////////////
		task_int CouchDBManager::get_docs_count(const std::map<string_t, value> &oMap, const string_t &db /*= string_t()*/) const {
			assert(this->is_valid());
			string_t uri = (db.empty()) ? this->database() : db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			const std::map<string_t, value> *pMap = &oMap;
			return task_int{ [this,pMap,db]()->int {
				FindOptions oBase{};
				for (auto it = pMap->begin(); it != pMap->end(); ++it) {
					string_t key = (*it).first;
					value v = (*it).second;
					oBase.add_fetch_item(key, v);
				}
				oBase.add_return_field(STRING_ID);
				int nRet = 0;
				const int nCount = 128;
				oBase.set_limit(nCount);
				string_t urix = (db.empty()) ? this->database() : db;
				urix += STRING_FIND;
				http_client client(this->baseUrl());
				bool done{ false };
				int offset{ 0 };
				while (!done) {
					FindOptions sel{ oBase };
					sel.set_skip(offset);
					http_request request(methods::POST);
					request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
					request.headers().add(HEADER_COMMIT, STRING_TRUE);
					request.set_request_uri(urix);
					value vz = sel.to_value();
					request.set_body(vz);
					int nx = client.request(request).then([](http_response response) {
						return response.extract_json();
					}).then([](task<value> previousTask) {
						FindResponse r(previousTask.get());
						int n = static_cast<int>(r.size());
						return (n);
					}).get();
					if (nx < 1) {
						done = true;
						break;
					}
					nRet += nx;
					if (nx < nCount) {
						done = true;
						break;
					}
					else {
						offset += nx;
					}
				}// not done
				return (nRet);
			}
			};
		}// get_docs_count
		task_valuevector CouchDBManager::read_docs(const std::map<string_t, value> &oMap, int skip, int count, const string_t &db) const {
			assert(this->is_valid());
			string_t uri = (db.empty()) ? this->database() : db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			FindOptions oBase{};
			for (auto it = oMap.begin(); it != oMap.end(); ++it) {
				string_t key = (*it).first;
				value v = (*it).second;
				oBase.add_fetch_item(key, v);
			}
			if (skip > 0) {
				oBase.set_skip(skip);
			}
			if (count > 0) {
				oBase.set_limit(count);
			}
			uri += STRING_FIND;
			http_client client(this->baseUrl());
			http_request request(methods::POST);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			request.headers().add(HEADER_COMMIT, STRING_TRUE);
			request.set_request_uri(uri);
			value vz = oBase.to_value();
			request.set_body(vz);
			return client.request(request).then([](http_response response) {
				if (response.status_code() == status_codes::OK) {
					return response.extract_json();
				}
				else {
					throw http_exception(response.status_code());
				}
			}).then([](task<value> previousTask) {
				std::vector<value> oRet{};
				const value  &jsonvalue = previousTask.get();
				FindResponse r(jsonvalue);
				oRet = r.documents();
				return task_from_result(oRet);
			});
		}//read_docs
		task_bool CouchDBManager::delete_documents(const std::map<string_t, value> &oMap, const string_t &db /*= string_t()*/) const {
			assert(this->is_valid());
			const std::map<string_t, value> *pMap = &oMap;
			return task_bool{ [this,pMap,db]()->bool {
				string_t uri = (db.empty()) ? this->database() : db;
				if (uri.empty()) {
					throw std::invalid_argument("database name is missing.");
				}
				uri += STRING_FIND;
				http_client client(this->baseUrl());
				FindOptions oBase{};
				oBase.add_return_field(STRING_ID);
				oBase.add_return_field(STRING_REV);
				for (auto it = pMap->begin(); it != pMap->end(); ++it) {
					string_t key = (*it).first;
					value v = (*it).second;
					oBase.add_fetch_item(key, v);
				}//it
				int nMax{ 128 };
				oBase.set_limit(nMax);
				int offset{ 0 };
				bool done = false;
				while (!done) {
					oBase.set_skip(offset);
					http_request request(methods::POST);
					request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
					request.headers().add(HEADER_COMMIT, STRING_TRUE);
					request.set_request_uri(uri);
					value vz = oBase.to_value();
					request.set_body(vz);
					int nx = client.request(request).then([](http_response response) {
						if (response.status_code() == status_codes::OK) {
							return response.extract_json();
						}
						else {
							throw http_exception(response.status_code());
						}
					}).then([this,db](task<value> previousTask) {
						std::vector<value> oRet{};
						FindResponse r(previousTask.get());
						value vDeleted(true);
						for (auto it = r.begin(); it != r.end(); ++it) {
							const value &v = *it;
							if (v.has_field(STRING_ID) && v.has_field(STRING_REV)) {
								value vx = value::object();
								object &oo = vx.as_object();
								oo[STRING_ID] = v.at(STRING_ID);
								oo[STRING_REV] = v.at(STRING_REV);
								oo[STRING_DELETED] = vDeleted;
								oRet.push_back(vx);
							}// v
						}// it
						return this->bulk_docs(oRet, db);
					}).then([](task_updatevector vt){
						std::vector<UpdateResponse> vv = vt.get();
						int nRet = vv.size();
						return (nRet);
					}).get();
					if (nx < nMax) {
						done = true;
						break;
					}
					else {
						offset += nx;
					}
				}// not done
				return (true);
			} };
		}//delete_documents
		task_stringvector CouchDBManager::get_docs_ids(const std::map<string_t, value> &oMap, int skip, int count, const string_t &db) const {
			assert(this->is_valid());
			string_t uri = (db.empty()) ? this->database() : db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			FindOptions oBase{};
			oBase.add_return_field(STRING_ID);
			for (auto it = oMap.begin(); it != oMap.end(); ++it) {
				string_t key = (*it).first;
				value v = (*it).second;
				oBase.add_fetch_item(key, v);
			}
			if (skip > 0) {
				oBase.set_skip(skip);
			}
			if (count > 0) {
				oBase.set_limit(count);
			}
			uri += STRING_FIND;
			http_client client(this->baseUrl());
			http_request request(methods::POST);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			request.headers().add(HEADER_COMMIT, STRING_TRUE);
			request.set_request_uri(uri);
			value vz = oBase.to_value();
			request.set_body(vz);
			return client.request(request).then([](http_response response) {
				if (response.status_code() == status_codes::OK) {
					return response.extract_json();
				}
				else {
					throw http_exception(response.status_code());
				}
			}).then([](task<value> previousTask) {
				std::vector<string_t> oRet{};
				FindResponse r(previousTask.get());
				for (auto it = r.begin(); it != r.end(); ++it) {
					const value &v = *it;
					if (v.has_field(STRING_ID)) {
						string_t id = v.at(STRING_ID).as_string();
						oRet.push_back(id);
					}// v
				}// it
				return task_from_result(oRet);
			});
		}//get_docs_ids
		 /////////////////////////////////////
		pplx::task<std::vector<value>> CouchDBManager::read_docs(const FindOptions &sel,
			const string_t &db /*=string_t()*/) const {
			assert(this->is_valid());
			string_t uri = (db.empty()) ? this->database() : db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			value vz = sel.to_value();
			if (vz.is_null()) {
				throw std::invalid_argument("invalid find selector.");
			}
			uri += STRING_FIND;
			http_client client(this->baseUrl());
			http_request request(methods::POST);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			request.headers().add(HEADER_COMMIT, STRING_TRUE);
			request.set_request_uri(uri);
			request.set_body(vz);
			return client.request(request).then([](http_response response) {
				if (response.status_code() == status_codes::OK) {
					return response.extract_json();
				}
				else {
					throw http_exception(response.status_code());
				}
			}).then([](task<value> previousTask) {
				std::vector<value> oRet;
				FindResponse r(previousTask.get());
				oRet = r.documents();
				return task_from_result(oRet);
			});
		}//read_docs
		///////////////////////
	}// namespace persist
}// namespace geninfo
 /////////////////////////////////////
