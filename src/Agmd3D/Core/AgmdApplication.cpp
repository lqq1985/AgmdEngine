/*
============================================================================
Agmd3D - 3D Engine
Author : Cyril Basset (basset.cyril@gmail.com - https://github.com/Agamand)
https://github.com/Agamand/AgmdEngine
============================================================================
*/

#ifdef __BORLANDC__
#pragma hdrstop
#endif
#ifdef USE_WX
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#endif // USE_WX
//#define MULTITHREADING

#include <Core/SceneMgr/ASceneMgr.h>
#include <Core/AgmdApplication.h>
#include <Core/Driver.h>
#include <Core/Enums.h>
#include <Core/MediaManager.h>
#include <Core/ResourceManager.h>
//#include <Windowsx.h>
#include <time.h>
#include <Loaders/Loaders.h>
#include <Core/Camera/FPCamera.h>
#include <Core/Camera/TPCamera.h>
#include <Core/GUI/GUIMgr.h>
#include <Core/RenderingMode/RenderingMode.h>
#include <Core/Model/Material.h>
#include <Core/Shader/ShaderPipeline.h>
#include <Core/Tools/Statistics.h>
#include <Debug/Profiler.h>
#include <Editor/EditorFrame.h>
#include <Core/Controller/Controller.h>
#include <Thread/ThreadPool.h>
#include <windowsx.h>


using namespace AgmdUtilities;

#ifdef USE_WX
/*
bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    // Create the main frame window
    m_frame = new AgmdFrame(m_app,NULL, wxT("wxWidgets Penguin Sample"),
        wxDefaultPosition, wxDefaultSize);

#if wxUSE_ZLIB
    if (wxFileExists(wxT("penguin.dxf.gz")))
        m_frame->GetCanvas()->LoadDXF(wxT("penguin.dxf.gz"));
#else
    if (wxFileExists(wxT("penguin.dxf")))
        frame->GetCanvas()->LoadDXF(wxT("penguin.dxf"));
#endif
    
    
    m_frame->Show(true);

    return true;
}

MyApp::MyApp( Agmd::AgmdApplication* app )
{
    m_app = app;
}*/

//IMPLEMENT_APP(MyApp)

// ---------------------------------------------------------------------------
// AgmdFrame
// ---------------------------------------------------------------------------
bool Application::OnInit()
    {
        if ( !wxApp::OnInit() )
            return false;

// Create the main frame window
/* Show the frame */
        

        return true;
    }
    Application::Application(bool dframe, wxString& str) : m_dframe(dframe),m_name(str),m_frame(NULL)
    {}


    wxBEGIN_EVENT_TABLE(AgmdFrame, wxFrame)
    EVT_MENU(wxID_OPEN, AgmdFrame::OnMenuFileOpen)
    EVT_MENU(wxID_EXIT, AgmdFrame::OnMenuFileExit)
    EVT_MENU(wxID_HELP, AgmdFrame::OnMenuHelpAbout)
    wxEND_EVENT_TABLE()

// AgmdFrame constructor
    AgmdFrame::AgmdFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style)
    : wxFrame(frame, wxID_ANY, title, pos, size, style)
{
    SetIcon(wxICON(sample));

// Make the "File" menu
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(wxID_OPEN, wxT("&Open..."));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, wxT("E&xit\tALT-X"));
// Make the "Help" menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_HELP, wxT("&About"));

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(helpMenu, wxT("&Help"));
    SetMenuBar(menuBar);

    Show(true);

    m_canvas = new GLCanvas(this, wxID_ANY, wxDefaultPosition,
        GetClientSize(), wxSUNKEN_BORDER);
    m_canvas->Show(true);
    m_canvas->InitGL();
}

// File|Open... command
void AgmdFrame::OnMenuFileOpen( wxCommandEvent& WXUNUSED(event) )
{
    wxString filename = wxFileSelector(wxT("Choose DXF Model"), wxT(""), wxT(""), wxT(""),
#if wxUSE_ZLIB
        wxT("DXF Drawing (*.dxf;*.dxf.gz)|*.dxf;*.dxf.gz|All files (*.*)|*.*"),
#else
        wxT("DXF Drawing (*.dxf)|*.dxf)|All files (*.*)|*.*"),
#endif
        wxFD_OPEN);
    if (!filename.IsEmpty())
    {
        m_canvas->LoadDXF(filename);
//                                                                                                                                                                                                                        ->Refresh(false);
    }
}

// File|Exit command
void AgmdFrame::OnMenuFileExit( wxCommandEvent& WXUNUSED(event) )
{
// true is to force the frame to close
    Close(true);
}

