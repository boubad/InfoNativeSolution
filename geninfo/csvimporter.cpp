//csvimporter.cpp
//////////////////////////
#include "stdafx.h"
#include "statpersistmanager.h"
#include "etudiant.h"
///////////////////////////////////
namespace info {
	namespace domain {
		/////////////////////////
		static const string_type KI_GROUPE{ U("GROUPE") };
		static const string_type KI_DOSSIER{ U("DOSSIER") };
		static const string_type KI_LASTNAME{ U("LASTNAME") };
		static const string_type KI_FIRSTNAME{ U("FIRSTNAME") };
		static const string_type KI_SEXE{ U("SEXE") };
		static const string_type KI_BIRTHYEAR{ U("BIRTHYEAR") };
		static const string_type KI_SERIEBAC{ U("SERIEBAC") };
		static const string_type KI_OPTIONBAC{ U("OPTIONBAC") };
		static const string_type KI_DEPARTEMENT{ U("DEPARTEMENT") };
		static const string_type KI_LYCEE{ U("LYCEE") };
		static const string_type KI_VILLE{ U("VILLE") };
		static const string_type KI_SUP{ U("ETUDESSUPERIEURES") };
		static const string_type KI_APB{ U("APB") };
		static const string_type KI_MENTIONBAC{ U("MENTIONBAC") };
		static const string_type KI_EMAIL{ U("EMAIL") };
		static const string_type KI_REDOUBLANT{ U("REDOUBLANT") };
		static const string_type KI_ANNEE{ U("ANNEE") };
		//
		/////////////////////////////////////
		static const int IMPORT_BUF_SIZE{ 2048 };
		static const string_type ST_DELIM{ U(";") };
		////////////////////////////////////
		using namespace info;
		/////////////////////////////////////
		static item_ptr read_line(istream_t &in,  const std::map<size_t, string_type> &header,
			bool &bEnd) {
			item_ptr oRet{};
			bEnd = false;
			if (in.eof()) {
				bEnd = true;
				return (oRet);
			}// eof
			string_type sin{};
			std::getline(in, sin);
			std::vector<string_t> data{};
			info_split_string(sin, data,ST_DELIM);
			const size_t n = data.size();
			Etudiant oEtud{};
			for (size_t i = 0; i < n; ++i) {
				string_type sval = trim(data[i]);
				if (sval.empty()) {
					continue;
				}
				auto it = header.find(i);
				if (it == header.end()) {
					continue;
				}
				string_type var = (*it).second;
				if (var == KI_ANNEE) {
					oEtud.annee(sval);
				}
				if (var == KI_GROUPE) {
					oEtud.groupe(sval);
				}
				else if (var == KI_DOSSIER) {
					oEtud.dossier(sval);
				}
				else if (var == KI_LASTNAME) {
					oEtud.lastname(sval);
				}
				else if (var == KI_FIRSTNAME) {
					oEtud.firstname(sval);
				}
				else if (var == KI_SEXE) {
					oEtud.sexe(sval);
				}
				else if (var == KI_BIRTHYEAR) {
					oEtud.birth(sval);
				}
				else if (var == KI_SERIEBAC) {
					oEtud.seriebac(sval);
				}
				else if (var == KI_OPTIONBAC) {
					oEtud.optionbac(sval);
				}
				else if (var == KI_DEPARTEMENT) {
					oEtud.departement(sval);
				}
				else if (var == KI_LYCEE) {
					oEtud.lycee(sval);
				}
				else if (var == KI_VILLE) {
					oEtud.ville(sval);
				}
				else if (var == KI_SUP) {
					oEtud.etudessuperieures(sval);
				}
				else if (var == KI_APB) {
					oEtud.apb(sval);
				}
				else if (var == KI_MENTIONBAC) {
					oEtud.mentionbac(sval);
				}
				else if (var == KI_REDOUBLANT) {
					oEtud.redoublant(sval);
				}
				else if (var == KI_EMAIL) {
					oEtud.email(sval);
				}
			}// i
			if (!oEtud.is_storeable()) {
				return (oRet);
			}
			oRet.reset(new Etudiant{oEtud});
			assert(oRet.get() != nullptr);
			return (oRet);
		}// read_line
		static itemptrvector_ptr import_etuds_stream(istream_t &in) {
			std::map<size_t, string_type> headers{};
			string_type sin{};
			std::getline(in, sin);
			std::vector<string_t> data{};
			info_split_string(sin, data,ST_DELIM);
			const size_t n = data.size();
			for (size_t i = 0; i < n; ++i) {
				string_type s = to_upper(trim(data[i]));
				if (!s.empty()) {
					headers[i] = s;
				}
			}// s
			bool bEnd = false;
			itemptrvector_ptr oRet{ new itemptr_vector{} };
			itemptr_vector *vv = oRet.get();
			assert(vv != nullptr);
			do {
				item_ptr p = read_line(in, headers, bEnd);
				if (p.get() != nullptr) {
					vv->push_back(p);
				}
			} while (!bEnd);
			if (vv->empty()) {
				oRet.reset();
			}
			return (oRet);
		}//import_etuds_stream
		///////////////////////////////
		extern task_bool info_import_etudiants(istream_t &in, DomainPersistManager *pMan) {
			assert(pMan != nullptr);
			istream_t *pin = &in;
			return task_bool{ [pin,pMan]()->bool {
				itemptrvector_ptr pp = import_etuds_stream(*pin);
				bool bRet{ false };
				if (pp.get() != nullptr) {
					bRet = pMan->maintains_elements_async(pp).get();
				}// pp
				return (bRet);
			} };
		}//info_import_etudiants
		extern task_bool info_import_etudiants_file(const string_type &filename, DomainPersistManager *pMan) {
			assert(pMan != nullptr);
			const string_type *pname = &filename;
			return task_bool{ [pname,pMan]()->bool {
				bool bRet{ false };
				ifstream_t in{ pname->c_str() };
				if (in.is_open()) {
					itemptrvector_ptr pp = import_etuds_stream(in);
					if (pp.get() != nullptr) {
						bRet = pMan->maintains_elements_async(pp).get();
					}// pp
				}
				return (bRet);
			} };
		}//info_import_etudiants
/////////////////////////////////////////
	}// namespace domain
}// namespace info
