#ifndef WAYLAND_APP_H
#define WAYLAND_APP_H

#include <wayland-client.h>


class WaylandApp
{
public:
  WaylandApp(const char *name=0);
  ~WaylandApp();
  
  /**
  * Run main loop
  */
  void run();

  void display(wl_display *d) { m_display = d; }
  wl_display *display() { return m_display; }
  void compositor(wl_compositor *c) { m_compositor = c; }
  wl_compositor *compositor() { return m_compositor; }
  void shell(wl_shell *s) { m_shell = s; }
  wl_shell *shell() { return m_shell; }
  void shm(wl_shm *s) { m_shm = s; }
  wl_shm *shm() { return m_shm; }


private:
  wl_display *m_display;
  wl_registry *m_registry;
  wl_compositor *m_compositor;
  wl_shell *m_shell;
  wl_shm *m_shm;
};

#endif /* WAYLAND_APP_H */
