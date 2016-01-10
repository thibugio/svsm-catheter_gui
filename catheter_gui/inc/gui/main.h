#ifndef MAIN_H
#define MAIN_H

#pragma once    
#include "CatheterWindow.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <boost/shared_ptr.hpp>

class CatheterGui : public wxApp {
    public:
    virtual bool OnInit();
};

#endif