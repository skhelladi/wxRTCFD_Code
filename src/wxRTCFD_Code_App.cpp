/***************************************************************
 * Name:      wxRTCFD_Code_App.cpp
 * Purpose:   Code for Application Class
 * Author:    Sofiane KHELLADI (sofiane.khelladi@gmail.com)
 * Created:   2022-12-19
 * Copyright: Sofiane KHELLADI (https://sofiane.khelladi.page)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "wxRTCFD_Code_App.h"
#include "wxRTCFD_Code_Main.h"

IMPLEMENT_APP(wxRTCFD_Code_App);

bool wxRTCFD_Code_App::OnInit()
{
    wxRTCFD_Code_Frame* frame = new wxRTCFD_Code_Frame(0L);
    
    frame->Show();
    
    return true;
}
