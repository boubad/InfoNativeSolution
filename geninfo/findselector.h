#pragma once
#ifndef FINDSELECTOR_H
#define FINDSELECTOR_H
//////////////////////
#include  "docbase.h"
///////////////////////
namespace info {
	namespace persist {
		/////////////////////
		class FindOptions : public DocBase
		{
		private:
			int     skip;
			int     limit;
			value   selector;
			value   sort;
			value   fields;
			value   use_index;
		public:
			FindOptions();
			FindOptions(const FindOptions &other);
			FindOptions & operator=(const FindOptions &other);
			~FindOptions();
			operator value(void) const { return (this->to_value()); }
		public:
			void set_skip(int n);
			void set_limit(int n);
			void add_fetch_item(const string_t &field, const value &v);
			void add_return_field(const string_t &field);
			void add_sort_field(const string_t &field, bool bAsc = true);
			void set_use_index(const value &v);
		public:
			virtual value to_value(void) const;
		};// class FindSelector
		  ////////////////////////////////////////
		class CreateIndexData : public DocBase {
		private:
			string_t name;
			value fields;
		public:
			CreateIndexData();
			CreateIndexData(const CreateIndexData &other);
			CreateIndexData & operator=(const CreateIndexData &other);
			~CreateIndexData();
			operator value(void) const { return (this->to_value()); }
		public:
			void set_name(const string_t &s);
			void add_index_field(const string_t &field, bool bAsc = true);
		public:
			virtual value to_value(void) const;
		};// class CreateIndexData

		  ///////////////////////////////////////////
	}// namespace persist
}// namespace info
#endif // FINDSELECTOR_H
