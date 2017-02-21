#pragma once
#include "persist_manager.h"
//////////////////////////
namespace info {
	namespace domain {
		/////////////////////////
		class Dataset;
		class Variable;
		class Indiv;
		class Value;
		class DomainPersistManager;
		////////////////////////
		using dataset_ptr = std::shared_ptr<Dataset>;
		using task_dataset = pplx::task<dataset_ptr>;
		using variable_ptr = std::shared_ptr<Variable>;
		using task_variable = pplx::task<variable_ptr>;
		using indiv_ptr = std::shared_ptr<Indiv>;
		using task_indiv = pplx::task<indiv_ptr>;
		///////////////////////
		class DomainManager
		{
		private:
			std::unique_ptr<IPersistManager> m_persist;
			std::unique_ptr<DomainPersistManager> m_man;
		private:
			DomainManager(const DomainManager & other) = delete;
			DomainManager & operator=(const DomainManager &other) = delete;
		public:
			DomainManager(IPersistManager *pPersist);
			DomainManager(const string_type &baseUrl, const string_type &databaseName);
			virtual ~DomainManager();
		public:
			//
			task_dataset find_dataset_by_sigle_async(const string_type &sigle);
			task_dataset find_dataset_async(const Dataset &oSet);
			task_dataset maintains_dataset_async(const Dataset &oSet);
			task_bool remove_dataset_async(const Dataset &oSet);
			task_int get_datasets_count(void);
			pplx::task<std::vector<dataset_ptr>> get_datasets(int offset = 0, int count = 0);
			task_bool maintains_datasets_async(const std::vector<dataset_ptr> &oVec, bool bDelete = false);
			//
		};// class DomainManager
	} // namespace domain
}// namespace info

