//blob.cpp
/////////////////////////
#include "stdafx.h"
#include "blob.h"
///////////////////////////
namespace info {
	////////////////////////////
	string_type Blob::DEFAULT_MIMETYPE{ U("image/jpeg") };
	/////////////////////////////////
	Blob::Blob() {
	}
	Blob::Blob(const void *pData, size_t nSize, const string_type &stype /* = DEFAULT_MIMETYPE*/) {
		const byte *pSrc = reinterpret_cast<const byte *>(pData);
		if ((pSrc != nullptr) && (nSize > 0)) {
			m_data = std::vector<byte>{ pSrc, pSrc + nSize };
		}
		this->content_type(stype);
	}
	Blob::Blob(const std::vector<byte> &data, const string_type &stype /* = DEFAULT_MIMETYPE*/) {
		this->content_type(stype);
		size_t n = data.size();
		if (n > 0) {
			m_data = data;
		}
	}
	void Blob::data(const std::vector<byte> &data, const string_type &stype /*= DEFAULT_MIMETYPE*/) {
		this->content_type(stype);
		m_data = data;
	}// data
	Blob::~Blob() {
	}
	const std::set<string_type> & Blob::all_content_types(void) const {
		return (this->m_types);
	}
	const string_type &Blob::name(void) const {
		return (this->m_name);
	}
	void Blob::name(const string_type &s) {
		string_type ss = trim(s);
		if (!ss.empty()) {
			m_name = ss;
		}
	}
	bool Blob::empty(void) const {
		return (m_data.empty());
	}
	bool Blob::is_valid(void) const {
		return ((!m_data.empty()) && (!m_types.empty()) && (!m_name.empty()) && (!m_id.empty()));
	}
	void Blob::clean(void) {
		m_data.clear();
		m_types.clear();
	}
	size_t Blob::size(void) const {
		return (this->m_data.size());
	}
	string_type Blob::content_type(void) const {
		string_type sRet{};
		if (!m_types.empty()) {
			sRet = *(m_types.begin());
		}
		return (sRet);
	}
	void Blob::content_type(const string_type &s) {
		string_type delim{U(",")};
		std::vector<string_type> vec{};
		info_split_string(s, vec,delim);
		for (auto ss : vec) {
			auto sx = trim(ss);
			if (!sx.empty()) {
				m_types.insert(sx);
			}
		}// vec
	}
	const byte *Blob::data(string_type &stype, size_t &n) const {
		stype = this->content_type();
		n = m_data.size();
		return (m_data.data());
	}
	void Blob::data(const void *pData, size_t nSize, const string_type &stype /*= DEFAULT_MIMETYPE*/) {
		const byte *pSrc = reinterpret_cast<const byte *>(pData);
		const byte *pOld = this->m_data.data();
		if ((pOld != pSrc) && (nSize > 0)) {
			m_data = std::vector<byte>{ pSrc, pSrc + nSize };
		}
		this->content_type(stype);
	}
	void Blob::to_vector(std::vector<byte> &vec) const {
		vec = m_data;
	}// to_vector
	const string_type Blob::id(void) const {
		return (this->m_id);
	}
	void Blob::id(const string_type &s) {
		this->m_id = s;
	}
	///////////////////////////////
}// namespace info
