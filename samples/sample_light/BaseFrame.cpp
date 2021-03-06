///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "BaseFrame.h"

///////////////////////////////////////////////////////////////////////////

BaseFrame::BaseFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);
    m_mgr.SetManagedWindow(this);
    m_mgr.SetFlags(wxAUI_MGR_DEFAULT);

    m_menubar1 = new wxMenuBar(0);
    FileMenu = new wxMenu();
    wxMenuItem* QuitItem;
    QuitItem = new wxMenuItem(FileMenu, wxID_ANY, wxString(wxT("Quit")), wxEmptyString, wxITEM_NORMAL);
    FileMenu->Append(QuitItem);

    m_menubar1->Append(FileMenu, wxT("File"));

    this->SetMenuBar(m_menubar1);

    m_viewPanel = new wxPanel(this, wxID_VIEWPANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    m_viewPanel->SetExtraStyle(wxWS_EX_PROCESS_IDLE);

    m_mgr.AddPane(m_viewPanel, wxAuiPaneInfo().Name(wxT("3D view")).Left().CloseButton(false).Dock().Resizable().FloatingSize(wxDefaultSize).CentrePane());

    wxBoxSizer* bSizer3;
    bSizer3 = new wxBoxSizer(wxVERTICAL);


    m_viewPanel->SetSizer(bSizer3);
    m_viewPanel->Layout();
    bSizer3->Fit(m_viewPanel);
    m_notebook1 = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(300, -1), 0);
    m_mgr.AddPane(m_notebook1, wxAuiPaneInfo().Right().PinButton(true).Gripper().Dock().Resizable().FloatingSize(wxDefaultSize).DockFixed(false));

    planetProperties = new wxScrolledWindow(m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
    planetProperties->SetScrollRate(5, 5);
    wxBoxSizer* bSizer2;
    bSizer2 = new wxBoxSizer(wxVERTICAL);

    m_propertyPlanet = new wxPropertyGrid(planetProperties, wxID_PROPERTY, wxDefaultPosition, wxSize(-1, -1), wxPG_DEFAULT_STYLE | wxPG_SPLITTER_AUTO_CENTER);
    Lighting = m_propertyPlanet->Append(new wxPropertyCategory(wxT("Lighting"), wxT("Lighting")));
    position_x = m_propertyPlanet->Append(new wxFloatProperty(wxT("Name"), wxT("Name")));
    m_propertyGridItem17 = m_propertyPlanet->Append(new wxStringProperty(wxT("Name"), wxT("Name")));
    Shadow = m_propertyPlanet->Append(new wxPropertyCategory(wxT("Shadow"), wxT("Shadow")));
    m_offset = m_propertyPlanet->Append(new wxFloatProperty(wxT("Name"), wxT("Name")));
    m_bias = m_propertyPlanet->Append(new wxFloatProperty(wxT("Name"), wxT("Name")));
    bSizer2->Add(m_propertyPlanet, 1, wxALL | wxEXPAND, 5);


    planetProperties->SetSizer(bSizer2);
    planetProperties->Layout();
    bSizer2->Fit(planetProperties);
    m_notebook1->AddPage(planetProperties, wxT("Properties"), false);
    m_panel3 = new wxPanel(m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    m_notebook1->AddPage(m_panel3, wxT("a page"), false);


    m_mgr.Update();
    this->Centre(wxBOTH);

    // Connect Events
    this->Connect(wxEVT_IDLE, wxIdleEventHandler(BaseFrame::OnIdle));
    m_propertyPlanet->Connect(wxEVT_PG_CHANGED, wxPropertyGridEventHandler(BaseFrame::OnPropertyChanged), NULL, this);
}

BaseFrame::~BaseFrame()
{
    // Disconnect Events
    this->Disconnect(wxEVT_IDLE, wxIdleEventHandler(BaseFrame::OnIdle));
    m_propertyPlanet->Disconnect(wxEVT_PG_CHANGED, wxPropertyGridEventHandler(BaseFrame::OnPropertyChanged), NULL, this);

    m_mgr.UnInit();
}
