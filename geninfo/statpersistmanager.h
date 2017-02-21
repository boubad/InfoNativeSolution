#pragma once
#ifndef STATPERSISTMANAGER_H
#define STATPERSISTMANAGER_H
///////////////////////////////
#include "domainpersistmanager.h"
//////////////////////////////
namespace info {
	namespace domain {
		///////////////////////////////
		class Dataset;
		class Variable;
		class Indiv;
		class Value;
		class Etudiant;
		/////////////////////////////
		class StatPersistManager : public  DomainPersistManager {
		private:
			std::unique_ptr<IPersistManager> m_persist;
		private:
			StatPersistManager(const StatPersistManager &other) = delete;
			StatPersistManager & operator=(const StatPersistManager &other) = delete;
		public:
			StatPersistManager(IPersistManager *pPersist = nullptr);
			StatPersistManager(const string_type &baseUrl, const string_type &databaseName);
			virtual ~StatPersistManager();
		public:
			task_int get_datasets_count_async(const Dataset &model) const;
			task_itemvector get_datasets_async(const Dataset &model,int offset = 0, int count = 0) const;
			task_item find_dataset_async(const Dataset &model) const;
			task_item maintains_dataset_async(const Dataset &model) const;
			task_bool remove_dataset_async(const Dataset &model) const;
			//
			task_int get_dataset_variables_count_async(const Dataset &oSet) const;
			task_itemvector get_dataset_variables_async(const Dataset &oSet, int offset = 0, int count = 0) const;
			task_item find_variable_async(const Variable &model) const;
			task_item maintains_variable_async(const Variable &model) const;
			task_bool remove_variable_async(const Variable &model) const;
			task_bool maintains_variables_async(const std::vector<Variable> &oVars, bool bDelete = false) const;
			//
			task_int get_dataset_indivs_count_async(const Dataset &oSet) const;
			task_itemvector get_dataset_indivs_async(const Dataset &oSet, int offset = 0, int count = 0) const;
			task_item find_indiv_async(const Indiv &model) const;
			task_item maintains_indiv_async(const Indiv &model) const;
			task_bool remove_indiv_async(const Indiv &model) const;
			task_bool maintains_indivs_async(const std::vector<Indiv> &oInds, bool bDelete = false) const;
			//
			task_int get_dataset_values_count_async(const Dataset &oSet) const;
			task_int get_variable_values_count_async(const Variable &oVar) const;
			task_int get_indiv_values_count_async(const Indiv &oInd) const;
			task_itemvector get_dataset_values_async(const Dataset &oSet, int offset = 0, int count = 0) const;
			task_itemvector get_variable_values_async(const Variable &oVar, int offset = 0, int count = 0) const;
			task_itemvector get_indiv_values_async(const Indiv &oInd, int offset = 0, int count = 0) const;
			task_item find_value_async(const Value &model) const;
			task_item maintains_value_async(const Value &model) const;
			task_bool remove_value_async(const Value &model) const;
			task_bool maintains_values_async(const std::vector<Value> &oVals, bool bDelete = false) const;
			//
			task_int get_etudiants_count_async(const Etudiant &model) const;
			task_itemvector get_etudiants_async(const Etudiant &model,int offset = 0, int count = 0) const;
			task_item find_etudiant(const Etudiant &model) const;
			task_item maintains_Etudiant(const Etudiant &model) const;
			task_bool remove_Etudiant(const Etudiant &model) const;
			task_bool maintains_etudiants_async(const std::vector<Etudiant> &oVals, bool bDelete = false) const;
		};// class StatPersistManager
		////////////////////////////////
	}// namespace domain
}// namespace info
/////////////////////////////
#endif // STATPERSISTMANAGER_H
