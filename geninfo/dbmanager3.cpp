// dbmanager3.cpp
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
		static const string_t STRING_SLASH = U("/");
		static const string_t STRING_ALL_DOCS = U("_all_docs");
		static const string_t STRING_KEYS = U("keys");
		static const string_t STRING_DOCS = U("docs");
		//static const string_t STRING_DOC = U("doc");
		static const string_t STRING_BULK_DOCS = U("_bulk_docs");
		///////////////////////////////////////////////
		task_stringvector  CouchDBManager::get_all_docs_ids(const AllDocsOptions &options /*= AllDocsOptions()*/,
			const string_t &db /*= string_t()*/) const {
			assert(this->is_valid());
			string_t uri = (db.empty()) ? this->database() : db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			uri = uri + STRING_SLASH + STRING_ALL_DOCS;
			http_client client(this->baseUrl());
			http_request request(methods::GET);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			string_t sreq = options.toString();
			if (!sreq.empty()) {
				uri = uri + sreq;
			}
			request.set_request_uri(uri);
			return client.request(request).then([](http_response response) {
				if (response.status_code() == status_codes::OK) {
					return response.extract_json();
				}
				else {
					throw http_exception(response.status_code());
				}
			}).then([](task<value> previousTask) {
		std::vector<string_t> oRet {};
				const value  & jsonvalue = previousTask.get();
				AllDocsResponse rsp = AllDocsResponse(jsonvalue);
				auto vv = rsp.rows();
				for (auto it = vv.begin(); it != vv.end(); ++it) {
					const AllDocsItem &vx = *it;
					string_t s = vx.id();
					if (!s.empty()) {
						oRet.push_back(s);
					}
				}// it
				return task_from_result(oRet);
			});
		}//get_all_docs_ids
		task<AllDocsResponse>  CouchDBManager::get_all_docs(const AllDocsOptions &options /*= AllDocsOptions()*/,
			const string_t &db /*= string_t()*/) const {
			assert(this->is_valid());
			string_t uri = (db.empty()) ? this->database() : db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			uri = uri + STRING_SLASH + STRING_ALL_DOCS;
			http_client client(this->baseUrl());
			http_request request(methods::GET);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			string_t sreq = options.toString();
			if (!sreq.empty()) {
				uri = uri + sreq;
			}
			request.set_request_uri(uri);
			return client.request(request).then([](http_response response) {
				if (response.status_code() == status_codes::OK) {
					return response.extract_json();
				}
				else {
					throw http_exception(response.status_code());
				}
			}).then([](task<value> previousTask) {
				AllDocsResponse oRet{ previousTask.get() };
				return task_from_result(oRet);
			});
		}//get_all_docs
		task_valuevector CouchDBManager::get_array_docs_values(const stringt_vector  &keys,
			const AllDocsOptions &options /*= AllDocsOptions()*/,
			const string_t &db /*= string_t()*/) const {
			assert(this->is_valid());
			string_t uri = (db.empty()) ? this->database() : db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			std::vector<value> vx{};
			size_t nx = keys.size();
			for (size_t i = 0; i < nx; ++i) {
				string_t s = keys[i];
				if (!s.empty()) {
					value vw(s);
					vx.push_back(vw);
				}
			}// i
			if (vx.empty()) {
		std::vector<value> ox { };
				return task_from_result(ox);
			}
			value oAr = value::array(vx);
			value doc = value::object();
			doc[STRING_KEYS] = oAr;
			uri = uri + STRING_SLASH + STRING_ALL_DOCS;
			http_client client(this->baseUrl());
			http_request request(methods::POST);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			AllDocsOptions oo{ options };
			oo.include_docs = true;
			string_t sreq = options.toString();
			if (!sreq.empty()) {
				uri = uri + sreq;
			}
			request.set_request_uri(uri);
			request.set_body(doc);
			return client.request(request).then([](http_response response) {
				if (response.status_code() == status_codes::OK) {
					return response.extract_json();
				}
				else {
					throw http_exception(response.status_code());
				}
			}).then([](task<value> previousTask) {
				std::vector<value> oRet;

				const value  &jsonvalue = previousTask.get();
				AllDocsResponse rsp = AllDocsResponse(jsonvalue);
				auto vv = rsp.rows();
				for (auto it = vv.begin(); it != vv.end(); ++it) {
					const AllDocsItem &vx = *it;
					value val = vx.doc();
					if ((!val.is_null()) && val.is_object()) {
						oRet.push_back(val);
					}
				}// it
				return task_from_result(oRet);
			});
		}//get_array_docs_values
		task_alldocsresponse CouchDBManager::get_array_docs(const stringt_vector &keys,
			const AllDocsOptions &options /*= AllDocsOptions() */, const string_t &db /*= String()*/) const {
			assert(this->is_valid());
			string_t uri = (db.empty()) ? this->database() : db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			value_vector vx{};
			size_t nx = keys.size();
			for (size_t i = 0; i < nx; ++i) {
				string_t s = keys[i];
				if (!s.empty()) {
					value vw{ s };
					vx.push_back(vw);
				}
			}// i
			if (vx.empty()) {
				AllDocsResponse ox{};
				return task_from_result(ox);
			}
			value oAr = value::array(vx);
			value doc = value::object();
			doc[STRING_KEYS] = oAr;
			uri = uri + STRING_SLASH + STRING_ALL_DOCS;
			http_client client(this->baseUrl());
			http_request request(methods::POST);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			string_t sreq = options.toString();
			if (!sreq.empty()) {
				uri = uri + sreq;
			}
			request.set_request_uri(uri);
			request.set_body(doc);
			return client.request(request).then([](http_response response) {
				if (response.status_code() == status_codes::OK) {
					return response.extract_json();
				}
				else {
					throw http_exception(response.status_code());
				}
			}).then([](task<value> previousTask) {
				AllDocsResponse oRet{ previousTask.get() };
				return task_from_result(oRet);
			});
		}//get_array_docs
		task_updatevector CouchDBManager::bulk_docs(const value_vector &oVal, const string_t &db /*= string_t()*/) const {
			if (oVal.empty()) {
				std::vector<UpdateResponse> x{};
				return task_from_result(x);
			}
			assert(this->is_valid());
			string_t uri = (db.empty()) ? this->database() : db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			uri = uri + STRING_SLASH + STRING_BULK_DOCS;
			http_client client(this->baseUrl());
			http_request request(methods::POST);
			request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
			request.set_request_uri(uri);
			value oAr = value::array(oVal);
			value doc = value::object();
			doc[STRING_DOCS] = oAr;
			request.set_body(doc);
			return client.request(request).then([](http_response response) {
				if (response.status_code() == status_codes::Created) {
					return response.extract_json();
				}
				throw http_exception(response.status_code());
			}).then([](task<value> previousTask) {
				std::vector<UpdateResponse> oRet{};
				const value & jsonvalue = previousTask.get();
				if ((!jsonvalue.is_null()) && jsonvalue.is_array()) {
					size_t n = jsonvalue.size();
					for (size_t i = 0; i < n; ++i) {
						const value &v = jsonvalue.at(i);
						UpdateResponse inf(v);
						oRet.push_back(inf);
					}// i
				}// array
				return task_from_result(oRet);
			});
		}//:bulk_docs
		//////////////////////////////////////////////

	}// namespace persist
}// namespace info
