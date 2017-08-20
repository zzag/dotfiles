#include <wayland-client.h>
#include <iostream>

using std::cout;
using std::endl;


static void 
registry_handler(void *data, wl_registry *registry, uint32_t serial, 
        const char *interface, uint32_t version) {
    cout << interface << " v" << version << endl;
}

static void 
registry_remover(void *data, wl_registry *registry, uint32_t serial) {
}


static const wl_registry_listener registry_listener = {
    registry_handler,
    registry_remover
};


int main() {
    wl_display *display;
    wl_registry *registry;

    display = wl_display_connect(0);

    if (!display) {
        cout << "Unable to connect to wayland compositor" << endl;
        return -1;
    }

    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, 0);

    wl_display_dispatch(display);
    wl_display_roundtrip(display);

    wl_display_disconnect(display);

    return 0;
}
