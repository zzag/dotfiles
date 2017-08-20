#include <wayland-client.h>
#include <wayland-egl.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <cairo.h>
#include <cairo-gl.h>
#include <string>
#include <iostream>


//
//
// clang++ -o cairo_egl cairo_egl.cc `pkg-config --libs --cflags wayland-client glesv2 egl wayland-egl cairo`
//
//

using std::string;
using std::cout;
using std::endl;


struct Surface {
    wl_display *display;
    wl_registry *registry;
    wl_compositor *compositor;
    wl_surface *surface;
    wl_shell *shell;
    wl_shell_surface *shell_surface;

    cairo_device_t *cairo_device;
    cairo_surface_t *cairo_surface;
    cairo_t *cr;
    EGLDisplay dpy;
    EGLContext ctx;
    EGLConfig config;
    EGLSurface egl_surface;
    wl_egl_window *window;
    int width;
    int height;
    int buffer_size;
    int i;
};


static void
registry_handler(void *data, wl_registry *registry, uint32_t serial, const char *interface, uint32_t version) {
    Surface *surface = reinterpret_cast<Surface *>(data);
    string intfc(interface);

    if (intfc == "wl_compositor") {
        surface->compositor = reinterpret_cast<wl_compositor *>(wl_registry_bind(registry, serial, &wl_compositor_interface, 1));
    }
    if (intfc == "wl_shell") {
        surface->shell = reinterpret_cast<wl_shell *>(wl_registry_bind(registry, serial, &wl_shell_interface, 1));
    }
}


static void
registry_remove(void *data, wl_registry *registry, uint32_t serial) {}


static wl_registry_listener registry_listener = {
    registry_handler,
    registry_remove
};


void init_egl(Surface *surface) {
    EGLint context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 1,
        EGL_BLUE_SIZE, 1,
        EGL_GREEN_SIZE, 1,
        EGL_ALPHA_SIZE, 1,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    EGLint major, minor, n, count;
    EGLConfig *configs;
    EGLBoolean ret;


    surface->dpy = eglGetDisplay((EGLNativeDisplayType)surface->display);
    if (surface->dpy == EGL_NO_DISPLAY) {
        cout << ":: EGL no display" << endl;
        return;
    }

    ret = eglInitialize(surface->dpy, &major, &minor);
    if (ret != EGL_TRUE) {
        cout << ":: Unable to initialize EGL" << endl;
        return;
    }

    cout << ":: Major " << major << endl;
    cout << ":: Minor " << minor << endl;

    eglBindAPI(EGL_OPENGL_ES_API);

    if (!eglGetConfigs(surface->dpy, 0, 0, &count) || count < 1) {
        cout << ":: Unable to get configs" << endl;
        return;
    }

    configs = new EGLConfig[count];
    eglChooseConfig(surface->dpy, config_attribs, configs, count, &n);
    cout << ":: " << n << endl;

    for (int i = 0; i < n; i++) {
        EGLint size;
        eglGetConfigAttrib(surface->dpy, configs[i], EGL_BUFFER_SIZE, &size);

        if (surface->buffer_size == size) {
            cout << ":: Found config" << endl;
            surface->config = configs[i];
            break;
        }
    }

    delete[] configs;

    surface->ctx = eglCreateContext(surface->dpy, surface->config, EGL_NO_CONTEXT, context_attribs);
}


void draw_some_stuff(Surface *s) {
    cairo_set_operator(s->cr, CAIRO_OPERATOR_SOURCE);
    cairo_set_source_rgba(s->cr, 0, 0, 0, 0.5);
    cairo_paint(s->cr);
    cairo_set_operator(s->cr, CAIRO_OPERATOR_OVER);

    cairo_set_source_rgb(s->cr, 1.0, 0.0, 0.0);
    cairo_set_line_width(s->cr, 10);

    cairo_move_to(s->cr, 10, 10);
    cairo_line_to(s->cr, 100, 100);
    cairo_stroke(s->cr);

    cairo_move_to(s->cr, 100, 30);
    cairo_set_source_rgb(s->cr, s->i / 255.0, 1.0, s->i / 255.0);
    cairo_select_font_face(s->cr, "Droid Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(s->cr, 30);
    cairo_show_text(s->cr, "(Hello)");

    cairo_gl_surface_swapbuffers(s->cairo_surface);
    s->i += 1;
    s->i = s->i % 255;
}


void create_window(Surface *surface) {
    surface->surface = wl_compositor_create_surface(surface->compositor);
    surface->shell_surface = wl_shell_get_shell_surface(surface->shell, surface->surface);
    wl_shell_surface_set_toplevel(surface->shell_surface);
    surface->window = wl_egl_window_create(surface->surface, surface->width, surface->height);
    surface->egl_surface = eglCreateWindowSurface(surface->dpy, surface->config, surface->window, 0);

    eglMakeCurrent(surface->dpy, surface->egl_surface, surface->egl_surface, surface->ctx);

    surface->cairo_device = cairo_egl_device_create(surface->dpy, surface->ctx);
    if (cairo_device_status(surface->cairo_device) != CAIRO_STATUS_SUCCESS) {
        cout << ":: Unable to create cairo_device_t" << endl;
        return;
    }

    surface->cairo_surface = cairo_gl_surface_create_for_egl(surface->cairo_device, surface->egl_surface, 
            surface->width, surface->height);
    if (!surface->cairo_surface) {
        cout << ":: Unable to create cairo_surface_t" << endl;
        return;
    }

    surface->cr = cairo_create(surface->cairo_surface);
}


int main() {
    Surface *s = new Surface;

    s->display = wl_display_connect(0);
    if (!s->display) {
        cout << ":: Unable to connect to wayland compositor" << endl;
        return -1;
    }

    s->registry = wl_display_get_registry(s->display);
    wl_registry_add_listener(s->registry, &registry_listener, s);
    wl_display_dispatch(s->display);
    wl_display_roundtrip(s->display);

    // set some stuff
    s->buffer_size = 32;
    s->width = 320;
    s->height = 240;
    s->i = 0;

    init_egl(s);
    create_window(s);

    while(wl_display_dispatch_pending(s->display) != -1) {
        draw_some_stuff(s);
    }

    wl_display_disconnect(s->display);
    delete s;

    return 0;
}

