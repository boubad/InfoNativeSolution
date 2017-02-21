/*
 * couchdbpersistmanager.cpp
 *
 *  Created on: 26 janv. 2017
 *      Author: boubad
 */
#include "stdafx.h"
#include "couchdbpersistmanager.h"
#include "couchdbmanager.h"
#include "couchdoc.h"
#include "findselector.h"
#include  "blob.h"
//////////////////////////////////
namespace info {
namespace persist {
//////////////////////////////////
static const string_type STRING_ID { U("_id") };
static const string_type STRING_REV {U("_rev") };
static const string_t STRING_ATTACHMENTS { U("_attachments") };
static const string_t STRING_CREATED { U("created") };
static const string_t STRING_EXISTS { U("exists") };
////////////////////////////////////////
namespace internal {
static void convert_findselector(const anymap_type &oMap,
		std::map<string_t, value> &destMap) {
	for (auto it = oMap.begin(); it != oMap.end(); ++it) {
		const any &va = (*it).second;
		const string_type & s = (*it).first;
		value vx = any_to_value(va);
		destMap[s] = vx;
	} // it
} //convert_findselector
static void convert_valueobject_tomap(const value &vo, anymap_type &oMap) {
	oMap.clear();
	if ((!vo.is_null()) && vo.is_object()) {
		const object &oo = vo.as_object();
		for (auto it = oo.begin(); it != oo.end(); ++it) {
			const string_t &sx = (*it).first;
			const value &vx = (*it).second;
			if (vx.is_null()) {
				oMap[sx] = any { };
				continue;
			}
			if (vx.is_object()) {
				continue;
			}
			any vv = value_to_any(vx);
			oMap[sx] = vv;
		} // it
	} // vo
} //convert_valueobject_tomap
static void convert_map_tovalueobject(const anymap_type &oMap, value &vRet) {
	vRet = value::object();
	object &oo = vRet.as_object();
	for (auto it = oMap.begin(); it != oMap.end(); ++it) {
		const string_type &sx = (*it).first;
		const any &va = (*it).second;
		value vz = any_to_value(va);
		oo[sx] = vz;
	}
} //convert_map_tovalueobject
} // namespace internal
/////////////////////////////////
CouchDBPersistManager::CouchDBPersistManager(const string_type &baseUrl,
		const string_type &data) {
	m_man.reset(new CouchDBManager { baseUrl, data });
	m_id = baseUrl + string_type { U("_") } + data;
	m_database = data;
}
CouchDBPersistManager::~CouchDBPersistManager() {

}
bool CouchDBPersistManager::is_valid(void) const {
	return (m_man.get() != nullptr);
}
CouchDBManager *CouchDBPersistManager::get_manager(void) const {
	assert(m_man.get() != nullptr);
	return (this->m_man.get());
}
////////////////////////////////////
string_type CouchDBPersistManager::get_persister_id(void) const {
	return (m_id);
}
string_type CouchDBPersistManager::get_attachment_url(const string_type &id,
		const string_type &name) const {
	string_type sRet { };
	CouchDBManager *pMan = this->get_manager();
	return pMan->get_attachment_url(id, name, m_database);
}
task_bool CouchDBPersistManager::chek_index_async(const index_vector &oinds,
		const string_type &index_name) const {
	CouchDBManager *pMan = this->get_manager();
	CreateIndexData data { };
	data.set_name(index_name);
	for (auto p : oinds) {
		string_t s { };
		data.add_index_field(p.first, p.second);
	}
	return pMan->create_index(data).then([](task_createindex t) {
		CreateIndexResponse rsp = t.get();
		string_t rx = rsp.result;
		return ((rx == STRING_EXISTS) || (rx == STRING_CREATED));
	});
}
task_int CouchDBPersistManager::get_count_async(const anymap_type &oMap) const {

	const anymap_type *pMap = &oMap;
	return task_int { [pMap,this]()->int {
		CouchDBManager *pMan = this->get_manager();
		std::map<string_t, value> opts {};
		internal::convert_findselector(*pMap, opts);
		int nRet = pMan->get_docs_count(opts, m_database).get();
		return (nRet);
	} };
} 	 	// get_count
task_anymapvector CouchDBPersistManager::get_elements_async(
		const anymap_type &oModel, int offset /*= 0*/,
		int count /*= 0*/) const {
	CouchDBManager *pMan = this->get_manager();
	std::map<string_t, value> opts { };
	internal::convert_findselector(oModel, opts);
	return pMan->read_docs(opts, offset, count).then([](task_valuevector t) {
		value_vector vv {t.get()};
		anymapvector_ptr oRet = std::make_shared<anymap_vector>();
		anymap_vector *pv = oRet.get();
		assert(pv != nullptr);
		for (auto it = vv.begin(); it != vv.end(); ++it) {
			anymap_type m {};
			internal::convert_valueobject_tomap(*it, m);
			if (!m.empty()) {
				pv->push_back(m);
			}
		} 		// it
			return (oRet);
		});
} 		//get_elements
task_bool CouchDBPersistManager::delete_documents_async(const anymap_type &oModel) {
	CouchDBManager *pMan = this->get_manager();
	std::map<string_t, value> oMap{};
	for (auto it = oModel.begin(); it != oModel.end(); ++it) {
		const string_type &sx = (*it).first;
		const any &va = (*it).second;
		value vz = any_to_value(va);
		oMap[sx] = vz;
	}
	return pMan->delete_documents(oMap);
}//delete_documents_async
task_anymap CouchDBPersistManager::find_element_async(
		const anymap_type &oMap) const {
	CouchDBManager *pMan = this->get_manager();
	const anymap_type *pMap = &oMap;
	return task_anymap { [pMap,pMan]()->anymap_ptr {
		anymap_ptr oRet {};
		auto it = pMap->find(STRING_ID);
		if (it != pMap->end()) {
			any v = (*it).second;
			string_type id {};
			if (info_any_to_string(v, id)) {
				if (!id.empty()) {
					ReadDocOptions opts {};
					opts.attachments = true;
					value vp = pMan->read_document(id, opts).get();
					if (!vp.is_null()) {
						anymap_ptr p = std::make_shared<anymap_type>();
						anymap_type *x = p.get();
						assert(x != nullptr);
						internal::convert_valueobject_tomap(vp, *x);
						return (p);
					}
				} 		// id
		} 		// sid
	} 		// has id
	std::map<string_t, value> opts {};
	internal::convert_findselector(*pMap, opts);
	value_vector vv = pMan->read_docs(opts, 0, 1).get();
	anymap_ptr p {};
	if (!vv.empty()) {
		p = std::make_shared<anymap_type>();
		anymap_type *x = p.get();
		assert(x != nullptr);
		internal::convert_valueobject_tomap(*(vv.begin()), *x);
	} 		// not empty
	return (p);
}	};
} //find_element
task_anymap CouchDBPersistManager::maintains_element_async(
		const anymap_type &oMap) const {
	CouchDBManager *pMan = this->get_manager();
	const anymap_type *pMap = &oMap;
	return task_anymap { [pMap, pMan]()->anymap_ptr {
		anymap_ptr oRet {};
		value vo {};
		internal::convert_map_tovalueobject(*pMap, vo);
		UpdateResponse r = pMan->maintains_document(vo).get();
		if (r.ok()) {
			oRet = std::make_shared<anymap_type>(*pMap);
			anymap_type *x = oRet.get();
			assert(x != nullptr);
			(*x)[STRING_ID] = any {r.id()};
			(*x)[STRING_REV] = any {r.rev()};
		} // ok
			return (oRet);
		} };
} 		//maintains_element
task_bool CouchDBPersistManager::remove_element_async(
		const anymap_type &oMap) const {
	CouchDBManager *pMan = this->get_manager();
	const anymap_type *pMap = &oMap;
	return task_bool { [pMan, pMap, this]()->bool {
		anymap_ptr p = this->find_element_async(*pMap).get();
		if (p.get() == nullptr) {
			return (false);
		}
		auto jt = pMap->find(STRING_ID);
		if (jt == pMap->end()) {
			return (false);
		}
		const any &vid = (*jt).second;
		string_type sid {};
		if (!info_any_to_string(vid, sid)) {
			return (false);
		}
		if (sid.empty()) {
			return (false);
		}
		UpdateResponse r = pMan->delete_document(sid).get();
		return (r.ok());
	} };
} 		//remove_element
task_bool CouchDBPersistManager::maintains_elements_async(
		anymapvector_ptr vecPtr) const {
	anymap_vector *pvec = vecPtr.get();
	assert(pvec != nullptr);
	CouchDBManager *pMan = this->get_manager();
	return task_bool { [pvec,pMan]()->bool {
		value_vector docs {};
		for (auto it = pvec->begin(); it != pvec->end(); ++it) {
			const anymap_type &oMap = *it;
			value vo {};
			internal::convert_map_tovalueobject(oMap, vo);
			docs.push_back(vo);
		} 		// it
			std::vector<UpdateResponse> ts = pMan->maintains_documents(docs).get();
			bool bRet {true};
			for (auto it = ts.begin(); it != ts.end(); ++it) {
				const UpdateResponse &r = *it;
				if (!r.ok()) {
					bRet = false;
					break;
				}
			}
			return (bRet);
		} };
} 		//maintains_elements
task_bool CouchDBPersistManager::remove_elements_async(
		anymapvector_ptr vecPtr) const {
	anymap_vector *pvec = vecPtr.get();
	assert(pvec != nullptr);
	CouchDBManager *pMan = this->get_manager();
	return task_bool { [pvec,pMan]()->bool {
		value_vector docs {};
		for (auto it = pvec->begin(); it != pvec->end(); ++it) {
			const anymap_type &oMap = *it;
			value vo {};
			internal::convert_map_tovalueobject(oMap, vo);
			docs.push_back(vo);
		} 		// it
			std::vector<UpdateResponse> ts = pMan->maintains_documents(docs,true).get();
			bool bRet {true};
			for (auto it = ts.begin(); it != ts.end(); ++it) {
				const UpdateResponse &r = *it;
				if (!r.ok()) {
					bRet = false;
					break;
				}
			}
			return (bRet);
		} };
} 		//remove_elements
//
task_bool CouchDBPersistManager::maintains_blob_async(const Blob &oBlob) const {
	assert(oBlob.is_valid());
	const Blob *pBlob = &oBlob;
	CouchDBManager *pMan = this->get_manager();
	return task_bool {
			[pMan, pBlob]()->bool {
				string_t sid = pBlob->id();
				string_t sname = pBlob->name();
				string_t scontent = pBlob->content_type();
				UpdateResponse r = pMan->maintains_attachment(sid, sname, scontent, pBlob->data_vector()).get();
				return (r.ok());
			} };

} 		//maintains_blob
task_bool CouchDBPersistManager::remove_blob_async(const Blob &oBlob) const {
	assert(oBlob.is_valid());
	const Blob *pBlob = &oBlob;
	CouchDBManager *pMan = this->get_manager();
	return task_bool { [pMan, pBlob]()->bool {
		string_type id = pBlob->id();
		string_t sname = pBlob->name();
		UpdateResponse r = pMan->remove_attachment(id, sname).get();
		return (r.ok());
	} };

} 		//remove_blob
task_blob CouchDBPersistManager::read_blob_async(const Blob &oModel) const {
	const Blob *pModel = &oModel;
	CouchDBManager *pMan = this->get_manager();
	return task_blob { [pMan,pModel]()->blob_ptr {
		blob_ptr oRet {};
		string_t sid = pModel->id();
		string_t sname = pModel->name();
		AttachmentData dx = pMan->read_attachment(sid, sname).get();
		unsigned char *pData = dx.dataPtr.get();
		size_t n = dx.length;
		string_type type = dx.content_type;
		if ((n < 1) || (pData == nullptr) || type.empty()) {
			return (oRet);
		}
		oRet = std::make_shared<Blob>(pData,n);
		Blob *p = oRet.get();
		assert(p != nullptr);
		p->id(sid);
		p->content_type(type);
		p->name(pModel->name());
		return (oRet);
	} };

} 		//read_blob
task_stringsvector CouchDBPersistManager::read_all_blobs_async(
		const string_type &id) const {
	const string_type *pid = &id;
	CouchDBManager *pMan = this->get_manager();
	return task_stringsvector { [pMan, pid]()->stringsvector_ptr {
		stringsvector_ptr oRet {};
		string_t sid = *pid;
		ReadDocOptions options {};
		options.attachments = true;
		value v = pMan->read_document(sid, options).get();
		oRet.reset(new std::vector<string_type> {});
		std::vector<string_type> *pRet = oRet.get();
		assert(pRet != nullptr);
		if (!v.is_null()) {
			if (v.has_field(STRING_ATTACHMENTS)) {
				const value &vx = v.at(STRING_ATTACHMENTS);
				if ((!vx.is_null()) && vx.is_object()) {
					const object &obj = vx.as_object();
					for (auto it = obj.begin(); it != obj.end(); ++it) {
						const string_t key = (*it).first;
						if (!key.empty()) {
							pRet->push_back(key);
						}
					} 		// it
		} 		// object
	} 		// attachment
}
return (oRet);
}	};
} 		//read_all_blobs
//////////////////
} 		// namespace persist
} 		// namespace info
