#include <iostream>
#include <xcb/xcb.h>
#include <unistd.h>

using std::cout;
using std::endl;

int main()
{
    xcb_connection_t *c;
    xcb_window_t window;
    xcb_screen_t *screen;
    
    c = xcb_connect( 0, 0 );
    if( !c )
    {
	cout << ":: xcb_connect() failed" << endl;
	return -1;
    }
    
    screen = xcb_setup_roots_iterator( xcb_get_setup( c ) ).data;
    window = xcb_generate_id( c );
    
    xcb_create_window( c, XCB_COPY_FROM_PARENT, window, screen->root, 
		       10, 40, 100, 100, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT, 
		       screen->root_visual, 0, 0 );
    
    xcb_map_window( c, window );
    
    xcb_flush( c );
    
    sleep( 5 );
    xcb_unmap_window( c, window );
    xcb_flush( c );
    
    cout << ":: " << endl;
    sleep( 5 );
    
    xcb_disconnect( c );
    
    return 0;
}