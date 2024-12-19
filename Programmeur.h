#include "Employe.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <wx/wx.h>

#ifndef PROGRAMMEUR_H
#define PROGRAMMEUR_H


class Programmeur : public Employe
{
private:		
		std::string langProg;
		std::string niveau;		
		std::vector<std::string> projet;
	
	public:
		//constructeurs	
		Programmeur(){};

		Programmeur(std::string n, std::string p, int a, int s, std::string lp, std::string niv, std::vector<std::string> pr);
		//destructeur
        ~Programmeur();
        
		//methodes d'access et modificación de membres
		std::string getLang();		
		
		std::string getNiveau();
		
		std::vector<std::string> getProjet();

		void setProjet(const std::vector<std::string>& nP);		

        static bool comp(std::unique_ptr<Employe>& a, std::unique_ptr<Employe>& b);
        
        std::string toLowerCase(const std::string& entree) override;     

		//Declaration des methodes herités et d'autres propres a la classe Manager, ceux qui sont herités d'employe ont la commande "override"
		//parce qu'il faut qu'on le redefine dans classe derivé 

		std::string afficher(std::vector<std::string> resultat) override;

		void lireEmploye(
					wxTextCtrl* fichier, std::vector<std::unique_ptr<Employe>>& emp
					) override;		
		std::vector<std::string> trouverProjet(
					std::vector<std::unique_ptr<Employe>>& vecProg, const std::string& argument
					) override;
		std::vector<std::string> compterProjet(
					std::vector<std::unique_ptr<Employe>>& vecProg, const std::string& nomProjet
					) override;
		std::vector<std::string> trouverProgrammeurs( 
					std::vector<std::unique_ptr<Employe>>& vecProg, const std::string& nomManager
					);		
		void ajouterEmpProjet(
					std::vector<std::unique_ptr<Employe>>& vecProg, const std::string& programmeur, const std::string& projetAjouter
					) override;
		void retirerProjet(
					std::vector<std::unique_ptr<Employe>>& vecProg, const std::string& aRetirer, const std::string& projet
					) override;
        wxString avecModification(std::vector<std::unique_ptr<Employe>>& vecProg) override;             
                

};

#endif // PROGRAMMEUR_H
