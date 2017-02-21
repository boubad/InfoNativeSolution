#pragma once
#ifndef INFO_DOCUTILS_H
#define INFO_DOCUTILS_H
//////////////////////////////
#include "info_rest.h"
////////////////////////////
namespace info
{
	namespace persist {
		//////////////////////
		class AttachmentDesc
		{
		public:
			AttachmentDesc();
			AttachmentDesc(const value &oMap);
			AttachmentDesc(const AttachmentDesc &other);
			AttachmentDesc & operator=(const AttachmentDesc &other);
			~AttachmentDesc();
		public:
			bool stub;
			size_t revpos;
			size_t length;
			size_t encoded_length;
			string_t encoding;
			string_t content_type;
			string_t data;
			string_t digest;
			string_t name;
			string_t url;
		};
		//////////////////////////////////
		class RevisionsData {
		public:
			RevisionsData();
			RevisionsData(const value &oMap);
			RevisionsData(const RevisionsData &other);
			RevisionsData & operator=(const RevisionsData &other);
			~RevisionsData();
		public:
			size_t  start(void) const;
			const std::vector<string_t> & ids(void) const;
		private:
			size_t _start;
			std::vector<string_t> _ids;
		};// class RevisionsData
		class RevisionInfo {
		public:
			RevisionInfo();
			RevisionInfo(const value &oMap);
			RevisionInfo(const RevisionInfo &other);
			RevisionInfo & operator=(const RevisionInfo &other);
			~RevisionInfo();
		public:
			const string_t & rev(void) const;
			const string_t & status(void) const;
			bool is_available(void) const;
			bool is_deleted(void) const;
		private:
			string_t _rev;
			string_t _status;
		};// class RevisionInfo
		  ////////////////////
	}// namespace persist
}// namespace info
 /////////////////////////////////
#endif // DOCUTILS_H
