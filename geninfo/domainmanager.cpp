#include "stdafx.h"
#include "domainmanager.h"
#include "domainpersistmanager.h"
#include "couchdbpersisterfactory.h"
#include "dataset.h"
#include "variable.h"
#include "indiv.h"
#include "value.h"
#include "infostrings.h"
/////////////////////////
namespace info {
	namespace domain {
		DomainManager::DomainManager(const string_type &baseUrl, const string_type &databaseName)
		{
			m_persist.reset(info::persist::CouchDBPersisterFactory::create(baseUrl, databaseName));
			m_man.reset(new DomainPersistManager{ m_persist.get() });
		}//DomainManager
		DomainManager::DomainManager(IPersistManager *pPersist)
		{
			m_man.reset(new DomainPersistManager{ pPersist });
		}//DomainManager
		DomainManager::~DomainManager()
		{
			m_man.reset();
			m_persist.reset();
		}
		//////////////////////////////////
		task_dataset DomainManager::find_dataset_by_sigle_async(const string_type &sigle) {
			return task_dataset{ [this,sigle]()->dataset_ptr {
				dataset_ptr oRet{};
				Dataset model{sigle};
				item_ptr p = this->m_man->find_element_async(model).get();
				if (p.get() != nullptr) {
					anymap_type oMap{};
					p->to_map(oMap);
					oRet = std::make_shared<Dataset>(oMap);
				}
				return (oRet);
			}
			};
		}//find_dataset_by_sigle_async
		task_dataset DomainManager::find_dataset_async(const Dataset &oSet) {
			const Dataset *pSet = &oSet;
			return task_dataset{ [this,pSet]()->dataset_ptr {
				dataset_ptr oRet{};
				item_ptr p = this->m_man->find_element_async(*pSet).get();
				if (p.get() != nullptr) {
					anymap_type oMap{};
					p->to_map(oMap);
					oRet = std::make_shared<Dataset>(oMap);
				}
				return (oRet);
			}
			};
		}//find_dataset_async
		task_dataset DomainManager::maintains_dataset_async(const Dataset &oSet) {
			const Dataset *pSet = &oSet;
			return task_dataset{ [this,pSet]()->dataset_ptr {
				dataset_ptr oRet{};
				if (pSet->is_storeable()) {
					item_ptr p = this->m_man->maintains_element_async(*pSet).get();
					if (p.get() != nullptr) {
						anymap_type oMap{};
						p->to_map(oMap);
						oRet = std::make_shared<Dataset>(oMap);
					}
				}// storeable
				return (oRet);
			}
			};
		}// maintains_dataset_async
		task_bool DomainManager::remove_dataset_async(const Dataset &oSet) {
			const Dataset *pSet = &oSet;
			return task_bool{ [this, pSet]()->bool {
				bool bRet = false;
				dataset_ptr p = this->find_dataset_async(*pSet).get();
				if (p.get() != nullptr) {
					string_type sigle = p->sigle();
					anymap_type oMap{};
					oMap[KEY_DATASETSIGLE] = any{sigle};
					bRet = this->m_man->delete_documents_async(oMap).get();
					bRet = bRet && this->m_man->remove_element_async(*p).get();
				}// p
				return (bRet);
			} };
		}//remove_dataset_async
		task_int DomainManager::get_datasets_count(void) {
			return task_int{ [this]()->int {
				Dataset oSet{};
				int nRet = this->m_man->get_count_async(oSet).get();
				return nRet;
			} };
		}
		pplx::task<std::vector<dataset_ptr>> DomainManager::get_datasets(int offset /*= 0*/, int count /*= 0*/) {
			return pplx::task<std::vector<dataset_ptr> >{[this, offset, count]()->std::vector<dataset_ptr> {
				std::vector<dataset_ptr> oRet{};
				Dataset oSet{};
				itemptrvector_ptr vv = this->m_man->get_elements_async(oSet, offset, count).get();
				if (vv.get() != nullptr) {
					for (item_ptr p : *vv) {
						if (p.get() != nullptr) {
							anymap_type oMap{};
							p->to_map(oMap);
							dataset_ptr x = std::make_shared<Dataset>(oMap);
							oRet.push_back(x);
						}
					}// p
				}// vv
				return (oRet);
			}};
		}
		task_bool DomainManager::maintains_datasets_async(const std::vector<dataset_ptr> &oVec, bool bDelete /*= false*/) {
			const std::vector<dataset_ptr> *pVec = &oVec;
			return task_bool{ [this,pVec,bDelete]()->bool {
				bool bRet{false};
				itemptrvector_ptr vv{new std::vector<item_ptr>{}};
				std::vector<task_bool> oTasks{};
				for (auto p : *pVec) {
					if (bDelete) {
						oTasks.push_back(this->remove_dataset_async(*p));
					}
					else {
						vv->push_back(p);
					}
				}// p
				if (bDelete) {
					pplx::when_all(oTasks.begin(), oTasks.end()).wait();
					bRet = true;
				}
				else {
					this->m_man->maintains_elements_async(vv).wait();
				}
				return (bRet);
			} };
		}//:maintains_datasets_async
		/////////////////////////////
	}// namespace domain
}// namespace info
