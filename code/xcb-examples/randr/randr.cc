#include <iostream>
#include <xcb/xcb.h>
#include <xcb/randr.h>

using std::cout;
using std::endl;

int main()
{
    xcb_connection_t *c;
    const xcb_query_extension_reply_t *extData;
    xcb_randr_get_screen_resources_current_reply_t *screenInfo;
    xcb_screen_t *screen;
    xcb_window_t window;
    
    c = xcb_connect( 0, 0 );
    if( !c )
    {
	cout << ":: xcb_connect() failed" << endl;
	return -1;
    }
    
    extData = xcb_get_extension_data( c, &xcb_randr_id );
    if( !extData->present )
    {
	cout << ":: RandR is not present" << endl;
    }
    
    window = xcb_generate_id( c );
    screen = xcb_setup_roots_iterator( xcb_get_setup( c ) ).data;
    
    xcb_create_window( c, XCB_COPY_FROM_PARENT, window, screen->root,
		       0, 0, 256, 256, 0, 
		       XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual,
		       0, 0 );
    xcb_map_window( c, window );
    xcb_flush( c );
    
    screenInfo = xcb_randr_get_screen_resources_current_reply( c, xcb_randr_get_screen_resources_current( c, window ), 0 );
    if( !screenInfo )
    {
	cout << ":: xcb_randr_get_screen_resources_current() failed" << endl;
	return -1;
    }
    
    cout << ":: num_crtcs = " << screenInfo->num_crtcs << endl;
    cout << ":: num_outputs = " << screenInfo->num_outputs << endl;
    cout << ":: num_modes = " << screenInfo->num_modes << endl;
    
    xcb_randr_mode_info_t *modes;
    modes = xcb_randr_get_screen_resources_current_modes( screenInfo );
    int length = xcb_randr_get_screen_resources_current_modes_length( screenInfo );
    
    for( int index = 0 ; index < length ; index++ )
    {
	cout << ">> [" << index << "] " << modes[ index ].id << " " << modes[ index ].width << "x" << modes[ index ].height << endl;
	
	
    }
    
    int namesLength = xcb_randr_get_screen_resources_current_names_length( screenInfo );
    cout << namesLength << endl;
    uint8_t *names = xcb_randr_get_screen_resources_current_names( screenInfo );
    cout << names << endl;
    
    xcb_disconnect( c );
    
    return 0;
}