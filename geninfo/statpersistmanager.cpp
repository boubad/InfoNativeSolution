//statpersistmanager.cpp
//////////////////////////////
#include "stdafx.h"
#include "statpersistmanager.h"
#include "dataset.h"
#include "variable.h"
#include "indiv.h"
#include "value.h"
#include "etudiant.h"
///////////////////////
#include "infostrings.h"
#include "couchdbpersistmanager.h"
////////////////////////////
namespace info {
	namespace domain {
		///////////////////////////////
		StatPersistManager::StatPersistManager(IPersistManager *pPersist) :DomainPersistManager(pPersist) {
		}//StatPersistManager
		StatPersistManager::StatPersistManager(const string_type &baseUrl, const string_type &databaseName) :
			m_persist(new info::persist::CouchDBPersistManager{ baseUrl,databaseName }) {
			this->set_persist_manager(m_persist.get());
		}//StatPersistManager
		StatPersistManager::~StatPersistManager() {
			m_persist.reset();
		}//~StatPersistManager
		//////////////////////////////////
		task_int StatPersistManager::get_datasets_count_async(const Dataset &model) const {
			assert(this->is_valid());
			const Dataset *pSet = &model;
			return task_int{ [this,pSet]()->int {
				Dataset oSet{};
					status_type s = pSet->status();
					if (s != status_type::unknown) {
						oSet.status(s);
					}
					string_type ss = pSet->annee();
					if (!ss.empty()) {
						oSet.annee(ss);
					}
				int nRet = this->get_count_async(oSet).get();
				return (nRet);
			} };
		}//get_datasets_count_async
		task_itemvector StatPersistManager::get_datasets_async(const Dataset &model, int offset /*= 0*/, int count /*= 0*/) const {
			assert(this->is_valid());
			const Dataset *pSet = &model;
			return task_itemvector{ [this,pSet,offset,count]()->itemptrvector_ptr {
				Dataset oSet{};
				status_type s = pSet->status();
				if (s != status_type::unknown) {
					oSet.status(s);
				}
				string_type ss = pSet->annee();
				if (!ss.empty()) {
					oSet.annee(ss);
				}
				itemptrvector_ptr oRet = this->get_elements_async(oSet, offset, count).get();
				return (oRet);
			} };
		}//get_datasets_async
		task_item StatPersistManager::find_dataset_async(const Dataset &model) const {
			assert(this->is_valid());
			const Dataset *pSet = &model;
			return task_item{ [this,pSet]()->item_ptr {
				item_ptr oRet = this->find_element_async(*pSet).get();
				return (oRet);
			} };
		}//find_dataset_async
		task_item StatPersistManager::maintains_dataset_async(const Dataset &model) const {
			assert(this->is_valid());
			const Dataset *pSet = &model;
			return task_item{ [this,pSet]()->item_ptr {
				item_ptr oRet = this->maintains_element_async(*pSet).get();
				return (oRet);
			} };
		}// maintains_dataset_async
		task_bool StatPersistManager::remove_dataset_async(const Dataset &model) const {
			assert(this->is_valid());
			const Dataset *pSet = &model;
			return task_bool{ [this,pSet]()->bool {
				anymap_type o{};
				string_type s{pSet->sigle()};
				o[KEY_DATASETSIGLE] = s;
				bool bRet = this->delete_documents_async(o).get();
				bRet = this->remove_element_async(*pSet).get();
				return(bRet);
			} };
		}// remove_dataset_async
		//////////////////////////////////
		task_int  StatPersistManager::get_dataset_variables_count_async(const Dataset &oSet) const {
			assert(this->is_valid());
			const Dataset *pSet = &oSet;
			return task_int{ [this,pSet]()->int {
				Variable model{};
				model.dataset_sigle(pSet->sigle());
				status_type s = pSet->status();
				if (s != status_type::unknown) {
					model.status(s);
				}
				int nRet = this->get_count_async(model).get();
				return (nRet);
			} };
		}//get_dataset_variables_count_async
		task_itemvector StatPersistManager::get_dataset_variables_async(const Dataset &oSet, int offset /*= 0*/, int count /*= 0*/) const {
			assert(this->is_valid());
			const Dataset *pSet = &oSet;
			return task_itemvector{ [this,pSet,offset,count]()->itemptrvector_ptr {
				Variable model{};
				model.dataset_sigle(pSet->sigle());
				status_type s = pSet->status();
				if (s != status_type::unknown) {
					model.status(s);
				}
				itemptrvector_ptr oRet = this->get_elements_async(model, offset, count).get();
				return (oRet);
			} };
		}//get_dataset_variables_async
		task_item StatPersistManager::find_variable_async(const Variable &model) const {
			assert(this->is_valid());
			const Variable *pVar = &model;
			return task_item{ [this,pVar]()->item_ptr {
				item_ptr oRet = this->find_element_async(*pVar).get();
				return (oRet);
			} };
		}//find_variable_async
		task_item  StatPersistManager::maintains_variable_async(const Variable &model) const {
			assert(this->is_valid());
			const Variable *pVar = &model;
			return task_item{ [this,pVar]()->item_ptr {
				item_ptr oRet = this->maintains_element_async(*pVar).get();
				return (oRet);
			} };
		}//maintains_variable_async
		task_bool StatPersistManager::remove_variable_async(const Variable &model) const {
			assert(this->is_valid());
			const Variable *pVar = &model;
			return task_bool{ [this,pVar]()->bool {
				anymap_type o{};
				string_type s{ pVar->sigle() };
				o[KEY_VARIABLESIGLE] = s;
				bool bx = this->delete_documents_async(o).get();
				bx = this->remove_element_async(*pVar).get();
				return (bx);
			} };
		}//remove_variable
		task_bool  StatPersistManager::maintains_variables_async(const std::vector<Variable> &oVars, bool bDelete /*= false*/) const {
			assert(this->is_valid());
			const std::vector<Variable> *pVars = &oVars;
			return task_bool{ [this,pVars,bDelete]()->bool {
				itemptrvector_ptr pv{new itemptr_vector{} };
				itemptr_vector *vv = pv.get();
				assert(vv != nullptr);
				std::vector<task_bool> oTasks{};
				for (auto it = pVars->begin(); it != pVars->end(); ++it) {
					const Variable &v = *it;
					if (!bDelete) {
						if (v.is_storeable()) {
							item_ptr p{ new Variable{ *it } };
							assert(p.get() != nullptr);
							vv->push_back(p);
						}
					}
					else if (v.is_persisted()) {
						oTasks.push_back(this->remove_variable_async(v));
					}
				}// it
				bool bRet{ true };
				if (!vv->empty()) {
					if (bDelete && (!oTasks.empty())) {
						auto r = pplx::when_all(oTasks.begin(), oTasks.end()).get();
					}
					else {
						bRet = this->maintains_elements_async(pv).get();
					}
				}// not empty
				return (bRet);
			} };
		}//maintains_variables_async
		////////////////////////////////////
		task_int  StatPersistManager::get_dataset_indivs_count_async(const Dataset &oSet) const {
			assert(this->is_valid());
			const Dataset *pSet = &oSet;
			return task_int{ [this,pSet]()->int {
				Indiv model{};
				model.dataset_sigle(pSet->sigle());
				status_type s = pSet->status();
				if (s != status_type::unknown) {
					model.status(s);
				}
				int nRet = this->get_count_async(model).get();
				return (nRet);
			} };
		}//get_dataset_indivs_count_async
		task_itemvector StatPersistManager::get_dataset_indivs_async(const Dataset &oSet, int offset /*= 0*/, int count /*= 0*/) const {
			assert(this->is_valid());
			const Dataset *pSet = &oSet;
			return task_itemvector{ [this,pSet,offset,count]()->itemptrvector_ptr {
				Indiv model{};
				model.dataset_sigle(pSet->sigle());
				status_type s = pSet->status();
				if (s != status_type::unknown) {
					model.status(s);
				}
				itemptrvector_ptr oRet = this->get_elements_async(model, offset, count).get();
				return (oRet);
			} };
		}//get_dataset_indivs_async
		task_item StatPersistManager::find_indiv_async(const Indiv &model) const {
			assert(this->is_valid());
			const Indiv *pInd = &model;
			return task_item{ [this,pInd]()->item_ptr {
				item_ptr oRet = this->find_element_async(*pInd).get();
				return (oRet);
			} };
		}//find_indiv_async
		task_item  StatPersistManager::maintains_indiv_async(const Indiv &model) const {
			assert(this->is_valid());
			const Indiv *pInd = &model;
			return task_item{ [this,pInd]()->item_ptr {
				item_ptr oRet = this->maintains_element_async(*pInd).get();
				return (oRet);
			} };
		}//maintains_indiv_async
		task_bool StatPersistManager::remove_indiv_async(const Indiv &model) const {
			assert(this->is_valid());
			const Indiv *pInd = &model;
			return task_bool{ [this,pInd]()->bool {
				IPersistManager *pPersist = this->get_persist_manager();
				anymap_type o{};
				string_type s{ pInd->sigle() };
				o[KEY_INDIVSIGLE] = s;
				bool bRet = this->delete_documents_async(o).get();
				bRet = this->remove_element_async(*pInd).get();
				return (bRet);
			} };
		}//remove_indiv_async
		task_bool  StatPersistManager::maintains_indivs_async(const std::vector<Indiv> &oInds, bool bDelete /*= false*/) const {
			assert(this->is_valid());
			const std::vector<Indiv> *pInds = &oInds;
			return task_bool{ [this,pInds,bDelete]()->bool {
				itemptrvector_ptr pv{ new itemptr_vector{} };
				itemptr_vector *vv = pv.get();
				assert(vv != nullptr);
				std::vector<task_bool> oTasks{};
				for (auto it = pInds->begin(); it != pInds->end(); ++it) {
					const Indiv &v = *it;
					if (!bDelete) {
						if (v.is_storeable()) {
item_ptr p{ new Indiv{ *it } };
assert(p.get() != nullptr);
vv->push_back(p);
						}
					}
					else if (v.is_persisted()) {
						oTasks.push_back(this->remove_indiv_async(v));
					}
				}// it
				bool bRet{ true };
				if (!vv->empty()) {
					if (bDelete && (!oTasks.empty())) {
						auto r = pplx::when_all(oTasks.begin(), oTasks.end()).get();
					}
					else {
						bRet = this->maintains_elements_async(pv).get();
					}
				}// not empty
				return (bRet);
			} };
		}//maintains_indivs_async
		//////////////////////////////////////
		task_int StatPersistManager::get_dataset_values_count_async(const Dataset &oSet) const {
			assert(this->is_valid());
			const Dataset *pSet = &oSet;
			return task_int{ [this,pSet]()->int {
				Value model{};
				model.dataset_sigle(pSet->sigle());
				int nRet = this->get_count_async(model).get();
				return (nRet);
			} };
		}//get_dataset_values_count_async
		task_int StatPersistManager::get_variable_values_count_async(const Variable &oVar) const {
			assert(this->is_valid());
			const Variable *pVar = &oVar;
			return task_int{ [this,pVar]()->int {
				Value model{};
				model.dataset_sigle(pVar->dataset_sigle());
				model.variable_sigle(pVar->sigle());
				int nRet = this->get_count_async(model).get();
				return (nRet);
			} };
		}//get_variable_values_count_async
		task_int StatPersistManager::get_indiv_values_count_async(const Indiv &oInd) const {
			assert(this->is_valid());
			const Indiv *pInd = &oInd;
			return task_int{ [this,pInd]()->int {
				Value model{};
				model.dataset_sigle(pInd->dataset_sigle());
				model.indiv_sigle(pInd->sigle());
				int nRet = this->get_count_async(model).get();
				return (nRet);
			} };
		}//get_indiv_values_count_async
		task_itemvector StatPersistManager::get_dataset_values_async(const Dataset &oSet, int offset /*= 0*/, int count /*= 0*/) const {
			assert(this->is_valid());
			const Dataset *pSet = &oSet;
			return task_itemvector{ [this,pSet,offset,count]()->itemptrvector_ptr {
				Value model{};
				model.dataset_sigle(pSet->sigle());
				itemptrvector_ptr oRet = this->get_elements_async(model, offset, count).get();
				return (oRet);
			} };
		}// get_dataset_values_async
		task_itemvector StatPersistManager::get_variable_values_async(const Variable &oVar, int offset /*= 0*/, int count /*= 0*/) const {
			assert(this->is_valid());
			const Variable *pVar = &oVar;
			return task_itemvector{ [this,pVar,offset,count]()->itemptrvector_ptr {
				Value model{};
				model.dataset_sigle(pVar->dataset_sigle());
				model.variable_sigle(pVar->sigle());
				itemptrvector_ptr oRet = this->get_elements_async(model, offset, count).get();
				return (oRet);
			} };
		}// get_variable_values_async
		task_itemvector StatPersistManager::get_indiv_values_async(const Indiv &oInd, int offset /*= 0*/, int count /*= 0*/) const {
			assert(this->is_valid());
			const Indiv *pInd = &oInd;
			return task_itemvector{ [this,pInd,offset,count]()->itemptrvector_ptr {
				Value model{};
				model.dataset_sigle(pInd->dataset_sigle());
				model.indiv_sigle(pInd->sigle());
				itemptrvector_ptr oRet = this->get_elements_async(model, offset, count).get();
				return (oRet);
			} };
		}//get_indiv_values_async
		task_item StatPersistManager::find_value_async(const Value &model) const {
			assert(this->is_valid());
			const Value *pVal = &model;
			return task_item{ [this,pVal]()->item_ptr {
				item_ptr oRet = this->find_element_async(*pVal).get();
				return (oRet);
			} };
		}//find_value_async
		task_item StatPersistManager::maintains_value_async(const Value &model) const {
			assert(this->is_valid());
			const Value *pVal = &model;
			return task_item{ [this,pVal]()->item_ptr {
				item_ptr oRet{};
				if (pVal->is_empty()) {
					item_ptr px = this->find_value_async(*pVal).get();
					if (px.get() != nullptr) {
						bool b = this->remove_element_async(*pVal).get();
					}// px
				}
				else {
					oRet = this->maintains_element_async(*pVal).get();
				}
				return (oRet);
			}
			};
		}//maintains_value_async
		task_bool StatPersistManager::remove_value_async(const Value &model) const {
			assert(this->is_valid());
			const Value *pVal = &model;
			return task_bool{ [this,pVal]()->bool {
				bool oRet = this->remove_element_async(*pVal).get();
				return (oRet);
			} };
		}//remove_value_async
		task_bool StatPersistManager::maintains_values_async(const std::vector<Value> &oVals, bool bDelete /*= false*/) const {
			assert(this->is_valid());
			const std::vector<Value> *pVals = &oVals;
			return task_bool{ [this,pVals,bDelete]()->bool {
				std::vector<task_item> oTasks{};
				itemptrvector_ptr pv{ new itemptr_vector{} };
				itemptr_vector *vv = pv.get();
				assert(vv != nullptr);
				for (auto it = pVals->begin(); it != pVals->end(); ++it) {
					const Value &v = *it;
					if (!bDelete) {
						if (v.is_storeable()) {
							oTasks.push_back(this->maintains_value_async(v));
						}
					}
					else if (v.is_persisted()) {
						item_ptr p{ new Value{ *it } };
						assert(p.get() != nullptr);
						vv->push_back(p);
					}
				}// it
				bool bRet{ true };
				if (!oTasks.empty()) {
					pplx::when_all(oTasks.begin(), oTasks.end()).wait();
				}
				if (!vv->empty()) {
						bRet = this->remove_elements_async(pv).get();
					}
				return (bRet);
			} };
		}//maintains_values_async
		///////////////////////////////////////
		task_int StatPersistManager::get_etudiants_count_async(const Etudiant &model) const {
			assert(this->is_valid());
			const Etudiant *pEtud = &model;
			return task_int{ [this,pEtud]()->int {
				int nRet = this->get_count_async(*pEtud).get();
				return (nRet);
			} };
		}//get_etudiants_count_async
		task_itemvector StatPersistManager::get_etudiants_async(const Etudiant &model, int offset/*= 0*/, int count /*= 0*/) const {
			assert(this->is_valid());
			const Etudiant *pEtud = &model;
			return task_itemvector{ [this,pEtud,offset,count]()->itemptrvector_ptr {
				itemptrvector_ptr oRet = this->get_elements_async(*pEtud, offset, count).get();
				return (oRet);
			} };
		}//get_etudiants_async
		task_item StatPersistManager::find_etudiant(const Etudiant &model) const {
			assert(this->is_valid());
			const Etudiant *pEtud = &model;
			return task_item{ [this,pEtud]()->item_ptr {
				item_ptr oRet = this->find_element_async(*pEtud).get();
				return (oRet);
			} };
		}//find_etudiant
		task_item StatPersistManager::maintains_Etudiant(const Etudiant &model) const {
			assert(this->is_valid());
			const Etudiant *pEtud = &model;
			return task_item{ [this,pEtud]()->item_ptr {
				item_ptr oRet = this->maintains_element_async(*pEtud).get();
				return (oRet);
			} };
		}//maintains_Etudiant
		task_bool StatPersistManager::remove_Etudiant(const Etudiant &model) const {
			assert(this->is_valid());
			const Etudiant *pEtud = &model;
			return task_bool{ [this,pEtud]()->bool {
				bool oRet = this->remove_element_async(*pEtud).get();
				return (oRet);
			} };
		}//remove_Etudiant
		task_bool StatPersistManager::maintains_etudiants_async(const std::vector<Etudiant> &oEtuds, bool bDelete /*= false*/) const {
			assert(this->is_valid());
			const std::vector<Etudiant> *pEtuds = &oEtuds;
			return task_bool{ [this,pEtuds,bDelete]()->bool {
				itemptrvector_ptr pv{ new itemptr_vector{} };
				itemptr_vector *vv = pv.get();
				assert(vv != nullptr);
				for (auto it = pEtuds->begin(); it != pEtuds->end(); ++it) {
					const Etudiant &v = *it;
					if (!bDelete) {
						if (v.is_storeable()) {
							item_ptr p{ new Etudiant{ *it } };
							assert(p.get() != nullptr);
							vv->push_back(p);
						}
					}
					else if (v.is_persisted()) {
						item_ptr p{ new Etudiant{ *it } };
						assert(p.get() != nullptr);
						vv->push_back(p);
					}
				}// it
				bool bRet{ true };
				if (!vv->empty()) {
					if (bDelete) {
						bRet = this->remove_elements_async(pv).get();
					}
					else {
						bRet = this->maintains_elements_async(pv).get();
					}
				}// not empty
				return (bRet);
			} };
		}//maintains_etudiants_async
	}// namespace domain
}// namespace info
