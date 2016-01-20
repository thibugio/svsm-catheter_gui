#include "CatheterWindow.h"

CatheterWindow::CatheterWindow(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition),
    parentPanel(new wxPanel(this, wxID_ANY)) {
    
    createCatheterWindow();

    // set focus on the select playfile button
    controlPanel->selectButton->SetFocus();

    // fit the Window size around its child components
    this->Fit();
}

//destructor
CatheterWindow::~CatheterWindow() {
    OnSafeExit(wxCommandEvent());
    delete safeExitButton;
    delete editPanel;
    delete statusPanel;
    delete controlPanel;
}

//copy constructor
CatheterWindow::CatheterWindow(const CatheterWindow& C) 
    : wxFrame(NULL, wxID_ANY, C.GetTitle(), wxDefaultPosition), 
      parentPanel(C.parentPanel) {
    
    createCatheterWindow();

    this->SetSize(C.GetSize());
    
    statusPanel->SetText((C.statusPanel)->Read());
    
    editPanel->copyEditWindow(C.editPanel);
    editPanel->setCmdCount((C.editPanel)->getCmdCount());
    
    controlPanel->playfilePath = (C.controlPanel)->playfilePath;
    controlPanel->portName = (C.controlPanel)->portName;
    controlPanel->playfileSaved = (C.controlPanel)->playfileSaved;
    controlPanel->serialConnected = (C.controlPanel)->serialConnected;
    
    safeExitButton->SetLabel((C.safeExitButton)->GetLabel());
}

//assignment
CatheterWindow & CatheterWindow::operator=(const CatheterWindow& C) {
    if (this != &C) {
        delete parentPanel;
        delete editPanel;
        delete statusPanel;
        delete controlPanel;
        delete safeExitButton;

        this->Reparent(C.parentPanel);
        this->parentPanel = C.parentPanel; //look up wx operator=

        this->editPanel = new EditPanel(parentPanel);
        this->statusPanel = new StatusPanel(parentPanel);
        this->editPanel = new EditPanel(parentPanel);
        this->controlPanel = new ControlPanel(parentPanel);
        this->safeExitButton = new wxButton(parentPanel, wxID_ANY);

        statusPanel->SetText((C.statusPanel)->Read());

        editPanel->copyEditWindow((C.editPanel));
        editPanel->setCmdCount((C.editPanel)->getCmdCount());

        controlPanel->playfilePath = (C.controlPanel)->playfilePath;
        controlPanel->portName = (C.controlPanel)->portName;
        controlPanel->playfileSaved = (C.controlPanel)->playfileSaved;
        controlPanel->serialConnected = (C.controlPanel)->serialConnected;

        safeExitButton->SetLabel((C.safeExitButton)->GetLabel());
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
    // close gui window
    this->Close(true);
}

void CatheterWindow::createCatheterWindow() {
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
}