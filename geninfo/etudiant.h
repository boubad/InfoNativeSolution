#pragma once
#ifndef PERSON_H
#define PERSON_H
/////////////////////////
#include "baseitem.h"
//////////////////////////////
namespace info {
namespace domain {
//////////////////////////////
class Etudiant: public BaseItem {
private:
	string_type m_firstname;
	string_type m_lastname;
	string_type m_sexe;
	string_type m_birth;
	string_type m_dossier;
	string_type m_apb;
	string_type m_sup;
	string_type m_red;
	string_type m_avatar;
	string_type m_seriebac;
	string_type m_optionbac;
	string_type m_mentionbac;
	string_type m_departement;
	string_type m_ville;
	string_type m_groupe;
	string_type m_email;
	string_type m_lycee;
	string_type m_annee;
	std::set<string_type> m_sets;
public:
	Etudiant();
	Etudiant(const anymap_type &oMap);
	Etudiant(const Etudiant &other);
	Etudiant & operator=(const Etudiant &other);
	virtual ~Etudiant();
public:
	const string_type &annee(void) const {
		return (this->m_annee);
	}
	void annee(const string_type &s) {
		this->m_annee = to_upper(trim(s));
	}
	bool has_avatar(void) const {
		return (!this->m_avatar.empty());
	}
	const string_type & optionbac(void) const {
		return (this->m_optionbac);
	}
	void optionbac(const string_type &s) {
		this->m_optionbac = to_upper(trim(s));
	}
	const string_type & seriebac(void) const {
		return (this->m_seriebac);
	}
	void seriebac(const string_type &s) {
		this->m_seriebac = to_upper(trim(s));
	}
	const std::set<string_type> & datasets(void) const {
		return (this->m_sets);
	}
	void add_dataset(const string_type &s) {
		string_type ss = to_upper(trim(s));
		if (!ss.empty()) {
			this->m_sets.insert(ss);
		}
	}
	const string_type & avatar(void) const {
		return (this->m_avatar);
	}
	void avatar(const string_type &s) {
		this->m_avatar = trim(s);
	}
	const string_type & redoublant(void) const {
		return (this->m_red);
	}
	void redoublant(const string_type &s) {
		this->m_red = to_upper(trim(s));
	}
	const string_type & etudessuperieures(void) const {
		return (this->m_sup);
	}
	void etudessuperieures(const string_type &s) {
		this->m_sup = to_upper(trim(s));
	}
	const string_type & apb(void) const {
		return (this->m_apb);
	}
	void apb(const string_type &s) {
		this->m_apb = to_upper(trim(s));
	}
	const string_type & dossier(void) const {
		return (this->m_dossier);
	}
	void dossier(const string_type &s) {
		this->m_dossier = to_upper(trim(s));
	}
	const string_type & birth(void) const {
		return (this->m_birth);
	}
	void birth(const string_type &s) {
		this->m_birth = to_upper(trim(s));
	}
	const string_type & sexe(void) const {
		return (this->m_sexe);
	}
	void sexe(const string_type &s) {
		this->m_sexe = to_upper(trim(s));
	}
	const string_type & lastname(void) const {
		return (this->m_lastname);
	}
	void lastname(const string_type &s) {
		this->m_lastname = to_upper(trim(s));
	}
	const string_type & firstname(void) const {
		return (this->m_firstname);
	}
	void firstname(const string_type &s) {
		string_type ss = trim(s);
		if (ss.length() > 1) {
			this->m_firstname = to_upper(
					string_type { ss.begin(), ss.begin() + 1 })
					+ to_lower(string_type { ss.begin() + 1, ss.end() });
		} else {
			this->m_firstname = to_upper(ss);
		}
	}
	const string_type & mentionbac(void) const {
		return (this->m_mentionbac);
	}
	void mentionbac(const string_type &s) {
		this->m_mentionbac = to_upper(trim(s));
	}
	const string_type & departement(void) const {
		return (this->m_departement);
	}
	void departement(const string_type &s) {
		this->m_departement = to_upper(trim(s));
	}
	const string_type & ville(void) const {
		return (this->m_ville);
	}
	void ville(const string_type &s) {
		this->m_ville = to_upper(trim(s));
	}
	const string_type & groupe(void) const {
		return (this->m_groupe);
	}
	void groupe(const string_type &s) {
		this->m_groupe = to_upper(trim(s));
	}
	const string_type & email(void) const {
		return (this->m_email);
	}
	void email(const string_type &s) {
		this->m_email = trim(s);
	}
	const string_type & lycee(void) const {
		return (this->m_lycee);
	}
	void lycee(const string_type &s) {
		this->m_lycee = to_upper(trim(s));
	}
public:
	virtual item_ptr clone(void) const;
	virtual string_type toString(void) const;
	virtual void clear(void);
	//
	virtual int compare_to(const InfoItem &other) const;
	virtual string_type type(void) const;
	virtual bool is_storeable(void) const;
	virtual void to_map(anymap_type &oMap) const;
};
// class Person
//////////////////////////////
}// namespace domain
}		// namespace info
///////////////////////////
#endif // PERSON_H
