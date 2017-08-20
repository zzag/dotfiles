#include <iostream>
#include <xcb/xcb.h>
#include <xcb/render.h>
#include <unistd.h>

using std::cout;
using std::endl;

void printFormat( xcb_render_query_pict_formats_reply_t *reply )
{
    xcb_render_pictforminfo_iterator_t formatIter;
    
    formatIter = xcb_render_query_pict_formats_formats_iterator( reply );
    
    while( formatIter.rem )
    {
	xcb_render_pictforminfo_t *info;
	
	info = formatIter.data;
	
	cout << ":: Index = " << formatIter.rem << endl;
	cout << ":: ID = " << info->id << endl;
	cout << ":: type = " << ( int )info->type << endl;
	cout << ":: depth = " << ( int )info->depth << endl;
	cout << ":: pictformat " << info->id << endl;
	cout << ":: red_shift = " << info->direct.red_shift << endl;
	cout << ":: red_mask = " << info->direct.red_mask << endl;
	
	cout << ":: green_shift = " << info->direct.green_shift << endl;
	cout << ":: green_mask = " << info->direct.green_mask << endl;
	
	cout << ":: blue_shift = " << info->direct.blue_shift << endl;
	cout << ":: blue_mask = " << info->direct.blue_mask << endl;
	
	cout << ":: alpha_shift = " << info->direct.alpha_shift << endl;
	cout << ":: alpha_mask = " << info->direct.alpha_mask << endl;
	cout << endl;
	
	xcb_render_pictforminfo_next( &formatIter );
    }
}

xcb_render_pictformat_t getPictformatFromVisual( xcb_render_query_pict_formats_reply_t *r, xcb_visualid_t v )
{
    xcb_render_pictscreen_iterator_t screenIter;
    xcb_render_pictscreen_t *screen;
    
    xcb_render_pictdepth_iterator_t depthIter;
    xcb_render_pictdepth_t *depth;
    
    xcb_render_pictvisual_iterator_t visualIter;
    xcb_render_pictvisual_t *visual;
    
    screenIter = xcb_render_query_pict_formats_screens_iterator( r );
    
    while( screenIter.rem )
    {
	screen = screenIter.data;
	
	depthIter = xcb_render_pictscreen_depths_iterator( screen );
	while( depthIter.rem )
	{
	    depth = depthIter.data;
	    
	    visualIter = xcb_render_pictdepth_visuals_iterator( depth );
	    while( visualIter.rem )
	    {
		visual = visualIter.data;
		
		if( visual->visual == v )
		    return visual->format;
		
		xcb_render_pictvisual_next( &visualIter );
	    }
	    
	    xcb_render_pictdepth_next( &depthIter );
	}
	
	xcb_render_pictscreen_next( &screenIter );
    }
    
    xcb_render_pictformat_t ret;
    ret = 0;
    return ret;
}

xcb_render_pictforminfo_t *getPictforminfo(
    xcb_render_query_pict_formats_reply_t *reply, 
    xcb_render_pictforminfo_t *query )
{
    xcb_render_pictforminfo_iterator_t forminfoIter;
    
    forminfoIter = xcb_render_query_pict_formats_formats_iterator( reply );

    while( forminfoIter.rem )
    {
        xcb_render_pictforminfo_t *format;
        format  = forminfoIter.data;
        xcb_render_pictforminfo_next( &forminfoIter );

        if( query->id && query->id != format->id )
	    continue;
	
	if( query->type != format->type )
	    continue;
	
	if( query->depth && query->depth != format->depth )
	    continue;
	
	if( query->direct.red_mask && query->direct.red_mask != format->direct.red_mask )
	    continue;
	
	if( query->direct.green_mask && query->direct.green_mask != format->direct.green_mask )
	    continue;
	
	if( query->direct.blue_mask && query->direct.blue_mask != format->direct.blue_mask )
	    continue;
	
	if( query->direct.alpha_mask && query->direct.alpha_mask != format->direct.alpha_mask )
	    continue;
	
        return format; 
    }
    
    return 0;
}

xcb_render_fixed_t makeFixed( int16_t i, int16_t f )
{
    return ( i << 16 ) | ( f & 0xffff );
}



