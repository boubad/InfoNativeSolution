#pragma once
#ifndef DOCBASE_H
#define DOCBASE_H
/////////////////////////
#include "info_rest.h"
////////////////////////////
namespace info
{
	namespace persist {
		////////////////////////////////////////////
		class DocBase {
		public:
			DocBase() {}
			DocBase(const DocBase & /*other*/) {}
			DocBase & operator=(const DocBase & /* other other*/) { return (*this); }
			virtual ~DocBase() {}
		public:
			virtual string_t toString(void) const { return (string_t{}); }
			virtual value to_value(void) const { return (value{}); }
		};// class DocBase
		/////////////////////////////////////////////
	}// namespace persist
}// namespace info
///////////////////////
#endif // DOCBASE_H