// Help|About command
void AgmdFrame::OnMenuHelpAbout( wxCommandEvent& WXUNUSED(event) )
{
    wxMessageBox(wxT("Agmd Sample"));
}
// ---------------------------------------------------------------------------
// TestGLCanvas
// ---------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
    EVT_SIZE(GLCanvas::OnSize)
    EVT_PAINT(GLCanvas::OnPaint)
    EVT_ERASE_BACKGROUND(GLCanvas::OnEraseBackground)
    EVT_MOUSE_EVENTS(GLCanvas::OnMouse)
    EVT_KEY_DOWN(GLCanvas::keyPressed)
    EVT_KEY_UP(GLCanvas::keyReleased)
//EVT_MOTION(GLCanvas::OnMouseMotion)
    EVT_IDLE(GLCanvas::OnIdle)
wxEND_EVENT_TABLE()

GLCanvas::GLCanvas(wxWindow *parent,
    wxWindowID id,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name)
    : wxGLCanvas(parent, id, NULL, pos, size,
    style | wxFULL_REPAINT_ON_RESIZE, name)
{
    m_application = Agmd::AgmdApplication::getApplication();
// Explicitly create a new rendering context instance for this canvas.
    m_glRC = new wxGLContext(this);
    this->SetExtraStyle( wxWS_EX_PROCESS_IDLE );
    SetCurrent(*m_glRC);

}

GLCanvas::~GLCanvas()
{
    delete m_glRC;
}

void GLCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    if(!m_application->isReady())
        return;
    wxPaintDC dc(this);
    SetCurrent(*m_glRC);
    m_application->draw();
    SwapBuffers();
//printf("redraw !! \n");
}

void GLCanvas::OnSize(wxSizeEvent& WXUNUSED(event))
{
    SetCurrent(*m_glRC);
    int w, h;
    GetClientSize(&w, &h);
//xprintf("%d, %d\n",w,h);
    m_application->onResize(ivec2(w,h));
    Refresh(false);
}

void GLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{

}


void GLCanvas::LoadDXF(const wxString& filename)
{

}

void GLCanvas::OnMouse(wxMouseEvent& event)
{
    if(event.IsButton())
        m_application->OnClick(event.GetButton(),vec2(event.m_x,event.m_y),!event.ButtonDown(event.GetButton()));
    else m_application->OnMove(vec2(event.m_x,event.m_y));
    event.Skip();
}
void GLCanvas::OnMouseMotion(wxMouseEvent& event)
{
    m_application->OnMove(vec2(event.m_x,event.m_y));
}
void GLCanvas::OnKeyDown(wxKeyEvent& event)
{

    wxChar uc = event.GetUnicodeKey();
    if ( uc != WXK_NONE )
    {
        m_application->OnKey(uc,false);
    }
    else // No Unicode equivalent.
    {
        m_application->OnKey(event.GetKeyCode(),false);
    }
    
}
void GLCanvas::OnKeyUp(wxKeyEvent& event)
{
    wxChar uc = event.GetUnicodeKey();
    if ( uc != WXK_NONE )
    {
        m_application->OnKey(uc,true);
    }
    else // No Unicode equivalent.
    {
        m_application->OnKey(event.GetKeyCode(),true);
    }
}
void GLCanvas::keyPressed(wxKeyEvent& event) {
//printf("%c pressed\n",event.GetKeyCode());
    m_application->OnKey(event.GetKeyCode(),false);
}
void GLCanvas::keyReleased(wxKeyEvent& event) {
//printf("%c release\n",event.GetKeyCode());
    m_application->OnKey(event.GetKeyCode(),true);
}
void GLCanvas::InitGL()
{
    SetCurrent(*m_glRC);
}

void GLCanvas::ResetProjectionMode()
{

}

void GLCanvas::OnIdle( wxIdleEvent& event )
{
    Refresh(false);
}
#endif


namespace Agmd
{
    void UpdateJob(); // JOB for multiTread
    void RenderJob(); // JOB for multiTread
    void UpdateAndRender(); // monothread render
    AgmdApplication* AgmdApplication::s_application = NULL;

    AgmdApplication::AgmdApplication(ivec2 screenSize, ADriverType d) :
        m_Instance(GetModuleHandle("Agmd3D.dll")),
#ifndef USE_WX
        m_Hwnd(NULL),
#else
    m_frame            (NULL),
    m_glcanvas        (NULL),
#endif
        m_driverType(d),
        m_isRunning(true),
        camera(NULL),
        last_mouse_pos(0, 0),
        mouseState(MOUSE_NONE),
        m_lastTime(0),
        m_fps(0),
        m_fpsTimer(SECONDS_IN_MS),
        m_deltaTime(0),
        m_ScreenSize(screenSize),
        m_createDefaultFrame(false),
        m_isReady(false),
        m_frameName("Agmd sample application"),
        m_sceneController(NULL)

