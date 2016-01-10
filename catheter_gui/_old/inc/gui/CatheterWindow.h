#ifndef CATHETER_WINDOW_H
#define CATHETER_WINDOW_H

#pragma once

#include "EditPanel.h"
#include "StatusPanel.h"
#include "ControlPanel.h"

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#include <wx/wxprec.h>
#include <wx/panel.h>
#ifndef WX_PRECOMP
    // Include your minimal set of headers here, or wx.h
    #include <wx/wx.h>
#endif
#include <boost/shared_ptr.hpp>

class CatheterWindow : public wxFrame {
    public:
    //constructor
	CatheterWindow(const wxString& title);
    //destructor
    ~CatheterWindow();
    //copy constructor
    CatheterWindow(const CatheterWindow& C);
    //assignment
    CatheterWindow & operator=(const CatheterWindow& C);

    StatusPanel* getStatusPanel() { return statusPanel; }
    EditPanel* getEditPanel() { return editPanel; }
    ControlPanel* getControlPanel() { return controlPanel; }
    wxPanel* getParentPanel() { return parentPanel; }

    StatusPanel* getStatusPanel(const CatheterWindow& c) { return statusPanel; }
    EditPanel* getEditPanel(const CatheterWindow& c) { return editPanel; }
    ControlPanel* getControlPanel(const CatheterWindow& c) { return controlPanel; }
    wxButton* getSafeExitButton(const CatheterWindow& c) { return safeExitButton; }

    private:
    void OnSafeExit(wxCommandEvent& event);
    void createCatheterWindow();

    // panels contained within the main window
    wxPanel* parentPanel;
    StatusPanel* statusPanel;
    EditPanel* editPanel;
    ControlPanel* controlPanel;

    // main window buttons
    wxButton* safeExitButton;    
};

#endif // CatheterWindow_H