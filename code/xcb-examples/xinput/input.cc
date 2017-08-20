#include <iostream>
#include <xcb/xcb.h>
#include <xcb/xinput.h>
#include <cstring>

using std::cout;
using std::endl;

const char *name = "testname";

int main()
{
    xcb_connection_t *c;
    const xcb_query_extension_reply_t *inputReply;
    xcb_input_get_extension_version_reply_t *inputExt;
    xcb_generic_error_t *e;
    xcb_input_list_input_devices_reply_t *inputDevices;
    xcb_window_t window;
    xcb_screen_t *screen;
    xcb_generic_event_t *ev;
    
    c = xcb_connect( 0, 0 );
    
    screen = xcb_setup_roots_iterator( xcb_get_setup( c ) ).data;
    window = xcb_generate_id( c );
    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t val[] = {
	screen->white_pixel, XCB_EVENT_MASK_EXPOSURE
    };
    
    xcb_create_window( c, XCB_COPY_FROM_PARENT, window, screen->root, 0, 0, 400, 400, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, mask, val );
    xcb_map_window( c, window );
    
    inputReply = xcb_get_extension_data( c, &xcb_input_id );
    if( !inputReply->present )
    {
	cout << ":: X Server doesn\'t kepp XInput extension" << endl;
	return -1;
    }
    
    e = 0;
    inputExt = xcb_input_get_extension_version_reply( c, xcb_input_get_extension_version( c, strlen( name ), name ), &e );
    if( e )
	cout << ":: some error" << endl;
    
    if( !inputExt->present )
	cout << ":: X Server doesn\'t keep XInput" << endl;
    cout << ":: XInput " << inputExt->server_major << "." << inputExt->server_minor << endl;
    
    inputDevices = xcb_input_list_input_devices_reply( c, xcb_input_list_input_devices( c ), 0 );
    int count = xcb_input_list_input_devices_devices_length( inputDevices );
    xcb_input_device_info_iterator_t devs;
    devs = xcb_input_list_input_devices_devices_iterator( inputDevices );
    
    cout << ":: Count input devices = " << count << endl;
    
    const xcb_input_event_mask_t eventClass[] =
    {
	XCB_INPUT_XI_EVENT_MASK_MOTION
    };
    
    xcb_input_xi_select_events( c, window,
					      1,
                                              eventClass );
    
    xcb_flush( c );
    
    while( ( ev = xcb_wait_for_event( c ) ) )
    {
	switch( ev->response_type & ~0x80 )
	{
	    case XCB_EXPOSE:
		cout << ":: expose " << ( int )ev->response_type << endl;
		break;
	}
	
	if( ev->response_type == inputReply->first_event + XCB_INPUT_DEVICE_MOTION_NOTIFY )
	    cout << ":: motion" << endl;
	
	cout << ":: response_type = " << ( int )ev->response_type << endl;
	    
    }
	
    xcb_disconnect( c );
    
    return 0;
}