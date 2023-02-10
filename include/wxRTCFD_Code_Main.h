/***************************************************************
 * Name:      wxRTCFD_Code_Main.h
 * Purpose:   Defines Application Frame
 * Author:    Sofiane KHELLADI (sofiane.khelladi@gmail.com)
 * Created:   2022-12-19
 * Copyright: Sofiane KHELLADI (https://sofiane.khelladi.page)
 * License:
 **************************************************************/

#ifndef WXRTCFD_CODE_MAIN_H
#define WXRTCFD_CODE_MAIN_H

#include "wxRTCFD_Code_App.h"

#include "GUIFrame.h"

#include "region.h"
#include "draw.h"

class wxRTCFD_Code_Frame : public GUIFrame
{
public:
    wxRTCFD_Code_Frame(wxFrame *frame);
    ~wxRTCFD_Code_Frame();
    void simulate();
    void update();
    shared_ptr<Region> region;
//    shared_ptr<Draw> draw;
    Draw *draw;

    wxTimer *m_timer;

private:
    virtual void OnClose(wxCloseEvent &event);
    virtual void OnQuit(wxCommandEvent &event);
    virtual void OnAbout(wxCommandEvent &event);

    virtual void onRunButtonClick(wxCommandEvent &event);
    virtual void onCheckBoxChecked(wxCommandEvent &event);
    virtual void onPropertyGridChanged(wxPropertyGridEvent &event);

    void OnCasePropertyChanged(int value);
    void OnObstaclePropertyChanged(int value);
    void OnScalarPropertyChanged(int value);
    void OnObstaclePositionPropertyChanged(bool value);
    void OnObstacleDisplayPropertyChanged(bool value);
    void OnTracerPropertyChanged(bool value);
    void OnStreamlinesPropertyChanged(bool value);
    void OnVelocityVectorsPropertyChanged(bool value);
    void OnCdPropertyChanged(bool value);
    void OnDensityPropertyChanged(double value);
    void OnOverrelaxationPropertyChanged(double value);
    void OnResolutionPropertyChanged(int value);
    void OnNumThreadsPropertyChanged(int value);
    void OnSolidDensityPropertyChanged(double value);


    bool compute;
};

#endif // WXRTCFD_CODE_FBMAIN_H
