#include "EditPanel.h"
#include "catheter_gui_common.h"
#include "common_utils.h"

#include <vector>

#define CHANNEL_COL 0
#define CURRENT_COL 1
#define DIR_COL 2
#define DELAY_COL 3

#define NFIELDS 4

EditPanel::EditPanel(wxPanel* parent) 
    : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN),
      parent(parent),
      cmd_grid(new wxGrid(this, wxID_ANY)) {
    
    cmd_count = 0;
    formatMA = true;
    dir_choices[DIR_POS] = wxT("pos");
    dir_choices[DIR_NEG] = wxT("neg");

    cmd_grid->CreateGrid(NCHANNELS, NFIELDS);

    cmd_grid->SetColLabelValue(CHANNEL_COL, wxT("Channel"));
    setCurrentColLabel();
    cmd_grid->SetColLabelValue(DIR_COL, wxT("Direction"));
    cmd_grid->SetColLabelValue(DELAY_COL, wxT("Delay (ms)"));
    cmd_grid->HideRowLabels();

    // set column attributes
    cmd_grid->GetTable()->SetAttrProvider(new wxGridCellAttrProvider());
    for (int i = 0; i < NFIELDS; i++)
        cmd_grid->GetTable()->GetAttrProvider()->SetColAttr(new wxGridCellAttr(), i);

    cmd_grid->SetColFormatNumber(CHANNEL_COL); //channel address
    setCurrentColFormat(); //current (mA--(float) or res--(int); default to mA)
    cmd_grid->SetColFormatNumber(DELAY_COL); //delay

    wxGridCellAttr* dirAttr = cmd_grid->GetTable()->GetAttrProvider()->GetAttr(0, DIR_COL, wxGridCellAttr::Col);
    wxGridCellAttr* channelAttr = cmd_grid->GetTable()->GetAttrProvider()->GetAttr(0, CHANNEL_COL, wxGridCellAttr::Col);
    if (channelAttr == NULL || dirAttr == NULL) {
        printf("attribute is NULL\n");
        exit(EXIT_FAILURE);
    }
    dirAttr->SetEditor(new wxGridCellChoiceEditor(2, dir_choices));
    channelAttr->SetEditor(new wxGridCellNumberEditor(1, NCHANNELS));
    cmd_grid->SetColAttr(DIR_COL, dirAttr);
    cmd_grid->SetColAttr(CHANNEL_COL, channelAttr);
    
    for (int i = 1; i < cmd_grid->GetNumberRows(); i++) {
        setRowReadOnly(i, true);
    }
    addCommandRow();
    
    cmd_grid->Fit();
    this->Fit();

    cmd_grid->Connect(wxEVT_GRID_CELL_CHANGING, wxGridEventHandler(EditPanel::OnGridCellChanging));
    cmd_grid->Connect(wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler(EditPanel::OnGridCellLeftClick));
    cmd_grid->Connect(wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler(EditPanel::OnGridCellLeftClick));
    //cmd_grid->Connect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(EditPanel::OnGridCellChanging));
    // not implemented
    //cmd_grid->Connect(wxEVT_GRID_TABBING, wxGridEventHandler(EditPanel::OnGridTabbing));
    // why doesnt this work?
    //cmd_grid->Connect(wxEVT_GRID_LABEL_RIGHT_CLICK, wxGridEventHandler(EditPanel::OnGridLabelLeftClick));
}

void EditPanel::SetParent(wxPanel* p) {
    this->Reparent(p);
    boost::shared_ptr<wxPanel> temp(p);
    this->parent = temp;

    cmd_grid->Reparent(this);
}

void EditPanel::OnSafeExit() {
    //foo
}

void EditPanel::OnSafeInit() {
    //foo
}

void EditPanel::resetEditPanel() {
    // leave 'NCHANNELS_DEFAULT' rows in the table
    int excess_rows = cmd_grid->GetNumberRows() - NCHANNELS;
    if (excess_rows > 0)
        cmd_grid->DeleteRows(NCHANNELS + 1, excess_rows);
    // clear data from table
    cmd_grid->ClearGrid();
}