    {
        assert(m_Instance != NULL);

        s_application = this;
        RegisterLoaders();
    }

    AgmdApplication::AgmdApplication(const std::string& frameName, ADriverType d) :
        m_Instance(GetModuleHandle("Agmd3D.dll")),

#ifndef USE_WX
        m_Hwnd(NULL),
#else
        m_frame            (NULL),
        m_glcanvas        (NULL),
        m_createDefaultFrame(true),
#endif
        m_driverType(d),
        m_isRunning(true),
        camera(NULL),
        last_mouse_pos(0, 0),
        mouseState(MOUSE_NONE),
        m_lastTime(0),
        m_fps(0),
        m_fpsTimer(SECONDS_IN_MS),
        m_deltaTime(0),
        m_isReady(false),
        m_ScreenSize(1920, 1080),
        m_frameName(frameName),
        m_sceneController(NULL)
    {
        assert(m_Instance != NULL);
        s_application = this;
        RegisterLoaders();
    }

    AgmdApplication::~AgmdApplication()
    {
        //         if (m_Hwnd)
        //         {
        //             DestroyWindow(m_Hwnd);
        //             UnregisterClass(L"AgmdEngine", m_Instance);
        //         }
    }


    bool AgmdApplication::OnInit()
    {
        return true;
    }

    void AgmdApplication::Run(int argc, char** argv)
    {
        m_threadPool = new CThreadPool();
#ifdef USE_WX
        m_wxApplication = new Application(m_createDefaultFrame,wxString(m_frameName));
#endif        
        if (m_driverType == DRIVER_DX11)
            m_renderer = new Plugin("DXRender.dll");
        else
            m_renderer = new Plugin("GLRender.dll");


#ifdef USE_WX
        wxApp::SetInstance(m_wxApplication);
        wxEntryStart( argc, argv );
        m_wxApplication->OnInit();
#endif
        //new BaseFrame(NULL);
        MakeWindow();

#ifdef USE_WX
        if(!m_frame)
        {
#ifdef USE_EDITOR
            m_frame = new EditorFrame(NULL);
#else
            m_frame =new AgmdFrame(NULL, wxString(m_frameName),
                wxDefaultPosition, wxDefaultSize);
#endif // USE_EDITOR
            m_frame->Show(true);
        }else m_frame->Show(true);
        Driver::Get().Initialize(NULL);
#else 
        Driver::Get().Initialize(m_Hwnd);
#endif

        Driver::Get().SetScreen(m_ScreenSize);

        Material* mat = new Material(ShaderPipeline::GetDefaultPipeline());
        Image img = Image();
        img.Fill(Color::white);
        Texture t = Texture();
        t.CreateFromImage(img, PXF_A8R8G8B8, 0, "white_texture");
        mat->SetTexture(t, 0, TRenderPass::RENDERPASS_DEFERRED);
        ResourceManager::Instance().Add("DEFAULT_MATERIAL", mat);

        init();
#ifdef USE_WX
        if(m_frame)
            m_isReady = true;
        m_wxApplication->MainLoop();
#else   

        //allocate RenderQueue;

        m_renderQueuePool = new ARenderQueue[2];
        m_renderQueue[0] = m_renderQueuePool;
        m_renderQueue[1] = m_renderQueuePool + 1;
#ifdef MULTITHREADING
		m_threadPool->start();
		m_threadPool->AddJobToPool(UpdateJob);
#endif
        MainLoop();
#endif
    }

    void AgmdApplication::Exit()
    {
        m_isRunning = false;
    }

