/*
 * couchddpersistmanager.h
 *
 *  Created on: 26 janv. 2017
 *      Author: boubad
 */
#pragma once
#ifndef COUCHDDPERSISTMANAGER_H_
#define COUCHDDPERSISTMANAGER_H_
 ////////////////////////
#include "persist_manager.h"
////////////////////
namespace info {
	namespace persist {
		/////////////////////////
		class CouchDBManager;
		/////////////////////////
		class CouchDBPersistManager : public IPersistManager {
		private:
			string_type m_id;
			string_type m_database;
			std::shared_ptr<CouchDBManager> m_man;
			//
			CouchDBManager *get_manager(void) const;
			CouchDBPersistManager(const CouchDBPersistManager &other) = delete;
			CouchDBPersistManager & operator=(const CouchDBPersistManager &other) = delete;
		public:
			CouchDBPersistManager(const string_type &url, const string_type &db);
			virtual ~CouchDBPersistManager();
			bool is_valid(void) const;
		public:
			virtual string_type get_persister_id(void) const;
			virtual task_bool chek_index_async(const index_vector &oinds, const string_type &indexname) const;
			virtual string_type get_attachment_url(const string_type &id,
				const string_type &name) const;
			virtual task_int get_count_async(const anymap_type &oMap) const;
			virtual task_anymapvector get_elements_async(const anymap_type &oModel,
				int offset = 0, int count = 0) const;
			virtual task_anymap find_element_async(const anymap_type & oModel) const;
			virtual task_anymap maintains_element_async(
				const anymap_type &oModel) const;
			virtual task_bool remove_element_async(const anymap_type &oModel) const;
			virtual task_bool maintains_elements_async(anymapvector_ptr vecPtr) const;
			virtual task_bool remove_elements_async(anymapvector_ptr vecPtr) const;
			virtual task_bool maintains_blob_async(const Blob &pBlob) const;
			virtual task_bool remove_blob_async(const Blob &oBlob) const;
			virtual task_blob read_blob_async(const Blob &oModel) const;
			virtual task_stringsvector read_all_blobs_async(
				const string_type &id) const;
			virtual task_bool delete_documents_async(const anymap_type &oModel);
		};
		////////////
	}// namespace persist
} // namespace info
//////////////////////
#endif /* COUCHDDPERSISTMANAGER_H_ */
