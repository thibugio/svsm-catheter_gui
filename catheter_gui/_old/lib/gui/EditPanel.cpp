#include "EditPanel.h"

#include <vector>

EditPanel::EditPanel(wxPanel* parent) 
    : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN),
      parent(parent),
      cmd_grid(new CatheterCmdGrid(this)) {

    createEditPanel();

    this->Fit();
}

EditPanel::~EditPanel() {
    OnSafeExit();
    delete cmd_grid;
}

EditPanel::EditPanel(const EditPanel& C) :
    wxPanel(C.parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN),
    parent(C.parent),
    cmd_grid(new CatheterCmdGrid(this)) {
    
    createEditPanel();

    SetSize(C.GetSize());

    cmd_grid->copyEditWindow(C.cmd_grid);
    setCmdCount((C.cmd_grid)->getCmdCount());
}

EditPanel & EditPanel::operator=(const EditPanel& C) {
    if (this != &C) {
        delete cmd_grid;
        delete parent;
        
        Reparent(C.parent);
        parent = C.parent;

        cmd_grid = new CatheterCmdGrid(C.parent);
        cmd_grid->copyEditWindow(C.cmd_grid);
        setCmdCount((C.cmd_grid)->getCmdCount());
    }
    return *this;
}

void EditPanel::copyEditWindow(EditPanel* p) {
    cmd_grid->copyEditWindow(p->cmd_grid);
}

void EditPanel::setCmdCount(int count) {
    cmd_grid->setCmdCount(count);
}

void EditPanel::OnSafeExit() {
    cmd_grid->OnSafeExit();
}

void EditPanel::OnSafeInit() {
    cmd_grid->OnSafeInit();
}

void EditPanel::resetEditPanel() {
    cmd_grid->resetEditWindow();
}

void EditPanel::setCommandChannel(int cmd_num, int chan) {
    if (!checkCmdRange(cmd_num)) {
        return;
    }
    cmd_grid->setCommandChannel(cmd_num, chan);
}

void EditPanel::setCommandCurrent(int cmd_num, double current) {
    if (!checkCmdRange(cmd_num)) {
        return;
    }
    cmd_grid->setCommandCurrent(cmd_num, current);
}

void EditPanel::setCommandDirection(int cmd_num, dir_t dir) {
    if (!checkCmdRange(cmd_num)) {
        return;
    }
    cmd_grid->setCommandDirection(cmd_num, dir);
}

void EditPanel::setCommandDelay(int cmd_num, int ms) {
    if (!checkCmdRange(cmd_num)) {
        return;
    }
    cmd_grid->setCommandDelay(cmd_num, ms);
}

unsigned int EditPanel::getCommandChannel(int cmd_num) {
    checkCmdRange(cmd_num); //for warning msg
    return cmd_grid->getCommandChannel(cmd_num);
}

double EditPanel::getCommandCurrentMA(int cmd_num) {
    checkCmdRange(cmd_num); //for warning msg
    return cmd_grid->getCommandCurrentMA(cmd_num);
}

dir_t EditPanel::getCommandDirection(int cmd_num) {
    checkCmdRange(cmd_num); //for warning msg
    return cmd_grid->getCommandDirection(cmd_num);
}

unsigned int EditPanel::getCommandDelay(int cmd_num) {
    checkCmdRange(cmd_num); //for warning msg
    return cmd_grid->getCommandDelay(cmd_num);
}

void EditPanel::setCommand(int cmd_num, CatheterChannelCmd cmd) {
    if (!checkCmdRange(cmd_num)) {
        return;
    }
    setCommandChannel(cmd_num, cmd.channel);
    setCommandCurrent(cmd_num, cmd.currentMA);
    setCommandDelay(cmd_num, cmd.delayMS);
}

void EditPanel::appendCommand(CatheterChannelCmd cmd) {
    appendCommandRow();
    setCommand(cmd_grid->getCmdCount() - 1, cmd);
}

void EditPanel::appendCommands(std::vector<CatheterChannelCmd> cmds) {
    for (int i = 0; i < cmds.size(); i++)
        appendCommand(cmds[i]);
}

std::vector<CatheterChannelCmd> EditPanel::getCommands() {
    std::vector<CatheterChannelCmd> cmdVect;
    for (int i = 0; i < cmd_grid->getCmdCount(); i++) {
        if (cmd_grid->isRowComplete(i)) {
            cmdVect.push_back(getCommand(i));
        }
    }
    return cmdVect;
}

CatheterChannelCmd EditPanel::getCommand(int cmd_num) {
    CatheterChannelCmd cmd;
    if (!checkCmdRange(cmd_num)) {
        wxLogError(wxT("Error retrieving command; requested command number too high"));
    } else if (!cmd_grid->isRowComplete(cmd_num)) {
        wxLogError(wxT("Error retrieving command; requested command number is incomplete"));
    } else {
        cmd.channel = getCommandChannel(cmd_num);
        cmd.currentMA = getCommandCurrentMA(cmd_num);
        cmd.delayMS = getCommandDelay(cmd_num);
        cmd.poll = false;
        //debug
        wxMessageBox(wxString::Format("chan: %d  MA: %3.3f  MS: %d", cmd.channel, cmd.currentMA, cmd.delayMS), "");        
    }
    return cmd;
}

int EditPanel::getCmdCount() {
    return cmd_grid->getCmdCount();
}

/////////////////////
// private methods //
/////////////////////

void EditPanel::OnGridCellChanging(wxGridEvent& event) {    
    int row = event.GetRow();    
    int col = event.GetCol();
    if (col == CURRENT_COL) {
        // make sure sign of current agrees with direction and check bounds    
        double current;
        event.GetString().ToDouble(&current);
        if (current > 1000 || current < -1000)
            event.Veto();
        else
            setCommandCurrent(row, current);      
        event.Skip();
    } else if (col == DELAY_COL) {
        if (wxAtoi(event.GetString()) < 0)
            event.Veto();
    }
    
    // if row is complete, increment cmd_count and add another ReadOnly row
    if (cmd_grid->isRowComplete(row))
        appendCommandRow();
}

void EditPanel::OnGridCellLeftClick(wxGridEvent& event) {
    int row = event.GetRow();
    if (!checkCmdRange(row))
        event.Veto();
}

void EditPanel::appendCommandRow() {
    cmd_grid->incrementCmdCount();
    if (cmd_grid->getCmdCount() >= cmd_grid->GetNumberRows())
        cmd_grid->AppendRows(1);
    setRowReadOnly(cmd_grid->getCmdCount() - 1, false);
}

void EditPanel::setRowReadOnly(int row, bool ReadOnly) {
    if (!checkCmdRange(row)) {
        return;
    }
    cmd_grid->SetReadOnly(row, CHANNEL_COL, ReadOnly);
    cmd_grid->SetReadOnly(row, CURRENT_COL, ReadOnly);
    cmd_grid->SetReadOnly(row, DIR_COL, ReadOnly);
    cmd_grid->SetReadOnly(row, DELAY_COL, ReadOnly);
}

bool EditPanel::checkCmdRange(int cmd_num) {
    bool valid = (cmd_num < cmd_grid->getCmdCount());

    if (!valid)
        wxMessageBox("attempted to access row out of range");

    return valid;
}

void EditPanel::createEditPanel() {
    cmd_grid->Connect(wxEVT_GRID_CELL_CHANGING, wxGridEventHandler(EditPanel::OnGridCellChanging));
    cmd_grid->Connect(wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler(EditPanel::OnGridCellLeftClick));
    cmd_grid->Connect(wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler(EditPanel::OnGridCellLeftClick));
}