    void AgmdApplication::MakeWindow()
    {
        const int left = (GetDeviceCaps(GetDC(NULL), HORZRES) - m_ScreenSize.x) / 2;
        const int top = (GetDeviceCaps(GetDC(NULL), VERTRES) - m_ScreenSize.y) / 2;


        WNDCLASSEX WindowClass;
        WindowClass.cbSize = sizeof(WNDCLASSEX);
        WindowClass.style = 0;
        WindowClass.lpfnWndProc = GlobalWindowProc;
        WindowClass.cbClsExtra = 0;
        WindowClass.cbWndExtra = 0;
        WindowClass.hInstance = m_Instance;
        WindowClass.hIcon = NULL;
        WindowClass.hCursor = 0;
        WindowClass.hbrBackground = 0;
        WindowClass.lpszMenuName = NULL;
        WindowClass.lpszClassName = ENGINE_NAME;
        WindowClass.hIconSm = NULL;
        ATOM a = RegisterClassEx(&WindowClass);

        m_Hwnd = CreateWindow(ENGINE_NAME, ENGINE_NAME, 0, left, top, m_ScreenSize.x, m_ScreenSize.y, NULL, NULL, m_Instance, NULL);
        auto err = GetLastError();
        assert(m_Hwnd != NULL);
        ShowWindow(m_Hwnd, SW_NORMAL);
        LONG lStyle = GetWindowLong(m_Hwnd, GWL_STYLE);
        lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
        SetWindowLong(m_Hwnd, GWL_STYLE, lStyle);
        LONG lExStyle = GetWindowLong(m_Hwnd, GWL_EXSTYLE);
        lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
        SetWindowLong(m_Hwnd, GWL_EXSTYLE, lExStyle);
        SetWindowPos(m_Hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
        ShowCursor(0);
    }
#ifdef USE_WINAPI
    void AgmdApplication::MainLoop()
    {
        MSG Message;
        while (m_isRunning)
        {
            if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }
            else
            {
#ifndef MULTITHREADING
                UpdateAndRender();
#endif
                //draw();


                //uncomment following to have monothread engine
                //UpdateJob();
                //Render();
            }
        }
    }


    LRESULT CALLBACK AgmdApplication::GlobalWindowProc(HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam)
    {
        return s_application->WindowProc(Hwnd, Message, WParam, LParam);
    }


    LRESULT CALLBACK AgmdApplication::WindowProc(HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam)
    {
        GUIMgr& guimgr = GUIMgr::Instance();
        camera = Camera::getCurrent();
        if (Message == WM_CHAR)
        {
        }
        switch (Message)
        {
        case WM_DESTROY:
            Exit();
            return 0;
        case WM_KEYUP:
        case WM_KEYDOWN:

            OnKey((char)LOWORD(WParam), Message == WM_KEYUP ? true : false);
            return 0;
        case WM_XBUTTONDOWN:
            OnClick(0, vec2(last_mouse_pos), false);
            return 0;
        case WM_RBUTTONDOWN:
            OnClick(3, vec2(last_mouse_pos), false);
            return 0;
        case WM_MBUTTONDOWN:
            OnClick(2, vec2(last_mouse_pos), false);
            return 0;
        case WM_LBUTTONDOWN:
            OnClick(1, vec2(last_mouse_pos), false);
            return 0;

        case WM_XBUTTONUP:
            OnClick(0, vec2(last_mouse_pos), true);
            return 0;
        case WM_RBUTTONUP:
            OnClick(3, vec2(last_mouse_pos), true);
            return 0;
        case WM_MBUTTONUP:

            OnClick(2, vec2(last_mouse_pos), true);
            return 0;
        case WM_LBUTTONUP:
            OnClick(1, vec2(last_mouse_pos), true);
            return 0;
        case WM_MOUSEWHEEL:
            //camera->onMouseWheel((float)GET_WHEEL_DELTA_WPARAM(WParam));
            return 0;
        case WM_MOUSEMOVE:
            ivec2 posDiff = last_mouse_pos - ivec2(GET_X_LPARAM(LParam), m_ScreenSize.y - GET_Y_LPARAM(LParam));
            vec2 pos(GET_X_LPARAM(LParam), m_ScreenSize.y - GET_Y_LPARAM(LParam));
            OnMove(pos);
            last_mouse_pos.x = GET_X_LPARAM(LParam);
            last_mouse_pos.y = m_ScreenSize.y - GET_Y_LPARAM(LParam);
            return 0;
        }
        return DefWindowProc(Hwnd, Message, WParam, LParam);
    }
#endif


    void AgmdApplication::Render(ARenderQueue& q)
    {
        RenderingMode* current = RenderingMode::getRenderingMode();
        Driver& driver = Driver::Get();
        driver.InitScene();
        if (current != NULL)
            current->compute(q);
        driver.EndScene();
    }

    void AgmdApplication::UpdateTransform()
    {
        Driver& driver = Driver::Get();
        ASceneMgr* scene = driver.GetActiveScene();
        for (a_uint32 i = 0, len = scene->m_nodePool.size(); i < len; ++i)
        {
            ANode& node = scene->m_nodePool[i];
            if (node.GetTransform().needUpdate())
            {
                node.GetTransform().update(node.m_parent ? &node.m_parent->GetTransform() : NULL);
                node.m_dirtyFlag |= DTRANSFORM_UPDATE;
                //if(node.m_camera)
                //node.m_camera->updateBuffer(node.GetTransform().modelMatrix()*node.m_camera->look());
            }
        }
    }

