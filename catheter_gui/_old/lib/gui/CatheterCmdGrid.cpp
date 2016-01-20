#include "CatheterCmdGrid.h"

CatheterCmdGrid::CatheterCmdGrid(wxPanel* parent) :
    wxGrid(parent, wxID_ANY),
    cmd_count(0),
    dir_choices{ wxT("neg"), wxT("pos") } {

    createCatheterCmdGrid();

    Fit();
}

CatheterCmdGrid::~CatheterCmdGrid() {
    OnSafeExit();
}

CatheterCmdGrid::CatheterCmdGrid(const CatheterCmdGrid& C) :
    wxGrid(C.parent, wxID_ANY),
    cmd_count(C.cmd_count),
    dir_choices{ wxT("neg"), wxT("pos") } {

    createCatheterCmdGrid();    

    copyEditWindow(&C);
    SetSize(C.GetSize());
}

CatheterCmdGrid & CatheterCmdGrid::operator=(const CatheterCmdGrid& C) {
    if (this != &C) {
        delete parent;

        Reparent(C.parent);
        parent = C.parent;
        
        cmd_count = C.cmd_count;    

        copyEditWindow(&C);
        SetSize(C.GetSize());
    }
    return *this;
}

void CatheterCmdGrid::copyEditWindow(CatheterCmdGrid* grid) {
    ClearGrid();
    setNumberRows(grid->GetNumberRows());

    for (int i = 0; i < grid->GetNumberRows(); i++) {
        setCommandChannel(i, grid->getCommandChannel(i));
        setCommandCurrent(i, grid->getCommandCurrentMA(i));
        setCommandDelay(i, grid->getCommandDelay(i));
    }
}

void CatheterCmdGrid::copyEditWindow(const CatheterCmdGrid* grid) {
    ClearGrid();
    setNumberRows(grid->GetNumberRows());

    for (int i = 0; i < grid->GetNumberRows(); i++) {
        setCommandChannel(i, grid->getCommandChannel(i));
        setCommandCurrent(i, grid->getCommandCurrentMA(i));
        setCommandDelay(i, grid->getCommandDelay(i));
    }
}

void CatheterCmdGrid::incrementCmdCount() { cmd_count++; }
void CatheterCmdGrid::decrementCmdCount() { cmd_count--; }
void CatheterCmdGrid::setCmdCount(int count) {
    cmd_count = count;
}
int CatheterCmdGrid::getCmdCount() {
    return cmd_count;
}

void CatheterCmdGrid::setCommandChannel(int cmd_num, int chan) {
    SetCellValue(cmd_num, CHANNEL_COL, wxString::Format("%d", chan));
}

void CatheterCmdGrid::setCommandCurrent(int cmd_num, double current) {
    SetCellValue(cmd_num, CURRENT_COL, wxString::Format("%3.3f", current));
    if (current < 0)
        setCommandDirection(cmd_num, DIR_NEG);
    else
        setCommandDirection(cmd_num, DIR_POS);
}

void CatheterCmdGrid::setCommandDirection(int cmd_num, dir_t dir) {
    SetCellValue(cmd_num, DIR_COL, dir_choices[dir]);
    double current = getCommandCurrentMA(cmd_num);
    if ((dir == DIR_NEG && current > 0) || (dir == DIR_POS && current < 0))
        setCommandCurrent(cmd_num, -1 * current);
}

void CatheterCmdGrid::setCommandDelay(int cmd_num, int ms) {
    SetCellValue(cmd_num, DELAY_COL, wxString::Format("%d", ms));
}

unsigned int CatheterCmdGrid::getCommandChannel(int cmd_num) {
    return wxAtoi(GetCellValue(cmd_num, CHANNEL_COL));
}

double CatheterCmdGrid::getCommandCurrentMA(int cmd_num) {
    return wxAtof(GetCellValue(cmd_num, CURRENT_COL));
}

dir_t CatheterCmdGrid::getCommandDirection(int cmd_num) {
    return (GetCellValue(cmd_num, DIR_COL).Cmp(dir_choices[DIR_POS]) ? DIR_NEG : DIR_POS);
}

