#ifndef DLGMODIFIER_H
#define DLGMODIFIER_H
#include "Interface.h"

class DlgModifier : public wxDialog{
public:
    DlgModifier(wxFrame* parent, const wxString& title);    
    ~DlgModifier();
    
    wxString GetOperation() const;
    wxString GetCateg() const;
    wxString GetCible() const;
    wxString GetAttribut() const;
    wxString GetAMod() const;    
    
private:
   
    void OnOk(wxCommandEvent &event);
    void OnAnnuler(wxCommandEvent &event);
    void OnRetirer(wxString categ, wxString cible, wxString attribut, wxString aModifier);
    void OnAjouter(wxString categ, wxString cible, wxString attribut, wxString aModifier);    
    
    TexteFrame* parent;
    wxRadioBox* categorie;
    wxRadioBox* attribut;
    wxRadioBox* operation; 
    wxTextCtrl* cible;
    wxTextCtrl* aModifer;    
    wxButton* btn_Ok;
    wxButton* btn_Annuler;
    
    Manager m;
    Programmeur p;
    
    //variable pour suivre les modifications
    bool modifie;
    
    enum{
        ID_ON_OK = 55, 
        ID_ANN
    };
    
    wxDECLARE_EVENT_TABLE();
};

#endif // DLGMODIFIER_H
