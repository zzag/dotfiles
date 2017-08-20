#include <wayland-client.h>
#include "app.h"
#include "surface.h"
#include "colors_surface.h"

// compile with:
// clang++ main.cc app.cc surface.cc -o rendering -lwayland-client

int main()
{
  WaylandApp app;
  WaylandSurface *surface1 = new WaylandSurface(app, 200, 200);
  WaylandSurface *surface2 = new ColorsSurface(app, 400, 300);

  surface1->show();
  surface2->show();

  app.run();

  delete surface1;
  delete surface2;
  return 0;
}