    void AgmdApplication::UpdateBounds()
    {
        Driver& driver = Driver::Get();
        ASceneMgr* scene = driver.GetActiveScene();

        //need to be reversed sceneTree;
        for (a_uint32 i = 0, len = scene->m_nodePool.size(); i < len; ++i)
        {
            ANode& node = scene->m_nodePool[i];
            if (node.m_dirtyFlag & DTRANSFORM_UPDATE)
            {
                node.m_dirtyFlag = (node.m_dirtyFlag & ~DTRANSFORM_UPDATE);//  | DBOUNDS_UPDATE; // DBOUNDS_UPDATE not used now 
                node.m_localBounds = node.m_baseBounds.GetTransformedBounding(node.GetTransform().localModelMatrix());
                node.m_globalBounds = node.m_baseBounds.GetTransformedBounding(node.GetTransform().modelMatrix());
                if (node.m_model) //if node is renderable
                    scene->m_octree.InsertElement(&node);
                //if(node.m_camera)
                //node.m_camera->updateBuffer(node.GetTransform().modelMatrix()*node.m_camera->look());
            }
        }
    }

    void AgmdApplication::UpdateScript(a_uint32 time)
    {
        Driver& driver = Driver::Get();
        ASceneMgr* scene = driver.GetActiveScene();
        for (a_uint32 i = 0, len = scene->m_nodePool.size(); i < len; ++i)
        {
            ANode& node = scene->m_nodePool[i];
            if (node.m_controller)
                node.m_controller->update(time);
        }
    }

    void AgmdApplication::FillRenderQueue(ARenderQueue& queue)
    {
        Driver& driver = Driver::Get();
        ASceneMgr* scene = driver.GetActiveScene();
        queue.Clear();
        for (a_uint32 i = 0, len = scene->m_nodePool.size(); i < len; ++i)
        {
            ANode& node = scene->m_nodePool[i];
            if (node.m_model) //if node is renderable
                queue.Push(&node);
        }
    }

    void AgmdApplication::Culling(ARenderQueue& queue)
    {
        Driver& driver = Driver::Get();
        ASceneMgr* scene = driver.GetActiveScene();
        Camera* cam = Camera::getCurrent(CAMERA_3D);
        queue.Clear();
        Frustum* frustrum = cam->GetFrustrum();

        if (scene->m_octree.Empty())
            return;

        std::stack<OcTree<ANode>*> _stack;
        _stack.push(scene->m_octree.Top());

        while (_stack.size())
        {
            OcTree<ANode>* current = _stack.top();
            _stack.pop();

            if (frustrum->IsIn(current->m_bbox))
            {
                for (a_uint32 i = 0, len = current->m_data.size(); i < len; ++i)
                {
                    ANode* node = current->m_data[i];
                    if (frustrum->IsIn(node->m_globalBounds))
                        queue.Push(node); // ok visible push to renderqueue
                }


                for (a_uint32 i = 0; i < 8; ++i)
                {
                    if ((a_uint32)(current->m_children[i]) == 0xcdcdcdcd)
                        printf("");
                    if (current->m_children[i])
                        _stack.push(current->m_children[i]);
                }
            }
        }
    }


    enum ProcessFlags
    {
        PROCESS_UPDATE = 0x01,
        PROCESS_RENDER = 0x02,
        ALL_PROCESS = PROCESS_UPDATE | PROCESS_RENDER
    };

    a_uint32 process_flag = PROCESS_UPDATE;
    a_uint32 ujob = 0,
        rjob = 0;

    void RestartJob()
    {
        process_flag |= ALL_PROCESS;
        auto app = AgmdApplication::getApplication();
        app->m_threadPool->AddJobToPool(UpdateJob);
        app->m_threadPool->AddJobToPool(RenderJob);
    }

    void UpdateJob()
    {
        a_uint32 id = ++ujob;
        //printf("UpdateJob, %i\n",id);
        auto app = AgmdApplication::getApplication();


        a_uint32 time = clock();
        a_uint32 time_diff = time - app->m_lastTime;
        app->m_deltaTime = time_diff;
        //do some stuff
        AgmdApplication::UpdateScript(time_diff);
        AgmdApplication::UpdateTransform();
        AgmdApplication::UpdateBounds();
        ARenderQueue& queue = *app->m_renderQueue[0];
        AgmdApplication::Culling(queue);
        app->m_lastTime = time;
        app->m_threadPool->GetMutex().Lock();
        std::swap(app->m_renderQueue[0], app->m_renderQueue[1]);
        process_flag &= ~PROCESS_UPDATE;
        //printf("UpdateJob, %i ended pflag : %i\n",id,process_flag);
        if (!(process_flag & PROCESS_RENDER))
        {
            //printf("UpdateJob, %i start new job\n",id);
            RestartJob();
        }
        app->m_threadPool->GetMutex().Unlock();
    }

