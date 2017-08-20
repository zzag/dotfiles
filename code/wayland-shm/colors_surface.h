#ifndef COLORS_SURFACE_H
#define COLORS_SURFACE_H

#include "surface.h"
#include <cmath>


class ColorsSurface : public WaylandSurface
{
public:
  ColorsSurface(WaylandApp &app, int width, int height) :
    WaylandSurface(app, width, height)
  {
    m_time = 0;
  }

  void redraw() override
  {
    dirty(0, 0, m_width, m_height);

    for (int i = 0; i < m_height; i++)
    {
      for (int j = 0; j < m_width; j++)
      {
        int offset = i*m_stride + j*4;
        m_data[offset + 3] = static_cast<char>(fabs(sin(m_time * 0.016 + 0.8)) * 255);
        m_data[offset + 2] = static_cast<char>(fabs(sin(m_time * 0.016 + 0.2)) * 255);
        m_data[offset + 1] = static_cast<char>(fabs(sin(m_time * 0.016)) * 255);
      }
    }

    m_time++;
  }

private:
  int m_time;
};

#endif /* COLORS_SURFACE_H */
