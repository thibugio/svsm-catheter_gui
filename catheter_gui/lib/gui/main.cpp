#include "main.h"

IMPLEMENT_APP(CatheterGui);

bool CatheterGui::OnInit() {
    CatheterWindow* mainWindow = new CatheterWindow(wxT("Catheter GUI"));
    if (mainWindow) {
        mainWindow->Show(true);
        return true;
    }
    return false;
}