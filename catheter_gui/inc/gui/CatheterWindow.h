#pragma once

#ifndef CATHETER_WINDOW_H
#define CATHETER_WINDOW_H

#include "catheter_gui_common.h"
#include "EditPanel.h"
#include "StatusPanel.h"
#include "ControlPanel.h"

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

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

    void OnSafeExit(wxCommandEvent& event);

    wxPanel* parentPanel;
    StatusPanel* statusPanel;
    EditPanel* editPanel;
    ControlPanel* controlPanel;
    wxButton* safeExitButton;
};

#endif // CatheterWindow_H