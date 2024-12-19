#include "Programmeur.h"
#include "Manager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <format>
#include <memory>
#include <wx/wx.h>

//definition de constructeurs, methodes d'acces, de modification et autres methodes de la classe

Programmeur::Programmeur(std::string n, std::string p, int a, int s, std::string lp, std::string niv, std::vector<std::string> pr)
				:Employe(n, p, a, s), langProg(lp), niveau(niv), projet(pr){
				}

std::string Programmeur::getLang(){
	return langProg;
}

std::string Programmeur::getNiveau(){
	return niveau;
}
 
std::vector<std::string> Programmeur::getProjet(){
	return projet;
}

void Programmeur::setProjet(const std::vector<std::string>& nouveauProjet){
    projet = nouveauProjet;
}

//fonction helper pour trier le vector selon le nom de l'employe
bool Programmeur::comp(std::unique_ptr<Employe>& a, std::unique_ptr<Employe>& b) {
    return a->getNom() < b->getNom();
} 

//fonction helper pour convertir les entrées de l'utilisateur et les attributs qu'on compare en minuscule
//pour eviter des erreurs d'entrée
std::string Programmeur::toLowerCase(const std::string& entree) {
    std::string lower = entree;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

//methode qui affichera les resultats dans le format voulu
std::string Programmeur::afficher(std::vector<std::string> resultat) {
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
void Programmeur::lireEmploye(wxTextCtrl* fichier, std::vector<std::unique_ptr<Employe>>& vecProg) {	
    std::string ligne;
    
    //ignore l'entete et la dernier ligne pour eviter erreur innecesaires et recuperer les données pertinantes
	for(int i = 1; i < fichier->GetNumberOfLines() -1; ++i) {        
        try{
            ligne = fichier->GetLineText(i).ToStdString();
            
            std::stringstream ss(ligne);
            std::string nom, prenom, langPro, niveau, projetsStr, ageStr, salaireStr;
            int age, salaire;

            if(!std::getline(ss, nom, ';') || !std::getline(ss, prenom, ';') ||
               !std::getline(ss, ageStr, ';') || !std::getline(ss, salaireStr, ';') ||
               !std::getline(ss, langPro, ';') || !std::getline(ss, niveau, ';') ||
               !std::getline(ss, projetsStr, ';')){
                wxLogMessage(L"Ligne %d ignoré: pas assez d'information", i);
                continue;
            }

            try{
               age = stoi(ageStr);
               salaire = stoi(salaireStr); 
            } catch (const std::invalid_argument&) {
                wxLogMessage(L"Ligne %d ignoré: format numerique non valide", i);
                continue; 
            } catch (const std::out_of_range&) {
                wxLogMessage(L"Ligne %d ignoré: valeur numerique hors de portée", i);
                continue;
            }

            std::vector<std::string> projets;            
            
            //transform les projets de string a vector pour faciliter la manipulation
            std::stringstream p(projetsStr);
            std::string value;
            while(std::getline(p, value, ',')){
                projets.push_back(value);
            }            

        //push un pointeur intelligent vers un objet de type Programmeur dans le vecteur
        vecProg.push_back(std::make_unique<Programmeur>(nom, prenom, age, salaire, langPro, niveau, projets));
        } catch (const std::exception& e){
            wxLogMessage(L"Érreur traitant la ligne %d: %s", i, e.what());
            continue;
        }               
    }
    //tri du vector
    if (!vecProg.empty()) {
        std::sort(vecProg.begin(), vecProg.end(), Programmeur::comp);
    }
}

//Cherche et retourne les programmeurs qui travaillent dans un projet donné
std::vector<std::string> Programmeur::compterProjet(
                        std::vector<std::unique_ptr<Employe>>& vecProg, const std::string& nomProjet) {
    
    std::vector<std::string> empProjet;    

    for(const auto& emp : vecProg){//parcours le conteneur de pointeurs vers programmeurs et managers
        if(auto prog = dynamic_cast<Programmeur*>(emp.get())){//filtre ceux qui pointent vers le type Programmeur
            const auto& projets = prog->getProjet();//recupere les projets
            for(const auto& projet : projets){//parcours le projets
                if(toLowerCase(projet) == toLowerCase(nomProjet)){//cherche des concordance entre le projet passé en paremetre et les projets existants
                   empProjet.push_back(prog->getPrenom() +" "+ prog->getNom());//on ajoute le nom complet du programmeur qui a le projet trouvé
                }
            }           
        }                
    }

    return empProjet; // Retourne le vector avec la liste de programmeurs associés au projet 
}

//cherche et retourne les noms des programmeurs associés à un manager spécifique
std::vector<std::string> Programmeur::trouverProgrammeurs( 
    std::vector<std::unique_ptr<Employe>>& vecEmp, const std::string& nomManager) {
    
    std::vector<std::string> equipe;
    // Trouver le manager et son equipe    
    for (const auto& emp : vecEmp) {
        if (auto man = dynamic_cast<Manager*>(emp.get())) {//parmis les managers
            if ( toLowerCase(man->getNom()) == toLowerCase(nomManager)) {//chercher le manager passé en parametres insensible a la casse
                equipe = man->getEquipe();//recupere son equipe                
                break;
            }
        }
    }
    // si le manager n'est pas trouve
    if (equipe.empty()) {
        wxLogMessage(L"Aucun programmeur trouvé pour manager '%s'", nomManager);
        return {}; 
    }
    
    //tri de l'equipe selon le nom     
    std::sort(equipe.begin(), equipe.end(), [](const std::string& n1, const std::string& n2 ){
        return (n1 < n2);
    });
    
    // trouve les programmeurs dans l'equipe
    std::vector<std::string> resultatEquipe;
    for (const auto& nom : equipe) {//parcours l'equipe
        for (const auto& emp : vecEmp) {
            if (auto prog = dynamic_cast<Programmeur*>(emp.get())) {//parmis les employes on cible les programmeurs
                if (toLowerCase(prog->getNom()) == toLowerCase(nom)) {//cherche les noms de l'equipe du manager dans la liste de programmeurs
                    resultatEquipe.push_back(prog->getPrenom() + " " + prog->getNom());//recupere le nom complet du programmeur si trouvé 
                    break;
                }
            }
        }
    }    
    return resultatEquipe;//retourne vector avec les noms complets de l'equipe du manager
}

//on cherche et retourne les projets d'un programmeur donné
std::vector<std::string> Programmeur::trouverProjet(
                std::vector<std::unique_ptr<Employe>>& vecProg, const std::string& nomPersonne) {
    
    std::vector<std::string> projets; 
    
    for (const auto& emp : vecProg){
        if(auto prog = dynamic_cast<Programmeur*>(emp.get())){//parmis les programmeurs
            if(toLowerCase(prog->getNom()) == toLowerCase(nomPersonne)){ //si le nom du programmeur est trouve  
                projets = prog->getProjet();//recuperer le projet du programmeur
            }
        }    	  
    }
    return projets;//on retourne le vector avec tous les projets trouvés
}

//on ajoute un projet a un programmeur
void Programmeur::ajouterEmpProjet(
                std::vector<std::unique_ptr<Employe>>& vecProg, const std::string& programmeur, const std::string& projetAjouter){
    
    std::vector<std::string> projets; 
    
    for(const auto& emp: vecProg){
        if(auto prog = dynamic_cast<Programmeur*>(emp.get())){//filtre vers programmeurs
            if(toLowerCase(prog->getNom()) == toLowerCase(programmeur)){//si on trouve le programmeur passé en parametre
                projets = prog->getProjet();//recupere les projets
                projets.push_back(projetAjouter);//ajoute le projet passé en paramentre aux projets existantes
                prog->setProjet(projets);//assigne le nouveau vector de projets au programeur
                wxLogMessage(L"Le projet '%s' a été ajouté avec succès au programmeur '%s'.", projetAjouter, programmeur);
                return;
            }
        }        
    }
    
    wxLogMessage(L"Échec : Programmeur '%s' introuvable.", programmeur);
}

//on retire un projet a un programmeur
void Programmeur::retirerProjet(
                    std::vector<std::unique_ptr<Employe>>& vecProg, const std::string& programmeur, const std::string& projet){
    
    std::vector<std::string> projets= trouverProjet(vecProg, programmeur);//on cherche d'abord les projets associés au programmeur
    
    for( auto i = 0u; i < projets.size(); ++i){//parcours le projet
        if (toLowerCase(projets[i]) == toLowerCase(projet)){//cherche un match
            projets.erase(projets.begin() + i);//on elimine le projet passe en parametre de l'ancienne liste de projets
            --i;                       
        }
    }   

    for(const auto& emp : vecProg){
        if(auto prog = dynamic_cast<Programmeur*>(emp.get())){
            if(toLowerCase(prog->getNom()) == toLowerCase(programmeur)){                
                prog->setProjet(projets); // on assigne la nouvelle liste de projets au programmeur
                wxLogMessage(L"Le projet '%s' a été retiré avec succès du programmeur '%s'.", projet, programmeur);
                return;
            }
        }        
    }
    
    wxLogMessage(L"Échec : Programmeur '%s' ou projet '%s' introuvable.", programmeur, projet);
}

//affichage de la liste de programmeurs dans le format .csv
wxString Programmeur::avecModification(std::vector<std::unique_ptr<Employe>>& vecProg){
     wxString result;
     
    for (const auto& emp : vecProg) {        
        if (auto prog = dynamic_cast<Programmeur*>(emp.get())) {
            result += wxString::Format("%s;%s;%d;%d;%s;%s;%s\n",
                                       prog->getNom(),
                                       prog->getPrenom(),
                                       prog->getAge(),
                                       prog->getSalaire(),
                                       prog->getLang(),
                                       prog->getNiveau(),
                                       wxString::FromUTF8(afficher(prog->getProjet())));
        }
    }
    return result;
}


Programmeur::~Programmeur()
{
}