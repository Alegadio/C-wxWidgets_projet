#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/sizer.h>
#include "FenetreAide.h"

FenetreAide::FenetreAide() :
        wxFrame(nullptr, wxID_ANY, "Aide", wxDefaultPosition, wxSize(500, 400))
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    
    wxRichTextCtrl* textAide = new wxRichTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(500, 400), wxTE_READONLY | wxTE_MULTILINE);
    
    textAide->SetValue(
    L"Bienvenue dans la section d'aide\n\n"
    L"Ce programme a été conçu pour effectuer les tâches suivantes :\n"
    L"1. Charger les informations des employés à partir de fichiers .csv.\n"
    L"2. Afficher des informations spécifiques, comme les projets d'un employé ciblé ou les employés impliqués dans un projet donné.\n"
    L"3. Modifier les informations des employés en ajoutant ou retirant des projets, entre autres modifications.\n"
    L"4. Afficher et sauvegarder les informations des employés modifiés.\n\n"
    L"Comment utiliser le programme :\n"
    L"1. Ouvrez les fichiers .csv contenant les informations des employés (managers et programmeurs).\n"
    L"2. Utilisez l'outil *Afficher* pour renseigner les informations demandées et afficher les résultats souhaités.\n"
    L"3. Utilisez l'outil *Modifier* pour effectuer des modifications, comme ajouter ou retirer un employé (manager ou programmeur) ou un projet, en remplissant les champs demandés.\n"
    L"4. Utilisez l'outil *Imprimer* pour afficher les informations des employes après avoir effectué les modifications.\n"
    L"5. Utilisez l'outil *Sauvegarder sous* pour enregistrer les informations modifiées sous un nouveau nom.\n\n"
    L"Pour toute assistance supplémentaire, veuillez contacter : algarciadio@gmail.com"   
    );
    
    sizer->Add(textAide, 1,wxEXPAND | wxALL, 10);
    
    SetSizerAndFit(sizer);
}

FenetreAide::~FenetreAide()
{
}

