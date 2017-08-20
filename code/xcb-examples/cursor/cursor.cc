#include <iostream>
#include <unistd.h>
#include <xcb/xcb.h>
#include <cstring>

using std::cout;
using std::endl;

void setCursor( xcb_connection_t *c, xcb_window_t window, xcb_screen_t *screen )
{
    xcb_font_t font;
    xcb_cursor_t cursor;
    xcb_gcontext_t gc;
    
    font = xcb_generate_id( c );
    xcb_open_font( c, font, strlen( "cursor" ), "cursor" );
    
    cursor = xcb_generate_id( c );
    xcb_create_glyph_cursor( c, cursor, font, font, 58, 59, 50000, 0, 0, 0, 0, 0 );
    
    gc = xcb_generate_id( c );
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT;
    uint32_t list[]{
	screen->white_pixel,
	screen->white_pixel,
	font
    };
    
    xcb_create_gc( c, gc, window, mask, list );
    
    uint32_t maskCursor = XCB_CW_CURSOR;
    uint32_t listCursor[]{
	cursor
    };
    
    xcb_change_window_attributes( c, window, maskCursor, listCursor );
}

int main(){
    
    xcb_connection_t *c;
    xcb_screen_t *screen;
    xcb_window_t window;
    xcb_generic_event_t *ev;
    xcb_gcontext_t gc;
    xcb_font_t font;
    
    c = xcb_connect( 0, 0 );
    if( !c )
    {
	cout << ":: xcb_connect() failed" << endl;
	return -1;
    }
    
    screen = xcb_setup_roots_iterator( xcb_get_setup( c ) ).data;
    window = xcb_generate_id( c );
    
    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t list[]{ screen->black_pixel, XCB_EVENT_MASK_EXPOSURE };
    xcb_create_window( c, XCB_COPY_FROM_PARENT, window, 
		       screen->root, 0, 0, 256, 256, 0, 
		       XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual,
		       mask, list );
    xcb_map_window( c, window );
    xcb_flush( c );
    
    gc = xcb_generate_id( c );
    font = xcb_generate_id( c );
    
    const char *fontName = "5x7";
    
    xcb_open_font( c, font, strlen( fontName ), fontName );
    uint32_t maskFont = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT;
    uint32_t listFont[]{ screen->white_pixel, screen->black_pixel, font };
    xcb_create_gc( c, gc, window, maskFont, listFont );
    
    while( ( ev = xcb_wait_for_event( c ) ) )
    {
	switch( ev->response_type & ~0x80 )
	{
	    case XCB_EXPOSE:
	    {
		cout << ":: expose" << endl;
		
		const char *text = "hello";
		
		xcb_image_text_8( c, strlen( text ), window, gc, 0, 20, text );
		setCursor( c, window, screen );
		xcb_flush( c );
		
		break;
	    }
	}
    }
    
    xcb_disconnect( c );
    
    return 0;
}