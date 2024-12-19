#include "Employe.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <wx/wx.h>

#ifndef C_MAN
#define C_MAN

/*Declaration de la classe Manager qui herite de la clase Employe avec des propietes especifiques aux managers, et 
 des constructeurs et methodes appropies aux besoins des differents taches. 
*/

class Manager:public Employe{

	private:
		std::vector<std::string> equipe;
		int budget;		
		std::vector<std::string> projetSup;
	
	public: 
		//constructeurs
		Manager(){};		

		Manager(std::string nom, std::string prenom, int age, int salaire, std::vector<std::string> equipe, int budget, std::vector<std::string> projetSup);
		
		//methodes d'access et modificación de membres
		std::vector<std::string>& getEquipe();
		
		int getBudget();
		
		std::vector<std::string> getProjetSup();

		std::vector<std::string> setProjetSup(std::vector<std::string> nP);

		void setEquipe(std::vector<std::string> nE);

		Manager& operator+=(const std::string& membreAjouter);	

		//Declaration des methodes herités et d'autres propres a la classe Manager, ceux qui sont herités d'employe ont la commande "override"
		//parce qu'il faut qu'on le redefine dans classe derivé 
        
        std::string toLowerCase(const std::string& entree) override;
        
        static bool comp(std::unique_ptr<Employe>& a, std::unique_ptr<Employe>& b);

		std::string afficher (std::vector<std::string> resultat) override;

		void lireEmploye(
					wxTextCtrl* fichier, std::vector<std::unique_ptr<Employe>>& emp
					) override;
		std::vector<std::string> trouverManagerProgrammeur(
					std::vector<std::unique_ptr<Employe>>& vecMan, const std::string& nomProgrammeur
					);
		std::vector<std::string> trouverProjet(
					std::vector<std::unique_ptr<Employe>>& vecMan, const std::string& argument
					) override;
		std::vector<std::string> compterProjet(		
					std::vector<std::unique_ptr<Employe>>& vecMan, const std::string& nomProjet
					) override;		
		void retirerProjet(
					std::vector<std::unique_ptr<Employe>>& vecMan, const std::string& manCible, const std::string& projetRetirer
					) override;
		void ajouterEmpProjet(
					std::vector<std::unique_ptr<Employe>>& vecMan, const std::string& manCible, const std::string& projetAjouter
					) override;
		void retirerProgrammeur(
					std::vector<std::unique_ptr<Employe>>& vecMan, const std::string& manager, const std::string& programmeur
					);
		void ajouterProgMan(
					std::vector<std::unique_ptr<Employe>>& vecMan, const std::string& manager, const std::string& programmeur
					);
        wxString avecModification(std::vector<std::unique_ptr<Employe>>& vecMan) override;

};

#endif
