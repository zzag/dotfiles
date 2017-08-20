#include "app.h"
#include <string>
#include <cstring>


/**
 * Listen for new available global objects
 *
 */
void handleGlobal(void *data,
    wl_registry *registry,
    uint32_t name,
    const char *interface,
    uint32_t version)
{
  WaylandApp *app = reinterpret_cast<WaylandApp *>(data);

  if (!strcmp(interface, "wl_compositor"))
  {
    app->compositor(reinterpret_cast<wl_compositor *>(
      wl_registry_bind(registry, name, &wl_compositor_interface, version)
    ));
  }
  else if (!strcmp(interface, "wl_shell"))
  {
    app->shell(reinterpret_cast<wl_shell *>(
      wl_registry_bind(registry, name, &wl_shell_interface, version)
    ));
  }
  else if (!strcmp(interface, "wl_shm"))
  {
    app->shm(reinterpret_cast<wl_shm *>(
      wl_registry_bind(registry, name, &wl_shm_interface, version)
    ));
  }
}


/**
 * Listen for object that are no longer available
 */
void handleGlobalRemove(void *data,
    wl_registry *registry,
    uint32_t name)
{
}

const wl_registry_listener registryListener = {
  .global = handleGlobal,
  .global_remove = handleGlobalRemove
};


WaylandApp::WaylandApp(const char *name)
{
  m_display = wl_display_connect(name);
  if (!m_display) { throw std::string("Unable to open a connectin to the compositor"); }

  m_registry = wl_display_get_registry(m_display);
  wl_registry_add_listener(m_registry, &registryListener, this);
  wl_display_roundtrip(m_display);
  wl_registry_destroy(m_registry);
}


WaylandApp::~WaylandApp()
{
  wl_shell_destroy(m_shell);
  wl_compositor_destroy(m_compositor);
  wl_shm_destroy(m_shm);
  wl_registry_destroy(m_registry);
  wl_display_disconnect(m_display);
}


void
WaylandApp::run()
{
  while(wl_display_dispatch(m_display) != -1) {}
}
