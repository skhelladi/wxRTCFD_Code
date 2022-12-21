/***************************************************************
 * Name:      wxRTCFD_Code_App.h
 * Purpose:   Defines Application Class
 * Author:    Sofiane KHELLADI (sofiane.khelladi@gmail.com)
 * Created:   2022-12-19
 * Copyright: Sofiane KHELLADI (https://sofiane.khelladi.page)
 * License:
 **************************************************************/

#ifndef WXRTCFD_CODE_APP_H
#define WXRTCFD_CODE_APP_H

#include <wx/app.h>
#include <wx/msgdlg.h>
#include <string>

using namespace std;

class wxRTCFD_Code_App : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // WXRTCFD_CODE_APP_H