void draw( xcb_connection_t *c,  xcb_render_query_pict_formats_reply_t *reply )
{
    xcb_screen_t *screen;
    xcb_window_t window;
    xcb_render_pictformat_t format;
    xcb_render_pictforminfo_t query;
    
    xcb_render_pictforminfo_t *alphaMask;
    xcb_render_pictformat_t alphaMaskFormat;
    
    xcb_render_pictforminfo_t *surfaceMask;
    xcb_render_pictformat_t surfaceMaskFormat;
    
    xcb_pixmap_t windowSurface;
        
    screen = xcb_setup_roots_iterator( xcb_get_setup( c ) ).data;
    window = xcb_generate_id( c );
    
    uint32_t windowMask = XCB_CW_EVENT_MASK;
    uint32_t windowList[] = { XCB_EVENT_MASK_EXPOSURE };
    
    xcb_create_window( c, XCB_COPY_FROM_PARENT, 
		       window, screen->root,
		       0, 0, 600, 600, 0, 
		       XCB_WINDOW_CLASS_INPUT_OUTPUT, 
		       screen->root_visual,
		       windowMask, windowList );
    xcb_map_window( c, window );
    xcb_flush( c );
    
    format = getPictformatFromVisual( reply, screen->root_visual );
    
    query.id = 0;
    query.type = XCB_RENDER_PICT_TYPE_DIRECT;
    query.depth = 8;
    query.direct.red_mask = 0;
    query.direct.green_mask = 0;
    query.direct.blue_mask = 0;
    query.direct.alpha_mask = 255;
    
    alphaMask = getPictforminfo( reply, &query );
    if( !alphaMask )
    {
	cout << ":: getPictforminfo() failed" << endl;
	return;
    }
    alphaMaskFormat = alphaMask->id;
    
    query.depth = 32;
    query.direct.red_mask = 0;
    query.direct.green_mask = 0;
    query.direct.blue_mask = 0;
    query.direct.alpha_mask = 255;
    
     surfaceMask = getPictforminfo( reply, &query );
     if( !surfaceMask )
     {
	 cout << ":: getPictforminfo() failed" << endl;
	 return;
     }
     surfaceMaskFormat = surfaceMask->id;
     
     xcb_rectangle_t windowRect;
     windowRect.x = 0;
     windowRect.y = 0;
     windowRect.width = 600;
     windowRect.height = 600;
     
     xcb_render_color_t backColor;
     backColor.red = 0x0;
     backColor.green = 0xffff;
     backColor.blue = 0xffff;
     backColor.alpha = 0xffff;
     
     uint32_t value_mask = 1<<0;
     uint32_t value_list[] = { 1 };
     xcb_render_picture_t windowPict;
     windowPict = xcb_generate_id( c );
     xcb_render_create_picture( c, windowPict, window, format, value_mask, value_list );
     
     windowSurface = xcb_generate_id( c );
     xcb_create_pixmap( c, 32, windowSurface, window, 600, 600 );
     
     xcb_render_picture_t pict;
     pict = xcb_generate_id( c );
     xcb_render_create_picture( c, pict, windowSurface, surfaceMaskFormat, value_mask, value_list );
     
     xcb_rectangle_t pictRect;
     pictRect.x = 0;
     pictRect.y = 0;
     pictRect.width =600;
     pictRect.height = 600;
     
     xcb_render_color_t pictColor;
     pictColor.red = 0xffff;
     pictColor.green = 0xff0;
     pictColor.blue = 0xff;
     pictColor.alpha = 0x0fff;
     xcb_render_picture_t emptyMask = { 0 };
     
     xcb_generic_event_t *ev;
     while( ( ev = xcb_wait_for_event( c ) ) )
     {
	 switch( ev->response_type & ~0x80 )
	 {
	     case XCB_EXPOSE:
		 
		 xcb_render_fill_rectangles( c, XCB_RENDER_PICT_OP_SRC, pict, pictColor, 1, &pictRect );
		 xcb_render_fill_rectangles( c, XCB_RENDER_PICT_OP_SRC, windowPict, backColor, 1, &windowRect);
		 
		 xcb_render_composite( c, XCB_RENDER_PICT_OP_OVER,
				       pict, emptyMask, windowPict, 30, 30, 0, 0, 30, 30, 270, 270 );
		 
		 
		 
		 xcb_flush( c );
		 break;
	 }
     }
}

int main()
{
    xcb_connection_t *c;
    const xcb_query_extension_reply_t *renderData;
    xcb_render_query_version_reply_t *renderVersion;
    xcb_render_query_pict_formats_reply_t *pictFormats;
    
    c = xcb_connect( 0, 0 );
    if( !c )
    {
	cout << ":: xcb_connect() failed" << endl;
	return -1;
    }
    
    renderData = xcb_get_extension_data( c, &xcb_render_id );
    if( !renderData->present )
    {
	cout << ":: xcb_get_extension_data() failed" << endl;
	return -1;
    }
    
    renderVersion = xcb_render_query_version_reply( c, 
						    xcb_render_query_version( c, 0, 11 ), 0 );
    
    pictFormats = xcb_render_query_pict_formats_reply( c, 
						       xcb_render_query_pict_formats( c ), 0 );

    printFormat( pictFormats );
    draw( c, pictFormats );
    
    xcb_disconnect( c );
    
    return 0;
}