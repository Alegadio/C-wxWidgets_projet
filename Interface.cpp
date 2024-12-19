#include <wx/wx.h>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <format>
#include <wx/tokenzr.h>
#include "Employe.h"
#include "Programmeur.h"
#include "Manager.h"
#include "DlgAfficher.h"
#include "DlgModifier.h"
#include "FenetreAide.h"
#include "Interface.h"


using namespace std;


IMPLEMENT_APP(Interface);

bool Interface::OnInit(){   
    
    const wxString Titre(L"Gestion des employés");   
    TexteFrame *frame = new TexteFrame(Titre, 50, 50, 800, 600);   
    frame->Show(TRUE);    
    SetTopWindow(frame);    
    return true;
}

TexteFrame::TexteFrame(const wxString titre, int xpos, int ypos, int width, int height)
           : wxFrame((wxFrame *)NULL, -1, titre, wxPoint(xpos, ypos), wxSize(width, height)){                
      //variables globales
       programmeursCharge = false; 
       managersCharge = false;
       modifie = false;
       
       employes.reserve(70);
      
       leTexte = (wxTextCtrl *) NULL;
       imprimer = (wxMenu *) NULL;               
       menuBar = (wxMenuBar *) NULL;
       fichierMenu = (wxMenu *) NULL;
       optionsMenu = (wxMenu *) NULL;
       infoMenu = (wxMenu *) NULL;
       aideMenu = (wxMenu *) NULL;               
       
       leTexte = new wxTextCtrl(this, -1,
                    "Veuillez ouvrir les fichiers .csv avec les informations des employes",     
                    wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH2);
       
       imprimer = new wxMenu;
       imprimer->Append(ID_PRINT_MAN, "&Imprimer managers");
       imprimer->Append(ID_PRINT_PROG, "&Imprimer programmeurs");
       
       //Création des menus               
       fichierMenu = new wxMenu;
       fichierMenu->Append(ID_OUVRIR, "&Ouvrir fichier");
       fichierMenu->Append(ID_SAUVEGARDER_SOUS, "&Sauvegarder sous");
       fichierMenu->AppendSeparator();
       fichierMenu->Append(ID_QUITTER, "&Quitter");
        
       optionsMenu = new wxMenu;
       optionsMenu->Append(ID_AFFICHER, "&Afficher");
       optionsMenu->Append(ID_MODIFIER, "&Modifier");
        
       infoMenu = new wxMenu; 
       infoMenu->Append(ID_APROPOS,"&Apropos");

       aideMenu = new wxMenu;
       aideMenu->Append(ID_AIDE, "&Aide");
        
       menuBar = new wxMenuBar;
       menuBar->Append(fichierMenu, "&Fichier");
       menuBar->Append(optionsMenu, "&Options");
       menuBar->Append(infoMenu, "&Info");
       menuBar->Append(aideMenu, "&Aide");
       menuBar->Append(imprimer, "&Imprimer");
        
       SetMenuBar(menuBar);
        
       CreateStatusBar(2);                
       SetStatusText("Application - TP3_IFT1169", 0);
       SetStatusText(leTexte->GetName(), 1);
        }
           
    TexteFrame::~TexteFrame(){}

BEGIN_EVENT_TABLE(TexteFrame, wxFrame)
    EVT_MENU(ID_OUVRIR, TexteFrame::OnFichierOuvrir)
    EVT_MENU(ID_SAUVEGARDER_SOUS, TexteFrame::OnFichierSauvegarderSous)
    EVT_MENU(ID_QUITTER, TexteFrame::OnFichierQuitter)
    EVT_MENU(ID_AFFICHER, TexteFrame::OnAfficher)    
    EVT_MENU(ID_MODIFIER, TexteFrame::OnModifier)
    EVT_MENU(ID_APROPOS, TexteFrame::OnInfoApropos)
    EVT_MENU(ID_AIDE, TexteFrame::OnAide) 
    EVT_MENU(ID_PRINT_MAN, TexteFrame::OnPrintMan)
    EVT_MENU(ID_PRINT_PROG, TexteFrame::OnPrintProg)
END_EVENT_TABLE()

//methodes d'acces et modification
wxString TexteFrame::GetTextValue() const {
    return leTexte->GetValue();
}

void TexteFrame::SetTextValue(const wxString& value) {
    leTexte->SetValue(value);
}

