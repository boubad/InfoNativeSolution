#pragma once
#ifndef DOMAINPERSISTMANAGER_H
#define DOMAINPERSISTMANAGER_H
///////////////////////////////
#include "infocommon.h"
#include "persist_manager.h"
#include "infoitem.h"
////////////////////////////////////
namespace info {
namespace domain {
//////////////////////////////
class ItemFactory;
////////////////////////////////
using task_item = pplx::task<item_ptr>;
using task_itemvector = pplx::task<itemptrvector_ptr>;
///////////////////////////
class DomainPersistManager {
private:
	DomainPersistManager(const DomainPersistManager &other) = delete;
	DomainPersistManager & operator=(const DomainPersistManager &other) = delete;
private:
	IPersistManager *m_pman;
public:
	DomainPersistManager(IPersistManager *pMan = nullptr);

	virtual ~DomainPersistManager();
public:
	bool is_valid(void) const;
	IPersistManager *get_persist_manager(void) const;
	void set_persist_manager(IPersistManager *pMan);
public:
	task_int get_count_async(const InfoItem &oModel) const;
	task_itemvector get_elements_async(const InfoItem &oModel, int offset = 0,
			int count = 0) const;
	task_item find_element_async(const InfoItem &oModel) const;
	task_item maintains_element_async(const InfoItem &oModel) const;
	task_bool remove_element_async(const InfoItem &oModel) const;
	task_bool maintains_elements_async(itemptrvector_ptr vecPtr) const;
	task_bool remove_elements_async(itemptrvector_ptr vecPtr) const;
	task_bool maintains_blob_async(const Blob &oBlob) const;
	task_bool remove_blob_async(const Blob &pBlob) const;
	task_blob read_blob_async(const Blob &oModel) const;
	task_stringsvector read_all_blobs_async(const InfoItem &oItem) const;
	task_bool delete_documents_async(const anymap_type &oModel) const;
};
// class ItemPersistManager
//////////////////////////
extern task_bool info_import_etudiants(istream_t &in, DomainPersistManager *pMan);
extern task_bool info_import_etudiants_file(const string_type &filename, DomainPersistManager *pMan);
/////////////////////////////
}// namespace domain
} // namespace inf
////////////////////////////////
#endif // DOMAINPERSISTMANAGER_H
