#pragma once
#ifndef INFO_REST_H
#define INFO_REST_H
/////////////////////////////////////
#include "infocommon.h"
////////////////////////////////////////
namespace info {
	namespace persist {
		/////////////////////////////////
		using any = info::any;
		using string_type = info::string_type;
		using anymap_type = info::anymap_type;
		//////////////////////////////////
		using value = web::json::value;
		using object = web::json::object;
		using array = web::json::array;
		//////////////////////
		extern string_t url_encode(const string_t &url);
		/////////////////////////////////////
		extern value any_to_value(const any &va);
		extern any value_to_any(const value &v);
		extern value anymap_to_value(const anymap_type &oMap);
		extern  anymap_type value_to_anymap(const value &v);
		/////////////////////////////////
	}// namespace persist
}// namespace info
///////////////////////////////////
#endif // INFO_REST_U
