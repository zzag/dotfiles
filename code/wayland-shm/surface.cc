#include "surface.h"
#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>


/**
 * Create temporary file
 *  \param size: size of the file in bytes
 *  \return: file descriptor or -1 on failure
 */
int createTempfileWithCloexec(int size)
{
  int fd = -1;

  std::string filenameBuild = getenv("XDG_RUNTIME_DIR"); // something like /run/user/:pid
  filenameBuild += "/wayland-client-XXXXXX";
  char *filename = new char[filenameBuild.length() + 1];
  strcpy(filename, filenameBuild.c_str());

  fd = mkostemp(filename, O_CLOEXEC);
  if (fd >= 0) { unlink(filename); }
  delete[] filename;

  if (ftruncate(fd, size) < 0)
  {
    close(fd);
    return -1;
  }

  return fd;
}


/**
 * Called when compositor has been finished rendering
 */
void redraw(void *data, wl_callback *callback, uint32_t time);
const wl_callback_listener frameListener = {
  .done = redraw
};
void redraw(void *data, wl_callback *callback, uint32_t time)
{
  wl_callback_destroy(callback);
  WaylandSurface *surface = reinterpret_cast<WaylandSurface *>(data);

  surface->redraw();

  callback = wl_surface_frame(surface->surface());
  wl_callback_add_listener(callback, &frameListener, data);
  wl_surface_attach(surface->surface(), surface->buffer(), 0, 0);
  wl_surface_commit(surface->surface());
}


WaylandSurface::WaylandSurface(WaylandApp &app, int width, int height)
{
  m_width = width;
  m_height = height;
  m_stride = m_width * 4;
  m_format = WL_SHM_FORMAT_XRGB8888;
  int size = m_stride * m_height;

  int m_fd = createTempfileWithCloexec(size);
  if (m_fd < 0) { throw std::string("Failed to create temporary file"); }

  m_data = reinterpret_cast<char *>(
    mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0));
  if (m_data == MAP_FAILED)
  {
    close(m_fd);
    throw std::string("Unable to map file");
  }

  // create shm_pool and buffer
  m_pool = wl_shm_create_pool(app.shm(), m_fd, size);
  m_buffer = wl_shm_pool_create_buffer(m_pool,
    0, m_width, m_height, m_stride, m_format);

  // borrow surface
  m_surface = wl_compositor_create_surface(app.compositor());
  m_shellSurface = wl_shell_get_shell_surface(app.shell(), m_surface);
}


WaylandSurface::~WaylandSurface()
{
  munmap(m_data, m_stride * m_height);
  close(m_fd);
  wl_shell_surface_destroy(m_shellSurface);
  wl_surface_destroy(m_surface);
  wl_buffer_destroy(m_buffer);
  wl_shm_pool_destroy(m_pool);
}


void
WaylandSurface::show()
{
  wl_shell_surface_set_toplevel(m_shellSurface);
  wl_surface_attach(m_surface, m_buffer, 0, 0);

  wl_callback *frameCallback = wl_surface_frame(m_surface);
  wl_callback_add_listener(frameCallback, &frameListener, this);
  wl_surface_commit(m_surface);
}


void
WaylandSurface::dirty(int x, int y, int width, int height)
{
  wl_surface_damage(m_surface, x, y, width, height);
}