    void RenderJob()
    {
        a_uint32 id = ++rjob;
        //printf("RenderJob, %i\n",id);
        auto app = AgmdApplication::getApplication();
        app->m_threadPool->GetMutex().Lock();
        Camera* c = Camera::getCurrent();
        c->updateBuffer(c->getNode()->GetTransform().modelMatrix() * c->look());
        ARenderQueue& queue = *app->m_renderQueue[1];
        app->m_threadPool->GetMutex().Unlock();
        AgmdApplication::Render(queue);
        app->m_threadPool->GetMutex().Lock();
        process_flag &= ~PROCESS_RENDER;


        //printf("RenderJob, %i ended pflag : %i\n",id,process_flag);
        if (!(process_flag & PROCESS_UPDATE))
        {
            //printf("RenderJob, %i start new job\n",id);
            RestartJob();
        }
        app->m_threadPool->GetMutex().Unlock();
    }

    void UpdateAndRender()
    {
        //Update Pass
        auto app = AgmdApplication::getApplication();
        a_uint32 time = clock();
        a_uint32 time_diff = time - app->m_lastTime;
        app->m_deltaTime = time_diff;
        //do some stuff
        AgmdApplication::UpdateScript(time_diff);
        AgmdApplication::UpdateTransform();
        AgmdApplication::UpdateBounds();
        ARenderQueue& updateQueue = *app->m_renderQueue[0];
        AgmdApplication::Culling(updateQueue);
        app->m_lastTime = time;
        std::swap(app->m_renderQueue[0], app->m_renderQueue[1]);

        //Render Pass
        Camera* c = Camera::getCurrent();
        c->updateBuffer(c->getNode()->GetTransform().modelMatrix() * c->look());
        ARenderQueue& renderQueue = *app->m_renderQueue[1];
        AgmdApplication::Render(renderQueue);
    }


    void AgmdApplication::draw()
    {
        //         Timer timer;
        //         Timer global_timer;
        //         global_timer.start();
        //         int frame = 0;
        //         float renderTime = 0, guiTime = 0;
        //         a_uint32 time = clock();
        //         a_uint32 time_diff = time - m_lastTime;
        //         m_deltaTime = time_diff;
        //         m_lastTime = time;
        //         Driver& render =  Driver::Get();
        //         render.GetStatistics().ResetStatistics();
        //         render.OnUpdate(time_diff);
        //         GUIMgr::Instance().Update(time_diff);
        //         OnUpdate(time_diff);
        //         render.InitScene();
        //         //Render 3D objects
        //         timer.start();
        //         RenderingMode* current = RenderingMode::getRenderingMode();
        //         if(current != NULL)
        //             current->compute();
        //         OnRender3D();
        //         renderTime = (float)timer.getElapsedTimeInMicroSec();
        //         timer.stop();
        //         timer.start();
        //         //Render 2D GUI
        //         GUIMgr::Instance().DrawGUI();
        //         OnRender2D();
        //         timer.stop();
        //         guiTime = (float)timer.getElapsedTimeInMicroSec();
        // 
        //         frame++;
        //         //if(time_diff < 15)
        //         //Sleep(15-time_diff);
        //         render.EndScene();
        //         global_timer.stop();
        //         if(m_fpsTimer <= time_diff)
        //         {
        //             m_fps = ((float)frame*SECONDS_IN_MS)/(SECONDS_IN_MS + time_diff - m_fpsTimer);
        //             m_fpsTimer = SECONDS_IN_MS;
        //             render.GetStatistics().SetMainTime((float)time_diff);
        //             render.GetStatistics().SetRenderingTime(renderTime);
        //             render.GetStatistics().SetGuiTime(guiTime);
        //         }else m_fpsTimer -= time_diff;
    }

