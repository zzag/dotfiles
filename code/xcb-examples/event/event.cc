#include <iostream>
#include <xcb/xcb.h>

using std::cout;
using std::endl;

int main()
{
    xcb_connection_t *c;
    xcb_window_t window;
    xcb_screen_t *screen;
    xcb_generic_event_t *e;
    
    c = xcb_connect( 0, 0 );
    if( !c )
    {
	cout << ":: xcb_connect() failed" << endl;
	return -1;
    }
    
    screen = xcb_setup_roots_iterator( xcb_get_setup( c ) ).data;
    window = xcb_generate_id( c );
    
    uint32_t mask = XCB_GC_FOREGROUND | XCB_CW_EVENT_MASK;
    uint32_t args[] ={
	
	screen->black_pixel,
	XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_ENTER_WINDOW |
	XCB_EVENT_MASK_LEAVE_WINDOW | XCB_EVENT_MASK_KEY_PRESS |
	XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_POINTER_MOTION
	
    } ;
    
    xcb_create_window( c, XCB_COPY_FROM_PARENT, window, 
		       screen->root, 0, 0, 256, 256, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, 
		       screen->root_visual, mask, args );
    
    xcb_map_window( c, window );
    xcb_flush( c );
    
    while( ( e = xcb_wait_for_event( c ) ) )
    {
	switch( e->response_type & ~0x80 )
	{
	    case XCB_EXPOSE:
		
		cout << ":: exposure" << endl;
		
		
		
		break;
		
	    case XCB_ENTER_NOTIFY:
		
		cout << ":: enter window" << endl;
		break;
		
	    case XCB_LEAVE_NOTIFY:
		
		cout << ":: leave window" << endl;
		break;
		
	    case XCB_KEY_RELEASE:
	    {
		xcb_key_release_event_t *ev1 = ( xcb_key_release_event_t *)e;
		cout << ":: release " << ev1->detail << endl;
	    }
		break;
		
	    case XCB_KEY_PRESS:
	    {
		xcb_key_press_event_t *ev0 = ( xcb_key_press_event_t *)e;
		cout << ":: press " << ev0->detail << endl;
	    }
	    break;
	    
	    case XCB_MOTION_NOTIFY:
	    {
		xcb_motion_notify_event_t *ev2 = ( xcb_motion_notify_event_t *)e;
		cout << ":: motion " << ev2->event_x << "x" << ev2->event_y << endl;
	    }
	    break;
	}
    }
    
    xcb_disconnect( c );
    
    return 0;
}