void TexteFrame::AppendTextValue(const wxString& value){
    leTexte->AppendText(value);
}

std::vector<std::unique_ptr<Employe>>& TexteFrame::GetEmployes(){
    return employes;
}

void TexteFrame::SetModifie(bool& etat){
    modifie = etat;
}

//validation de l'entete du fichier
bool TexteFrame::ValiderEntete(const wxString &entete, const wxString &typeFichier) {
     const wxString enteteManagers("Nom;Prenom;Age;Salaire;Equipe;Budget;Projets Supervises");
     const wxString enteteProgrammeurs("Nom;Prenom;Age;Salaire;Langage de Programmation;Niveau d'Experience;Projets en Cours");
     
     if (typeFichier == "programmeur") {
        return entete == enteteProgrammeurs;
    } else if (typeFichier == "manager") {
        return entete == enteteManagers;
    }
    return false;       
}

//validation du nombre de colonnes du fichier
bool TexteFrame::ValiderColonnes(int colonnesAttendues, wxTextCtrl* contenu) {
    int nbLignes = contenu->GetNumberOfLines()-1; // Nombre total de lignes dans le contrôle texte

    for (int i = 0; i < nbLignes; ++i) {
        // Récupération du texte de la ligne et découpage en colonnes
        wxString ligne = contenu->GetLineText(i);
        wxStringTokenizer tokenizer(ligne, ";");
        int nbColonnes = tokenizer.CountTokens();

        // Vérification du nombre de colonnes
        if (nbColonnes != colonnesAttendues) {
            wxMessageBox(
                wxString::Format(
                    "Une ligne contient %d colonnes au lieu de %d.\nContenu de la ligne : %s",
                    nbColonnes, colonnesAttendues, ligne), "Erreur", wxOK | wxICON_ERROR);
            return false; // Validation échouée
        }
    }
    return true; // Toutes les lignes ont le bon nombre de colonnes
}

//Mis a jour du contenu des fichiers apres modifications effectues
void TexteFrame::MisAJourContenu(const wxString &typeEmploye, const wxString &contenu) {
    if(employes.empty()){
        wxMessageBox("Veuillez ouvrir les fichiers .csv avec les informations des employes", "Erreur", wxOK | wxICON_ERROR);
        return;
    }
    const wxString enteteManagers("Nom;Prenom;Age;Salaire;Equipe;Budget;Projets Supervises");
    const wxString enteteProgrammeurs("Nom;Prenom;Age;Salaire;Langage de Programmation;Niveau d'Experience;Projets en Cours");
     
    leTexte->SetValue((typeEmploye == "Manager" ? enteteManagers : enteteProgrammeurs) + "\n" + contenu);
    modifie = true;   
}

//appel de fonction OnFichierSauvegarderSous apres detecter modifications
void TexteFrame::OnFichierModifie(bool& modifie){
    //si le fichier a été modifié, proposer de le sauvegarder
    if (modifie) {
        int choix = wxMessageBox(
            wxString::FromUTF8("Des modifications ont étés effectuées. Voulez-vous sauvegarder avant de continuer?"),
            "Confirmer la sauvegarde",  wxYES_NO | wxCANCEL | wxICON_QUESTION);

        if (choix == wxYES) {
            wxCommandEvent eventSave;
            OnFichierSauvegarderSous(eventSave);
        } else if (choix == wxCANCEL) {        
            return;
        } else if (choix == wxNO) {        
        wxLogMessage("Vous avez choisi de ne pas sauvegarder.");
        }
    }
}


