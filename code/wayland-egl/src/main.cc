#include <iostream>
#include <signal.h>
#include "wayland_app.h"
#include "wayland_surface.h"


using std::cout;
using std::endl;


// I know that globals are evil
// it is global because signalInt() needs app instance to break main loop
WaylandApp *app = 0;


void signalInt(int sigNum)
{
  app->stop();
}


void setSIGINTHandler()
{
  struct sigaction sigint;
  sigint.sa_handler = signalInt;
  sigemptyset(&sigint.sa_mask);
  sigint.sa_flags = SA_RESETHAND;
  sigaction(SIGINT, &sigint, 0);
}


int main()
{
  app = new WaylandApp;
  WaylandSurface *surface = new WaylandSurface(app);

  // break main loop on ^C
  setSIGINTHandler();

  surface->show();
  app->run();

  delete surface;
  delete app;
  return 0;
}
