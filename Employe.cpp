#include "Employe.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <wx/wx.h>

//initalization de membres d'employe que les classes Manager et Programmeur vont heriter
Employe::Employe(const std::string& nom, const std::string& prenom, int age, int salaire)
		:nom(nom), prenom(prenom), age(age), salaire(salaire) {
}

Employe::~Employe() {}

std::string Employe::getNom(){
	return nom;

} 
std::string Employe::getPrenom(){
	return prenom;
}

int Employe::getAge(){
	return age;
}

int Employe::getSalaire(){
	return salaire;
}

//definition sans implementation des methodes qui sont virtuels pures
std::string Employe::afficher(std::vector<std::string> resultat) {
                        std::string s;
						return s;
                        };
                        
std::string Employe::toLowerCase(const std::string& entree){
                        std::string s;
						return s;
                        }
bool Employe::comp(std::unique_ptr<Employe>& a, std::unique_ptr<Employe>& b){
                        return a->getNom() >= b->getNom();
                        };

void Employe::lireEmploye(
					wxTextCtrl* fichier, std::vector<std::unique_ptr<Employe>>& emp
					){}
std::vector<std::string> Employe::trouverProjet(
					std::vector<std::unique_ptr<Employe>>& vecEmp, const std::string& argument
					){
						std::vector<std::string> empProjets;
						return empProjets;
					}
std::vector<std::string> Employe::compterProjet(		
					std::vector<std::unique_ptr<Employe>>& vecEmp, const std::string& nomProjet
					){
						std::vector<std::string> empCompter;
						return empCompter;
					}
void Employe::retirerProjet(
					std::vector<std::unique_ptr<Employe>>& vecEmp, const std::string& aRetirer, const std::string& projet
					){}
void Employe::ajouterEmpProjet(
					std::vector<std::unique_ptr<Employe>>& vecEmp, const std::string& aAjouter, const std::string& projet
					){}
wxString Employe::avecModification(std::vector<std::unique_ptr<Employe>>& vecEmp){
                        wxString emp;
						return emp;
                    }


