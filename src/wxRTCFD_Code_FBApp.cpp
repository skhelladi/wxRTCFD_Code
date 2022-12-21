/***************************************************************
 * Name:      wxRTCFD_Code_FBApp.cpp
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

#include "wxRTCFD_Code_FBApp.h"
#include "wxRTCFD_Code_FBMain.h"

IMPLEMENT_APP(wxRTCFD_Code_FBApp);

bool wxRTCFD_Code_FBApp::OnInit()
{
    wxRTCFD_Code_FBFrame* frame = new wxRTCFD_Code_FBFrame(0L);
    
    frame->Show();
    
    return true;
}
