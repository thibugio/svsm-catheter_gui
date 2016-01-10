#include "CatheterWindow.h"
#include "catheter_gui_common.h"
#include "pc_utils.h"

#include <wx/filedlg.h>
#include <wx/wfstream.h>

#include <vector>

ControlPanel::ControlPanel(wxPanel* m_parent)
    : wxPanel(m_parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN),
      parent(m_parent) {

    createControlPanel();
}

ControlPanel::~ControlPanel() {
    OnSafeExit();
    connectButton->Destroy();
    selectButton->Destroy();
    newButton->Destroy();
    saveButton->Destroy();
    sendButton->Destroy();
    resetButton->Destroy();
}

ControlPanel::ControlPanel(const ControlPanel& C)
    : wxPanel(C.parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN),
    parent(C.parent) {

    createControlPanel();

    SetSize(C.GetSize());

    playfilePath = C.playfilePath;
    portName = C.portName;
    playfileSaved = C.playfileSaved;
    serialConnected = C.serialConnected;
}

ControlPanel & ControlPanel::operator=(const ControlPanel& C) {
    if (this != &C) {
        delete parent;
        delete connectButton;
        delete selectButton;
        delete newButton;
        delete saveButton;
        delete sendButton;
        delete resetButton;

        Reparent(C.parent);
        parent = C.parent;

        connectButton = new wxButton(this, ID_CONNECT_BUTTON, wxT("Refresh Serial Connection"));
        selectButton = new wxButton(this, ID_SELECT_BUTTON, wxT("Select Playfile"));
        newButton = new wxButton(this, ID_NEW_BUTTON, wxT("New Playfile"));
        saveButton = new wxButton(this, ID_SAVE_BUTTON, wxT("Save Playfile"));
        sendButton = new wxButton(this, ID_SEND_BUTTON, wxT("Send Commands"));
        resetButton = new wxButton(this, ID_RESET_BUTTON, wxT("Send Reset"));

        SetSize(C.GetSize());

        playfilePath = C.playfilePath;
        portName = C.portName;
        playfileSaved = C.playfileSaved;
        serialConnected = C.serialConnected;
    }
    return *this;
}

void ControlPanel::OnSafeExit() {
    // save current Playfile
    if (!playfileSaved) {
        if (wxMessageBox(wxT("Current content has not been saved!"), wxT("Proceed?"),
            wxICON_QUESTION | wxYES_NO, this) == wxNO) {
            OnSavePlayfile(wxCommandEvent());
            return;
        }
    }

    // close serial connection
    if (serialConnected) {
        // send Reset Command
        sendResetCommand();

        //disconnect serial
        setStatusPanelText(wxString::Format(wxT("Closing Serial Connection on port %s\n"), portName));
        serialConnected = false;
    }
}

void ControlPanel::OnSafeInit() {
    // open serial connection
    //todo
    // send Reset command
    if (serialConnected)
        sendResetCommand();
}
void ControlPanel::OnConnect(wxCommandEvent& event) {
    // refresh connection to serial port
    setStatusPanelText(wxString::Format(wxT("Refreshing Serial Connection for port %s\n"), portName));
}

void ControlPanel::OnSelectPlayfile(wxCommandEvent& event) {
    if (!playfileSaved) {
        if (wxMessageBox(wxT("Current content has not been saved!"), wxT("Proceed?"),
            wxICON_QUESTION | wxYES_NO, this) == wxNO) {
            OnSavePlayfile(wxCommandEvent());
            return;
        }
    }

    wxString path = openFileDialog(wxT("Select an Existing Playfile"), playfile_wildcard);

    if (!path.IsEmpty()) {
        playfileSaved = false;
        playfilePath = path;
        loadPlayfile(playfilePath);
        setStatusPanelText(wxString::Format(wxT("Editing Existing Playfile %s\n"), playfilePath));
    }
}

void ControlPanel::OnNewPlayfile(wxCommandEvent& event) {
    if (!playfileSaved) {
        if (wxMessageBox(wxT("Current content has not been saved!"), wxT("Proceed?"),
            wxICON_QUESTION | wxYES_NO, this) == wxNO) {
            OnSavePlayfile(wxCommandEvent());
            return;
        }
    }

    //clear edit panel
    getEditPanel()->resetEditWindow();

    playfileSaved = false;
    playfilePath = wxEmptyString;

    setStatusPanelText(wxT("Editing New Playfile\n"));
}

void ControlPanel::OnSavePlayfile(wxCommandEvent& event) {        
    wxString path = saveFileDialog(wxT("Save Playfile"), playfile_wildcard);
    if (!path.IsEmpty()) {
        playfileSaved = true;
        playfilePath = path;
        // save contents of edit panel to playfilePath
        unloadPlayfile(playfilePath);
        setStatusPanelText(wxString::Format(wxT("Saved Playfile as %s"), playfilePath));
    }        
}

