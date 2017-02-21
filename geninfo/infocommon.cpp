//info_common.cpp
/////////////////////
#include "stdafx.h"
#include "infocommon.h"
///////////////////////////
namespace info {
	//////////////////////////
#ifndef _MSC_VER
	using namespace boost;
#endif // _MSC_VER
	using namespace std;
	////////////////////////
	extern string_type trim(const string_type &s) {
		string_type sRet{};
		const size_t n = s.length();
		if (n > 0) {
			auto pStart = s.c_str();
			auto pEnd = pStart + n - 1;
			while ((*pStart != 0) && std::isblank(*pStart)) {
				pStart++;
			}	// while
			while ((pStart < pEnd) && std::isblank(*pEnd)) {
				pEnd--;
			}
			pEnd++;
			sRet = string_type{ pStart, pEnd };
		}	// not empty
		return (sRet);
	}	// trim
	extern void info_split_string(const string_type &s,
		std::vector<string_type> &vec, const string_type &delim) {
		vec.clear();
		string_type ss{ s };
		size_t pos = ss.find_first_of(delim);
		while (pos != std::string::npos) {
			string_type sx{ ss.begin(), ss.begin() + pos };
			vec.push_back(trim(sx));
			ss = string_type{ ss.begin() + pos + 1, ss.end() };
			pos = ss.find_first_of(delim);
		} // while
		if (!ss.empty()) {
			vec.push_back(trim(ss));
		}
	} //info_split_string
	//////////////////////////////////
	extern string_type to_lower(const string_type &s) {
		if (s.empty()) {
			return (string_type{});
		}
		string_type sRet{s};
		std::locale loc{ };
		std::transform(s.begin(), s.end(), sRet.begin(), [loc](char_t c)->char_t {
			return (std::tolower(c));
		});
		return (sRet);
	}	// to_lower
	extern string_type to_upper(const string_type &s) {
		if (s.empty()) {
			return (string_type{});
		}
		string_type sRet{ s };
		std::locale loc{};
		std::transform(s.begin(), s.end(), sRet.begin(), [loc](char_t c)->char_t {
			return (std::toupper(c));
		});
		return (sRet);
	}	// to_upper
	/////////////////////////
	extern bool info_any_to_bool(const any &v, bool &res) {
		if (!INFO_ANY_EMPTY(v)) {
			if (v.type() == typeid(bool)) {
				res = any_cast<bool>(v);
				return (true);
			}
			else if (v.type() == typeid(char)) {
				char b = std::tolower(any_cast<char>(v));
				if ((b == 't') || (b == 'y') || (b == 'o') || (b == 'v')) {
					res = true;
					return (true);
				}
				if ((b == 'f') || (b == 'n') || (b == 'n')) {
					res = false;
					return (true);
				}
				res = (b != 0) ? true : false;
				return (true);
			}
			else if (v.type() == typeid(unsigned char)) {
				unsigned char bb = any_cast<unsigned char>(v);
				char b{ static_cast<char>(bb) };
				if ((b == 't') || (b == 'y') || (b == 'o') || (b == 'v')) {
					res = true;
					return (true);
				}
				if ((b == 'f') || (b == 'n') || (b == 'n')) {
					res = false;
					return (true);
				}
				res = (b != 0) ? true : false;
				return (true);
			}
			else if (v.type() == typeid(wchar_t)) {
				wchar_t b = std::tolower(any_cast<wchar_t>(v));
				if ((b == L't') || (b == L'y') || (b == L'o') || (b == L'v')) {
					res = true;
					return (true);
				}
				if ((b == L'f') || (b == L'n') || (b == L'n')) {
					res = false;
					return (true);
				}
				res = (b != 0) ? true : false;
				return (true);
			}
			else if (v.type() == typeid(short)) {
				short b = any_cast<short>(v);
				res = (b != 0) ? true : false;
				return (true);
			}
			else if (v.type() == typeid(unsigned short)) {
				unsigned short b = any_cast<unsigned short>(v);
				res = (b != 0) ? true : false;
				return (true);
			}
			else if (v.type() == typeid(int)) {
				int b = any_cast<int>(v);
				res = (b != 0) ? true : false;
				return (true);
			}
			else if (v.type() == typeid(unsigned int)) {
				unsigned int b = any_cast<unsigned int>(v);
				res = (b != 0) ? true : false;
				return (true);
			}
			else if (v.type() == typeid(long)) {
				long b = any_cast<long>(v);
				res = (b != 0) ? true : false;
				return (true);
			}
			else if (v.type() == typeid(unsigned long)) {
				unsigned long b = any_cast<unsigned long>(v);
				res = (b != 0) ? true : false;
				return (true);
			}
			else if (v.type() == typeid(float)) {
				float b = any_cast<float>(v);
				res = (b != 0) ? true : false;
				return (true);
			}
			else if (v.type() == typeid(double)) {
				double b = any_cast<double>(v);
				res = (b != 0) ? true : false;
				return (true);
			}
			else if (v.type() == typeid(string_type)) {
				string_type bb = to_lower(trim(any_cast<string_type>(v)));
				if (!bb.empty()) {
					auto b{ *(bb.begin()) };
					res = ((b == U('t')) || (b == U('y')) || (b == U('o'))
						|| (b == U('v')));
					return (true);
				}
			}
		} // not empty
		return (false);
	} //info_any_to_bool
	extern bool info_any_to_int(const any &v, int &res) {
		if (!INFO_ANY_EMPTY(v)) {
			if (v.type() == typeid(bool)) {
				bool b = any_cast<bool>(v);
				res = (b) ? 1 : 0;
				return (true);
			}
			else if (v.type() == typeid(char)) {
				char b = std::tolower(any_cast<char>(v));
				res = static_cast<int>(b);
				return (true);
			}
			else if (v.type() == typeid(unsigned char)) {
				unsigned char b = any_cast<unsigned char>(v);
				res = static_cast<int>(b);
				return (true);
			}
			else if (v.type() == typeid(wchar_t)) {
				wchar_t b = std::tolower(any_cast<wchar_t>(v));
				res = static_cast<int>(b);
				return (true);
			}
			else if (v.type() == typeid(short)) {
				short b = any_cast<short>(v);
				res = static_cast<int>(b);
				return (true);
			}
			else if (v.type() == typeid(unsigned short)) {
				unsigned short b = any_cast<unsigned short>(v);
				res = static_cast<int>(b);
				return (true);
			}
			else if (v.type() == typeid(int)) {
				res = any_cast<int>(v);
				return (true);
			}
			else if (v.type() == typeid(unsigned int)) {
				unsigned int b = any_cast<unsigned int>(v);
				res = static_cast<int>(b);
				return (true);
			}
			else if (v.type() == typeid(long)) {
				long b = any_cast<long>(v);
				res = static_cast<int>(b);
				return (true);
			}
			else if (v.type() == typeid(unsigned long)) {
				unsigned long b = any_cast<unsigned long>(v);
				res = static_cast<int>(b);
				return (true);
			}
			else if (v.type() == typeid(float)) {
				float b = any_cast<float>(v);
				res = static_cast<int>(b);
				return (true);
			}
			else if (v.type() == typeid(double)) {
				double b = any_cast<double>(v);
				res = static_cast<int>(b);
				return (true);
			}
			else if (v.type() == typeid(long double)) {
				long double b = any_cast<long double>(v);
				res = static_cast<int>(b);
				return (true);
			}
			else if (v.type() == typeid(std::string)) {
				std::string bb = any_cast<std::string>(v);
				if (!bb.empty()) {
					std::stringstream in(bb);
					in >> res;
					return (in.good());
				}
			}
			else if (v.type() == typeid(std::wstring)) {
				std::wstring bb = any_cast<std::wstring>(v);
				if (!bb.empty()) {
					std::wstringstream in(bb);
					in >> res;
					return (in.good());
				}
			}
		} // not empty
		return (false);
	} //info_any_to_int
	extern bool info_any_to_double(const any &v, double &res) {
		if (!INFO_ANY_EMPTY(v)) {
			if (v.type() == typeid(bool)) {
				bool b = any_cast<bool>(v);
				res = (b) ? 1 : 0;
				return (true);
			}
			else if (v.type() == typeid(char)) {
				char b = std::tolower(any_cast<char>(v));
				res = static_cast<double>(b);
				return (true);
			}
			else if (v.type() == typeid(unsigned char)) {
				unsigned char b = any_cast<unsigned char>(v);
				res = static_cast<double>(b);
				return (true);
			}
			else if (v.type() == typeid(wchar_t)) {
				wchar_t b = std::tolower(any_cast<wchar_t>(v));
				res = static_cast<double>(b);
				return (true);
			}
			else if (v.type() == typeid(short)) {
				short b = any_cast<short>(v);
				res = static_cast<double>(b);
				return (true);
			}
			else if (v.type() == typeid(unsigned short)) {
				unsigned short b = any_cast<unsigned short>(v);
				res = static_cast<double>(b);
				return (true);
			}
			else if (v.type() == typeid(int)) {
				int b = any_cast<int>(v);
				res = static_cast<double>(b);
				return (true);
			}
			else if (v.type() == typeid(unsigned int)) {
				unsigned int b = any_cast<unsigned int>(v);
				res = static_cast<double>(b);
				return (true);
			}
			else if (v.type() == typeid(long)) {
				long b = any_cast<long>(v);
				res = static_cast<double>(b);
				return (true);
			}
			else if (v.type() == typeid(unsigned long)) {
				unsigned long b = any_cast<unsigned long>(v);
				res = static_cast<double>(b);
				return (true);
			}
			else if (v.type() == typeid(float)) {
				float b = any_cast<float>(v);
				res = static_cast<double>(b);
				return (true);
			}
			else if (v.type() == typeid(double)) {
				res = any_cast<double>(v);
				return (true);
			}
			else if (v.type() == typeid(long double)) {
				long double b = any_cast<long double>(v);
				res = static_cast<double>(b);
				return (true);
			}
			else if (v.type() == typeid(std::string)) {
				std::string bb = any_cast<std::string>(v);
				if (!bb.empty()) {
					std::stringstream in(bb);
					in >> res;
					return (in.good());
				}
			}
			else if (v.type() == typeid(std::wstring)) {
				std::wstring bb = any_cast<std::wstring>(v);
				if (!bb.empty()) {
					std::wstringstream in(bb);
					in >> res;
					return (in.good());
				}
			}
		} // not empty
		return (false);
	}
	////////////////////////
	template<typename T>
	static string_t conv_to_string(const T &v) {
		stringstream_t os{ };
		os << v;
		return (os.str());
	} // conv_to_string
	///////////////////////////////////////
	extern bool info_any_to_string(const any &v, string_type &res) {
		if (!INFO_ANY_EMPTY(v)) {
			if (v.type() == typeid(string_type)) {
				res = any_cast<string_type>(v);
				return (true);
			}
			else if (v.type() == typeid(double)) {
				double b = any_cast<double>(v);
				res = conv_to_string(b);
				return (true);
			}
			else if (v.type() == typeid(int)) {
				int b = any_cast<int>(v);
				res = conv_to_string(b);
				return (true);
			}
			else if (v.type() == typeid(bool)) {
				bool b = any_cast<bool>(v);
				res = conv_to_string(b);
				return (true);
			}
			else if (v.type() == typeid(char)) {
				char b = any_cast<char>(v);
				res = conv_to_string(b);
				return (true);
			}
			else if (v.type() == typeid(unsigned char)) {
				unsigned char b = any_cast<unsigned char>(v);
				res = conv_to_string(b);
				return (true);
			}
			else if (v.type() == typeid(wchar_t)) {
				wchar_t b = any_cast<wchar_t>(v);
				res = conv_to_string(b);
				return (true);
			}
			else if (v.type() == typeid(short)) {
				short b = any_cast<short>(v);
				res = conv_to_string(b);
				return (true);
			}
			else if (v.type() == typeid(unsigned short)) {
				unsigned short b = any_cast<unsigned short>(v);
				res = conv_to_string(b);
				return (true);
			}
			else if (v.type() == typeid(unsigned int)) {
				unsigned int b = any_cast<unsigned int>(v);
				res = conv_to_string(b);
				return (true);
			}
			else if (v.type() == typeid(long)) {
				long b = any_cast<long>(v);
				res = conv_to_string(b);
				return (true);
			}
			else if (v.type() == typeid(unsigned long)) {
				unsigned long b = any_cast<unsigned long>(v);
				res = conv_to_string(b);
				return (true);
			}
			else if (v.type() == typeid(float)) {
				float b = any_cast<float>(v);
				res = conv_to_string(b);
				return (true);
			}
			else if (v.type() == typeid(long double)) {
				long double b = any_cast<long double>(v);
				res = conv_to_string(b);
				return (true);
			}
			else if (v.type() == typeid(long long)) {
				long long b = any_cast<long long>(v);
				res = conv_to_string(b);
				return (true);
			}
		} // not empty
		return (false);
	} //info_any_to_string
	//////////////////////////////
	extern void info_any_to_stringset(const any &src, std::set<string_type> &oSet) {
		oSet.clear();
		if (INFO_ANY_EMPTY(src)) {
			return;
		}
		if (src.type() == typeid(string_type)) {
			string_type s = any_cast<string_type>(src);
			std::vector<string_type> v{ };
			info_split_string(s, v);
			for (auto jt = v.begin(); jt != v.end(); ++jt) {
				string_type s = trim(*jt);
				if (!s.empty()) {
					oSet.insert(s);
				}
			}
		}
		else if (src.type() == typeid(std::vector<string_type>)) {
			std::vector<string_type> v = any_cast<std::vector<string_type>>(src);
			for (auto jt = v.begin(); jt != v.end(); ++jt) {
				string_type s = trim(*jt);
				if (!s.empty()) {
					oSet.insert(s);
				}
			}
		}
		else if (src.type() == typeid(std::set<string_type>)) {
			std::set<string_type> v = any_cast<std::set<string_type>>(src);
			for (auto jt = v.begin(); jt != v.end(); ++jt) {
				string_type s = trim(*jt);
				if (!s.empty()) {
					oSet.insert(s);
				}
			}
		}
	}	//info_any_to_stringset
	////////////////////////////
}	// namespace info
