#include "Manager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <format>
#include <memory>
#include <wx/wx.h>

//definition de constructeurs, methodes d'acces, de modification et autres methodes de la classe

Manager::Manager(std::string n, std::string p, int a, int s, std::vector<std::string> eq, int bg, std::vector<std::string> pS)
				:Employe(n, p, a, s), equipe(eq), budget(bg), projetSup(pS){
				}
		
std::vector<std::string>& Manager::getEquipe(){
	return equipe;
} 

int  Manager::getBudget(){
	return budget;
}

std::vector<std::string> Manager::getProjetSup(){
	return projetSup;
} 

std::vector<std::string> Manager::setProjetSup( std::vector<std::string> nouveauxProjets){
    return projetSup = nouveauxProjets;
}

void Manager::setEquipe(std::vector<std::string> nouveauEquipe){
    this->equipe = nouveauEquipe;
}

//fonction helper pour trier le vector selon le nom de l'employe
bool Manager::comp(std::unique_ptr<Employe>& a, std::unique_ptr<Employe>& b) {
    return a->getNom() < b->getNom();
} 

//fonction helper pour convertir les entrées de l'utilisateur et les attributs qu'on compare en minuscule
//pour eviter des erreurs d'entrée
std::string Manager::toLowerCase(const std::string& entree) {
    std::string lower = entree;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

//methode qui affichera les resultats dans le format voulu
std::string Manager::afficher (std::vector<std::string> resultat) {
    std::string s = "";
    
    for (const auto& item : resultat){        
            if(!s.empty()){            
                s += ", ";
            }
            s += item;         
    }   
    return s;
}

//Lit le fichier chiosi par l'utilisateur et recupere les donnée en forme de vector 
void Manager::lireEmploye(wxTextCtrl* fichier, std::vector<std::unique_ptr<Employe>>& vecEmp) {	
    std::string ligne, li;
   
    //ignore l'entete et la dernier ligne pour eviter erreur innecesaires et recuperer les données pertinantes
	for(int i = 1; i < fichier->GetNumberOfLines() -1; ++i) {       
        try {
            ligne = fichier->GetLineText(i).ToStdString();
           
            std::stringstream ss(ligne);
            std::string nom, prenom, equipeStr, projetSupStr, ageStr, salaireStr, budgetStr;
            int age, salaire, budget;
            
            if (!std::getline(ss, nom, ';') || !std::getline(ss, prenom, ';') ||
                !std::getline(ss, ageStr, ';') || !std::getline(ss, salaireStr, ';') ||
                !std::getline(ss, equipeStr, ';') || !std::getline(ss, budgetStr, ';') ||
                !std::getline(ss, projetSupStr, ';')) {
                wxLogMessage(L"Ligne %d ignoré: pas assez d'information", i);
                continue;
            }            
        
            try {
                age = std::stoi(ageStr);
                salaire = std::stoi(salaireStr);
                budget = std::stoi(budgetStr);
            } catch (const std::invalid_argument&) {
                wxLogMessage(L"Ligne %d ignoré: format numerique non valide", i);
                continue; 
            } catch (const std::out_of_range&) {
                wxLogMessage(L"Ligne %d ignoré: valeur numerique hors de portée", i);
                continue;
            }
            
           // Transformation des chaînes "equipeStr" et "projetSupStr" en vecteurs
            std::vector<std::string> equipe(5), projetSup(5);            
           
            // Fonction de découpage d'une chaîne en vecteur
            auto splitString = [](const std::string& str, char delimiter) {
                std::vector<std::string> result;
                std::stringstream sstream(str);
                std::string item;
                while (std::getline(sstream, item, delimiter)) {
                    if (!item.empty()) {
                        result.push_back(item);
                    }
                }
                return result;
            };
            
            // Découper par virgule
            equipe = splitString(equipeStr, ',');  
            projetSup = splitString(projetSupStr, ',');                

	//Push d'un pointeur intelligent vers un objet de type Manager dans le vecteur 
         vecEmp.push_back(std::make_unique<Manager>(nom, prenom, age, salaire, equipe, budget, projetSup));
         
        } catch (const std::exception& e){
            wxLogMessage(L"Érreur traitant la ligne %d: %s", i, e.what());
            continue;
        }         
    }
    //tri du vector
     if (!vecEmp.empty()) {
        std::sort(vecEmp.begin(), vecEmp.end(), Manager::comp);
    }
}

//Cherche et retourner le nom du manager auquel un programmeur spécifique est associé
std::vector<std::string> Manager::trouverManagerProgrammeur(
                std::vector<std::unique_ptr<Employe>>& vecMan, const std::string& nomProgrammeur) {
    
    std::vector<std::string> manager;
    
    for (const auto& emp : vecMan) {//parcours le conteneur de pointeurs vers manrammeurs et managers
        if(auto man = dynamic_cast<Manager*>(emp.get())){//filtre ceux qui pointent vers le type Manager
            const auto& equipe = man->getEquipe();//recupere les projets            
            for(const auto& membre : equipe){//parcours le projets
                if(toLowerCase(membre) == toLowerCase(nomProgrammeur)){//cherche des concordance entre le projet passé en paremetre et les projets existants
                    manager.push_back(man->getPrenom() + " " + man->getNom());//push le nom complet du manager qui a le projet
                }
            }
        }               
    }   
    return manager; //Petourne le vector avec les managers associés au projet
}

//Cherche et retourne les managers qui supervisent un projet donné
std::vector<std::string> Manager::compterProjet(
                        std::vector<std::unique_ptr<Employe>>& vecMan, const std::string& nomProjet) {
    
    std::vector<std::string> empProjet;    
    
    for(const auto& emp : vecMan){
        if(auto man = dynamic_cast<Manager*>(emp.get())){//filtre vers managers
            const auto& projets = man->getProjetSup();//recupere les projets
            for(const auto& projet : projets){//parcours les projets
                if(toLowerCase(projet) == toLowerCase(nomProjet)){//si le projet est trouvé dans les projets supervisés par le manager
                    empProjet.push_back(man->getPrenom() +" "+ man->getNom());//Ajoute le nom complet du manager qui supervise le projet
                }
            }           
        }        
    }
    return empProjet; // Retourne le vector avec la liste de managers associées au projet 
}

//On cherche et retourne les projets supervisés par un manager specifique
std::vector<std::string> Manager::trouverProjet(
                        std::vector<std::unique_ptr<Employe>>& vecMan, const std::string& nomPersonne) { 
    
    std::vector<std::string> projets;
    
    for (const auto& emp : vecMan){//parcours la liste d'employes
        if(auto man = dynamic_cast<Manager*>(emp.get())){//filtre vers managers
    	if(toLowerCase(man->getNom()) == toLowerCase(nomPersonne)){//si le nom du manager est trouve            
              projets =  man->getProjetSup();//Retourne le vector avec tous les projets trouvés   
            }            
        }                    
    }
    return projets;
}  

//retire un projet a un manger
void Manager::retirerProjet(
                std::vector<std::unique_ptr<Employe>>& vecMan, const std::string& manCible, const std::string& projetRetirer){
    
      
    std::vector<std::string> projets= trouverProjet(vecMan, manCible);//Cherche d'abord les projets associés au manager 

    for( auto i = 0u; i < projets.size(); ++i){
        if (toLowerCase(projets[i]) == toLowerCase(projetRetirer)){
            projets.erase(projets.begin() + i);//Elimine le projet passe en parametre de l'ancienne liste de projets
            --i;                       
        }        
    }
    for(const auto& emp : vecMan){
       if(auto man = dynamic_cast<Manager*>(emp.get())){ 
            if(toLowerCase(man->getNom()) == toLowerCase(manCible)){            
                man->setProjetSup(projets);// Assigne la nouvelle liste de projets au manager
                wxLogMessage(L"Le projet '%s' a été retiré avec succès du manager '%s'.", projetRetirer, manCible);
                return;           
            }
        }
    }
    wxLogMessage(L"Échec : manager '%s' introuvable.", manCible);
}

//Ajoute un manager dans un projet donné
void Manager::ajouterEmpProjet(std::vector<std::unique_ptr<Employe>>& vecMan, const std::string& manCible, const std::string& projetAjouter){
    
    std::vector<std::string> projets= trouverProjet(vecMan, manCible);//Cherche d'abord les projets associés au manager   

    projets.push_back(projetAjouter);//Ajoute le projet passé en parametres a la liste de projets du manager    

    for(const auto& emp : vecMan){
        if(auto man = dynamic_cast<Manager*>(emp.get())){
            if(toLowerCase(man->getNom()) == toLowerCase(manCible)){//chercher le manager passé en parametres insensible a la casse
                man->setProjetSup(projets);// Assigne le nouvel vector de projets au manager
                wxLogMessage(L"Le projet '%s' a été ajouté avec succès au manager '%s'.", projetAjouter, manCible);
                return;
            }
        }        
    }
    wxLogMessage(L"Échec : manager '%s' introuvable.", manCible);
}

//Retire un programmeur de l'equipe d'un manager
void Manager::retirerProgrammeur(std::vector<std::unique_ptr<Employe>>& vecMan, const std::string& manager, const std::string& programmeur){        

     for (const auto& emp : vecMan) {//parcours vector des employes
        if (auto man = dynamic_cast<Manager*>(emp.get())) { //cible les managers
            if (toLowerCase(man->getNom()) == toLowerCase(manager)) {//trouve le manager en question
                auto& equipe = man->getEquipe();//recupere l'equipe du manager
                for (const auto& membre : equipe){//parcours l'equipe
                    if(toLowerCase(membre) == toLowerCase(programmeur)){//si le programmeur a retirer est trouvé
                        equipe.erase(std::remove(equipe.begin(), equipe.end(), membre), equipe.end());//enleve le programeur de l'equipe
                        wxLogMessage(L"Le programmeur '%s' a été retiré avec succès du manager '%s'.", programmeur, manager);
                        return; 
                    }
                } 
            }
        }
    }
     wxLogMessage(L"Échec: programeur '%s' ou manager '%s' introuvables", programmeur, manager);         
}

//Ajoute un programmeur a l'equipe d'un manager
void Manager::ajouterProgMan(std::vector<std::unique_ptr<Employe>>& vecMan, const std::string& manager, const std::string& programmeur){
     std::vector<std::string> equipe;
       
    for(const auto& emp : vecMan){
        if(auto man = dynamic_cast<Manager*>(emp.get())){//filtre vers managers
            if(toLowerCase(man->getNom()) == toLowerCase(manager)){//si on trouve le nom du manager passe en parametre           
                equipe = man->getEquipe(); //recupere l'equipe du manager
                equipe.push_back(programmeur);//Ajoute le programmeur passé en parametre
                man->setEquipe(equipe);//assigne le nouveau equipe au manager
                wxLogMessage(L"Le programmeur '%s' a été ajouté avec succès au manager '%s'.", programmeur, manager);
                return;
            }
        }        
    }
   wxLogMessage(L"Échec: manager '%s' introuvable", manager); 
}

//affichage de la liste de programmeurs dans le format .csv
wxString Manager::avecModification(std::vector<std::unique_ptr<Employe>>& vecMan){
     wxString result;
     
    for (const auto& emp : vecMan) {        
        if (auto man = dynamic_cast<Manager*>(emp.get())) {
            result += wxString::Format("%s;%s;%d;%d;%s;%d;%s\n",
                                       man->getNom(),
                                       man->getPrenom(),
                                       man->getAge(),
                                       man->getSalaire(),
                                       wxString::FromUTF8(afficher(man->getEquipe())),
                                       man->getBudget(),
                                       wxString::FromUTF8(afficher(man->getProjetSup())));
        }
    }
    return result;
}
