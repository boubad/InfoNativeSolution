#pragma once
#ifndef BLOB_H
#define BLOB_H
////////////////////////
#include "infocommon.h"
/////////////////////////////////
namespace info {
	//////////////////////////////
	class Blob {
	private:
		static string_type DEFAULT_MIMETYPE;
		//
		Blob(const Blob &other) = delete;
		Blob & operator=(const Blob &other) = delete;
	public:
		Blob();
		Blob(const void *pData, size_t nSize, const string_type &stype = DEFAULT_MIMETYPE);
		Blob(const std::vector<byte> &data, const string_type &stype = DEFAULT_MIMETYPE);
		~Blob();
	public:
		const string_type id(void) const;
		void id(const string_type &s);
		const string_type &name(void) const;
		void name(const string_type &s);
		bool is_valid(void) const;
		bool empty(void) const;
		size_t size(void) const;
		string_type  content_type(void) const;
		void content_type(const string_type &s);
		const byte *data(string_type &stype, size_t &n) const;
		void data(const void *pData, size_t nSize, const string_type &stype = DEFAULT_MIMETYPE);
		void data(const std::vector<byte> &data, const string_type &stype = DEFAULT_MIMETYPE);
		void to_vector(std::vector<byte> &vec) const;
		const std::vector<byte> &data_vector(void) const {
			return (this->m_data);
		}
		void clean(void);
		const std::set<string_type> & all_content_types(void) const;
	private:
		string_type m_name;
		string_type m_id;
		std::set<string_type> m_types;
		std::vector<byte> m_data;
	}; // class Blob
	////////////////////////////////
}// namespace info
//////////////////////////
#endif // BLOB_H
