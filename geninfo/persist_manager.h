#pragma once
#ifndef PERSIST_MANAGER_H
#define PERSIST_MANAGER_H
/////////////////////////////////////
#include "infocommon.h"
/////////////////////////////
namespace info {
	/////////////////////////////////
	using index_vector = std::vector<std::pair<string_type, bool>>;
	//////////////////////////////////
	using task_void = pplx::task<void>;
	using task_int = pplx::task<int>;
	using task_bool = pplx::task<bool>;
	using task_anymap = pplx::task<anymap_ptr>;
	using task_anymapvector = pplx::task<anymapvector_ptr>;
	using task_blob = pplx::task<blob_ptr>;
	using task_stringsvector = pplx::task<stringsvector_ptr>;
	////////////////////////////////
	class IPersistManager {
	public:
		IPersistManager() {}
		virtual ~IPersistManager() {}
		//
		virtual string_type get_persister_id(void) const = 0;
		virtual task_bool chek_index_async(const index_vector &oinds, const string_type &indexname) const = 0;
		virtual string_type get_attachment_url(const string_type &id, const string_type &name) const = 0;
		virtual task_int get_count_async(const anymap_type &oMap) const = 0;
		virtual task_anymapvector get_elements_async(const anymap_type &oModel, int offset = 0, int count = 0) const = 0;
		virtual task_anymap find_element_async(const anymap_type & oModel) const = 0;
		virtual task_anymap maintains_element_async(const anymap_type &oModel) const = 0;
		virtual task_bool remove_element_async(const anymap_type &oModel) const = 0;
		virtual task_bool maintains_elements_async(anymapvector_ptr vecPtr) const = 0;
		virtual task_bool remove_elements_async(anymapvector_ptr vecPtr) const = 0;
		virtual task_bool maintains_blob_async(const Blob &pBlob) const = 0;
		virtual task_bool remove_blob_async(const Blob &oBlob) const = 0;
		virtual task_blob read_blob_async(const Blob &oModel) const = 0;
		virtual task_stringsvector read_all_blobs_async(const string_type &id) const = 0;
		virtual task_bool delete_documents_async(const anymap_type &oModel) = 0;
	};// class IPersistManager
	//////////////////////////
}// namespace info
//////////////////////////
#endif // PERSIST_MANAGER_H
