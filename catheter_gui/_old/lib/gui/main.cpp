#include "main.h"

IMPLEMENT_APP(CatheterGui);

bool CatheterGui::OnInit() {
    CatheterWindow* mainWindow = new CatheterWindow(wxT("Catheter GUI"));
    //StatusPanel* statusPanel;
    //EditPanel* editPanel;
    //ControlPanel* controlPanel;
    if (mainWindow) {
        //statusPanel = new StatusPanel(mainWindow->getParentPanel());
        //editPanel = new EditPanel(mainWindow->getParentPanel());
        //controlPanel = new ControlPanel(mainWindow->getParentPanel());
        //mainWindow->createCatheterWindow(statusPanel, editPanel, controlPanel);

        mainWindow->Show(true);
        mainWindow->Centre();
        return true;
    } else {
        return false;
    }
}