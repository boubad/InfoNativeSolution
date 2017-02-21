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
		static const string_t HEADER_IF_MATCH = U("If-Match");
	//	static const string_t HEADER_ETAG = U("ETag");
		static const string_t HEADER_CONTENT_TYPE = U("Content-Type");
		static const string_t STRING_SLASH = U("/");
		static const string_t STRING_QUOTE = U("\"");
		///////////////////////////////////////////////
		class BlobData {
		private:
			BlobData(const BlobData &other) = delete;
			BlobData & operator=(const BlobData &other) = delete;
		public:
			string_t id;
			string_t name;
			string_t mime;
			std::vector<unsigned char> data;
			//
			BlobData(const string_t &sid, const string_t &sname, const string_t &stype, const std::vector<unsigned char> &d):
				id(sid), name(sname), mime(stype), data(d) {
			}
			~BlobData(){}
		};// class BlobData
		////////////////////////////////////////////////
		task_updateresponse CouchDBManager::maintains_attachment(const string_t &id,
			const string_t &name, const string_t &stype, const std::vector<unsigned char> &data, const string_t &db /*= string_t()*/) const {
			assert(this->is_valid());
			if (id.empty() || name.empty() || stype.empty() || data.empty()) {
				throw invalid_argument("invalid input data.");
			}
			string_t uri = (db.empty()) ? this->database() : db;
			if (uri.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			manager_ptr odata = this->get_data();
			odata->uri = db;
			std::shared_ptr<BlobData> blob = std::make_shared<BlobData>(id,name,stype,data);
			assert(blob.get() != nullptr);
			return this->exists_document(id, db).then([odata,blob](task<string_t> tt) {
				string_t rev = tt.get();
				if (rev.empty()) {
					throw std::domain_error("parent document not found");
				}
				string_t urix = (odata->uri.empty()) ? odata->database : odata->uri;
				urix = urix + STRING_SLASH + url_encode(blob->id) + STRING_SLASH + url_encode(blob->name) + string_t(U("?rev=")) + rev;
				http_request request(methods::PUT);
				request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
				string_t srev = rev;
				srev = STRING_QUOTE + srev + STRING_QUOTE;
				request.headers().add(HEADER_IF_MATCH, srev);
				request.set_request_uri(urix);
				request.set_body(blob->data);
				request.headers().add(HEADER_CONTENT_TYPE, blob->mime);
				return odata->client->request(request).then([](http_response response) {
					int code = response.status_code();
					if (code < 400) {
						return response.extract_json();
					}
					else {
						throw http_exception(code);
					}
				}).then([](task<value> previousTask) {
					UpdateResponse oRet{ previousTask.get() };
					return task_from_result(oRet);
				});
			});
		}//maintains_attachment
		task_updateresponse CouchDBManager::remove_attachment(const string_t &id,
			const string_t &name, const string_t &db /*= string_t()*/) const {
			assert(this->is_valid());
			if (id.empty() || name.empty()) {
				throw invalid_argument("invalid input data.");
			}
			string_t sbase = (db.empty()) ? this->database() : db;
			if (sbase.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			manager_ptr odata = this->get_data();
			odata->uri = sbase + STRING_SLASH + url_encode(id) + STRING_SLASH + url_encode(name);
			return this->exists_document(id, db).then([odata](task<string_t> tt) {
				string_t rev = tt.get();
				if (rev.empty()) {
					throw std::domain_error("parent document not found");
				}
				string_t urix = odata->uri;
				urix = urix + string_t(U("?rev=")) + rev;
				http_request request(methods::DEL);
				request.headers().add(ACCEPT_STRING, APPLICATION_JSON_STRING);
				string_t srev = rev;
				srev = STRING_QUOTE + srev + STRING_QUOTE;
				request.headers().add(HEADER_IF_MATCH, srev);
				request.set_request_uri(urix);
				return odata->client->request(request).then([](http_response response) {
					auto code = response.status_code();
					if (code < 500) {
						return response.extract_json();
					}
					else {
						throw http_exception(code);
					}
				}).then([](task<value> previousTask) {
					UpdateResponse oRet{ previousTask.get() };
					return task_from_result(oRet);
				});
			});

		}//remove_attachment
		pplx::task<AttachmentData> CouchDBManager::read_attachment(const string_t &id,
			const string_t &name, const string_t &db /*= string_t()*/) const {
			assert(this->is_valid());
			if (id.empty() || name.empty()) {
				throw invalid_argument("invalid input data.");
			}
			string_t sbase = (db.empty()) ? this->database() : db;
			if (sbase.empty()) {
				throw std::invalid_argument("database name is missing.");
			}
			manager_ptr odata = this->get_data();
			odata->uri = sbase + STRING_SLASH + url_encode(id) + STRING_SLASH + url_encode(name);
			return this->exists_document(id, db).then([odata](task<string_t> tt) {
				string_t rev = tt.get();
				if (rev.empty()) {
					throw std::domain_error("parent document not found");
				}
				string_t urix = odata->uri;
				urix = urix + string_t(U("?rev=")) + rev;
				http_request request(methods::GET);
				string_t srev = rev;
				srev = STRING_QUOTE + srev + STRING_QUOTE;
				request.headers().add(HEADER_IF_MATCH, srev);
				request.set_request_uri(urix);
				std::shared_ptr<AttachmentData> oAttach(new AttachmentData{});
				assert(oAttach.get() != nullptr);
				return odata->client->request(request).then([oAttach](http_response response) {
					std::vector<unsigned char >  vec{};
					AttachmentData *pData = oAttach.get();
					assert(pData != nullptr);
					auto code = response.status_code();
					if (code < 400) {
						auto headers = response.headers();
						if (headers.has(HEADER_CONTENT_TYPE)) {
							string_t sx = headers[HEADER_CONTENT_TYPE];
							pData->content_type = sx;
						}
						return response.extract_vector();
					}
					else if (code < 500) {
						return task_from_result(vec);
					}
					else {
						throw http_exception(code);
					}
				}).then([oAttach](task<std::vector<unsigned char > > previousTask) {
					AttachmentData *pData = oAttach.get();
					assert(pData != nullptr);
					std::vector<unsigned char> vec = previousTask.get();
					size_t n = vec.size();
					pData->length = n;
					if (n > 0) {
								std::shared_ptr<unsigned char> oDest {new unsigned char[n] {}};
						unsigned char *pDest = oDest.get();
						assert(pDest != nullptr);
						for (size_t i = 0; i < n; ++i) {
							pDest[i] = vec[i];
						}// i
						pData->dataPtr = oDest;
					}
					return task_from_result(*pData);
				});
			});

		}//read_attachment
		string_t CouchDBManager::get_attachment_url(const string_t &id, const string_t &name, const string_t &db /*= string_t()*/) const {
			string_t sRet = this->baseUrl();
			sRet += STRING_SLASH;
			sRet += (db.empty()) ? url_encode(this->database()) : url_encode(db);
			sRet += STRING_SLASH + url_encode(id) + STRING_SLASH + url_encode(name);
			return (sRet);
		}//get_attachment_url
		///////////////////////////////////////////////
	}// namespace persist
}// namespace info
