#pragma once
#ifndef INFO_COMMON_H
#define INFO_COMMON_H
//////////////////////////
#include "stdafx.h"
//////////////////////////
namespace info {
	////////////////////////
#ifndef _MSC_VER
	using any = boost::any;
#define INFO_ANY_EMPTY(x) ((x).empty())
#else
	using any = std::any;
#define INFO_ANY_EMPTY(x) (!(x).has_value())
#endif // _MSC_VER
	//////////////////////
	using char_t = utility::char_t;
	using string_t = utility::string_t;
	using ostream_t = utility::ostream_t;
	using istream_t = utility::istream_t;
	using ostringstream_t = utility::ostringstream_t;
	using istringstream_t = utility::istringstream_t;
	using stringstream_t = utility::stringstream_t;
	using ifstream_t = utility::ifstream_t;
	using ofstream_t = utility::ofstream_t;
	//////////////////////////
	using string_type = string_t;
	class Blob;
	////////////////////////////
	using byte = unsigned char;
	using anymap_type = std::map<string_type, any>;
	using anymap_vector = std::vector<anymap_type>;
	using anymap_ptr = std::shared_ptr<anymap_type>;
	using anymapvector_ptr = std::shared_ptr<anymap_vector>;
	using blob_ptr = std::shared_ptr<Blob>;
	using strings_vector = std::vector<string_type>;
	using stringsvector_ptr = std::shared_ptr<strings_vector>;
	/////////////////////////
	extern string_type trim(const string_type &s);
	extern string_type to_lower(const string_type &s);
	extern string_type to_upper(const string_type &s);
	extern void info_split_string(const string_type &s, std::vector<string_type> &vec, const string_type &delim = string_type{ U(";,\t ") });
	extern void info_any_to_stringset(const any &src, std::set<string_type> &oSet);
	/////////////////////////////
	extern bool info_any_to_bool(const any &, bool &bRet);
	extern bool info_any_to_int(const any &, int &bRet);
	extern bool info_any_to_double(const any &, double &bRet);
	extern bool info_any_to_string(const any &, string_type &bRet);
	///////////////////////////////
}// namespace info
////////////////////////////
#endif // INFO_COMMON_H
