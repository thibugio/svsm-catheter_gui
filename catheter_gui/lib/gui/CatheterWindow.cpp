#include "CatheterWindow.h"

CatheterWindow::CatheterWindow(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition),
    parentPanel(new wxPanel(this, wxID_ANY)) {
    
    if (!parentPanel) {
        wxMessageBox("Catheter Window could not be initialized: parentPanel is NULL.");
        exit(EXIT_FAILURE);
    }

    statusPanel = new StatusPanel(parentPanel);
    editPanel = new EditPanel(parentPanel);
    controlPanel = new ControlPanel(parentPanel);
    safeExitButton = new wxButton(parentPanel, wxID_EXIT, wxT("Safe Exit"));

    Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CatheterWindow::OnSafeExit));

    wxBoxSizer* vbox_parent = new wxBoxSizer(wxVERTICAL);

    vbox_parent->Add(controlPanel, 1, wxEXPAND | wxALL, 5);
    vbox_parent->Add(editPanel, 1, wxEXPAND | wxALL, 5);
    vbox_parent->Add(statusPanel, 1, wxEXPAND | wxALL, 5);
    vbox_parent->Add(safeExitButton);

    parentPanel->SetSizer(vbox_parent);
    vbox_parent->SetSizeHints(parentPanel);
    vbox_parent->Fit(parentPanel);

    Centre();

    this->Fit();
}

//destructor
CatheterWindow::~CatheterWindow() {
    if (controlPanel)
        controlPanel->OnSafeExit();
    if (editPanel)
        editPanel->OnSafeExit();
    if (statusPanel)
        statusPanel->OnSafeExit();
    
    safeExitButton->Destroy();
    editPanel->Destroy();
    statusPanel->Destroy();
    controlPanel->Destroy();

    wxMessageBox("CatheterWindow destructor was called");
}

//copy constructor
CatheterWindow::CatheterWindow(const CatheterWindow& C) 
    : wxFrame(NULL, wxID_ANY, C.GetTitle(), wxDefaultPosition), 
      parentPanel(C.parentPanel) {
    
    if (!parentPanel) {
        wxMessageBox("Catheter Window could not be initialized: parentPanel is NULL.");
        exit(EXIT_FAILURE);
    }

    statusPanel = new StatusPanel(parentPanel);
    editPanel = new EditPanel(parentPanel);
    controlPanel = new ControlPanel(parentPanel);
    safeExitButton = new wxButton(parentPanel, wxID_EXIT, wxT("Safe Exit"));

    Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CatheterWindow::OnSafeExit));

    wxBoxSizer* vbox_parent = new wxBoxSizer(wxVERTICAL);

    vbox_parent->Add(controlPanel, 1, wxEXPAND | wxALL, 5);
    vbox_parent->Add(editPanel, 1, wxEXPAND | wxALL, 5);
    vbox_parent->Add(statusPanel, 1, wxEXPAND | wxALL, 5);
    vbox_parent->Add(safeExitButton);

    parentPanel->SetSizer(vbox_parent);
    vbox_parent->SetSizeHints(parentPanel);
    vbox_parent->Fit(parentPanel);

    Centre();

    SetSize(C.GetSize());
    
    statusPanel = C.statusPanel;
    
    editPanel = C.editPanel;
    
    controlPanel = C.controlPanel;
    
    safeExitButton->Reparent(parentPanel);
    safeExitButton->SetLabel((C.safeExitButton)->GetLabel());

    wxMessageBox("CatheterWindow copy constructor was called");
}

//assignment
CatheterWindow & CatheterWindow::operator=(const CatheterWindow& C) {
    if (this != &C) {
        delete parentPanel;
        delete editPanel;
        delete statusPanel;
        delete controlPanel;
        delete safeExitButton;

        SetSize(C.GetSize());

        parentPanel = C.parentPanel; //look up wx operator=

        statusPanel = C.statusPanel;

        editPanel = C.editPanel;

        controlPanel = C.controlPanel;

        safeExitButton->Reparent(parentPanel);
        safeExitButton->SetLabel((C.safeExitButton)->GetLabel());

        wxMessageBox("CatheterWindow operator= was called");
    }
    return *this;
}

void CatheterWindow::OnSafeExit(wxCommandEvent& WXUNUSED(event)) {
    if (controlPanel)
        controlPanel->OnSafeExit();
    if (editPanel)
        editPanel->OnSafeExit();
    if (statusPanel)
        statusPanel->OnSafeExit();
    Destroy();
}

