//itemfactory.cpp
///////////////////////////
#include "stdafx.h"
#include "itemfactory.h"
//////////////////////////
#include "generalitem.h"
#include "dataset.h"
#include "etudiant.h"
#include "variable.h"
#include "indiv.h"
#include "infostrings.h"
#include "value.h"
////////////////////////////
namespace info {
namespace domain {
//////////////////////////////////
item_ptr ItemFactory::create(const string_type &stype) {
	item_ptr oRet { };
	string_type s = to_lower(trim(stype));
	if (s.empty()) {
		return (oRet);
	}
	if (s == TYPE_VALUE) {
		oRet = std::make_shared<Value>();
	} else if (s == TYPE_INDIV) {
		oRet = std::make_shared<Indiv>();
	} else if (s == TYPE_VARIABLE) {
		oRet = std::make_shared<Variable>();
	} else if (s == TYPE_DATASET) {
		oRet = std::make_shared<Dataset>();
	} else if (s == TYPE_ETUDIANT) {
		oRet = std::make_shared<Etudiant>();
	} else if (s == TYPE_GENERAL) {
		oRet = std::make_shared<GeneralItem>();
	}
	return (oRet);
} // create
item_ptr ItemFactory::create(const anymap_type &oMap) {
	item_ptr oRet { };
	auto it = oMap.find(KEY_TYPE);
	if (it == oMap.end()) {
		return (oRet);
	}
	any v = (*it).second;
	string_type stype { };
	if (!info_any_to_string(v, stype)) {
		return (oRet);
	}
	string_type s = to_lower(trim(stype));
	if (s.empty()) {
		return (oRet);
	}
	if (s == TYPE_VALUE) {
		oRet = std::make_shared<Value>(oMap);
	} else if (s == TYPE_INDIV) {
		oRet = std::make_shared<Indiv>(oMap);
	} else if (s == TYPE_VARIABLE) {
		oRet = std::make_shared<Variable>(oMap);
	} else if (s == TYPE_DATASET) {
		oRet = std::make_shared<Dataset>(oMap);
	} else if (s == TYPE_ETUDIANT) {
		oRet = std::make_shared<Etudiant>(oMap);
	} else if (s == TYPE_GENERAL) {
		oRet = std::make_shared<GeneralItem>(oMap);
	}
	return (oRet);
} // create
//////////////////////////////////
}		// namespace data
}		// namespace info
