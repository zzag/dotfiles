#ifndef WAYLAND_APP_H
#define WAYLAND_APP_H

#include <wayland-client.h>
#include <EGL/egl.h>
#include <vector>
#include "surface.h"


class WaylandApp
{
public:
  WaylandApp(const char *socketName = 0);
  ~WaylandApp();

  /**
   * Run main loop
   */
  void run();

  /**
   * Break main loop
   */
  void stop();

  /**
   * Register a surface. `redraw()` will be called in the main loop
   */
  void schedule(ISurface *surface);

  wl_display *display() { return m_display; }
  void display(wl_display *display) { m_display = display; }

  wl_registry *registry() { return m_registry; }
  void registry(wl_registry *registry) { m_registry = registry; }

  wl_compositor *compositor() { return m_compositor; }
  void compositor(wl_compositor *compositor) { m_compositor = compositor; }

  wl_shell *shell() { return m_shell; }
  void shell(wl_shell *shell) { m_shell = shell; }

  EGLDisplay eglDisplay() { return m_eglDisplay; }
  EGLConfig eglConfig() { return m_eglConfig; }
  EGLContext eglContext() { return m_eglContext; }
  int eglMajor() { return m_major; }
  int eglMinor() { return m_minor; }

private:
  int initializeEGL();
  void finalizeEGL();

private:
  wl_display *m_display;
  wl_registry *m_registry;
  wl_compositor *m_compositor;
  wl_shell *m_shell;
  EGLDisplay m_eglDisplay;
  EGLConfig m_eglConfig;
  EGLContext m_eglContext;
  int m_major;
  int m_minor;
  std::vector<ISurface *> m_surfaces;
  bool m_running;
};

#endif /* WAYLAND_APP_H */
