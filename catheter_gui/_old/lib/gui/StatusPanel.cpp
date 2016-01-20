#include "StatusPanel.h"

StatusPanel::StatusPanel(wxPanel* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN),
    parent(parent),
    text(new wxStaticText(this, -1, wxEmptyString)) {}

StatusPanel::~StatusPanel() {
    OnSafeExit();
    delete text;
}

StatusPanel::StatusPanel(const StatusPanel& C) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN),
    parent(C.parent),
    text(new wxStaticText(this, -1, (C.text)->GetLabel())) {}

StatusPanel & StatusPanel::operator=(const StatusPanel& C) {
    if (this != &C) {
        delete text;
        delete parent;

        Reparent(C.parent);

        parent = C.parent; // is this fine since wxDECLARE_NO_COPY_CLASS() macro is used for wxPanelBase?
        text = new wxStaticText(this, -1, (C.text)->GetLabel());
    }
    return *this;
}

void StatusPanel::SetText(const wxString& msg) {
    text->SetLabel(msg);
}

wxString StatusPanel::Read() {
    return text->GetLabel();
}

void StatusPanel::OnSafeExit() {
    SetText(wxT("Exiting Catheter Gui Application."));
}

void StatusPanel::OnSafeInit() {
    SetText(wxT("Welcome to Catheter Gui Application."));
}