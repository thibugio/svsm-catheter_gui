#pragma once

#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include "catheter_gui_common.h"
#include "EditPanel.h"
#include "StatusPanel.h"

#include <wx/panel.h>
#ifndef WX_PRECOMP
    // Include your minimal set of headers here, or wx.h
    #include <wx/wx.h>    
#endif
#include <boost/shared_ptr.hpp>

class ControlPanel : public wxPanel {
    public:
    ControlPanel(wxPanel* parent);
    ~ControlPanel();
    ControlPanel(const ControlPanel& C);
    ControlPanel & operator=(const ControlPanel& C);

    // action button event handler methods
    void OnConnect(wxCommandEvent& event);
    void OnSelectPlayfile(wxCommandEvent& event);
    void OnNewPlayfile(wxCommandEvent& event);
    void OnSavePlayfile(wxCommandEvent& event);
    void OnSendCommands(wxCommandEvent& event);
    void OnSendReset(wxCommandEvent& event);
    void OnSafeExit();
    void OnSafeInit();

    private:
    void createControlPanel();
    StatusPanel* getStatusPanel();
    EditPanel* getEditPanel();
    void setStatusPanelText(const wxString& label);
    void loadPlayfile(const wxString& path);
    void unloadPlayfile(const wxString& path);
    void sendResetCommand();
    wxString openFileDialog(const wxString& prompt, const wxString& wildcard);
    wxString saveFileDialog(const wxString& prompt, const wxString& wildcard);

    wxPanel* parent;
    wxButton* connectButton;
    wxButton* selectButton;
    wxButton* newButton;
    wxButton* saveButton;
    wxButton* sendButton;
    wxButton* resetButton;

    wxString playfilePath;
    wxString portName;
    bool playfileSaved;
    bool serialConnected;    
};

enum {
    ID_CONNECT_BUTTON = 1024,
    ID_SELECT_BUTTON,
    ID_NEW_BUTTON,
    ID_SAVE_BUTTON,
    ID_SEND_BUTTON,
    ID_RESET_BUTTON
};

#endif