    void AgmdApplication::OnClick(int click, vec2 pos, bool up)
    {
        GUIMgr& guimgr = GUIMgr::Instance();
        camera = Camera::getCurrent();
        //printf("click %i, up : %i\n",click,up);
#if defined(USE_EDITOR) && defined(USE_WX)
        ((EditorFrame*)m_frame)->OnClick(click,pos,up);

#endif
        switch (click)
        {
        case 1:
            if (up)
                mouseState &= ~MOUSE_LEFT;
            else mouseState |= MOUSE_LEFT;
            for (a_uint32 i = 0, len = m_inputListener.size(); i < len; i++)
                m_inputListener[i]->OnClick(MOUSE_LEFT, mouseState, vec2(pos) / vec2(m_ScreenSize) * 2.0f - vec2(1), up);
            guimgr.AddEvent(EventEntry(EV_ON_MOUSE_BUTTON, ivec2(pos.x, m_ScreenSize.y - pos.y), ivec2(0), mouseState, 0));
            return;

        case 2:
            if (up)
                mouseState &= ~MOUSE_MIDDLE;
            else mouseState |= MOUSE_MIDDLE;
            for (a_uint32 i = 0, len = m_inputListener.size(); i < len; i++)
                m_inputListener[i]->OnClick(MOUSE_MIDDLE, mouseState, vec2(pos) / vec2(m_ScreenSize) * 2.0f - vec2(1), up);
            guimgr.AddEvent(EventEntry(EV_ON_MOUSE_BUTTON, ivec2(pos.x, m_ScreenSize.y - pos.y), ivec2(0), mouseState, 0));
            return;
        case 3:
            if (up)
                mouseState &= ~MOUSE_RIGHT;
            else mouseState |= MOUSE_RIGHT;
            for (a_uint32 i = 0, len = m_inputListener.size(); i < len; i++)
                m_inputListener[i]->OnClick(MOUSE_RIGHT, mouseState, vec2(last_mouse_pos) / vec2(m_ScreenSize) * 2.0f - vec2(1), up);
            guimgr.AddEvent(EventEntry(EV_ON_MOUSE_BUTTON, ivec2(last_mouse_pos.x, m_ScreenSize.y - last_mouse_pos.y), ivec2(0), mouseState, 0));
            return;
            /*case WM_XBUTTONDOWN:
                mouseState |= MOUSE_NONE;
                for(a_uint32 i = 0, len = m_inputListener.size(); i < len; i++)
                    m_inputListener[i]->OnClick(MOUSE_NONE,mouseState,vec2(last_mouse_pos)/vec2(m_ScreenSize)*2.0f-vec2(1),false);
                guimgr.AddEvent(EventEntry(EV_ON_MOUSE_BUTTON,last_mouse_pos,ivec2(0),mouseState,0));
                return;
            case WM_RBUTTONDOWN:
                mouseState |= MOUSE_RIGHT;
                for(a_uint32 i = 0, len = m_inputListener.size(); i < len; i++)
                    m_inputListener[i]->OnClick(MOUSE_RIGHT,mouseState,vec2(last_mouse_pos)/vec2(m_ScreenSize)*2.0f-vec2(1),false);
                //OnClick(mouseState,vec2(last_mouse_pos)/vec2(m_ScreenSize));
                guimgr.AddEvent(EventEntry(EV_ON_MOUSE_BUTTON,last_mouse_pos,ivec2(0),mouseState,0));
                return;
            case WM_MBUTTONDOWN:
                mouseState |= MOUSE_MIDDLE;
                for(a_uint32 i = 0, len = m_inputListener.size(); i < len; i++)
                    m_inputListener[i]->OnClick(MOUSE_MIDDLE,mouseState,vec2(last_mouse_pos)/vec2(m_ScreenSize)*2.0f-vec2(1),false);
                camera->onMouseWheel(true);
                guimgr.AddEvent(EventEntry(EV_ON_MOUSE_BUTTON,last_mouse_pos,ivec2(0),mouseState,0));
                return;
            case WM_LBUTTONDOWN:
                mouseState |= MOUSE_LEFT;
                for(a_uint32 i = 0, len = m_inputListener.size(); i < len; i++)
                    m_inputListener[i]->OnClick(MOUSE_LEFT,mouseState,vec2(last_mouse_pos)/vec2(m_ScreenSize)*2.0f-vec2(1),false);
                //OnClick(mouseState,vec2(last_mouse_pos)/vec2(m_ScreenSize));
                guimgr.AddEvent(EventEntry(EV_ON_MOUSE_BUTTON,last_mouse_pos,ivec2(0),mouseState,0));
                return;
    
            case WM_XBUTTONUP:
                mouseState &= ~MOUSE_NONE;
                for(a_uint32 i = 0, len = m_inputListener.size(); i < len; i++)
                    m_inputListener[i]->OnClick(MOUSE_NONE,mouseState,vec2(last_mouse_pos)/vec2(m_ScreenSize)*2.0f-vec2(1),true);
                guimgr.AddEvent(EventEntry(EV_ON_MOUSE_BUTTON,last_mouse_pos,ivec2(0),mouseState,0));
                return;
            case WM_RBUTTONUP:
                mouseState &= ~MOUSE_RIGHT;
                for(a_uint32 i = 0, len = m_inputListener.size(); i < len; i++)
                    m_inputListener[i]->OnClick(MOUSE_RIGHT,mouseState,vec2(last_mouse_pos)/vec2(m_ScreenSize)*2.0f-vec2(1),true);
                guimgr.AddEvent(EventEntry(EV_ON_MOUSE_BUTTON,last_mouse_pos,ivec2(0),mouseState,0));
                return;
            case WM_MBUTTONUP:
    
                mouseState &= ~MOUSE_MIDDLE;
                for(a_uint32 i = 0, len = m_inputListener.size(); i < len; i++)
                    m_inputListener[i]->OnClick(MOUSE_MIDDLE,mouseState,vec2(last_mouse_pos)/vec2(m_ScreenSize)*2.0f-vec2(1),true);
                camera->onMouseWheel(false);
                guimgr.AddEvent(EventEntry(EV_ON_MOUSE_BUTTON,last_mouse_pos,ivec2(0),mouseState,0));
                return;
            case WM_LBUTTONUP:
                mouseState &= ~MOUSE_LEFT;
                for(a_uint32 i = 0, len = m_inputListener.size(); i < len; i++)
                    m_inputListener[i]->OnClick(MOUSE_LEFT,mouseState,vec2(last_mouse_pos)/vec2(m_ScreenSize)*2.0f-vec2(1),true);
                guimgr.AddEvent(EventEntry(EV_ON_MOUSE_BUTTON,last_mouse_pos,ivec2(0),mouseState,0));
                return;*/
        }
    }

