#include "wayland_surface.h"


WaylandSurface::WaylandSurface(WaylandApp *app, int width, int height) :
    m_surface(0),
    m_shellSurface(0),
    m_eglWindow(0),
    m_width(width),
    m_height(height),
    m_app(app)
{
  m_surface = wl_compositor_create_surface(app->compositor());
  m_shellSurface = wl_shell_get_shell_surface(app->shell(), m_surface);

  m_eglWindow = wl_egl_window_create(m_surface, m_width, m_height);
  m_eglSurface = eglCreateWindowSurface(
    app->eglDisplay(),
    app->eglConfig(),
    m_eglWindow,
    0
  );

  app->schedule(this);
}


WaylandSurface::~WaylandSurface()
{
  eglMakeCurrent(m_app->eglDisplay(), EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  eglDestroySurface(m_app->eglDisplay(), m_eglSurface);
  wl_egl_window_destroy(m_eglWindow);
  wl_shell_surface_destroy(m_shellSurface);
  wl_surface_destroy(m_surface);
}


void
WaylandSurface::show()
{
  wl_shell_surface_set_toplevel(m_shellSurface);
}


void
WaylandSurface::redraw()
{
  eglMakeCurrent(m_app->eglDisplay(), m_eglSurface, m_eglSurface, m_app->eglContext());
  repaint();
  eglSwapBuffers(m_app->eglDisplay(), m_eglSurface);
}
