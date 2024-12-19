#ifndef INTERFACE_H
#define INTERFACE_H



class Interface : public wxApp {
public:
    virtual bool OnInit();

};

class TexteFrame : public wxFrame {
    public:
        TexteFrame(const wxString titre, int xpos, int ypos, int width, int height);
        ~TexteFrame();
        
        //methodes d'acces et modification
        wxString GetTextValue() const;
        std::vector<std::unique_ptr<Employe>>& GetEmployes();
        void SetTextValue(const wxString& value);
        void SetModifie(bool& etat);        
        void AppendTextValue(const wxString& value);
        
        //methodes qui declanchent des actions sur les menus
        void OnFichierOuvrir(wxCommandEvent &event);
        void OnFichierSauvegarderSous(wxCommandEvent &event);
        void OnFichierQuitter(wxCommandEvent &event);
        void OnInfoApropos(wxCommandEvent &event);
        void OnAfficher(wxCommandEvent &event);
        void OnModifier(wxCommandEvent &event);        
        void OnAide(wxCommandEvent &event);
        void OnPrintProg(wxCommandEvent &event);
        void OnPrintMan(wxCommandEvent &event);
  
       //helper methodes  
       void MisAJourContenu(const wxString &entete, const wxString &contenu);//imprime sur le texteFrame le contenu modifie       
       void OnFichierModifie(bool &modifie);        
       bool ValiderColonnes(int colonnesAttendues, wxTextCtrl* contenu);                    
       bool ValiderEntete(const wxString &entete, const wxString &enteteAttendu);
        
    protected:
        DECLARE_EVENT_TABLE();
    
    
    private:
        wxTextCtrl *leTexte;        
        wxMenuBar *menuBar;        
        wxMenu *optionsMenu;
        wxMenu *fichierMenu;
        wxMenu *infoMenu;
        wxMenu *aideMenu;        
        wxMenu *imprimer;
        
        std::vector<std::unique_ptr<Employe>> employes;
        
        Manager m;
        Programmeur p;
        
        bool modifie, programmeursCharge, managersCharge;       
        
        enum {
            ID_QUITTER = 1,
            ID_OUVRIR,
            ID_SAUVEGARDER_SOUS,
            ID_AFFICHER,
            ID_MODIFIER,
            ID_AIDE,
            ID_PRINT_MAN,
            ID_PRINT_PROG,
            ID_APROPOS
        };
};

#endif // INTERFACE_H
