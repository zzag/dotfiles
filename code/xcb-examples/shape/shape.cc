#include <iostream>
#include <xcb/xcb.h>
#include <xcb/shape.h>

using std::cout;
using std::endl;

xcb_rectangle_t fullShape = { 0, 0, 600, 600 };
xcb_rectangle_t rects[] = {
    { 0, 0, 100, 100 },
    { 150, 150, 50, 50 }
};

xcb_rectangle_t rects1[] = {
    { 0, 0, 100, 100 },
    { 150, 150, 200, 200 }
};

int main()
{
    xcb_connection_t *c;
    xcb_screen_t *screen;
    xcb_window_t window;
    xcb_generic_event_t *e;
    const xcb_query_extension_reply_t *shapeReply;
    xcb_shape_query_version_reply_t *shapeExt;
    xcb_pixmap_t windowShape;
    xcb_gcontext_t shapeGC;
    xcb_shape_kind_t kind;
    xcb_gcontext_t gc;
    bool run;
    xcb_colormap_t colormap;
    
    c = xcb_connect( 0, 0 );
    if( !c )
    {
	cout << ":: xcb_connect() failed" << endl;
	return -1;
    }
    
    screen = xcb_setup_roots_iterator( xcb_get_setup( c ) ).data;
    window = xcb_generate_id( c );
    windowShape = xcb_generate_id( c );
    shapeGC = xcb_generate_id( c );
    kind = xcb_generate_id( c );
    gc = xcb_generate_id( c );
    colormap = xcb_generate_id( c );
    
    uint32_t windowMask = XCB_CW_EVENT_MASK;
    uint32_t windowValueList[] = {
	XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS
    };
    
    xcb_create_window( c, XCB_COPY_FROM_PARENT, 
		       window, screen->root, 0, 0, 
		       600, 600, 0, 
		       XCB_WINDOW_CLASS_INPUT_OUTPUT, 
		       screen->root_visual, windowMask, windowValueList );
    xcb_map_window( c, window );
    
    shapeReply = xcb_get_extension_data( c, &xcb_shape_id );
    if( !shapeReply->present )
    {
	cout << ":: X Server doesn\'t keep XShape" << endl;
	return -1;
    }
    
    shapeExt = xcb_shape_query_version_reply( c, xcb_shape_query_version( c ), 0 );
    cout << ":: XShape " << shapeExt->major_version << "." << shapeExt->minor_version << endl;
    
    uint32_t shapeMask = XCB_GC_FOREGROUND;
    uint32_t shapeValueList[] = { 0 };
    xcb_create_pixmap( c, 1, windowShape, window, 600, 600 );
    xcb_create_gc( c, shapeGC, windowShape, shapeMask, shapeValueList );
    xcb_poly_fill_rectangle( c, windowShape, shapeGC, 1, &fullShape );
    xcb_flush( c );
    
    uint32_t newShapeMask = XCB_GC_FOREGROUND;
    uint32_t newShapeMaskValueList[] = { 1 };
    xcb_change_gc( c, shapeGC, newShapeMask, newShapeMaskValueList );
    xcb_poly_fill_rectangle( c, windowShape, shapeGC, 2, rects );
    
    xcb_shape_mask( c, XCB_SHAPE_SO_SET, XCB_SHAPE_SK_BOUNDING, window, 0, 0, windowShape );
    
    xcb_create_colormap( c, XCB_COLORMAP_ALLOC_NONE, colormap, window, screen->root_visual );
    xcb_alloc_color_reply_t *color = xcb_alloc_color_reply( c, xcb_alloc_color( c, colormap, 0x0000, 0xffff, 0xffff ), 0 );
    
    uint32_t gcMask = XCB_GC_FOREGROUND;
    uint32_t gcVal[] = { color->pixel };
    xcb_create_gc( c, gc, window, gcMask, gcVal );
    
    xcb_flush( c );
    
    
    run = true;
    while( ( e = xcb_wait_for_event( c ) ) && run )
    {
	switch( e->response_type & ~0x80 )
	{
	    case XCB_EXPOSE:
		
		xcb_poly_fill_rectangle( c, window, gc, 2, rects1 );
		xcb_flush( c );
		
		break;
		
	    case XCB_BUTTON_PRESS:
		    
		    run = false;
		    break;
	}
    }
    
    xcb_disconnect( c );
    
    return 0;
}