#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#pragma once
#include "EditPanel.h"
#include "StatusPanel.h"
#include <wx/wx.h>
#include <wx/panel.h>

class ControlPanel : public wxPanel {
    public:
    ControlPanel(wxPanel& parent, EditPanel& editPanel, StatusPanel& statusPanel);

    // action button event handler methods
    void OnConnect(wxCommandEvent& event);
    void OnSelectPlayfile(wxCommandEvent& event);
    void OnNewPlayfile(wxCommandEvent& event);
    void OnSavePlayfile(wxCommandEvent& event);
    void OnSendCommands(wxCommandEvent& event);
    void OnSendReset(wxCommandEvent& event);
    void OnSafeExit();

    // buttons contained in this panel
    wxButton* connectButton;
    wxButton* selectButton;
    wxButton* newButton;
    wxButton* saveButton;
    wxButton* sendButton;
    wxButton* resetButton;
    
    // the parent of this panel
    wxPanel& parent;
    // references to siblings of this panel
    EditPanel& editPanel;
    StatusPanel& statusPanel;
    wxString playfilePath;
    wxString portName;
    bool playfileSaved;
    bool serialConnected;

    private:
    void setStatusPanelText(const wxString& label);
    void loadPlayfile(const wxString& path);
    void unloadPlayfile(const wxString& path);
    void sendResetCommand();
    wxString openFileDialog(const wxString& prompt, const wxString& wildcard);
    wxString saveFileDialog(const wxString& prompt, const wxString& wildcard);
};

enum {
    ID_CONNECT_BUTTON = 500,
    ID_SELECT_BUTTON,
    ID_NEW_BUTTON,
    ID_SAVE_BUTTON,
    ID_SEND_BUTTON,
    ID_RESET_BUTTON
};

#endif