void EditPanel::setCommandChannel(int cmd_num, int chan) {
    if (cmd_num >= cmd_count)
        return;
    cmd_grid->SetCellValue(cmd_num, CHANNEL_COL, wxString::Format("%d", chan));
}

void EditPanel::setCommandCurrent(int cmd_num, double current) {
    if (cmd_num >= cmd_count)
        return;
    if (isFormatMA()) {
        cmd_grid->SetCellValue(cmd_num, CURRENT_COL, wxString::Format("%3.3f", current));
        if (current < 0)
            setCommandDirection(cmd_num, DIR_NEG);
        else
            setCommandDirection(cmd_num, DIR_POS);
    } else {
        cmd_grid->SetCellValue(cmd_num, CURRENT_COL, wxString::Format("%d", current));
    }    
}

void EditPanel::setCommandDirection(int cmd_num, dir_t dir) {
    if (cmd_num >= cmd_count)
        return;
    cmd_grid->SetCellValue(cmd_num, DIR_COL, dir_choices[dir]);
}

void EditPanel::setCommandDelay(int cmd_num, int ms) {
    if (cmd_num >= cmd_count)
        return;
    cmd_grid->SetCellValue(cmd_num, DELAY_COL, wxString::Format("%d", ms));
}

void EditPanel::setCommand(int cmd_num, CatheterChannelCmd cmd) {
    if (cmd_num >= cmd_count)
        return;
    setCommandChannel(cmd_num, cmd.channel);
    setCommandCurrent(cmd_num, cmd.currentMA);
    setCommandDelay(cmd_num, cmd.delayMS);
}

void EditPanel::addCommand(CatheterChannelCmd cmd) {
    addCommandRow();
    setCommand(cmd_count - 1, cmd);
}

unsigned int EditPanel::getCommandChannel(int cmd_num) {
    return wxAtoi(cmd_grid->GetCellValue(cmd_num, CHANNEL_COL));
}

double EditPanel::getCommandCurrentMA(int cmd_num) {
    if (isFormatMA())
        return wxAtof(cmd_grid->GetCellValue(cmd_num, CURRENT_COL));
    else {
        return res2ma(cmd_num);
    }
}

dir_t EditPanel::getCommandDirection(int cmd_num) {
    return (cmd_grid->GetCellValue(cmd_num, DIR_COL).Cmp(dir_choices[DIR_POS]) ? DIR_NEG : DIR_POS);
}

unsigned int EditPanel::getCommandDelay(int cmd_num) {
    return wxAtoi(cmd_grid->GetCellValue(cmd_num, DELAY_COL));
}

std::vector<CatheterChannelCmd> EditPanel::getCommands() {
    std::vector<CatheterChannelCmd> cmdVect;
    for (int i = 0; i < cmd_count; i++) {
        if (isRowComplete(i)) {
            cmdVect.push_back(getCommand(i));
        }
    }
    return cmdVect;
}

