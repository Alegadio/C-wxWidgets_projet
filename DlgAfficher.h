#ifndef DLG_AFFICHER_H
#define DLG_AFFICHER_H
#include "Interface.h"
class DlgAfficher : public wxDialog {
public:    
    DlgAfficher(wxFrame* parent, const wxString& title);    
    ~DlgAfficher();    
    
    wxString GetChoix() const;
    wxString GetCible() const;
    wxString GetAttribut() const;
    
    void OnOk(wxCommandEvent &event);
    void OnAnnuler(wxCommandEvent &event);
    void afficherManager(wxString cible, wxString attribut);
    void afficherProgrammeur(wxString cible, wxString attribut);
    void wxAfficher(const std::vector<std::string> &resultat, const wxString &attribut, const wxString &cible);

private:
    
    TexteFrame* parent;    
    wxRadioBox* categorie;
    wxRadioBox* attribut;
    wxTextCtrl* cible;   
    wxButton* btnOK;
    wxButton* btnAnnuler;
    
    Manager m;
    Programmeur p;

    enum {
        ID_BTN_OK = wxID_HIGHEST + 1,
        ID_ANNULER
    };
    
    wxDECLARE_EVENT_TABLE();

};

#endif // DLGENTREE_H
