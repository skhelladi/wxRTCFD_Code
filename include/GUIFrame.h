///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/advprops.h>
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/timer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define idMenuQuit 1000
#define idMenuAbout 1001

#include "settings.h"
///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrame
///////////////////////////////////////////////////////////////////////////////
class GUIFrame : public wxFrame
{
	private:

	protected:
		wxMenuBar* mbar;
		wxMenu* fileMenu;
		wxMenu* helpMenu;
		wxStatusBar* statusBar;
		wxSplitterWindow* m_splitter3;
		wxPanel* m_panel_properties;
		wxButton* m_button_run;
		wxCheckBox* m_checkBox_pause;
		wxPropertyGridManager* m_propertyGridManager;
		wxPropertyGridPage* m_propertyGridPage;
		wxPGProperty* m_propertyGridItem_prop;
		wxPGProperty* m_propertyGridItem_case;
		wxPGProperty* m_propertyGridItem_obstacle;
		wxPGProperty* m_propertyGridItem_obs_pos;
		wxPGProperty* m_propertyGridItem_obs_dis;
		wxPGProperty* m_propertyGridItem_gravity;
		wxPGProperty* m_propertyGridItem_postproc;
		wxPGProperty* m_propertyGridItem_scalar;
		wxPGProperty* m_propertyGridItem_tracer;
		wxPGProperty* m_propertyGridItem_streamlines;
		wxPGProperty* m_propertyGridItem_vel_vec;
		wxPGProperty* m_propertyGridItem_cd;
		wxPGProperty* m_propertyGridItem_fluid;
		wxPGProperty* m_propertyGridItem_density;
		wxPGProperty* m_propertyGridItem_solver;
		wxPGProperty* m_propertyGridItem_overrelaxation;
		wxPGProperty* m_propertyGridItem_resolution;
		wxPGProperty* m_propertyGridItem_nb_cpu;
		wxPGProperty* m_propertyGridItem_solid;
		wxPGProperty* m_propertyGridItem_fsi;
		wxPGProperty* m_propertyGridItem_sol_den;
		wxPanel* m_panel_scene;


		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void onRunButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onCheckBoxChecked( wxCommandEvent& event ) { event.Skip(); }
		virtual void onPropertyGridChanged( wxPropertyGridEvent& event ) { event.Skip(); }


	public:

		GUIFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("RTCFD Code"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1200,640 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~GUIFrame();

		void m_splitter3OnIdle( wxIdleEvent& )
		{
			m_splitter3->SetSashPosition( 250 );
			m_splitter3->Disconnect( wxEVT_IDLE, wxIdleEventHandler( GUIFrame::m_splitter3OnIdle ), NULL, this );
		}

};

