#include "stdafx.h"
#include "CppUnitTest.h"
////////////////////////
#include "test_serverdata.h"
#include "infotestdata.h"
////////////////////////////////////
#include <couchdbpersistmanager.h>
#include <domainpersistmanager.h>
#include <statpersistmanager.h>
#include <dataset.h>
#include <variable.h>
#include <indiv.h>
#include <value.h>
//////////////////////////
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestGenInfo
{
	///////////////////////////////
	using namespace info;
	using namespace info::persist;
	using namespace info::domain;
	using namespace std;
	////////////////////////////
	TEST_CLASS(ImportTest)
	{
	public:
		std::shared_ptr<CouchDBPersistManager> m_persist;
		std::shared_ptr<DomainPersistManager> m_man;
		std::shared_ptr<StatPersistManager> m_stat;
		//
		DomainPersistManager *get_manager(void) {
			return m_man.get();
		}// get_manager
	public:
		TEST_METHOD_INITIALIZE(SetUp) {
			m_persist.reset(new CouchDBPersistManager{ TestServerData::serverUrl(),TestServerData::databaseName() });
			IPersistManager *p = m_persist.get();
			Assert::IsNotNull(p);
			m_man.reset(new DomainPersistManager{ p });
			Assert::IsNotNull(m_man.get());
			m_stat.reset(new StatPersistManager{ p });
			Assert::IsNotNull(m_stat.get());
			//
		}//SetUp
		TEST_METHOD_CLEANUP(TearDown) {
			m_man.reset();
			m_persist.reset();
		}// TearDown
	public:
		TEST_METHOD(test_import_conso)
		{
			StatPersistManager *pMan = m_stat.get();
			Assert::IsNotNull(pMan);
			//
			string_type name{};
			std::vector<string_type> rownames{}, colnames{};
			std::vector<int> data{};
			size_t nRows{ 0 }, nCols{ 0 };
			InfoTestData::get_conso_data(name, nRows, nCols, data, rownames, colnames);
			Assert::IsTrue(nRows > 1);
			Assert::IsTrue(nCols > 0);
			Assert::IsTrue(data.size() >= static_cast<size_t>(nCols * nRows));
			Assert::IsTrue(rownames.size() >= nRows);
			Assert::IsTrue(colnames.size() >= nCols);
			//
			Dataset xset{ name };
			item_ptr px = pMan->find_dataset_async(xset).get();
			if (px.get() == nullptr) {
				xset.name(name);
				Assert::IsTrue(xset.is_storeable());
				px = pMan->maintains_dataset_async(xset).get();
			}// px
			Dataset *pSet = dynamic_cast<Dataset *>(px.get());
			Assert::IsNotNull(pSet);
			string_type setsigle{ pSet->sigle() };
			//
			itemptr_vector vars{};
			int nv = pMan->get_dataset_variables_count_async(*pSet).get();
			if (nv > 0) {
				itemptrvector_ptr p = pMan->get_dataset_variables_async(*pSet, 0, nv).get();
				itemptr_vector *pv = p.get();
				if (pv != nullptr) {
					vars = *pv;
				}
			}// nv
			std::vector<Variable> oVarsAdd{};
			for (string_type s : colnames) {
				string_type varsigle = to_upper(trim(s));
				auto it = std::find_if(vars.begin(), vars.end(), [varsigle](item_ptr p)->bool {
					Variable *pVar = dynamic_cast<Variable *>(p.get());
					return (pVar->sigle() == varsigle);
				});
				if (it == vars.end()) {
					Variable x{ setsigle,varsigle };
					x.name(varsigle);
					x.variable_type(variable_datatype::integer);
					x.genre(variable_kindtype::normal);
					Assert::IsTrue(x.is_storeable());
					oVarsAdd.push_back(x);
				}// not found
			}// s
			if (!oVarsAdd.empty()) {
				bool bRet = pMan->maintains_variables_async(oVarsAdd).get();
				Assert::IsTrue(bRet);
			}// addVars
			 //
			itemptr_vector inds{};
			int nr = pMan->get_dataset_indivs_count_async(*pSet).get();
			if (nr > 0) {
				itemptrvector_ptr p = pMan->get_dataset_indivs_async(*pSet, 0, nr).get();
				itemptr_vector *pv = p.get();
				if (pv != nullptr) {
					inds = *pv;
				}
			}// nr
			std::vector<Indiv> oIndsAdd{};
			for (string_type s : rownames) {
				string_type indsigle = to_upper(trim(s));
				auto it = std::find_if(inds.begin(), inds.end(), [indsigle](item_ptr p)->bool {
					Indiv *pInd = dynamic_cast<Indiv *>(p.get());
					return (pInd->sigle() == indsigle);
				});
				if (it == inds.end()) {
					Indiv x{ setsigle,indsigle };
					x.name(indsigle);
					Assert::IsTrue(x.is_storeable());
					oIndsAdd.push_back(x);
				}// not found
			}// s
			if (!oIndsAdd.empty()) {
				bool bRet = pMan->maintains_indivs_async(oIndsAdd).get();
				Assert::IsTrue(bRet);
			}// addVars
			 //
			itemptr_vector vals{};
			int nVals = pMan->get_dataset_values_count_async(*pSet).get();
			if (nVals > 0) {
				itemptrvector_ptr p = pMan->get_dataset_values_async(*pSet, 0, nVals).get();
				itemptr_vector *pv = p.get();
				if (pv != nullptr) {
					vals = *pv;
				}
			}// nVals
			std::vector<Value> oValsAdd{};
			for (size_t i = 0; i < nRows; ++i) {
				string_type indsigle = to_upper(trim(rownames[i]));
				for (size_t j = 0; j < nCols; ++j) {
					string_type varsigle = to_upper(trim(colnames[j]));
					auto it = std::find_if(vals.begin(), vals.end(), [indsigle, varsigle](item_ptr p)->bool {
						Value *px = dynamic_cast<Value *>(p.get());
						return (px->variable_sigle() == varsigle) && (px->indiv_sigle() == indsigle);
					});
					if (it == vals.end()) {
						any v{ data[i * nCols + j] };
						Value x{ setsigle,indsigle,varsigle,v };
						Assert::IsTrue(x.is_storeable());
						oValsAdd.push_back(x);
					}// not found
				}// sv
			}// si
			if (!oValsAdd.empty()) {
				bool bRet = pMan->maintains_values_async(oValsAdd).get();
				Assert::IsTrue(bRet);
			}// addVars
		}// test_import_mortal
		TEST_METHOD(test_import_mortal) {
			DomainPersistManager *pMan = get_manager();
			//
			string_type name{};
			std::vector<string_type> rownames{}, colnames{};
			std::vector<int> data{};
			size_t nRows{ 0 }, nCols{ 0 };
			InfoTestData::get_mortal_data(name, nRows, nCols, data, rownames, colnames);
			Assert::IsTrue(nRows > 1);
			Assert::IsTrue(nCols > 0);
			Assert::IsTrue(data.size() >= static_cast<size_t>(nCols * nRows));
			Assert::IsTrue(rownames.size() >= nRows);
			Assert::IsTrue(colnames.size() >= nCols);
			//
			Dataset xset{name};
			item_ptr px = pMan->find_element_async(xset).get();
			if (px.get() == nullptr) {
				xset.name(name);
				Assert::IsTrue(xset.is_storeable());
				px = pMan->maintains_element_async(xset).get();
			}// px
			Dataset *pSet = dynamic_cast<Dataset *>(px.get());
			Assert::IsNotNull(pSet);
			string_type setsigle{ pSet->sigle() };
			//
			Variable oVarModel{};
			oVarModel.dataset_sigle(setsigle);
			itemptr_vector vars{};
			int nv = pMan->get_count_async(oVarModel).get();
			if (nv > 0) {
				itemptrvector_ptr p = pMan->get_elements_async(oVarModel, 0, nv).get();
				itemptr_vector *pv = p.get();
				if (pv != nullptr) {
					vars = *pv;
				}
			}// nv
			itemptr_vector oVarsAdd{};
			for (string_type s : colnames) {
				string_type varsigle = to_upper(trim(s));
				auto it = std::find_if(vars.begin(), vars.end(), [varsigle](item_ptr p)->bool {
					Variable *pVar = dynamic_cast<Variable *>(p.get());
					return (pVar->sigle() == varsigle);
				});
				if (it == vars.end()) {
					Variable x{setsigle,varsigle};
					x.name(varsigle);
					x.variable_type(variable_datatype::integer);
					x.genre(variable_kindtype::normal);
					Assert::IsTrue(x.is_storeable());
					item_ptr xx{ new Variable{x} };
					Assert::IsNotNull(xx.get());
					oVarsAdd.push_back(xx);
				}// not found
			}// s
			if (!oVarsAdd.empty()) {
				itemptrvector_ptr pz = std::make_shared<itemptr_vector>();
				itemptr_vector *pzz = pz.get();
				Assert::IsNotNull(pzz);
				*pzz = oVarsAdd;
				bool bRet = pMan->maintains_elements_async(pz).get();
				Assert::IsTrue(bRet);
			}// addVars
			//
			Indiv oIndModel{};
			oIndModel.dataset_sigle(setsigle);
			itemptr_vector inds{};
			int nr = pMan->get_count_async(oIndModel).get();
			if (nr > 0) {
				itemptrvector_ptr p = pMan->get_elements_async(oIndModel, 0, nr).get();
				itemptr_vector *pv = p.get();
				if (pv != nullptr) {
					inds = *pv;
				}
			}// nr
			itemptr_vector oIndsAdd{};
			for (string_type s : rownames) {
				string_type indsigle = to_upper(trim(s));
				auto it = std::find_if(inds.begin(), inds.end(), [indsigle](item_ptr p)->bool {
					Indiv *pInd = dynamic_cast<Indiv *>(p.get());
					return (pInd->sigle() == indsigle);
				});
				if (it == inds.end()) {
					Indiv x{ setsigle,indsigle };
					x.name(indsigle);
					Assert::IsTrue(x.is_storeable());
					item_ptr xx{ new Indiv{ x } };
					Assert::IsNotNull(xx.get());
					oIndsAdd.push_back(xx);
				}// not found
			}// s
			if (!oIndsAdd.empty()) {
				itemptrvector_ptr pz = std::make_shared<itemptr_vector>();
				itemptr_vector *pzz = pz.get();
				Assert::IsNotNull(pzz);
				*pzz = oIndsAdd;
				bool bRet = pMan->maintains_elements_async(pz).get();
				Assert::IsTrue(bRet);
			}// addVars
			//
			Value oValModel{};
			oValModel.dataset_sigle(setsigle);
			itemptr_vector vals{};
			int nVals = pMan->get_count_async(oValModel).get();
			if (nVals > 0) {
				itemptrvector_ptr p = pMan->get_elements_async(oValModel, 0, nVals).get();
				itemptr_vector *pv = p.get();
				if (pv != nullptr) {
					vals = *pv;
				}
			}// nVals
			itemptr_vector oValsAdd{};
			for (size_t i = 0; i < nRows; ++i) {
				string_type indsigle = to_upper(trim(rownames[i]));
				for (size_t j = 0; j < nCols; ++j) {
					string_type varsigle = to_upper(trim(colnames[j]));
					auto it = std::find_if(vals.begin(), vals.end(), [indsigle, varsigle](item_ptr p)->bool {
						Value *px = dynamic_cast<Value *>(p.get());
					return (px->variable_sigle() == varsigle) && (px->indiv_sigle() == indsigle);
					});
					if (it == vals.end()) {
						any v{data[i * nCols + j]};
						Value x{setsigle,indsigle,varsigle,v};
						Assert::IsTrue(x.is_storeable());
						item_ptr xx{ new Value{ x } };
						Assert::IsNotNull(xx.get());
						oValsAdd.push_back(xx);
					}// not found
				}// sv
			}// si
			if (!oValsAdd.empty()) {
				itemptrvector_ptr pz = std::make_shared<itemptr_vector>();
				itemptr_vector *pzz = pz.get();
				Assert::IsNotNull(pzz);
				*pzz = oValsAdd;
				bool bRet = pMan->maintains_elements_async(pz).get();
				Assert::IsTrue(bRet);
			}// addVars
		}// test_import_mortal
	};// class PersistManagerTest

	  ///////////////////////////
}// namespace UnitTest_GenData
