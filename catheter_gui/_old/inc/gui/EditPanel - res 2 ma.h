#pragma once
#ifndef EDIT_PANEL_H
#define EDIT_PANEL_H

#include "common_utils.h"
#include "catheter_gui_common.h"

#include <wx/wxprec.h>
#include <wx/panel.h>
#include <wx/grid.h>
#ifndef WX_PRECOMP
    // Include your minimal set of headers here, or wx.h
    #include <wx/wx.h>    
#endif
#include <boost/shared_ptr.hpp>

#define SUPPORT_MA_2_RES false

class EditPanel : public wxPanel {
    public:
    EditPanel(wxPanel* parent);
    void resetEditPanel();
    // modify a field in a channel command
    void setCommandChannel(int cmd_num, int chan);
    void setCommandCurrent(int cmd_num, double current);
    void setCommandDirection(int cmd_num, dir_t dir);
    void setCommandDelay(int cmd_num, int ms);
    // get a field in a channel command
    unsigned int getCommandChannel(int cmd_num);
    double getCommandCurrentMA(int cmd_num);
    dir_t getCommandDirection(int cmd_num);
    unsigned int getCommandDelay(int cmd_num);
    // modify or create a full channel command
    void setCommand(int cmd_num, CatheterChannelCmd cmd);
    void addCommand(CatheterChannelCmd cmd);
    // retrieve all channel commands from the edit fields
    std::vector<CatheterChannelCmd> getCommands();
    int getCmdCount();
    bool isFormatMA();
    void OnSafeExit();
    void OnSafeInit();

    boost::shared_ptr<wxPanel> parent;
    boost::shared_ptr<wxGrid> cmd_grid;

    wxString dir_choices[2];

    private:
    // grid event handlers
    void OnGridCellChanging(wxGridEvent& event);
    void OnGridCellLeftClick(wxGridEvent& event);
    void OnGridTabbing(wxGridEvent& event);
    void OnGridLabelLeftClick(wxGridEvent& event);
    // helper methods
    CatheterChannelCmd getCommand(int cmd_num);
    bool isRowComplete(int row);
    void addCommandRow();
    void setRowReadOnly(int row, bool readOnly);
    void setCurrentColFormat();
    void setCurrentColLabel();
    void toggleCurrentCol();
    double res2ma(int cmd_num);
    unsigned int ma2res(int cmd_num);    

    int cmd_count;
    bool formatMA;
};

#endif