void TexteFrame::OnFichierOuvrir(wxCommandEvent &event){    
        wxFileDialog *dlg = new wxFileDialog(this, "Ouvrir un fichier", "", "",
                                "Fichiers csv(*.csv)|*.csv", wxFD_OPEN | wxFD_FILE_MUST_EXIST);        
                        
        if(dlg->ShowModal() == wxID_OK){        
            wxString fileName = dlg->GetPath();                       
            
            leTexte->LoadFile(fileName);             
            wxString entete = leTexte->GetLineText(0);//Recupere la premier ligne pour validation d'entete
            
            //validation du nombre de colonnes
            ValiderColonnes(7, leTexte);
            
            //Validation, lecture et recuperation de données dans le fichier selon le type d'employe
            if(ValiderEntete(entete, "programmeur")){                          
                p.lireEmploye(leTexte, employes);               
                programmeursCharge = true;                
            } else if (ValiderEntete(entete, "manager")){            
                 m.lireEmploye(leTexte, employes);                 
                 managersCharge = true;                  
            } else {
                leTexte->SetValue("Veuillez ouvrir les fichiers .csv avec les informations des employes");    
                wxMessageBox("Le Fichier ne peut pas etre lu, format non conforme", "Erreur", wxOK | wxICON_ERROR);
                return;
            }                   
        }        
        
        //validation de la recuperation de toutes les informations nécessaires pour les traitements
        if (managersCharge && programmeursCharge) {
            leTexte->Clear();
            wxLogMessage(L"Informations des managers et des programmeurs récupérées avec succès.\n" 
                              L"Nombre d'employés trouvés: " + wxString::Format(wxT("%i"), employes.size()));
        } else if (managersCharge) {
            wxMessageBox(L"Veuillez également ouvrir le fichier des programmeurs.", "Information", wxOK | wxICON_INFORMATION);
        } else if (programmeursCharge) {
            wxMessageBox(L"Veuillez également ouvrir le fichier des managers.", "Information", wxOK | wxICON_INFORMATION);
        } else {
            wxMessageBox(L"Aucun fichier n'a été chargé. Veuillez ouvrir les fichiers requis.", "Information", wxOK | wxICON_INFORMATION);
        }   
}

void TexteFrame::OnFichierSauvegarderSous(wxCommandEvent &event) {
     wxFileDialog *dlg = new wxFileDialog(this, "Sauvegarde sous", "", "", "Fichiers csv (*.csv)|.csv|Fichiers texte (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT, 
        wxDefaultPosition);

    // Affiche la boîte de dialogue et vérifie si l'utilisateur a validé
    if (dlg->ShowModal() == wxID_OK) {
        wxString fileName = dlg->GetPath(); // Récupère le chemin complet du fichier        
        if (leTexte->SaveFile(fileName)) { // Sauvegarde réussie
            wxMessageBox(L"Fichier sauvegardé avec succès.","Sauvegarde", wxOK | wxICON_INFORMATION);
        } else { // Échec de la sauvegarde
            wxMessageBox(L"Échec de la sauvegarde du fichier.", "Erreur", wxOK | wxICON_ERROR);
        }
    }
    
    dlg->Destroy();
}

void TexteFrame::OnAfficher(wxCommandEvent &event){    
    //charger les employes avant d'afficher des requetes
    if (employes.empty()) {
        wxMessageBox("Veuillez charger les informations des employes avant d'afficher des informations.", "Erreur", wxOK | wxICON_WARNING);
        return;
    }
   
    auto dlg = std::make_unique<DlgAfficher>(this, L"Choisissez les informations à afficher");  
    if(dlg->ShowModal() == wxID_OK){       
    }    
}

void TexteFrame::OnModifier(wxCommandEvent &event){     
     //charger les informations des employes avant de faire des modifications
     if (employes.empty()) {
        wxMessageBox("Veuillez charger les informations des employes avant de modifier des informations.", "Erreur", wxOK | wxICON_WARNING);
        return;
    } 
    
    auto dlg = std::make_unique<DlgModifier>(this, L"Choisissez les informations à modifier");    
    if(dlg->ShowModal() == wxID_OK){                  
    }          
}

void TexteFrame::OnPrintMan(wxCommandEvent &event){    
    MisAJourContenu("Manager", m.avecModification(employes));
}
void TexteFrame::OnPrintProg(wxCommandEvent &event){    
    MisAJourContenu("Programmeur", p.avecModification(employes));
}

void TexteFrame::OnInfoApropos(wxCommandEvent &event){
    //Information de l'auteur et du programme 
    wxLogMessage(L"Auteur: Alejandra Garcia Diosa\n Version: 1.0\n Année de réalisation: 2024");
}

void TexteFrame::OnAide(wxCommandEvent &event){
    //fenetre d'aide avec la description du programme
    FenetreAide *aideFen = new FenetreAide();
    aideFen->Show(TRUE);
}

void TexteFrame::OnFichierQuitter(wxCommandEvent &event){
   OnFichierModifie(modifie);//s'il a eu des modification, sauvegarder avant de quitter
   Close(TRUE);
}