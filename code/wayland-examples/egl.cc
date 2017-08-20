#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <iostream>
#include <string.h>


using std::cout;
using std::endl;


struct Data {
    wl_display *display;
    wl_registry *registry;
    wl_compositor *compositor;
    wl_shell *shell;
    wl_surface *surface;
    wl_shell_surface *shell_surface;
    wl_region *region;

    EGLDisplay egl_display;
    EGLConfig egl_config;
    EGLSurface egl_surface;
    EGLContext egl_context;
    wl_egl_window *egl_window;
};


const int WIDTH = 600;
const int HEIGHT = 400;


static void
registry_handler(void *data, wl_registry *registry, uint32_t serial,
        const char *interface, uint32_t version) {
    Data *private_data = reinterpret_cast<Data *>(data);

    if (strcmp(interface, "wl_compositor") == 0) {
        private_data->compositor = reinterpret_cast<wl_compositor *>(wl_registry_bind(registry, serial, &wl_compositor_interface, 3));
    }

    if (strcmp(interface, "wl_shell") == 0) {
        private_data->shell = reinterpret_cast<wl_shell *>(wl_registry_bind(registry, serial, &wl_shell_interface, 1));
    }
}

static void
registry_remover(void *data, wl_registry *registry, uint32_t serial) {
}


static const wl_registry_listener registry_listener = {
    registry_handler,
    registry_remover
};


int main() {
    Data *data;

    data = new Data;

    // connect and get some objects
    data->display = wl_display_connect(0);
    
    if (!data->display) {
        cout << "Unable to connect to wayland compositor" << endl;
        return -1;
    }

    data->registry = wl_display_get_registry(data->display);
    wl_registry_add_listener(data->registry, &registry_listener, data);

    wl_display_dispatch(data->display);
    wl_display_roundtrip(data->display);

    if (data->compositor == 0 || data->shell == 0) {
        cout << "Fatality!" << endl;
        return -1;
    }

    // create surface
    data->surface = wl_compositor_create_surface(data->compositor);
    data->shell_surface = wl_shell_get_shell_surface(data->shell, data->surface);
    wl_shell_surface_set_toplevel(data->shell_surface);

    data->region = wl_compositor_create_region(data->compositor);
    wl_region_add(data->region, 0, 0, WIDTH, HEIGHT);
    wl_surface_set_opaque_region(data->surface, data->region);

    // init egl
    EGLint major, minor, config_count, tmp_n;
    EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    EGLint context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    data->egl_display = eglGetDisplay((EGLNativeDisplayType)data->display);
    eglInitialize(data->egl_display, &major, &minor);
    eglGetConfigs(data->egl_display, 0, 0, &config_count);
    eglChooseConfig(data->egl_display, config_attribs, &(data->egl_config), 1, &tmp_n);
    data->egl_context = eglCreateContext(
            data->egl_display,
            data->egl_config,
            EGL_NO_CONTEXT,
            context_attribs);

    // init window
    data->egl_window = wl_egl_window_create(data->surface, WIDTH, HEIGHT);
    data->egl_surface = eglCreateWindowSurface(
            data->egl_display,
            data->egl_config,
            data->egl_window,
            0);
    eglMakeCurrent(data->egl_display, data->egl_surface, data->egl_surface, data->egl_context);

    glClearColor(0.5f, 0.5f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(data->egl_display, data->egl_surface);

    while (wl_display_dispatch(data->display) != -1) {
    }

    // disconnect
    wl_display_disconnect(data->display);

    delete data;
    return 0;
}
