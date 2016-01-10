#pragma once
#ifndef STATUS_PANEL_H
#define STATUS_PANEL_H

#include <wx/wxprec.h>
#include <wx/panel.h>
#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif
#include <boost/shared_ptr.hpp>

class StatusPanel : public wxPanel {
    public:
    StatusPanel(wxPanel* parent);
    ~StatusPanel();
    StatusPanel(const StatusPanel& C);
    StatusPanel & operator=(const StatusPanel& C);

    void SetText(const wxString& msg);
    wxString Read();

    void OnSafeExit();
    void OnSafeInit();

    // the parent of this panel
    private:
    wxPanel* parent;
    wxStaticText* text;
};

#endif