void ControlPanel::OnSendCommands(wxCommandEvent& event) {
    if (serialConnected) {
        setStatusPanelText(wxT("Sending Commands...\n"));
    } else {
        wxMessageBox("Serial Disconnected!");
    }
}

void ControlPanel::OnSendReset(wxCommandEvent& event) {
    if (serialConnected) {
        setStatusPanelText(wxT("Sending Reset Command...\n"));
        sendResetCommand();
    } else {
        wxMessageBox("Serial Disconnected!");
    }
}

/////////////////////
// private methods //
/////////////////////

StatusPanel* ControlPanel::getStatusPanel() {
    CatheterWindow* win = (CatheterWindow*)(parent->GetParent());
    return (StatusPanel*)((win->statusPanel));
}

EditPanel* ControlPanel::getEditPanel() {
    CatheterWindow* win = (CatheterWindow*)(parent->GetParent());
    return (EditPanel*)((win->editPanel));
}

void ControlPanel::setStatusPanelText(const wxString& label) {
    getStatusPanel()->SetText(label);
}

void ControlPanel::loadPlayfile(const wxString& path) {
    // load the playfile into the edit panel
    std::vector<CatheterChannelCmd> cmdVect;
    loadPlayFile(path.mb_str(), cmdVect);
    getEditPanel()->appendCommands(cmdVect);
}

void ControlPanel::unloadPlayfile(const wxString& path) {
    // save the contents of the edit panel into the file
    std::vector<CatheterChannelCmd> cmdVect = getEditPanel()->getCommands();
    writePlayFile(path.mb_str(), cmdVect);
}

wxString ControlPanel::openFileDialog(const wxString& prompt, const wxString& wildcard) {
    wxString path = wxEmptyString;

    wxFileDialog openDialog(this, prompt, wxGetCwd(), "", wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openDialog.ShowModal() != wxID_CANCEL) {
        wxFileInputStream playfile_stream(openDialog.GetPath());
        if (playfile_stream.IsOk()) {
            path = openDialog.GetPath();
        } else {
            wxLogError("Selected file could not be opened.");
        }
    }

    return path;
}

wxString ControlPanel::saveFileDialog(const wxString& prompt, const wxString& wildcard) {
    wxString path = wxEmptyString;

    wxFileDialog saveDialog(this, wxT("Save Playfile"), wxGetCwd(), "", wildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveDialog.ShowModal() != wxID_CANCEL) {
        wxFileOutputStream save_stream(saveDialog.GetPath());
        if (!save_stream.IsOk()) {
            wxLogError("Could not save to selected file");
        } else {
            path = saveDialog.GetPath();
        }
    }
    return path;
}

void ControlPanel::sendResetCommand() {

}

void ControlPanel::createControlPanel() {
    if (!parent) {
        wxMessageBox("ControlPanel could not be initialized: parent is NULL");
        exit(EXIT_FAILURE);
    }
    
    connectButton = new wxButton(this, ID_CONNECT_BUTTON, wxT("Refresh Serial Connection"));
    selectButton = new wxButton(this, ID_SELECT_BUTTON, wxT("Select Playfile"));
    newButton = new wxButton(this, ID_NEW_BUTTON, wxT("New Playfile"));
    saveButton = new wxButton(this, ID_SAVE_BUTTON, wxT("Save Playfile"));
    sendButton = new wxButton(this, ID_SEND_BUTTON, wxT("Send Commands"));
    resetButton = new wxButton(this, ID_RESET_BUTTON, wxT("Send Reset"));

    // connect the buttons to their handler methods
    Connect(ID_CONNECT_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ControlPanel::OnConnect));
    Connect(ID_SELECT_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ControlPanel::OnSelectPlayfile));
    Connect(ID_NEW_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ControlPanel::OnNewPlayfile));
    Connect(ID_SAVE_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ControlPanel::OnSavePlayfile));
    Connect(ID_SEND_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ControlPanel::OnSendCommands));
    Connect(ID_RESET_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ControlPanel::OnSendReset));

    // place buttons in the control panel
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
    hbox->Add(connectButton);
    hbox->Add(selectButton);
    hbox->Add(newButton);
    hbox->Add(saveButton);
    hbox->Add(sendButton);
    hbox->Add(resetButton);

    this->SetSizer(hbox);
    hbox->SetSizeHints(this);
    hbox->Fit(this);

    playfilePath = wxEmptyString;
    portName = wxEmptyString;
    playfileSaved = true;
    serialConnected = false;

    Centre();
}