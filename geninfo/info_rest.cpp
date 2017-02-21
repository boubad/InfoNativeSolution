//info_rest.cpp
#include "stdafx.h"
////////////////////////
#if defined(_MSC_VER)
#pragma warning(disable: 4996)
#endif // _MSC_VER
////////////////////////
#include "info_rest.h"
//////////////////////////////
namespace info {
namespace persist {
#ifndef _MSC_VER
using namespace boost;
#endif //_MSC_VER
/////////////////////////////////////
using namespace std;
/////////////////////////////
namespace {
std::string convert_to_string(const string_t &s) {
	if (s.empty()) {
		return std::string{};
	}
#ifdef _UTF16_STRINGS
	return utility::conversions::utf16_to_utf8(s);
#else
	std::string sRet = s;
	return sRet;
#endif
}
std::string encimpl(std::string::value_type v) {
	if (isalnum(v)) {
		return std::string() + v;
	}
	std::ostringstream enc { };
	enc << '%' << std::setw(2) << std::setfill('0') << std::hex
			<< std::uppercase << int(static_cast<unsigned char>(v));
	return enc.str();
}
}
static std::string urlencode(const std::string& url) {
	std::string qstr { };
	std::transform(url.begin(), url.end(),
	// Append the transform result to qstr
			boost::make_function_output_iterator(
					boost::bind(
							static_cast<std::string& (std::string::*)(
									const std::string&)>(&std::string::append), &qstr, _1)),
	encimpl);
	//return std::string(url.begin(), start + 1) + qstr;
	return qstr;
}

////////////////////////////////
extern string_t url_encode(const string_t &url) {
	std::string surl = convert_to_string(url);
	std::string sRes = urlencode(surl);
	return utility::conversions::to_string_t(sRes);
}			// url_encode
/////////////////////////////////////
extern any value_to_any(const value &v) {
	any vRet { };
	if (!v.is_null()) {
		if (v.is_string()) {
			vRet  = any{v.as_string()};
		} else if (v.is_boolean()) {
			vRet = any{v.as_bool()};
		} else if (v.is_integer()) {
			vRet = any{v.as_integer()};
		} else if (v.is_double()) {
			vRet = v.as_double();
		} else if (v.is_array()) {
			const array &oAr = v.as_array();
			std::vector<any> v { };
			for (auto va : oAr) {
				any vx = value_to_any(va);
				v.push_back(vx);
			}		// it
			vRet = any { v };
		} else if (v.is_object()) {
			vRet = any { value_to_anymap(v) };
		}
	}		// not null
	return (vRet);
}		//value_to_any
///////////////////////////////
extern value any_to_value(const any &va) {
	value vRet { };
	if (!INFO_ANY_EMPTY(va)){
		if (va.type() == typeid(string_t)) {
			vRet = value{any_cast<string_type>(va)};
		} else if (va.type() == typeid(int)) {
			vRet = value{any_cast<int>(va)};
		} else if (va.type() == typeid(double)) {
			vRet = value{any_cast<double>(va)};
		} else if (va.type() == typeid(bool)) {
			vRet = value{any_cast<bool>(va)};
		} else if (va.type() == typeid(float)) {
			float s = any_cast<float>(va);
			double b { static_cast<double>(s) };
			vRet = value { b };
		} else if (va.type() == typeid(std::vector<string_t>)) {
			std::vector<string_t> v = any_cast<std::vector<string_t>>(
					va);
			std::vector<value> vx { };
			for (auto s1 : v) {
				vx.push_back(value { s1 });
			}
			vRet = value { value::array(vx) };
		} else if (va.type() == typeid(std::set<string_t>)) {
			std::set<string_t> v = any_cast<std::set<string_t>>(va);
			std::vector<value> vx { };
			for (auto s1 : v) {
				vx.push_back(value { s1 });
			}
			vRet = value { value::array(vx) };
		} else if (va.type() == typeid(std::vector<any>)) {
			std::vector<any> v = any_cast<std::vector<any>>(va);
			std::vector<value> vx { };
			for (auto s1 : v) {
				value s2 = any_to_value(s1);
				vx.push_back(s2);
			}
			vRet = value { value::array(vx) };
		} else if (va.type() == typeid(short)) {
			short s = any_cast<short>(va);
			int b { static_cast<int>(s) };
			vRet = value { b };
		} else if (va.type() == typeid(unsigned short)) {
			unsigned short s = any_cast<unsigned short>(va);
			int b { static_cast<int>(s) };
			vRet = value { b };
		} else if (va.type() == typeid(unsigned int)) {
			unsigned int s = any_cast<unsigned int>(va);
			double b { static_cast<double>(s) };
			vRet = value { b };
		} else if (va.type() == typeid(long)) {
			long s = any_cast<long>(va);
			double b { static_cast<double>(s) };
			vRet = value { b };
		} else if (va.type() == typeid(long long)) {
			long long s = any_cast<long long>(va);
			double b { static_cast<double>(s) };
			vRet = value { b };
		} else if (va.type() == typeid(unsigned long)) {
			unsigned long s = any_cast<unsigned long>(va);
			double b { static_cast<double>(s) };
			vRet = value { b };
		} else if (va.type() == typeid(long double)) {
			long double s = any_cast<long double>(va);
			double b { static_cast<double>(s) };
			vRet = value { b };
		} else if (va.type() == typeid(char)) {
			char s = any_cast<char>(va);
			int b { static_cast<int>(s) };
			vRet = value { b };
		} else if (va.type() == typeid(wchar_t)) {
			wchar_t s = any_cast<wchar_t>(va);
			int b { static_cast<int>(s) };
			vRet = value { b };
		} else if (va.type() == typeid(anymap_type)) {
			anymap_type vm = any_cast<anymap_type>(va);
			vRet = anymap_to_value(vm);
		}
	} // empty
	return (vRet);
} // any_to_value
////////////////////////////////
extern value anymap_to_value(const anymap_type &oMap) {
	value oRet { value::object() };
	object &obj = oRet.as_object();
	for (auto it = oMap.begin(); it != oMap.end(); ++it) {
		const string_type key = (*it).first;
		if (!key.empty()) {
			value vx = any_to_value((*it).second);
			obj[key] = vx;
		}
	}		// it
	return (oRet);
}		// anymap_to_value
extern anymap_type value_to_anymap(const value &v) {
	anymap_type oRet { };
	if (!v.is_null()) {
		if (v.is_object()) {
			const object &oo = v.as_object();
			for (auto it = oo.begin(); it != oo.end(); ++it) {
				string_t k = (*it).first;
				any vr = value_to_any((*it).second);
				oRet[k] = vr;
			}		// it
		}		// object
	}		// not null
	return (oRet);
}		//value_to_anymap(
/////////////////////////////
}		// namespace persist
}		// namespace geninfo
