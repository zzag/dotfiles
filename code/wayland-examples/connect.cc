#include <wayland-client.h>
#include <iostream>


using std::cout;
using std::endl;


int main() {
    wl_display *display;

    display = wl_display_connect(0);

    if (!display) {
        cout << "Unable to connect to wayland compositor" << endl;
        return -1;
    }

    wl_display_disconnect(display);

    return 0;
}
