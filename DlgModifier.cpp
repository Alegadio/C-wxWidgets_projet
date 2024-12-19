#include <wx/wx.h>
#include <wx/radiobox.h>
#include "Employe.h"
#include "Programmeur.h"
#include "Manager.h"
#include "DlgAfficher.h"
#include "DlgModifier.h"
#include "Interface.h"


using namespace std;

DlgModifier::DlgModifier(wxFrame* parent, const wxString& title):
            wxDialog(parent, -1, title, wxDefaultPosition, wxSize(300,200), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER){
     
    this->parent = dynamic_cast<TexteFrame*>(parent);
        if (!this->parent) {
        wxMessageBox("Parent non valide!", "Erreur", wxOK | wxICON_ERROR);
        return;
    }
      
      modifie = false;      
     //radioBoxes pour controler l'entrée de l'utilisateur   
     const wxString types[] = {"Manager", "Programmeur"};
     const wxString aModifier[] =  {"Manager", "Programmeur", "Projet"};
     const wxString operations[] = {"Ajouter", "Retirer"};
     
     //input widgets pour saisir les attributs ciblés
     cible = new wxTextCtrl(this, wxID_ANY);
     aModifer = new wxTextCtrl(this, wxID_ANY);
     
     categorie = new wxRadioBox(this, wxID_ANY, L"Type d'employé :", wxDefaultPosition, wxSize(200, wxDefaultCoord),
                               WXSIZEOF(types), types, 1, wxRA_SPECIFY_ROWS);
     attribut = new wxRadioBox(this, wxID_ANY, L"Objet à modifier :", wxDefaultPosition, wxSize(200, wxDefaultCoord),
                               WXSIZEOF(aModifier), aModifier, 1, wxRA_SPECIFY_ROWS);                          
     operation = new wxRadioBox(this, wxID_ANY, "Type de modification :", wxDefaultPosition, wxSize(200, wxDefaultCoord),
                               WXSIZEOF(operations), operations, 1, wxRA_SPECIFY_ROWS);           

     btn_Ok = new wxButton(this, ID_ON_OK, "OK");
     btn_Annuler = new wxButton(this, ID_ANN, "Annuler");     
     
     wxBoxSizer *dialogSizer = new wxBoxSizer(wxVERTICAL);
     wxFlexGridSizer *textSizer = new wxFlexGridSizer(2, 2, 10, 10);

     textSizer->Add(new wxStaticText(this, wxID_ANY, L"Nom de l'objet à modifier (e.g., Diaz, projetH) :"), 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
     textSizer->Add(cible, 1, wxEXPAND | wxALL, 5);
     textSizer->Add(new wxStaticText(this, wxID_ANY, L"Valeur à modifier (e.g., projetF, Summers) :"), 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
     textSizer->Add(aModifer, 1, wxEXPAND | wxALL, 5);  

     dialogSizer->Add(operation, 0, wxALL | wxEXPAND, 10);
     dialogSizer->Add(categorie, 0, wxALL | wxEXPAND, 10);
     dialogSizer->Add(attribut, 0, wxALL | wxEXPAND, 10);
     dialogSizer->Add(textSizer, 0, wxALL | wxEXPAND, 10);          
     
     wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);   
     buttonSizer->Add(btn_Ok, 0, wxALL, 5);
     buttonSizer->Add(btn_Annuler, 0, wxALL, 5);     
     
     dialogSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);
    
     SetSizerAndFit(dialogSizer); 
}

DlgModifier::~DlgModifier()
{
}

//Methodes d'acces aux champs d'entrée
wxString DlgModifier::GetCateg() const{
    return categorie->GetStringSelection();
}

wxString DlgModifier::GetCible() const{
    return cible->GetValue();
}

wxString DlgModifier::GetAttribut() const {
    return attribut->GetStringSelection();
}

wxString DlgModifier::GetOperation() const {
    return operation->GetStringSelection();
}

wxString DlgModifier::GetAMod() const {
    return aModifer->GetValue();
}

//traitement des operations de retraite des employes ou projets
void DlgModifier::OnRetirer(wxString categ, wxString cible, wxString attribut, wxString aModifier){
    auto& employes = parent->GetEmployes();//recupere le vector d'employes de la fenetre parent
    
    if (categ == "Manager" && attribut == "Projet") {
        m.retirerProjet(employes, cible.ToStdString(), aModifier.ToStdString());//retire un projet a un manager               
    } else if (categ == "Manager" && attribut == "Programmeur") {
        m.retirerProgrammeur(employes, cible.ToStdString(), aModifier.ToStdString());//retire un programmeur a un manager               
    } else if (categ == "Programmeur" && attribut == "Projet") { 
        p.retirerProjet(employes, cible.ToStdString(), aModifier.ToStdString());//retire un projet a un programmeur               
    } else {
        wxMessageBox("Modification non permise.", "Erreur", wxOK | wxICON_ERROR);
        return;
    }
    modifie = true;             
    parent->SetModifie(modifie);//envoi modifie= true a la fenetre parent
}

//traitement des operations d'ajout des employes ou projets
void DlgModifier::OnAjouter(wxString categ, wxString cible, wxString attribut, wxString aModifier){
    auto& employes = parent->GetEmployes();//recupere le vector d'employes de la fenetre parent
    
    if (categ == "Manager" && attribut == "Projet") {
        m.ajouterEmpProjet(employes, cible.ToStdString(), aModifier.ToStdString());//Ajout un projet a un manager                
    } else if (categ == "Manager" && attribut == "Programmeur") {
        m.ajouterProgMan(employes, cible.ToStdString(), aModifier.ToStdString());//Ajout  un programmeur a un manager              
    } else if (categ == "Programmeur" && attribut == "Projet") {
         p.ajouterEmpProjet(employes, cible.ToStdString(), aModifier.ToStdString());//Ajout un projet a un programmeur                 
    } else {
        wxMessageBox("Modification non permise.", "Erreur", wxOK | wxICON_ERROR);
        return;
    }
    modifie = true;             
    parent->SetModifie(modifie);//envoi modifie= true a la fenetre parent
}

void DlgModifier::OnOk(wxCommandEvent &event){
   
    wxString categ = GetCateg();//recupere le type d'employe
    wxString cible = GetCible();//recupere l'employe objetive des modification a efectuer
    wxString attribut = GetAttribut();//recupere l'attribut a modifier
    wxString operation = GetOperation();//recupere le type d'operation a efectuer
    wxString aModifier = GetAMod();//recupere le nouveau attribut a ajouter ou retirer

    // Validation des entrées
    if (cible.IsEmpty() || aModifer->IsEmpty()) {
        wxMessageBox(L"Veuillez remplir tous les champs correctement.", "Erreur", wxOK | wxICON_ERROR);
        return;
    }
    //selon l'operation recuperé, on appel les fonctions pertinantes    
    operation == "Retirer"?  OnRetirer(categ, cible, attribut, aModifier): OnAjouter(categ, cible, attribut, aModifier);
   
    EndModal(wxID_OK);     
}

void DlgModifier::OnAnnuler(wxCommandEvent &event){
    EndModal(wxID_CANCEL);
}

BEGIN_EVENT_TABLE(DlgModifier, wxDialog)
    EVT_BUTTON(ID_ON_OK, DlgModifier::OnOk)
    EVT_BUTTON(ID_ANN, DlgModifier::OnAnnuler)    
END_EVENT_TABLE()