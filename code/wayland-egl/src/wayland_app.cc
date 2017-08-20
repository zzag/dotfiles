#include <cstring>
#include <string>
#include "wayland_app.h"


#define ENSURE(x) if (!(x)) { return -1; }


void handleGlobal(void *data,
     struct wl_registry *registry,
     uint32_t name,
     const char *interface,
     uint32_t version)
 {
   WaylandApp *app = reinterpret_cast<WaylandApp *>(data);

   // bind global objects
   if (!strcmp(interface, "wl_compositor")) {
     app->compositor(reinterpret_cast<wl_compositor *>(
       wl_registry_bind(registry, name, &wl_compositor_interface, version)
     ));
   }
   else if (!strcmp(interface, "wl_shell")) {
     app->shell(reinterpret_cast<wl_shell *>(
       wl_registry_bind(registry, name, &wl_shell_interface, version)
     ));
   }
 }


void handleGlobalRemove(void *data,
    struct wl_registry *wl_registry,
    uint32_t name)
{
}


const wl_registry_listener registryListener = {
  .global = handleGlobal,
  .global_remove = handleGlobalRemove
};


WaylandApp::WaylandApp(const char *socketName) :
    m_display(0),
    m_registry(0),
    m_compositor(0),
    m_shell(0),
    m_running(true),
    m_eglDisplay(0),
    m_eglConfig(0),
    m_eglContext(0)
{
  // open a connection to the compositor
  m_display = wl_display_connect(socketName);
  if (!m_display) { throw std::string("Unable to open a connection to the Wayland compositor"); }

  // get registry, bind a listener and wait until the compositor process requests
  m_registry = wl_display_get_registry(m_display);
  wl_registry_add_listener(m_registry, &registryListener, this);
  wl_display_roundtrip(m_display);

  // initialize GLES
  if (initializeEGL() < 0) { throw std::string("Unable to initialize EGL stuff"); }
}


WaylandApp::~WaylandApp()
{
  finalizeEGL();

  wl_shell_destroy(m_shell);
  wl_compositor_destroy(m_compositor);
  wl_registry_destroy(m_registry);
  wl_display_disconnect(m_display);
}


int
WaylandApp::initializeEGL()
{
  // get EGL display connection and initialize connection
  m_eglDisplay = eglGetDisplay((EGLNativeDisplayType)m_display);
  ENSURE(eglInitialize(m_eglDisplay, &m_major, &m_minor));
  ENSURE(eglBindAPI(EGL_OPENGL_ES_API));

  // choose config
  //  1. get total number of configs
  //  2. choose proper config
  EGLint configAttribs[] = {
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_RED_SIZE, 1,
    EGL_GREEN_SIZE, 1,
    EGL_BLUE_SIZE, 1,
    EGL_ALPHA_SIZE, 1,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
    EGL_NONE
	};
  EGLint configSize;
  EGLint numConfigs;
  EGLint bufferSize = 32;

  if (!eglGetConfigs(m_eglDisplay, 0, 0, &configSize) || configSize < 0) { return -1; }
  EGLConfig *configs = new EGLConfig[configSize];

  if (!eglChooseConfig(m_eglDisplay, configAttribs, configs, configSize, &numConfigs))
  {
    delete[] configs;
    return -1;
  }

  for (int i = 0; i < numConfigs; i++)
  {
    int size = 0;
    eglGetConfigAttrib(m_eglDisplay, configs[i], EGL_BUFFER_SIZE, &size);
    if (bufferSize == size)
    {
      m_eglConfig = configs[i];
      break;
    }
  }

  delete[] configs;
  ENSURE(m_eglConfig);

  // create context
  EGLint contextAttribs[] = {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE
  };

  m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, EGL_NO_CONTEXT, contextAttribs);
  ENSURE(m_eglContext != EGL_NO_CONTEXT);

  return 0;
}


void
WaylandApp::finalizeEGL()
{
  eglDestroyContext(m_eglDisplay, m_eglContext);
  eglTerminate(m_eglDisplay);
}


void
WaylandApp::run()
{
  while (m_running)
  {
    for (std::vector<ISurface *>::iterator iter = m_surfaces.begin(); iter < m_surfaces.end(); iter++)
    {
      (*iter)->redraw();
    }

    wl_display_dispatch_pending(m_display);
    wl_display_flush(m_display);
  }
}


void
WaylandApp::stop()
{
  m_running = false;
}


void
WaylandApp::schedule(ISurface *surface)
{
  m_surfaces.push_back(surface);
}