unsigned int CatheterCmdGrid::getCommandDelay(int cmd_num) {
    return wxAtoi(GetCellValue(cmd_num, DELAY_COL));
}

bool CatheterCmdGrid::isRowComplete(int row) {
    if (row >= GetNumberRows()) {
        return false;
    }
    bool row_complete = true;
    for (int i = 0; i < NFIELDS; i++) {
        if (GetTable()->IsEmptyCell(row, i)) {
            row_complete = false;
            break;
        }
    }
    return row_complete;
}

void CatheterCmdGrid::resetEditWindow() {
    // leave 'NCHANNELS_DEFAULT' rows in the table
    setNumberRows(NCHANNELS);
    // clear data from table
    ClearGrid();
}

void CatheterCmdGrid::setNumberRows(int nrows) {
    // default number of rows = NCHANNELS
    int delta = GetNumberRows() - nrows;
    if (delta > 0) { 
        DeleteRows(NCHANNELS, delta);
    } else if (delta < 0) {
        AppendRows(delta);
    }
}

double CatheterCmdGrid::res2ma(int cmd_num) {
    unsigned int res = wxAtoi(GetCellValue(cmd_num, CURRENT_COL));
    dir_t dir = getCommandDirection(cmd_num);
    unsigned int chan = getCommandChannel(cmd_num);
    return convert_current_by_channel(res, dir, chan);
}

unsigned int CatheterCmdGrid::ma2res(int cmd_num) {
    return convert_current_by_channel(getCommandCurrentMA(cmd_num), getCommandChannel(cmd_num));
}

void CatheterCmdGrid::OnSafeExit() { }
void CatheterCmdGrid::OnSafeInit() { }

void CatheterCmdGrid::createCatheterCmdGrid() {
    dir_choices[DIR_POS] = wxT("pos");
    dir_choices[DIR_NEG] = wxT("neg");

    CreateGrid(NCHANNELS, NFIELDS);

    SetColLabelValue(CHANNEL_COL, wxT("Channel"));
    SetColLabelValue(CURRENT_COL, wxT("Current (MA)"));
    SetColLabelValue(DIR_COL, wxT("Direction"));
    SetColLabelValue(DELAY_COL, wxT("Delay (ms)"));
    //HideRowLabels();

    // set column attributes
    GetTable()->SetAttrProvider(new wxGridCellAttrProvider());
    for (int i = 0; i < NFIELDS; i++)
        GetTable()->GetAttrProvider()->SetColAttr(new wxGridCellAttr(), i);

    SetColFormatNumber(CHANNEL_COL); //channel address
    SetColFormatFloat(CURRENT_COL); // MA current
    SetColFormatNumber(DELAY_COL); //delay

    wxGridCellAttr* dirAttr = GetTable()->GetAttrProvider()->GetAttr(0, DIR_COL, wxGridCellAttr::Col);
    wxGridCellAttr* channelAttr = GetTable()->GetAttrProvider()->GetAttr(0, CHANNEL_COL, wxGridCellAttr::Col);
    if (channelAttr == NULL || dirAttr == NULL) {
        printf("attribute is NULL\n");
        exit(EXIT_FAILURE);
    }
    dirAttr->SetEditor(new wxGridCellChoiceEditor(2, dir_choices));
    channelAttr->SetEditor(new wxGridCellNumberEditor(1, NCHANNELS));
    SetColAttr(DIR_COL, dirAttr);
    SetColAttr(CHANNEL_COL, channelAttr);

    for (int i = 1; i < GetNumberRows(); i++) {
        setRowReadOnly(i, true);
    }
    appendCommandRow();
}

void CatheterCmdGrid::appendCommandRow() {
    cmd_count++;
    if (cmd_count >= GetNumberRows())
        AppendRows(1);
    setRowReadOnly(cmd_count - 1, false);
}

void CatheterCmdGrid::setRowReadOnly(int row, bool ReadOnly) {
    SetReadOnly(row, CHANNEL_COL, ReadOnly);
    SetReadOnly(row, CURRENT_COL, ReadOnly);
    SetReadOnly(row, DIR_COL, ReadOnly);
    SetReadOnly(row, DELAY_COL, ReadOnly);
}