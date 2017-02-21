//domainpersistmanager.cpp
///////////////////////////////
#include "stdafx.h"
#include "domainpersistmanager.h"
///////////////////////////////
#include "infostrings.h"
////////////////////////
#include "itemfactory.h"
////////////////////////////////////
namespace info {
	namespace domain {
		//////////////////////////////
		namespace internal {
			std::atomic<bool> st_m_initialized{ false };
			static std::mutex st_m_mutex{ };
			//////////////////////////////////
			static task_bool internal_check_index(IPersistManager *pMan,
				const string_type &field, const string_type &name) {
				index_vector inds = { std::make_pair(field, true) };
				return pMan->chek_index_async(inds, name);
			} //internal_check_index
			static void perform_check_indexes(IPersistManager *pMan) {
				if (st_m_initialized) {
					return;
				}
				{
					assert(pMan != nullptr);
					std::lock_guard<std::mutex> oLock{ st_m_mutex };
					std::vector<task_bool> oTasks{ };

					oTasks.push_back(internal_check_index(pMan, KEY_TYPE, string_type{
							U("i_type") }));
					oTasks.push_back(internal_check_index(pMan, KEY_SIGLE, string_type{
							U("i_sigle") }));
					oTasks.push_back(internal_check_index(pMan, KEY_STATUS, string_type{
							U("i_status") }));
					oTasks.push_back(internal_check_index(pMan, KEY_ANNEE, string_type{
						U("i_annee") }));
					oTasks.push_back(
						internal_check_index(pMan, KEY_DATASETSIGLE, string_type{
								U("i_datasetsigle") }));
					oTasks.push_back(
						internal_check_index(pMan, KEY_VARIABLESIGLE, string_type{
								U("i_varsigle") }));
					oTasks.push_back(
						internal_check_index(pMan, KEY_INDIVSIGLE, string_type{
								U("i_indivsigle") }));
					oTasks.push_back(internal_check_index(pMan, KEY_DOSSIER, string_type{
							U("i_dossier") }));
					oTasks.push_back(internal_check_index(pMan, KEY_FIRSTNAME, string_type{
							U("i_firstname") }));
					oTasks.push_back(internal_check_index(pMan, KEY_LASTNAME, string_type{
							U("i_lastname") }));
					oTasks.push_back(internal_check_index(pMan, KEY_BIRTHYEAR, string_type{
							U("i_birthyear") }));
					oTasks.push_back(internal_check_index(pMan, KEY_SEXE, string_type{
							U("i_sexe") }));
					oTasks.push_back(internal_check_index(pMan, KEY_SERIEBAC, string_type{
							U("i_seriebac") }));
					oTasks.push_back(internal_check_index(pMan, KEY_OPTIONBAC, string_type{
							U("i_optionbac") }));
					oTasks.push_back(internal_check_index(pMan, KEY_SUP, string_type{
							U("i_sup") }));
					oTasks.push_back(internal_check_index(pMan, KEY_APB, string_type{
							U("i_apb") }));
					oTasks.push_back(
						internal_check_index(pMan, KEY_REDOUBLANT, string_type{
								U("i_redoublant") }));
					pplx::when_all(oTasks.begin(), oTasks.end()).wait();
					st_m_initialized.store(true);
				} // sync
			} // perform_check_indexes
		} // namespace internal
		///////////////////////////
		DomainPersistManager::DomainPersistManager(IPersistManager *pMan) :
			m_pman{ pMan } {
			if (m_pman != nullptr) {
				internal::perform_check_indexes(m_pman);
			}
		}
		DomainPersistManager::~DomainPersistManager() {

		}
		bool DomainPersistManager::is_valid(void) const {
			return (this->m_pman != nullptr);
		}
		IPersistManager *DomainPersistManager::get_persist_manager(void) const {
			return (this->m_pman);
		}
		void DomainPersistManager::set_persist_manager(IPersistManager *pMan) {
			this->m_pman = pMan;
			if (m_pman != nullptr) {
				internal::perform_check_indexes(m_pman);
			}
		}
		////////////////////////////
		task_int DomainPersistManager::get_count_async(const InfoItem &oModel) const {
			assert(this->is_valid());
			const InfoItem *pModel = &oModel;
			return task_int{ [this, pModel]()->int {
				anymap_type oMap{};
				pModel->to_map(oMap);
				return this->m_pman->get_count_async(oMap).get();
			} };
		}//get_count_async
		task_itemvector DomainPersistManager::get_elements_async(const InfoItem &oModel,
			int offset/* = 0*/, int count /*= 0*/) const {
			assert(this->is_valid());
			const InfoItem *pModel = &oModel;
			return task_itemvector{ [this,pModel,offset,count]()->itemptrvector_ptr {
				itemptrvector_ptr oRet{};
				anymap_type oMap{};
				pModel->to_map(oMap);
				anymapvector_ptr vv = this->m_pman->get_elements_async(oMap, offset, count).get();
				anymap_vector *pvec = vv.get();
				if (pvec != nullptr) {
					oRet.reset(new itemptr_vector{});
					itemptr_vector *pRet = oRet.get();
					assert(pRet != nullptr);
					for (auto it = pvec->begin(); it != pvec->end(); ++it) {
						item_ptr p{ ItemFactory::create(*it) };
						if (p.get() != nullptr) {
							pRet->push_back(p);
						}
					}		// it
				}		// pvec
				return (oRet);
			} };
		}//get_elements_async
		task_item DomainPersistManager::find_element_async(
			const InfoItem &oModel) const {
			assert(this->is_valid());
			const InfoItem *pModel = &oModel;
			return task_item{ [this,pModel]()->item_ptr {
				anymap_type oMap{};
				pModel->to_map(oMap);
				item_ptr oRet = this->m_pman->find_element_async(oMap).then([](anymap_ptr p) {
					if (p.get() != nullptr) {
						return ItemFactory::create(*p);
					}
					return  item_ptr{};
				}).get();
				return (oRet);
			} };
		}//find_element_async
		task_item DomainPersistManager::maintains_element_async(
			const InfoItem &oModel) const {
			assert(this->is_valid());
			const InfoItem *pModel = &oModel;
			return task_item{ [this,pModel]()->item_ptr {
				item_ptr oRet {};
				anymap_type oMap {};
				pModel->to_map(oMap);
				anymap_ptr p = this->m_pman->maintains_element_async(oMap).get();
				if (p.get() != nullptr) {
					oRet = ItemFactory::create(*p);
				}
				return (oRet);
			} };
		}//maintains_element_async
		task_bool DomainPersistManager::remove_element_async(
			const InfoItem &oModel) const {
			assert(this->is_valid());
			const InfoItem *pModel = &oModel;
			return task_bool{ [this,pModel]()->bool {
				anymap_type oMap {};
				pModel->to_map(oMap);
				bool b = m_pman->remove_element_async(oMap).get();
				return (b);
			} };
		}//remove_element_async
		task_bool DomainPersistManager::maintains_elements_async(
			itemptrvector_ptr vecPtr) const {
			assert(this->is_valid());
			assert(vecPtr.get() != nullptr);
			IPersistManager *pMan = get_persist_manager();
			return task_bool{ [pMan,vecPtr]()->bool {
				anymapvector_ptr vv {new anymap_vector {}};
				anymap_vector *px = vv.get();
				assert(px != nullptr);
				for (auto it = vecPtr->begin(); it != vecPtr->end(); ++it) {
					item_ptr o = *it;
					InfoItem *x = o.get();
					if ((x != nullptr) && x->is_storeable()) {
						anymap_type m {};
						x->to_map(m);
						px->push_back(m);
					}
				}		// it
					return pMan->maintains_elements_async(vv).get();
				} };
		}//maintains_elements_async
		task_bool DomainPersistManager::remove_elements_async(
			itemptrvector_ptr vecPtr) const {
			assert(this->is_valid());
			assert(vecPtr.get() != nullptr);
			IPersistManager *pMan = get_persist_manager();
			return task_bool{ [pMan,vecPtr]()->bool {
				anymapvector_ptr vv {new anymap_vector {}};
				anymap_vector *px = vv.get();
				assert(px != nullptr);
				for (auto it = vecPtr->begin(); it != vecPtr->end(); ++it) {
					item_ptr o = *it;
					InfoItem *x = o.get();
					if ((x != nullptr) && x->has_id()) {
						anymap_type m {};
						x->to_map(m);
						px->push_back(m);
					}
				}		// it
					return pMan->remove_elements_async(vv).get();
				} };
		}//remove_elements_async
		task_bool DomainPersistManager::maintains_blob_async(const Blob &oBlob) const {
			assert(this->is_valid());
			const Blob *pBlob = &oBlob;
			return task_bool{ [this, pBlob]()->bool {
				bool b = this->m_pman->maintains_blob_async(*pBlob).get();
				return (b);
			} };
		}//maintains_blob_async
		task_bool DomainPersistManager::remove_blob_async(const Blob &oBlob) const {
			assert(this->is_valid());
			const Blob *pBlob = &oBlob;
			return task_bool{ [this, pBlob]()->bool {
				bool b = this->m_pman->remove_blob_async(*pBlob).get();
				return (b);
			} };
		}//remove_blob_async
		task_blob DomainPersistManager::read_blob_async(const Blob &oModel) const {
			assert(this->is_valid());
			const Blob *pBlob = &oModel;
			return task_blob{ [this,pBlob]()->blob_ptr {
				blob_ptr o = this->m_pman->read_blob_async(*pBlob).get();
				return (o);
			} };
		}//read_blob_async
		task_stringsvector DomainPersistManager::read_all_blobs_async(
			const InfoItem &oModel) const {
			assert(oModel.has_id());
			assert(this->is_valid());
			const InfoItem *pItem = &oModel;
			return task_stringsvector{ [this,pItem]()->stringsvector_ptr {
				string_type id = pItem->id();
				stringsvector_ptr r = this->m_pman->read_all_blobs_async(id).get();
				return (r);
			} };
		}//read_all_blobs_async
		task_bool DomainPersistManager::delete_documents_async(const anymap_type &oModel) const {
			assert(this->is_valid());
			const anymap_type *pModel = &oModel;
			return task_bool{ [this,pModel]()->bool {
				return this->m_pman->delete_documents_async(*pModel).get();
			} };
		}//delete_documents_async
		////////////////////////////
	}// namespace domain
}		// namespace info
