#pragma once
/////////////////////////////
#ifndef COUCHDBMANAGER_H
#define COUCHDBMANAGER_H
//////////////////////////////////
#include "couchdoc.h"
#include "replicate.h"
#include "findselector.h"
///////////////////////////
namespace info {
	namespace persist {
		////////////////////////////
		using stringt_vector = std::vector<string_t>;
		using value_vector = std::vector<value>;
		///////////////////////////////
		using task_serverinfo = pplx::task<ServerInfo>;
		using task_updateresponse = pplx::task<UpdateResponse>;
		using task_updatevector = pplx::task<std::vector<UpdateResponse>>;
		using task_stringvector = pplx::task<stringt_vector>;
		using task_bool = pplx::task<bool>;
		using task_databaseinfo = pplx::task<DatabaseInfo>;
		using task_alldocsresponse = pplx::task<AllDocsResponse>;
		using task_value = pplx::task<value>;
		using task_valuevector = pplx::task<value_vector>;
		using task_stringt = pplx::task<string_t>;
		using task_replicate = pplx::task<ReplicateResponse>;
		using task_attachment = pplx::task<AttachmentData>;
		using task_createindex = pplx::task<CreateIndexResponse>;
		using task_int = pplx::task<int>;
		using task_string = pplx::task<string_t>;
		//////////////////////////////
		class CouchDBManager
		{
		public:
			class ManagerData {
			private:
				ManagerData(const ManagerData &other) = delete;
				ManagerData & operator=(const ManagerData &other) = delete;
			public:
				std::shared_ptr<web::http::client::http_client> client;
				string_t  database;
				string_t  uri;
				web::json::value doc;
				//
				ManagerData() {}
				~ManagerData() {}
			};// class ManagerData
		public:
			using manager_ptr = std::shared_ptr<ManagerData>;
		public:
			CouchDBManager();
			CouchDBManager(const string_t &baseUrl);
			CouchDBManager(const string_t &baseUrl, const string_t &data);
			CouchDBManager(const CouchDBManager &other);
			CouchDBManager & operator=(const CouchDBManager &other);
			virtual ~CouchDBManager();
		private:
			void check_doc(const value &v) const;
			manager_ptr get_data(void) const;
		public:
			bool is_valid(void) const;
			const string_t & baseUrl(void) const;
			void baseUrl(const string_t &s);
			const string_t & database(void) const;
			void database(const string_t &s);
			string_t get_attachment_url(const string_t &id, const string_t &name, const string_t &db = string_t()) const;
		public:
			task_serverinfo get_server_info(void) const;
			task_stringvector get_all_dbs(void) const;
			task_stringvector get_uuids(size_t count = 1) const;
			task_bool exists_database(const string_t &s = string_t()) const;
			task_databaseinfo get_database_info(const string_t &s = string_t()) const;
			task_updateresponse create_database(const string_t &db) const;
			task_updateresponse delete_database(const string_t &db) const;
			//
			task_stringt exists_document(const string_t &id, const string_t &db = string_t()) const;
			task_updateresponse create_document(const value &doc, const string_t &db = string_t()) const;
			task_value read_document(const string_t &id, const ReadDocOptions &options = ReadDocOptions(), const string_t &db = string_t()) const;
			task_updateresponse  maintains_document(value &doc, const string_t &db = string_t()) const;
			task_updateresponse  delete_document(const string_t &docid, const string_t &db = string_t()) const;
			task_updatevector maintains_documents(value_vector &oVal, bool bDelete = false, const string_t &db = string_t()) const;
			//
			task_alldocsresponse get_all_docs(const AllDocsOptions &options = AllDocsOptions(), const string_t &db = string_t()) const;
			task_stringvector get_all_docs_ids(const AllDocsOptions &options = AllDocsOptions(), const string_t &db = string_t()) const;
			task_alldocsresponse get_array_docs(const stringt_vector &keys, const AllDocsOptions &options = AllDocsOptions(), const string_t &db = string_t()) const;
			task_valuevector get_array_docs_values(const stringt_vector &keys, const AllDocsOptions &options = AllDocsOptions(), const string_t &db = string_t()) const;
			task_updatevector bulk_docs(const value_vector &oVal, const string_t &db = string_t()) const;

			//
			task_replicate replicate(const ReplicateOptions &opts) const;
			task_createindex create_index(const CreateIndexData &data, const string_t &database = string_t()) const;
			//
			task_int  get_docs_count(const std::map<string_t, value> &oMap, const string_t &db = string_t()) const;
			task_stringvector  get_docs_ids(const std::map<string_t, value> &oMap, int skip = 0, int count = 0, const string_t &db = string_t()) const;
			task_valuevector read_docs(const std::map<string_t, value> &oMap, int skip = 0, int count = 0, const string_t &db = string_t()) const;
			task_valuevector read_docs(const FindOptions &sel, const string_t &db = string_t()) const;
			//
			task_updateresponse maintains_attachment(const string_t &id,
				const string_t &name, const string_t &stype, const std::vector<unsigned char> &data, const string_t &database = string_t()) const;
			task_updateresponse remove_attachment(const string_t &id,
				const string_t &name, const string_t &database = string_t()) const;
			task_attachment read_attachment(const string_t &id,
				const string_t &name, const string_t &database = string_t()) const;
			//
			task_bool delete_documents(const std::map<string_t, value>&oMap, const string_t &database = string_t()) const;
		private:
			string_t _baseUrl;
			string_t _database;
		}; // class CouchDBLanager
	}// namespace persist
}// namespace couchdb
 /////////////////////////////////
#endif // __COUCHDBMANAGER_H__
