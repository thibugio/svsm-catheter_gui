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

#define CHANNEL_COL 0
#define CURRENT_COL 1
#define DIR_COL 2
#define DELAY_COL 3

#define NFIELDS 4

class CatheterCmdGrid : public wxGrid {
    public:
    CatheterCmdGrid(wxPanel* parent);

    ~CatheterCmdGrid();
    CatheterCmdGrid(const CatheterCmdGrid& C);
    CatheterCmdGrid & operator=(const CatheterCmdGrid& C);

    void copyEditWindow(CatheterCmdGrid* p);
    void copyEditWindow(const CatheterCmdGrid* p);
    void resetEditWindow();

    void setCommandChannel(int cmd_num, int chan);
    void setCommandCurrent(int cmd_num, double current);
    void setCommandDirection(int cmd_num, dir_t dir);
    void setCommandDelay(int cmd_num, int ms);

    unsigned int getCommandChannel(int cmd_num);
    double getCommandCurrentMA(int cmd_num);
    dir_t getCommandDirection(int cmd_num);
    unsigned int getCommandDelay(int cmd_num);

    int getCmdCount();
    void setCmdCount(int count);
    void incrementCmdCount();
    void decrementCmdCount();

    bool isRowComplete(int row);

    void OnSafeExit();
    void OnSafeInit();

    private:
    void createCatheterCmdGrid();
    void setNumberRows(int nrows);
    void appendCommandRow();
    void setRowReadOnly(int row, bool readOnly);
    double res2ma(int cmd_num);
    unsigned int ma2res(int cmd_num);

    wxPanel* parent;
    int cmd_count;
    wxString dir_choices[2];
};

#endif