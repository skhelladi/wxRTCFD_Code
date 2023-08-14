/***************************************************************
 * Name:      wxRTCFD_Code_FBMain.cpp
 * Purpose:   Code for Application Frame
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

#include "wxRTCFD_Code_Main.h"

// helper functions
enum wxbuildinfoformat
{
    short_f,
    long_f
};

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f)
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

wxRTCFD_Code_Frame::wxRTCFD_Code_Frame(wxFrame *frame)
    : GUIFrame(frame)
{
#if wxUSE_STATUSBAR
    statusBar->SetStatusText(_("RTCFDCode v0.01b"), 0);
#endif
    //! initialization

    m_propertyGridItem_case->SetValue("Wind tunnel");
    m_propertyGridItem_tracer->SetValue(true);
    m_propertyGridItem_density->SetValue(1000.0);
    m_propertyGridItem_overrelaxation->SetValue(1.9);
    m_propertyGridItem_resolution->SetValue(50);
    m_propertyGridItem_nb_cpu->SetValue(4);

    //    m_panel_scene->SetBackgroundColour( *wxWHITE);
    int initialWidth = 1024;
    int initialHeight = 500;

    region = make_shared<Region>(initialHeight, initialWidth, 1.0);
    region->setupRegion(-1, 1.9);
    draw = new Draw(m_panel_scene, region);

    wxBoxSizer *bSizer;
    bSizer = new wxBoxSizer(wxVERTICAL);
    bSizer->Add(draw, 1, wxEXPAND, 5);
    m_panel_scene->SetSizer(bSizer);
    m_panel_scene->SetAutoLayout(true);

    m_timer = new wxTimer(this);
    this->Connect(wxEVT_TIMER, wxTimerEventHandler(Draw::animate), NULL, this);

    compute = true;
    SetIcon(wxIcon(wxT("../pics/icon.xpm"), wxBITMAP_TYPE_XPM));
    // full screen main window
    SetSize(initialWidth, initialHeight);
    Center();

}

wxRTCFD_Code_Frame::~wxRTCFD_Code_Frame()
{
    delete draw;
}

void wxRTCFD_Code_Frame::simulate()
{
    if (!draw->region->paused)
        draw->region->fluid->simulate(draw->region->dt, draw->region->gravity, draw->region->numIters);
    draw->region->frameNr++;
}

void wxRTCFD_Code_Frame::update()
{
    simulate();
}

void wxRTCFD_Code_Frame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void wxRTCFD_Code_Frame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void wxRTCFD_Code_Frame::OnAbout(wxCommandEvent &event)
{
    wxString msg = "RTCFD_Code 0.01beta\n\n"
                   "- Built in 19.12.2022 under GPL3 LICENSE\n"
                   "- Based on " +
                   wxbuildinfo(long_f) +
                   "\n\n"
                   "Author:\nSofiane KHELLADI <sofiane.khelladi@ensam.eu>";
    wxMessageBox(msg, _("Welcome to..."));
}

void wxRTCFD_Code_Frame::onRunButtonClick(wxCommandEvent &event)
{
    int regionNr = m_propertyGridItem_case->GetValue().GetLong();

    wxAny value;

    value = m_propertyGridItem_obstacle->GetValue();
    draw->region->obstacle = indexToOBJ(value.As<int>());

    value = m_propertyGridItem_streamlines->GetValue();
    draw->region->showStreamlines = value.As<bool>();

    value = m_propertyGridItem_vel_vec->GetValue();
    draw->region->showVelocityVectors = value.As<bool>();

    value = m_propertyGridItem_tracer->GetValue();
    draw->region->showTracer = value.As<bool>();

    value = m_propertyGridItem_overrelaxation->GetValue();
    draw->region->overRelaxation = value.As<double>();

    value = m_propertyGridItem_resolution->GetValue();
    draw->region->resolution = value.As<int>();

    value = m_propertyGridItem_nb_cpu->GetValue();
    draw->region->fluid->numThreads = value.As<int>();

    value = m_propertyGridItem_density->GetValue();
    draw->region->fluid->density = value.As<double>();

    // draw->region->updateRegionSize(draw->height(),draw->width());

    if (compute)
    {
#if wxUSE_STATUSBAR
        statusBar->SetStatusText(_("running..."), 1);
#endif

        m_timer->Start(1);
        draw->region->setupRegion(regionNr, draw->region->overRelaxation, draw->region->resolution, draw->region->fluid->density, draw->region->fluid->numThreads);

        m_propertyGridItem_streamlines->SetValue(draw->region->showStreamlines);
        m_propertyGridItem_vel_vec->SetValue(draw->region->showVelocityVectors);
        m_propertyGridItem_tracer->SetValue(draw->region->showTracer);
        m_propertyGridItem_obs_pos->SetValue(draw->region->showObstaclePosition);
        m_propertyGridItem_nb_cpu->SetValue(draw->region->fluid->numThreads);

        compute = false;
        draw->region->paused = false;
        m_button_run->SetLabel("Stop");
        m_checkBox_pause->SetValue(false);
        update();
        // setToolTip(ui->comboBox->currentText());
    }
    else
    {
#if wxUSE_STATUSBAR
        statusBar->SetStatusText(_(""), 1);
#endif
        // timer->stop();
        m_timer->Stop();
        compute = true;
        m_button_run->SetLabel("Run");
        m_checkBox_pause->SetValue(false);
        draw->region->paused = true;
        update();
    }
    //    update();
}

void wxRTCFD_Code_Frame::onCheckBoxChecked(wxCommandEvent &event)
{
    draw->region->paused = event.IsChecked();
    if (draw->region->paused)
    {
#if wxUSE_STATUSBAR
        statusBar->SetStatusText(_("paused..."), 1);
#endif
    }
    else
    {
#if wxUSE_STATUSBAR
        statusBar->SetStatusText(_("running..."), 1);
#endif
    }
}

void wxRTCFD_Code_Frame::onPropertyGridChanged(wxPropertyGridEvent &event)
{
    wxPGProperty *property = event.GetProperty();
    // Do nothing if event did not have associated property
    if (!property)
        return;
    // GetValue() returns wxVariant, but it is converted transparently to
    // wxAny
    wxAny value = property->GetValue();
    // Also, handle the case where property value is unspecified
    if (value.IsNull())
        return;
    // Handle changes in values, as needed

    if (property->GetName() == "Case")
        OnCasePropertyChanged(value.As<int>());
    else if (property->GetName() == "Scalar")
        OnScalarPropertyChanged(value.As<int>());
    else if (property->GetName() == "Obstacle position")
        OnObstaclePositionPropertyChanged(value.As<bool>());
    else if (property->GetName() == "Tracer")
        OnTracerPropertyChanged(value.As<bool>());
    else if (property->GetName() == "Streamlines")
        OnStreamlinesPropertyChanged(value.As<bool>());
    else if (property->GetName() == "Velocity vectors")
        OnVelocityVectorsPropertyChanged(value.As<bool>());
    else if (property->GetName() == "Density")
        OnDensityPropertyChanged(value.As<double>());
    else if (property->GetName() == "Overrelaxation")
        OnOverrelaxationPropertyChanged(value.As<double>());
    else if (property->GetName() == "Resolution")
        OnResolutionPropertyChanged(value.As<int>());
    else if (property->GetName() == "CPU Number")
        OnNumThreadsPropertyChanged(value.As<int>());
    //    wxString strValue = property->GetValueAsString();

    //    wxString msg = property->GetName()+"value = "+strValue;
    //    wxMessageBox(msg, _("property value"));
}

void wxRTCFD_Code_Frame::OnCasePropertyChanged(int value)
{
    switch (value)
    {
    case 0: // Tank
    {
        m_propertyGridItem_scalar->SetValue("Pressure");
        break;
    }
    case 1: // Wind tunnel
    {
        m_propertyGridItem_scalar->SetValue("None");
        break;
    }
    case 2: // Paint
    {
        m_propertyGridItem_scalar->SetValue("None");
        break;
    }
    case 3: // Wind tunnel (high resolution)
    {
        m_propertyGridItem_scalar->SetValue("Pressure");
        break;
    }
    }
}

void wxRTCFD_Code_Frame::OnObstaclePropertyChanged(int value)
{
    switch (value)
    {
    case 1:
        draw->region->obstacle = CYLINDER;
        break;
    case 2:
        draw->region->obstacle = SQUARE;
        break;
    case 3:
        draw->region->obstacle = DIAMOND;
        break;
    case 4:
        draw->region->obstacle = NACA;
        break;
    case 5:
        draw->region->obstacle = ROTOR;
        break;
    default:
        draw->region->obstacle = CYLINDER;
        break;
    }
}

void wxRTCFD_Code_Frame::OnScalarPropertyChanged(int value)
{
    switch (value)
    {
    case 0: // no selection
    {
        draw->region->showPressure = false;
        draw->region->showVelocity = false;
        draw->region->showXVelocity = false;
        draw->region->showYVelocity = false;
        break;
    }
    case 1: // pressure
    {
        draw->region->showPressure = true;
        draw->region->showVelocity = false;
        draw->region->showXVelocity = false;
        draw->region->showYVelocity = false;
        break;
    }
    case 2: // velocity magnitude
    {
        draw->region->showPressure = false;
        draw->region->showVelocity = true;
        draw->region->showXVelocity = false;
        draw->region->showYVelocity = false;
        break;
    }
    case 3: // x-velocity
    {
        draw->region->showPressure = false;
        draw->region->showVelocity = false;
        draw->region->showXVelocity = true;
        draw->region->showYVelocity = false;
        break;
    }
    case 4: // y-velocity
    {
        draw->region->showPressure = false;
        draw->region->showVelocity = false;
        draw->region->showXVelocity = false;
        draw->region->showYVelocity = true;
        break;
    }
    }
}

void wxRTCFD_Code_Frame::OnObstaclePositionPropertyChanged(bool value)
{
    draw->region->showObstaclePosition = value;
}

void wxRTCFD_Code_Frame::OnTracerPropertyChanged(bool value)
{
    draw->region->showTracer = value;
}

void wxRTCFD_Code_Frame::OnStreamlinesPropertyChanged(bool value)
{
    draw->region->showStreamlines = value;
}

void wxRTCFD_Code_Frame::OnVelocityVectorsPropertyChanged(bool value)
{
    draw->region->showVelocityVectors = value;
}

void wxRTCFD_Code_Frame::OnDensityPropertyChanged(double value)
{
    draw->region->fluid->density = value;
}

void wxRTCFD_Code_Frame::OnOverrelaxationPropertyChanged(double value)
{
    draw->region->overRelaxation = value;
}

void wxRTCFD_Code_Frame::OnResolutionPropertyChanged(int value)
{
    draw->region->resolution = value;
}
void wxRTCFD_Code_Frame::OnNumThreadsPropertyChanged(int value)
{
    draw->region->fluid->numThreads = value;
}
