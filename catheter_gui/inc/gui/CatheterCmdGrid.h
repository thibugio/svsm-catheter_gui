#pragma once

#ifndef CATHETER_CMD_GRID_H
#define CATHETER_CMD_GRID_H

#include <wx/wxprec.h>
#include <wx/grid.h>
#include <wx/panel.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>    
#endif

#include "catheter_gui_common.h"
#include "common_utils.h"

class CatheterCmdGrid : public wxGrid {
    public:
    CatheterCmdGrid(wxPanel* parent);

    ~CatheterCmdGrid();
    CatheterCmdGrid(const CatheterCmdGrid& C);
    CatheterCmdGrid & operator=(const CatheterCmdGrid& C);

    void copyEditWindow(CatheterCmdGrid* p);
    void copyEditWindow(const CatheterCmdGrid* p);
    void resetEditWindow();
    void setRowReadOnly(int row, bool readOnly);

    void setCommandChannel(int cmd_num, int chan);
    void setCommandCurrent(int cmd_num, double current);
    void setCommandDirection(int cmd_num, dir_t dir);
    void setCommandDelay(int cmd_num, int ms);

    unsigned int getCommandChannel(int cmd_num);
    double getCommandCurrentMA(int cmd_num);
    dir_t getCommandDirection(int cmd_num);
    unsigned int getCommandDelay(int cmd_num);

    unsigned int getCommandChannel(int cmd_num) const;
    double getCommandCurrentMA(int cmd_num) const;
    dir_t getCommandDirection(int cmd_num) const;
    unsigned int getCommandDelay(int cmd_num) const;

    std::vector<CatheterChannelCmd> getCommands();
    CatheterChannelCmd getCommand(int cmd_num);

    int getCmdCount();
    void setCmdCount(int count);
    void incrementCmdCount();
    void decrementCmdCount();

    void OnSafeExit() {}
    void OnSafeInit() {}


    private:
    bool isRowComplete(int row);
    void setNumberRows(int nrows);
    void appendCommandRow();
    double res2ma(int cmd_num);
    unsigned int ma2res(int cmd_num);

    int cmd_count;
    wxString dir_choices[2];
};

#endif