CatheterChannelCmd EditPanel::getCommand(int cmd_num) {
    CatheterChannelCmd cmd;
    if (cmd_num >= cmd_count) {
        wxLogError(wxT("Error retrieving command; requested command number too high"));
    } else if (!isRowComplete(cmd_num)) {
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
    return cmd_count;
}

bool EditPanel::isFormatMA() {
    return formatMA;
}

/////////////////////
// private methods //
/////////////////////

void EditPanel::OnGridCellChanging(wxGridEvent& event) {    
    int row = event.GetRow();    
    int col = event.GetCol();
    if (col == CURRENT_COL) {
        // make sure sign of current agrees with direction and check bounds    
        if (isFormatMA()) {
            double current;
            event.GetString().ToDouble(&current);
            if (current > 1000 || current < -1000)
                event.Veto();
            else
                setCommandCurrent(row, current);
        } else {
            int res = wxAtoi(event.GetString());
            if (res < 0 || res >= DAC_RES)
                event.Veto();
            else
                setCommandCurrent(row, res);
        }       
        event.Skip();
    } else if (col == DELAY_COL) {
        if (wxAtoi(event.GetString()) < 0)
            event.Veto();
    }
    
    // if row is complete, increment cmd_count and add another ReadOnly row
    if (isRowComplete(row))
        addCommandRow();
}

void EditPanel::OnGridCellLeftClick(wxGridEvent& event) {
    int row = event.GetRow();
    if (row > cmd_count)
        event.Veto();
}

void EditPanel::OnGridTabbing(wxGridEvent& event) {
    //make the focus stay in the edit panel instead of current wierd behaviour
    wxMessageBox("Grid Tab Event");
}

void EditPanel::OnGridLabelLeftClick(wxGridEvent& event) {
    wxMessageBox("Grid Label Left Click Event");
    if (event.GetCol() == CURRENT_COL) {
        formatMA = !formatMA;
        toggleCurrentCol();
    } else {
        event.Veto();
    }
}

void EditPanel::addCommandRow() {
    cmd_count++;
    if (cmd_count >= cmd_grid->GetNumberRows())
        cmd_grid->AppendRows(1);
    setRowReadOnly(cmd_count - 1, false);
}

void EditPanel::setRowReadOnly(int row, bool ReadOnly) {
    if (row >= cmd_count)
        return;
    cmd_grid->SetReadOnly(row, CHANNEL_COL, ReadOnly);
    cmd_grid->SetReadOnly(row, CURRENT_COL, ReadOnly);
    cmd_grid->SetReadOnly(row, DIR_COL, ReadOnly);
    cmd_grid->SetReadOnly(row, DELAY_COL, ReadOnly);
}

bool EditPanel::isRowComplete(int row) {
    if (row >= cmd_count)
        return false;
    bool row_complete = true;
    for (int i = 0; i < NFIELDS; i++) {
        if (cmd_grid->GetTable()->IsEmptyCell(row, i)) {
            row_complete = false;
            break;
        }
    }
    return row_complete;
}

void EditPanel::setCurrentColFormat() {
    if (isFormatMA()) {
        cmd_grid->SetColFormatFloat(CURRENT_COL, 6, 3);
    } else {
        cmd_grid->SetColFormatNumber(CURRENT_COL);
    }
    
}

void EditPanel::setCurrentColLabel() {
    if (isFormatMA()) {
        cmd_grid->SetColLabelValue(CURRENT_COL, wxT("Current (mA)"));
    } else {
        cmd_grid->SetColLabelValue(CURRENT_COL, wxT("Current (DAC bits)"));
    }
}

void EditPanel::toggleCurrentCol() {    
    for (int i = 0; i < cmd_count; i++) {
        if (cmd_grid->GetCellValue(i, CURRENT_COL).IsEmpty())
            continue;
        if (isFormatMA()) {
            double ma = res2ma(i);
            cmd_grid->SetCellValue(i, CURRENT_COL, wxString::Format("3.3f", ma));
        } else {
            unsigned int res = ma2res(i);
            cmd_grid->SetCellValue(i, CURRENT_COL, wxString::Format("%d", res));
        }
    }
    setCurrentColLabel();
    setCurrentColFormat();
}

double EditPanel::res2ma(int cmd_num) {
    unsigned int res = wxAtoi(cmd_grid->GetCellValue(cmd_num, CURRENT_COL));
    dir_t dir = getCommandDirection(cmd_num);
    unsigned int chan = getCommandChannel(cmd_num);
    return convert_current_by_channel(res, dir, chan);
}

unsigned int EditPanel::ma2res(int cmd_num) {
    return convert_current_by_channel(getCommandCurrentMA(cmd_num), getCommandChannel(cmd_num));
}