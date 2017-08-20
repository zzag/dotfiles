#include <iostream>
#include <xcb/xcb.h>
#include <xcb/xinput.h>
#include <unistd.h>

using std::cout;
using std::endl;

int main()
{
    xcb_connection_t *c;
    xcb_screen_t *screen;
    xcb_window_t window;
    const xcb_query_extension_reply_t *xiReply;
    xcb_input_xi_query_version_reply_t *xiExt;
    xcb_generic_event_t *e;
    
    c = xcb_connect( 0, 0 );
    
    xiReply = xcb_get_extension_data( c, &xcb_input_id );
    
    if( !xiReply->present )
    {
	cout << ":: XI2 isn\'t available" << endl;
	return -1;
    }
    
    xiExt = xcb_input_xi_query_version_reply( c, xcb_input_xi_query_version( c, 2, 3 ), 0 );
    cout << ":: XI2 " << xiExt->major_version << "." << xiExt->minor_version << endl;
    
    
    
    screen = xcb_setup_roots_iterator( xcb_get_setup( c ) ).data;
    
    window = xcb_generate_id( c );
    
    uint32_t mask = XCB_CW_BACK_PIXEL;
    uint32_t vals[] = { screen->white_pixel };
    
    xcb_create_window( c, XCB_COPY_FROM_PARENT, window, screen->root, 0, 0, 400, 400, 0, 
		       XCB_WINDOW_CLASS_INPUT_OUTPUT, 
		       screen->root_visual, mask, vals );
    xcb_map_window( c, window );
    xcb_flush( c );
    
    /* The layout of the <masks> parameter to xcb_input_xi_select_events shoud be:
    ** first mask:             [ xcb_input_event_mask_t | <xcb_input_event_mask_t.mask_len> * uint32_t ]
    ** ...
    ** last (<num_mask>) mask: [ xcb_input_event_mask_t | <xcb_input_event_mask_t.mask_len> * uint32_t ]
    */
    struct {
        xcb_input_event_mask_t mask1_header;
        uint32_t               mask1_value[2];
        xcb_input_event_mask_t mask2_header;
        uint32_t               mask2_value[1];
    } masks;
    masks.mask1_header.deviceid = 2 /* core pointer */;
    masks.mask1_header.mask_len = sizeof (masks.mask1_value) / 4;
    /* Note: native byte order must be respected,
    ** so on big endian, values should be swapped!
    ** (32 higher bits, followed by 32 lower bits)
    */
    masks.mask1_value[0] =
        XCB_INPUT_XI_EVENT_MASK_MOTION |
        XCB_INPUT_XI_EVENT_MASK_BUTTON_PRESS |
        XCB_INPUT_XI_EVENT_MASK_BUTTON_RELEASE |
        XCB_INPUT_XI_EVENT_MASK_ENTER |
        XCB_INPUT_XI_EVENT_MASK_LEAVE;
    masks.mask1_value[1] = 0 /* mask bits above 32 would go there */;
    masks.mask2_header.deviceid = XCB_INPUT_DEVICE_ALL;
    masks.mask2_header.mask_len = sizeof (masks.mask2_value) / 4;
    masks.mask2_value[0] =
        XCB_INPUT_XI_EVENT_MASK_KEY_PRESS |
        XCB_INPUT_XI_EVENT_MASK_KEY_RELEASE |
        XCB_INPUT_XI_EVENT_MASK_FOCUS_IN |
        XCB_INPUT_XI_EVENT_MASK_FOCUS_OUT;
    
    xcb_input_xi_select_events( c, window, 2, &masks.mask1_header );
    xcb_flush( c );
    
    while( ( e = xcb_wait_for_event( c ) ) )
    {
	xcb_ge_event_t *ev = reinterpret_cast<xcb_ge_event_t *>( e );
	
	switch( ev->event_type )
	{
	    case XCB_INPUT_BUTTON_PRESS:
	    {
		cout << ":: button press" << endl;
	    }
	    break;
		
	    case XCB_INPUT_BUTTON_RELEASE:
	    {
		cout << ":: button release" << endl;
	    }
	    break;
		
	    case XCB_INPUT_MOTION:
	    {
		cout << ":: motion ";
		
		xcb_input_motion_event_t *mot = reinterpret_cast<xcb_input_motion_event_t *>(ev);
		
		cout << ( mot->event_x >> 16 ) << "x" << ( mot->event_y >> 16 ) << endl;
	    }
	    break;
		
	    case XCB_INPUT_ENTER:
		cout << ":: enter" << endl;
		break;
		
	    case XCB_INPUT_LEAVE:
		cout << ":: leave" << endl;
		break;
		
	    case XCB_INPUT_KEY_PRESS:
		cout << ":: key press" << endl;
		break;
		
	    case XCB_INPUT_KEY_RELEASE:
		cout << ":: key release" << endl;
		
	    case XCB_INPUT_FOCUS_IN:
		cout << ":: focus in" << endl;
		break;
		
	    case XCB_INPUT_FOCUS_OUT:
		cout << ":: focus out" << endl;
		break;
	}
    }
    
    xcb_disconnect( c );
    
    return 0;
}