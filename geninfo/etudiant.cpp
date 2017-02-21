//person.cpp
////////////////////////
#include "stdafx.h"
#include "etudiant.h"
#include "infostrings.h"
///////////////////////////////
namespace info {
	namespace domain {
		//////////////////////////////////
		Etudiant::Etudiant() {
		}
		Etudiant::Etudiant(const anymap_type &oMap) :
			BaseItem(oMap) {
				{
					auto it = oMap.find(KEY_ANNEE);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{};
						if (info_any_to_string(src, s)) {
							this->annee(s);
						}
					}
				} // annee
				{
					auto it = oMap.find(KEY_FIRSTNAME);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{ };
						if (info_any_to_string(src, s)) {
							this->firstname(s);
						}
					}
				} // firstname
				{
					auto it = oMap.find(KEY_LASTNAME);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{ };
						if (info_any_to_string(src, s)) {
							this->lastname(s);
						}
					}
				} // lastname
				{
					auto it = oMap.find(KEY_BIRTHYEAR);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{ };
						if (info_any_to_string(src, s)) {
							this->birth(s);
						}
					}
				} // birth
				{
					auto it = oMap.find(KEY_SEXE);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{ };
						if (info_any_to_string(src, s)) {
							this->sexe(s);
						}
					}
				} // firstname
				{
					auto it = oMap.find(KEY_DOSSIER);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{ };
						if (info_any_to_string(src, s)) {
							this->dossier(s);
						}
					}
				} // dossier
				{
					auto it = oMap.find(KEY_APB);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{ };
						if (info_any_to_string(src, s)) {
							this->apb(s);
						}
					}
				} // apb
				{
					auto it = oMap.find(KEY_SUP);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{ };
						if (info_any_to_string(src, s)) {
							this->etudessuperieures(s);
						}
					}
				} // sup
				{
					auto it = oMap.find(KEY_SERIEBAC);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{ };
						if (info_any_to_string(src, s)) {
							this->seriebac(s);
						}
					}
				} // seriebac
				{
					auto it = oMap.find(KEY_MENTIONBAC);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{};
						if (info_any_to_string(src, s)) {
							this->mentionbac(s);
						}
					}
				} // seriebac
				{
					auto it = oMap.find(KEY_REDOUBLANT);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{ };
						if (info_any_to_string(src, s)) {
							this->redoublant(s);
						}
					}
				} // seriebac
				{
					auto it = oMap.find(KEY_OPTIONBAC);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{ };
						if (info_any_to_string(src, s)) {
							this->optionbac(s);
						}
					}
				} // optionbac
				{
					auto it = oMap.find(KEY_AVATAR);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{ };
						if (info_any_to_string(src, s)) {
							this->avatar(s);
						}
					}
				} // avatar
				{
					auto it = oMap.find(KEY_DATASETS);
					if (it != oMap.end()) {
						any src{ (*it).second };
						info_any_to_stringset(src, this->m_sets);
					}
				} // datasets
				{
					auto it = oMap.find(KEY_DEPARTEMENT);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{};
						if (info_any_to_string(src, s)) {
							this->departement(s);
						}
					}
				} // departement
				{
					auto it = oMap.find(KEY_VILLE);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{};
						if (info_any_to_string(src, s)) {
							this->ville(s);
						}
					}
				} // ville
				{
					auto it = oMap.find(KEY_GROUPE);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{};
						if (info_any_to_string(src, s)) {
							this->groupe(s);
						}
					}
				} // groupe
				{
					auto it = oMap.find(KEY_EMAIL);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{};
						if (info_any_to_string(src, s)) {
							this->email(s);
						}
					}
				} // email
				{
					auto it = oMap.find(KEY_LYCEE);
					if (it != oMap.end()) {
						any src{ (*it).second };
						string_type s{};
						if (info_any_to_string(src, s)) {
							this->lycee(s);
						}
					}
				} // lycee
		}
		Etudiant::Etudiant(const Etudiant &other) :
			BaseItem(other), m_firstname(other.m_firstname), m_lastname(
				other.m_lastname), m_sexe(other.m_sexe), m_birth(other.m_birth), m_dossier(
					other.m_dossier), m_apb(other.m_apb), m_sup(other.m_sup), m_red(
						other.m_red), m_avatar(other.m_avatar), m_seriebac(
							other.m_seriebac), m_optionbac(other.m_optionbac),
			m_mentionbac(other.m_mentionbac), m_departement(other.m_departement), m_ville(other.m_ville),
			m_groupe(other.m_groupe), m_email(other.m_email), m_lycee(other.m_lycee),m_annee(other.m_annee),
			m_sets(other.m_sets) {

		}
		Etudiant & Etudiant::operator=(const Etudiant &other) {
			if (this != &other) {
				BaseItem::operator=(other);
				this->m_firstname = other.m_firstname;
				this->m_lastname = other.m_lastname;
				this->m_sexe = other.m_sexe;
				this->m_birth = other.m_birth;
				this->m_dossier = other.m_dossier;
				this->m_apb = other.m_apb;
				this->m_sup = other.m_sup;
				this->m_red = other.m_red;
				this->m_avatar = other.m_avatar;
				this->m_seriebac = other.m_seriebac;
				this->m_optionbac = other.m_optionbac;
				this->m_mentionbac = other.m_mentionbac;
				this->m_departement = other.m_departement;
				this->m_ville = other.m_ville;
				this->m_groupe = other.m_groupe;
				this->m_email = other.m_email;
				this->m_lycee = other.m_lycee;
				this->m_annee = other.m_annee;
				this->m_sets = other.m_sets;
			}
			return (*this);
		}
		Etudiant::~Etudiant() {

		}
		item_ptr Etudiant::clone(void) const {
			item_ptr oRet{ new Etudiant { *this } };
			return oRet;
		}
		string_type Etudiant::toString(void) const {
			string_type sRet{ };
#if defined(_MSC_VER)
			sRet = trim(this->m_lastname + string_type(L" ") + this->m_firstname);
#else
			sRet = trim(this->m_lastname + string_type(" ") + this->m_firstname);
#endif // _MSC_VER
			return (sRet);
		}
		void Etudiant::clear(void) {
			BaseItem::clear();
			this->m_firstname.clear();
			this->m_lastname.clear();
			this->m_sexe.clear();
			this->m_birth.clear();
			this->m_dossier.clear();
			this->m_apb.clear();
			this->m_sup.clear();
			this->m_red.clear();
			this->m_avatar.clear();
			this->m_seriebac.clear();
			this->m_optionbac.clear();
			this->m_mentionbac.clear();
			this->m_departement.clear();
			this->m_ville.clear();
			this->m_groupe.clear();
			this->m_email.clear();
			this->m_annee.clear();
			this->m_lycee.clear();
			this->m_sets.clear();
		}

		int Etudiant::compare_to(const InfoItem &other) const {
			const Etudiant *p = dynamic_cast<const Etudiant *>(&other);
			if (p == nullptr) {
				return (-1);
			}
			if (this->m_lastname < p->m_lastname) {
				return (-1);
			}
			else if (this->m_lastname > p->m_lastname) {
				return (1);
			}
			if (this->m_firstname < p->m_firstname) {
				return (-1);
			}
			else if (this->m_firstname > p->m_firstname) {
				return (1);
			}
			if (this->m_birth < p->m_birth) {
				return (-1);
			}
			else if (this->m_birth > p->m_birth) {
				return (1);
			}
			else {
				return (0);
			}
		}
		string_type Etudiant::type(void) const {
			return (TYPE_ETUDIANT);
		}
		bool Etudiant::is_storeable(void) const {
			return ((!this->m_lastname.empty()) && (!this->m_firstname.empty()));
		}
		void Etudiant::to_map(anymap_type &oMap) const {
			BaseItem::to_map(oMap);
			{
				string_type s = this->annee();
				if (!s.empty()) {
					oMap[KEY_ANNEE] = any{ s };
				}
			}
			{
				string_type s = this->firstname();
				if (!s.empty()) {
					oMap[KEY_FIRSTNAME] = any{ s };
				}
			}
			{
				string_type s = this->lastname();
				if (!s.empty()) {
					oMap[KEY_LASTNAME] = any{ s };
				}
			}
			{
				string_type s = this->sexe();
				if (!s.empty()) {
					oMap[KEY_SEXE] = any{ s };
				}
			}
			{
				string_type s = this->birth();
				if (!s.empty()) {
					oMap[KEY_BIRTHYEAR] = any{ s };
				}
			}
			{
				string_type s = this->dossier();
				if (!s.empty()) {
					oMap[KEY_DOSSIER] = any{ s };
				}
			}
			{
				string_type s = this->apb();
				if (!s.empty()) {
					oMap[KEY_APB] = any{ s };
				}
			}
			{
				string_type s = this->etudessuperieures();
				if (!s.empty()) {
					oMap[KEY_SUP] = any{ s };
				}
			}
			{
				string_type s = this->redoublant();
				if (!s.empty()) {
					oMap[KEY_REDOUBLANT] = any{ s };
				}
			}
			{
				string_type s = this->seriebac();
				if (!s.empty()) {
					oMap[KEY_SERIEBAC] = any{ s };
				}
			}
			{
				string_type s = this->optionbac();
				if (!s.empty()) {
					oMap[KEY_OPTIONBAC] = any{ s };
				}
			}
			{
				if (!this->m_sets.empty()) {
					std::vector<string_type> vv{ };
					for (auto it = this->m_sets.begin(); it != this->m_sets.end();
						++it) {
						vv.push_back(*it);
					}
					oMap[KEY_DATASETS] = any{ vv };
				} // empty

			}
			{
				string_type s = this->mentionbac();
				if (!s.empty()) {
					oMap[KEY_MENTIONBAC] = any{ s };
				}
			}
			{
				string_type s = this->departement();;
				if (!s.empty()) {
					oMap[KEY_DEPARTEMENT] = any{ s };
				}
			}
			{
				string_type s = this->ville();
				if (!s.empty()) {
					oMap[KEY_VILLE] = any{ s };
				}
			}
			{
				string_type s = this->groupe();
				if (!s.empty()) {
					oMap[KEY_GROUPE] = any{ s };
				}
			}
			{
				string_type s = this->email();
				if (!s.empty()) {
					oMap[KEY_EMAIL] = any{ s };
				}
			}
			{
				string_type s = this->lycee();
				if (!s.empty()) {
					oMap[KEY_LYCEE] = any{ s };
				}
			}
		} // to_map
		/////////////////////////////////////
	}		// namespace data
}		// namespace info
