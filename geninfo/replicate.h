#pragma once
#ifndef REPLICATE_H
#define REPLICATE_H
/////////////////////////////////
#include "docbase.h"
////////////////////////////
namespace info
{
	namespace persist {
		////////////////////////////////////
		class ReplicateOptions : public DocBase {
		public:
			ReplicateOptions();
			ReplicateOptions(const string_t &sSource, const string_t & sDest);
			ReplicateOptions(const ReplicateOptions &other);
			ReplicateOptions & operator=(const ReplicateOptions &other);
			~ReplicateOptions();
		public:
			bool is_valid(void) const;
			void add_docid(const string_t &id);
			virtual value to_value(void) const;
		public:
			bool cancel;
			bool continuous;
			bool create_target;
			string_t proxy;
			string_t source;
			string_t target;
			std::set<string_t> doc_ids;
		};// class ReplicateOptions
		  ///////////////////////////////////////
		class ReplicationItem {
		public:
			ReplicationItem();
			ReplicationItem(const value &v);
			ReplicationItem(const ReplicationItem &other);
			ReplicationItem & operator=(const ReplicationItem &other);
			~ReplicationItem();
		public:
			size_t doc_write_failures(void) const;
			size_t docs_read(void) const;
			size_t docs_written(void) const;
			size_t end_last_seq(void) const;
			string_t end_time(void) const;
			size_t missing_checked(void) const;
			size_t missing_found(void) const;
			size_t recorded_seq(void) const;
			string_t session_id(void) const;
			size_t start_last_seq(void) const;
			string_t start_time(void) const;
		private:
			value m_val;
		};// class ReplicationItem
		  ///////////////////////////////////////
		class ReplicateResponse {
		public:
			ReplicateResponse();
			ReplicateResponse(const value &v);
			ReplicateResponse(const ReplicateResponse &other);
			ReplicateResponse & operator=(const ReplicateResponse &other);
			~ReplicateResponse();
		public:
			std::vector<ReplicationItem> history(void) const;
			bool ok(void) const;
			size_t replication_id_version(void) const;
			string_t session_id(void) const;
			size_t source_last_seq(void) const;
		private:
			value m_val;
		}; // class ReplicateRespons
		   /////////////////////////////////////
	}// namespace persist
}// namespace geninfo
 ////////////////////////////////////
#endif // __REPLICATE_H__
