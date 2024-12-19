#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <wx/wx.h>

#ifndef C_EMP 
#define C_EMP

//Classe abstraite Employe dont on s'en sert uniquement pour heriter ses attributs et methodes. 

class Employe{
	protected: 
		std::string nom;
		std::string prenom;
		int age;
		int salaire;
		
	public:
		
	 Employe() = default;

	 virtual ~Employe() = 0; 
	
	 Employe(const std::string& nom, const std::string& prenom, int age, int salaire);
		
	 std::string getNom();
	  
	 std::string getPrenom();
	 
	 int getAge();
	 
	 int getSalaire();
     

	 // Declaration de methodes virtuels qui seront redefinies dans les classes derives Manager et Programmeur
     
     virtual std::string toLowerCase(const std::string& entree) = 0;
     
     static bool comp (std::unique_ptr<Employe>& a, std::unique_ptr<Employe>& b);

	 virtual void lireEmploye(
					wxTextCtrl* fichier, std::vector<std::unique_ptr<Employe>>& emp
					) = 0;
     virtual std::string afficher(
					std::vector<std::string> resultat
					) = 0; 
	 virtual std::vector<std::string> trouverProjet(
					std::vector<std::unique_ptr<Employe>>& vecEmp, const std::string& argument
					) = 0;
	 virtual std::vector<std::string> compterProjet(		
					std::vector<std::unique_ptr<Employe>>& vecEmp, const std::string& nomProjet
					) = 0;	
	 virtual void retirerProjet(
					std::vector<std::unique_ptr<Employe>>& vecEmp, const std::string& aRetirer, const std::string& projet
					) = 0;
	 virtual void ajouterEmpProjet(
					std::vector<std::unique_ptr<Employe>>& vecEmp, const std::string& aAjouter, const std::string& projet
					) = 0;
     virtual wxString avecModification(std::vector<std::unique_ptr<Employe>>& vecEmp) = 0;
};

#endif
