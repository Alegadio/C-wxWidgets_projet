#include <wx/wx.h>
#include <wx/radiobox.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/arrstr.h>
#include <string>
#include "Employe.h"
#include "Programmeur.h"
#include "Manager.h"
#include "DlgAfficher.h"
#include "Interface.h"

using namespace std;

DlgAfficher::DlgAfficher(wxFrame*parent, const wxString& title):
            wxDialog(parent, -1, title, wxDefaultPosition, wxSize(300,200), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    this->parent = dynamic_cast<TexteFrame*>(parent);
    if (!this->parent) {
        wxMessageBox("Parent non valide!", "Erreur", wxOK | wxICON_ERROR);
        return;
    }
    
    //options pour le radiobox
    const wxString choices[] = {"Manager", "Programmeur"}; 
    const wxString options[] = {"managers", "programmeurs", "projets"};
    
    cible = new wxTextCtrl(this, -1);    
    
    categorie = new wxRadioBox(this, wxID_ANY, L"Choisissez le type d'employé:", wxDefaultPosition, wxSize(200, wxDefaultCoord),
                               WXSIZEOF(choices), choices, 1, wxRA_SPECIFY_ROWS);
    attribut = new wxRadioBox(this, wxID_ANY, L"Choisissez l'attribut à afficher:", wxDefaultPosition, wxSize(200, wxDefaultCoord), 
                                WXSIZEOF(options), options, 1, wxRA_VERTICAL);
    
    btnOK = new wxButton(this, ID_BTN_OK, "OK");
    btnAnnuler = new wxButton(this, ID_ANNULER, "Annuler");     
            
    wxBoxSizer *dialogSizer = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer *textSizer = new wxFlexGridSizer(2, 2, 10, 10);
    
    textSizer->Add(new wxStaticText(this, wxID_ANY, "Cible (i.e Diaz, projetH) :"), 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    textSizer->Add(cible, 1, wxEXPAND | wxALL, 5);    
    
    dialogSizer->Add(categorie, 0, wxALL | wxEXPAND, 10);
    dialogSizer->Add(textSizer, 0, wxALL | wxEXPAND, 10);
    dialogSizer->Add(attribut, 0, wxALL | wxEXPAND, 10);
    
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(btnOK, 0, wxALL, 5);
    buttonSizer->Add(btnAnnuler, 0, wxALL, 5);
    
    dialogSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);
    
    SetSizerAndFit(dialogSizer); 
   
}

DlgAfficher::~DlgAfficher()
{
}

//methodes d'acces
wxString DlgAfficher::GetChoix() const{
    return categorie->GetStringSelection();
}

wxString DlgAfficher::GetCible() const{
    return cible->GetValue();
}

wxString DlgAfficher::GetAttribut() const {
    return attribut->GetStringSelection();
}

//Afficher les resultat des requetes sur le menu 'Afficher'
void DlgAfficher::wxAfficher( const vector<string> &resultat, const wxString &attribut, const wxString &cible){    
     wxString resultatRequete = "";
     wxString enteteRequete = attribut + " du " + cible + ":\n";
    //message si aucun resultat et trouvé
    if (resultat.empty()) {
        parent->AppendTextValue("****\n");
        wxMessageBox(L"Aucun " + attribut + L" trouvé pour " + cible +"\n****", "Information", wxOK | wxICON_INFORMATION);
        return;       
    }          
      
    for (unsigned int i = 0; i < resultat.size(); i++){
        resultatRequete += "**"+ wxString(resultat[i] + "**");   
            if (i < resultat.size() - 1) { 
                resultatRequete += ", ";
            }                     
    }
    
    parent->AppendTextValue(resultatRequete + "\n");//imprime l'information sur leTexte de TexteFrame
    wxMessageBox(enteteRequete + resultatRequete, "Information", wxOK | wxICON_INFORMATION);//affiche l'information sur une boite de message
}

//affiche les informations des managers
void DlgAfficher::afficherManager(wxString cible, wxString attribut){
    std::vector<std::string> resultat;//vector pour stocker le resultat des requetes
    auto& employes = parent->GetEmployes();//recupere la liste complet des employes a partir de la fenetre parent
    
    if (attribut == "managers") {
        resultat = m.compterProjet(employes, cible.ToStdString());//manager qui supervisent un projet donné
    } else if (attribut == "projets") {
        resultat = m.trouverProjet(employes, cible.ToStdString());//projets supervisées par un manager donné
    } else if (attribut == "programmeurs") { 
       resultat = p.trouverProgrammeurs(employes, cible.ToStdString());//equipe d'un manager                
    } else {
        wxMessageBox("Attribut non valide pour Manager.", "Erreur", wxOK | wxICON_ERROR);
        return;
    }
    wxAfficher(resultat, attribut, cible);//affiche le resultat de la requete        
}

//affiche les informations des programmeurs
void DlgAfficher::afficherProgrammeur(wxString cible, wxString attribut){    
    std::vector<std::string> resultat;
    auto& employes = parent->GetEmployes();//recupere la liste complet des employes a partir de la fenetre parent
    
    if (attribut == "programmeurs") {
        resultat = p.compterProjet(employes, cible.ToStdString());//programmeurs qui travaillent sur un projet donné
    } else if (attribut == "managers") {
        resultat = m.trouverManagerProgrammeur(employes, cible.ToStdString());//manager d'un programmeur donné
    } else if (attribut == "projets") {
        resultat = p.trouverProjet(employes, cible.ToStdString());//projets d'un programmeur donné
    } else {
        wxMessageBox("Attribut non valide pour Programmeur.", "Erreur", wxOK | wxICON_ERROR);
        return;
    }
    wxAfficher(resultat, attribut, cible);//affiche le resultat de la requete
}

void DlgAfficher::OnOk(wxCommandEvent &event){
    //recuperation des informations saisies
    wxString choix = GetChoix();
    wxString cible = GetCible();
    wxString attribut = GetAttribut();
    
    if (cible.IsEmpty()) {
        wxMessageBox("Veuillez remplir le champ 'Cible'.", "Erreur", wxOK | wxICON_ERROR);
        return;
    }
    //appel la bon fonction selo la choix
    choix == "Manager"? afficherManager(cible, attribut) : afficherProgrammeur(cible, attribut);
    
    EndModal(wxID_OK);
}

void DlgAfficher::OnAnnuler(wxCommandEvent &event){
    EndModal(wxID_CANCEL);
}

BEGIN_EVENT_TABLE(DlgAfficher, wxDialog)
    EVT_BUTTON(ID_BTN_OK, DlgAfficher::OnOk)
    EVT_BUTTON(ID_ANNULER, DlgAfficher::OnAnnuler)
END_EVENT_TABLE()