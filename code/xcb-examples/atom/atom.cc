#include <iostream>
#include <xcb/xcb.h>
#include <cstring>
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
    
    uint32_t mask = XCB_CW_BACK_PIXEL;
    uint32_t list[] = { screen->black_pixel };
    xcb_create_window( c, XCB_COPY_FROM_PARENT, window, screen->root, 
		       0, 0, 256, 256, 0 , XCB_WINDOW_CLASS_INPUT_OUTPUT, 
		       screen->root_visual, mask, list );
    xcb_map_window( c, window );
    xcb_flush( c );
    
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply( c, 
							    xcb_intern_atom( c, 0, strlen("WM_NAME"), "WM_NAME" ),
							    0 );
    char title[] = "testtt";
    int len = strlen( title );
    xcb_change_property( c, XCB_PROP_MODE_REPLACE, window, reply->atom,
			 XCB_ATOM_STRING, 8, len, title );
    xcb_flush( c );
    
    sleep( 5 );
    
    xcb_get_property_reply_t *greply = xcb_get_property_reply( c, 
							       xcb_get_property( c, 0, window, reply->atom, XCB_ATOM_STRING, 0, len ), 
							       0 );
    
    cout << ":: " << ( char *)xcb_get_property_value( greply ) << endl;
    
    xcb_disconnect( c );
    
    return 0;
}