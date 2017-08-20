#ifndef WAYLAND_SURFACE_H
#define WAYLAND_SURFACE_H

#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "wayland_app.h"
#include "surface.h"


class WaylandSurface : public ISurface
{
public:
  WaylandSurface(WaylandApp *app, int width=800, int height=600);
  virtual ~WaylandSurface();

  /**
   * Maps as toplevel surface
   */
  void show();

  /**
   * Redraw surface
   */
  void redraw() override;

  /**
   * Redraw handler
   */
  virtual void repaint()
  {
    glClearColor(0.5f, 0.5f, 0.0f, 0.7f);
    glClear(GL_COLOR_BUFFER_BIT);

    // render something
    // ...
  }

private:
  WaylandApp *m_app;
  wl_surface *m_surface;
  wl_shell_surface *m_shellSurface;
  wl_egl_window *m_eglWindow;
  EGLSurface m_eglSurface;
  int m_width;
  int m_height;
};

#endif /* WAYLAND_SURFACE_H */
