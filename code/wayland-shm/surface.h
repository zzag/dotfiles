#ifndef WAYLAND_SURFACE_H
#define WAYLAND_SURFACE_H

#include "app.h"
#include <wayland-client.h>


class WaylandSurface
{
public:
  WaylandSurface(WaylandApp &app, int width, int height);
  virtual ~WaylandSurface();

  /**
   * Show surface on the screen
   */
  void show();

  /**
   * Place rendering stuff here
   */
  virtual void redraw() {
    dirty(0, 0, m_width, m_height);

    for (int i = 0; i < m_height; i++)
    {
      for (int j = 0; j < m_width; j++)
      {
        m_data[i*m_stride + j*4] = 255;
      }
    }
  }

  /**
   * Mark region as dirty
   */
  void dirty(int x, int y, int width, int height);

  wl_surface *surface() { return m_surface; }
  wl_shell_surface *shellSurface() { return m_shellSurface; }
  wl_buffer *buffer() { return m_buffer; }

private:
  int m_fd;
  wl_shm_pool *m_pool;
  wl_buffer *m_buffer;
  wl_surface *m_surface;
  wl_shell_surface *m_shellSurface;

protected:
  char *m_data;
  int m_width;
  int m_stride;
  int m_height;
  int m_format;
};

#endif /* WAYLAND_SURFACE_H */
