///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "GUIFrame.h"

///////////////////////////////////////////////////////////////////////////

GUIFrame::GUIFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	mbar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* menuFileQuit;
	menuFileQuit = new wxMenuItem( fileMenu, idMenuQuit, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Alt+F4"), wxT("Quit the application"), wxITEM_NORMAL );
	fileMenu->Append( menuFileQuit );

	mbar->Append( fileMenu, wxT("&File") );

	helpMenu = new wxMenu();
	wxMenuItem* menuHelpAbout;
	menuHelpAbout = new wxMenuItem( helpMenu, idMenuAbout, wxString( wxT("&About") ) + wxT('\t') + wxT("F1"), wxT("Show info about this application"), wxITEM_NORMAL );
	helpMenu->Append( menuHelpAbout );

	mbar->Append( helpMenu, wxT("&Help") );

	this->SetMenuBar( mbar );

	statusBar = this->CreateStatusBar( 2, wxSTB_SIZEGRIP, wxID_ANY );
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	m_splitter3 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxSize( 250,-1 ), wxSP_3D );
	m_splitter3->Connect( wxEVT_IDLE, wxIdleEventHandler( GUIFrame::m_splitter3OnIdle ), NULL, this );

	m_panel_properties = new wxPanel( m_splitter3, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	bSizer5->SetMinSize( wxSize( 300,-1 ) );
	m_button_run = new wxButton( m_panel_properties, wxID_ANY, wxT("Run"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button_run->SetMinSize( wxSize( -1,40 ) );

	bSizer5->Add( m_button_run, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_checkBox_pause = new wxCheckBox( m_panel_properties, wxID_ANY, wxT("Pause"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_checkBox_pause, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer6->Add( bSizer5, 0, wxSHAPED, 5 );

	m_propertyGridManager = new wxPropertyGridManager(m_panel_properties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPGMAN_DEFAULT_STYLE|wxPG_BOLD_MODIFIED|wxPG_DESCRIPTION|wxPG_SPLITTER_AUTO_CENTER);

	m_propertyGridPage = m_propertyGridManager->AddPage( wxT("Properties"), wxArtProvider::GetBitmap( wxART_LIST_VIEW, wxART_OTHER ) );
	m_propertyGridItem_prop = m_propertyGridPage->Append( new wxPropertyCategory( wxT("Properties"), wxT("Properties") ) );
	m_propertyGridItem_case = m_propertyGridPage->Append( new wxEnumProperty( wxT("Case"), wxT("Case") ,getCaseList()) );
	m_propertyGridPage->SetPropertyHelpString( m_propertyGridItem_case, wxT("Choose case to simulate") );
	m_propertyGridItem_obstacle = m_propertyGridPage->Append( new wxEnumProperty( wxT("Obstacle"), wxT("Obstacle") ,getObstacleList()) );
	m_propertyGridPage->SetPropertyHelpString( m_propertyGridItem_obstacle, wxT("Choose obstacle") );
	m_propertyGridItem_obs_pos = m_propertyGridPage->Append( new wxBoolProperty( wxT("Obstacle position"), wxT("Obstacle position") ) );
	m_propertyGridPage->SetPropertyHelpString( m_propertyGridItem_obs_pos, wxT("Print obstacle position") );
	m_propertyGridItem_obs_dis = m_propertyGridPage->Append( new wxBoolProperty( wxT("Obstacle display"), wxT("Obstacle display") ) );
        m_propertyGridPage->SetPropertyHelpString( m_propertyGridItem_obs_dis, wxT("Print obstacle") );
	m_propertyGridItem_gravity = m_propertyGridPage->Append( new wxEnumProperty( wxT("Gravity options"), wxT("Gravity options"), getGravityList()) );
        m_propertyGridPage->SetPropertyHelpString( m_propertyGridItem_gravity, wxT("Choose option for gravity") );
	m_propertyGridItem_postproc = m_propertyGridPage->Append( new wxPropertyCategory( wxT("Post-processing"), wxT("Post-processing") ) );
	m_propertyGridItem_scalar = m_propertyGridPage->Append( new wxEnumProperty( wxT("Scalar"), wxT("Scalar") ,getScalarList()) );
	m_propertyGridPage->SetPropertyHelpString( m_propertyGridItem_scalar, wxT("Show scalar field") );
	m_propertyGridItem_tracer = m_propertyGridPage->Append( new wxBoolProperty( wxT("Tracer"), wxT("Tracer") ) );
	m_propertyGridPage->SetPropertyHelpString( m_propertyGridItem_tracer, wxT("Show tracer field") );
	m_propertyGridItem_streamlines = m_propertyGridPage->Append( new wxBoolProperty( wxT("Streamlines"), wxT("Streamlines") ) );
	m_propertyGridPage->SetPropertyHelpString( m_propertyGridItem_streamlines, wxT("Show streamlines") );
	m_propertyGridItem_vel_vec = m_propertyGridPage->Append( new wxBoolProperty( wxT("Velocity vectors"), wxT("Velocity vectors") ) );
	m_propertyGridPage->SetPropertyHelpString( m_propertyGridItem_vel_vec, wxT("Show velocity vectors") );
	m_propertyGridItem_cd = m_propertyGridPage->Append( new wxBoolProperty( wxT("Cd"), wxT("Cd") ) );
        m_propertyGridPage->SetPropertyHelpString( m_propertyGridItem_cd, wxT("Write time evolution of drag & lift coefficients in file") );
	m_propertyGridItem_fluid = m_propertyGridPage->Append( new wxPropertyCategory( wxT("Fluid"), wxT("Fluid") ) );
	m_propertyGridItem_density = m_propertyGridPage->Append( new wxFloatProperty( wxT("Density"), wxT("Density") ) );
	m_propertyGridPage->SetPropertyHelpString( m_propertyGridItem_density, wxT("Fluid density in kg/m³") );
	m_propertyGridItem_solver = m_propertyGridPage->Append( new wxPropertyCategory( wxT("Solver"), wxT("Solver") ) );
	m_propertyGridItem_overrelaxation = m_propertyGridPage->Append( new wxFloatProperty( wxT("Overrelaxation"), wxT("Overrelaxation") ) );
	m_propertyGridPage->SetPropertyHelpString( m_propertyGridItem_overrelaxation, wxT("Solver overrelaxation coefficient, choose a value in [1,2]") );
	m_propertyGridItem_resolution = m_propertyGridPage->Append( new wxIntProperty( wxT("Resolution"), wxT("Resolution") ) );
	m_propertyGridPage->SetPropertyHelpString( m_propertyGridItem_resolution, wxT("Grid resolution") );
	m_propertyGridItem_nb_cpu = m_propertyGridPage->Append( new wxIntProperty( wxT("CPU number"), wxT("CPU number") ) );
	m_propertyGridPage->SetPropertyHelpString( m_propertyGridItem_nb_cpu, wxT("Number of threads (default = 4)") );
	m_propertyGridItem_solid = m_propertyGridPage->Append( new wxPropertyCategory( wxT("Solid"), wxT("Solid") ) );
	m_propertyGridItem_fsi = m_propertyGridPage->Append( new wxBoolProperty( wxT("FSI"), wxT("FSI") ) );
        m_propertyGridPage->SetPropertyHelpString( m_propertyGridItem_fsi, wxT("Activate FSI") );
	m_propertyGridItem_sol_den = m_propertyGridPage->Append( new wxFloatProperty( wxT("Solid density"), wxT("Solid density") ) );
        m_propertyGridPage->SetPropertyHelpString( m_propertyGridItem_sol_den, wxT("Density of the solid in kg/m³") );
	bSizer6->Add( m_propertyGridManager, 100, wxEXPAND|wxALL, 5 );


	m_panel_properties->SetSizer( bSizer6 );
	m_panel_properties->Layout();
	bSizer6->Fit( m_panel_properties );
	m_panel_scene = new wxPanel( m_splitter3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel_scene->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT ) );

	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );


	m_panel_scene->SetSizer( bSizer );
	m_panel_scene->Layout();
	bSizer->Fit( m_panel_scene );
	m_splitter3->SplitVertically( m_panel_properties, m_panel_scene, 250 );
	bSizer1->Add( m_splitter3, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
//	m_timer1.SetOwner( this, wxID_ANY );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	fileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ), this, menuFileQuit->GetId());
	helpMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ), this, menuHelpAbout->GetId());
	m_button_run->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::onRunButtonClick ), NULL, this );
	m_checkBox_pause->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrame::onCheckBoxChecked ), NULL, this );
	m_propertyGridManager->Connect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( GUIFrame::onPropertyGridChanged ), NULL, this );
}

GUIFrame::~GUIFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	m_button_run->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::onRunButtonClick ), NULL, this );
	m_checkBox_pause->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrame::onCheckBoxChecked ), NULL, this );
	m_propertyGridManager->Disconnect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( GUIFrame::onPropertyGridChanged ), NULL, this );

}