    void AgmdApplication::OnMove(vec2 pos)
    {
        GUIMgr& guimgr = GUIMgr::Instance();
        camera = Camera::getCurrent();

        ivec2 posDiff = last_mouse_pos - ivec2(pos);
        for (a_uint32 i = 0, len = m_inputListener.size(); i < len; i++)
            m_inputListener[i]->OnMouseMotion(vec2(last_mouse_pos) / vec2(m_ScreenSize) * 2.0f - vec2(1), posDiff);
        //if(mouseState & MOUSE_RIGHT || mouseState & MOUSE_MIDDLE)
        //camera->onMouseMotion(posDiff.x, posDiff.y);
#if defined(USE_EDITOR) && defined(USE_WX)
        ((EditorFrame*)m_frame)->OnMove(pos,posDiff,mouseState);

#endif
        last_mouse_pos.x = (int)pos.x;
        last_mouse_pos.y = (int)pos.y;
        //OnMove(vec2(last_mouse_pos)/vec2(m_ScreenSize)*2.f-vec2(1));
        guimgr.AddEvent(EventEntry(EV_ON_MOUVE_MOVE, ivec2(last_mouse_pos.x, m_ScreenSize.y - last_mouse_pos.y), ivec2(posDiff.x, -posDiff.y), mouseState, 0));
    }

    void AgmdApplication::OnKey(a_char key, bool up)
    {
        camera = Camera::getCurrent();
        for (a_uint32 i = 0, len = m_inputListener.size(); i < len; i++)
            m_inputListener[i]->OnKey((char)key, up);
#if defined(USE_EDITOR) && defined(USE_WX)
        ((EditorFrame*)m_frame)->OnKey(key,up);

#endif
        //camera->onKeyboard(key,up);
        switch (key)
        {
        case VK_F1:
            RenderingMode::getRenderingMode()->setRenderMode(MODE_FILL);
            return;
        case VK_F2:
            RenderingMode::getRenderingMode()->setRenderMode(MODE_LINE);
            return;
        case VK_F3:
            RenderingMode::getRenderingMode()->setRenderMode(MODE_POINT);
            return;
        }
    }

    void AgmdApplication::onResize(ivec2 size)
    {
        m_ScreenSize = size;
        Driver::Get().SetViewPort(ivec2(0, 0), size);
        Driver::Get().SetScreen(m_ScreenSize);
        Camera* c;
        if (c = Camera::getCurrent(CAMERA_3D))
            c->resize(vec2(size));
        if (c = Camera::getCurrent(CAMERA_2D))
            c->resize(vec2(size));
        //        RenderingMode*mode;
        //         if((mode = RenderingMode::getRenderingMode()) != NULL)
        //         {
        //             mode->init();
        //         }
    }

#ifdef  USE_WX
    void AgmdApplication::CreateGlCanvas( wxWindow* frame )
    {
        if(!frame)
            return;
        m_glcanvas = new GLCanvas(frame);
        frame->GetSizer()->Add(m_glcanvas, 1, wxEXPAND | wxALL, 5);
    }

    wxFrame* AgmdApplication::getWxFrame()
    {
        return m_frame;
    }



    

#endif //USE_WX
}
