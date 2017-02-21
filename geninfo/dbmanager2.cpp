//dbmanager2.cpp
//////////////////////////
#include "stdafx.h"
///////////////////
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
		static const string_t STRING_ALL_DBS = U("_all_dbs");
		static const string_t STRING_UUIDS_COUNT = U("_uuids?count=");
		static const string_t STRING_UUIDS = U("uuids");
		static const string_t STRING_INDEX_URI = U("/_index");
		static const string_t HEADER_COMMIT = U("X-Couch-Full-Commit");
		static const string_t STRING_TRUE = U("true");
		static const string_t STRING_REPLICATE = U("_replicate");
		////////////////////////////////////////////////
		pplx::task<CreateIndexResponse> CouchDBManager::create_index(const CreateIndexData &data, const string_t &db) const {
			assert(this->is_valid());
			string_t uri = (db.empty()) ? this->database() : db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			uri += STRING_INDEX_URI;
			http_client client(this->baseUrl());
			http_request request(methods::POST);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			request.headers().add(HEADER_COMMIT, STRING_TRUE);
			request.set_request_uri(uri);
			value vz = data.to_value();
			request.set_body(vz);
			return client.request(request).then([](http_response response) {
				if (response.status_code() != status_codes::OK) {
					throw http_exception(response.status_code());
				}
				else {
					return response.extract_json();
				}
			}).then([=](task<value> previousTask) {
				CreateIndexResponse oRet{ previousTask.get() };
				return task_from_result(oRet);
			});
		}// create_index
		 ///////////////////////////////////////////
		pplx::task<ReplicateResponse> CouchDBManager::replicate(const ReplicateOptions &opts) const {
			assert(this->is_valid());
			http_client client(this->baseUrl());
			http_request request(methods::POST);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			request.set_request_uri(STRING_REPLICATE);
			value val = opts.to_value();
			request.set_body(val);
			return client.request(request).then([](http_response response) {
				if ((response.status_code() == status_codes::OK) || (response.status_code() == status_codes::Accepted)) {
					return response.extract_json();
				}
				else {
					throw http_exception(response.status_code());
				}
			}).then([](task<value> previousTask) {
				ReplicateResponse oRet{ previousTask.get() };
				return task_from_result(oRet);
			});
		}// replicate
		////////////////////////////////////////////////
		task_bool CouchDBManager::exists_database(const string_t &s /* = String */) const {
			assert(this->is_valid());
			string_t uri = (s.empty()) ? this->database() : s;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			http_client client(this->baseUrl());
			http_request request(methods::HEAD);
			request.set_request_uri(uri);
			return client.request(request).then([](http_response response) {
				bool bRet {(response.status_code() == status_codes::OK)};
				if ((!bRet) && (response.status_code() != status_codes::NotFound)) {
					throw http_exception(response.status_code());
				}
				return task_from_result(bRet);
			});
		}//exists_database
		task<DatabaseInfo> CouchDBManager::get_database_info(const string_t &s /*= string_t()*/) const {
			assert(this->is_valid());
			string_t uri = (s.empty()) ? this->database() : s;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			http_client client(this->baseUrl());
			http_request request(methods::GET);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			request.set_request_uri(uri);
			return client.request(request).then([](http_response response) {
				if (response.status_code() == status_codes::OK) {
					return response.extract_json();
				}
				else {
					throw http_exception(response.status_code());
				}
			}).then([=](task<value> t1) {
				DatabaseInfo oRet{ t1.get() };
				return task_from_result(oRet);
			});
		}//get_database_info
		task_updateresponse CouchDBManager::create_database(const string_t &db) const {
			assert(this->is_valid());
			string_t uri = db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			http_client client(this->baseUrl());
			http_request request(methods::PUT);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			request.set_request_uri(uri);
			return client.request(request).then([](http_response response) {
				return response.extract_json();
			}).then([=](task<value> t1) {
				UpdateResponse oRet{ t1.get() };
				return task_from_result(oRet);
			});
		}//create_database
		task_updateresponse CouchDBManager::delete_database(const string_t &db) const {
			assert(this->is_valid());
			string_t uri = db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			http_client client(this->baseUrl());
			http_request request(methods::DEL);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			request.set_request_uri(uri);
			return client.request(request).then([](http_response response) {
				return response.extract_json();
			}).then([](task<value> t1) {
				UpdateResponse oRet{ t1.get() };
				return task_from_result(oRet);
			});
		}//delete_database
		task_stringvector CouchDBManager::get_uuids(size_t count /* = 1 */) const {
			assert(this->is_valid());
			http_client client(this->baseUrl());
			http_request request(methods::GET);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			utility::stringstream_t os{};
			if (count < 1) {
				count = 1;
			}
			os << STRING_UUIDS_COUNT << count;
			string_t s = os.str();
			request.set_request_uri(s);
			return client.request(request).then([](http_response response) {
				if (response.status_code() == status_codes::OK) {
					return response.extract_json();
				}
				else {
					throw http_exception(response.status_code());
				}
			}).then([](task<value> previousTask) {
				std::vector<string_t> oRet{};
				const value  & jsonvalue = previousTask.get();
				if ((!jsonvalue.is_null()) && jsonvalue.is_object()) {
					if (jsonvalue.has_field(STRING_UUIDS)) {
						auto v = (jsonvalue.at(STRING_UUIDS)).as_array();
						size_t n = v.size();
						for (size_t i = 0; i < n; ++i) {
							string_t ss = (v.at(i)).as_string();
							oRet.push_back(ss);
						}
					}// uuids
				}// obj
				return task_from_result(oRet);
			});
		}//get_uuids
		task_stringvector CouchDBManager::get_all_dbs(void) const {
			assert(this->is_valid());
			http_client client(this->baseUrl());
			http_request request(methods::GET);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			request.set_request_uri(STRING_ALL_DBS);
			return client.request(request).then([](http_response response) {
				if (response.status_code() == status_codes::OK) {
					return response.extract_json();
				}
				else {
					throw http_exception(response.status_code());
				}
			}).then([](task<value> previousTask) {
				std::vector<string_t> oRet{};
				const value  & jsonvalue = previousTask.get();
				if ((!jsonvalue.is_null()) && (jsonvalue.is_array())) {
					size_t n = jsonvalue.size();
					for (size_t i = 0; i < n; ++i) {
						string_t s = (jsonvalue.at(i)).as_string();
						oRet.push_back(s);
					}
				}
				return task_from_result(oRet);
			});
		}//get_all_dbs
		task<ServerInfo> CouchDBManager::get_server_info(void) const {
			assert(this->is_valid());
			http_client client(this->baseUrl());
			http_request request(methods::GET);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			return client.request(request).then([](http_response response) {
				if (response.status_code() == status_codes::OK) {
					return response.extract_json();
				}
				else {
					throw http_exception(response.status_code());
				}
			}).then([](task<value> previousTask) {
				ServerInfo pRet{ previousTask.get() };
				return task_from_result(pRet);
			});
		}// get_server_info
		///////////////////////////////////////////////
	}// namespace persist
}// namespace info
