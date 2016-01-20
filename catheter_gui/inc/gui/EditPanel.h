#pragma once
#ifndef EDIT_PANEL_H
#define EDIT_PANEL_H

#include "common_utils.h"
#include "catheter_gui_common.h"
#include "CatheterCmdGrid.h"

#include <wx/wxprec.h>
#include <wx/panel.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>    
#endif
#include <boost/shared_ptr.hpp>

class EditPanel : public wxPanel {
    public:
    EditPanel(wxPanel* parent);

    ~EditPanel();
    EditPanel(const EditPanel& C);
    EditPanel & operator=(const EditPanel& C);

    void setCommandChannel(int cmd_num, int chan);
    void setCommandCurrent(int cmd_num, double current);
    void setCommandDirection(int cmd_num, dir_t dir);
    void setCommandDelay(int cmd_num, int ms);

    unsigned int getCommandChannel(int cmd_num);
    double getCommandCurrentMA(int cmd_num);
    dir_t getCommandDirection(int cmd_num);
    unsigned int getCommandDelay(int cmd_num);

    void setCommand(int cmd_num, CatheterChannelCmd cmd);

    void appendCommand(CatheterChannelCmd cmd);
    void appendCommands(std::vector<CatheterChannelCmd> cmds);

    CatheterChannelCmd getCommand(int cmd_num);
    std::vector<CatheterChannelCmd> getCommands();

    void copyEditWindow(EditPanel* p);
    void resetEditWindow();

    void setCmdCount(int count);
    int getCmdCount();

    void OnSafeExit();
    void OnSafeInit();

    CatheterCmdGrid* cmd_grid;


    private:
    void OnGridCellChanging(wxGridEvent& event);
    void OnGridCellLeftClick(wxGridEvent& event);

    bool checkCmdRange(int cmd_num);

    void appendCommandRow();

    void setRowReadOnly(int row, bool readOnly);   
};

#endif