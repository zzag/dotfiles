#include <iostream>
#include <xcb/xcb.h>
#include <unistd.h>

using std::cout;
using std::endl;

int main()
{
    xcb_connection_t *c;
    xcb_window_t window;
    xcb_gcontext_t ctx;
    xcb_screen_t *screen;
    xcb_generic_event_t *e;
    
    xcb_point_t points[] = {
	
	{ 10, 10 },
	{ 40, 40 }
	
    };
    
    c = xcb_connect( 0, 0 );
    if( !c )
    {
	cout << ":: xcb_connect() failed" << endl;
	return -1;
    }
    
    window = xcb_generate_id( c );
    ctx = xcb_generate_id( c );
    screen = xcb_setup_roots_iterator( xcb_get_setup( c ) ).data;
    
    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t list[] = {
	screen->black_pixel,
	XCB_EVENT_MASK_EXPOSURE
    };
    
    xcb_create_window( c, XCB_COPY_FROM_PARENT, window, 
		       screen->root, 0, 0, 256, 256, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, 
		       screen->root_visual, mask, list );
    xcb_map_window( c, window );
    xcb_flush( c );
    
    uint32_t gMask = XCB_GC_FOREGROUND;
    uint32_t gList = screen->white_pixel;
    xcb_create_gc( c, ctx, window, gMask, &gList );
    
    while( ( e = xcb_wait_for_event( c ) ) )
    {
	switch( e->response_type & ~0x80 )
	{
	    case XCB_EXPOSE:
		
		 xcb_poly_point( c, XCB_COORD_MODE_ORIGIN, window, ctx, 2, points );
		 xcb_flush( c );
		 break;
	}
    }
    
    xcb_free_gc( c, ctx );
    xcb_unmap_window( c, window );
    xcb_disconnect( c );
    
    return 0;
}