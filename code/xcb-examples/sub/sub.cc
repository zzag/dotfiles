#include <iostream>
#include <xcb/xcb.h>
#include <xcb/damage.h>
#include <unistd.h>

using std::cout;
using std::endl;

int main()
{
    xcb_connection_t *c;
    xcb_screen_t *screen;
    xcb_window_t window;
    xcb_generic_event_t *e;
    const xcb_query_extension_reply_t *damageReply;
    xcb_damage_query_version_reply_t *damExt;
    
    c = xcb_connect( 0, 0 );
    if( !c )
	return -1;
    
    screen = xcb_setup_roots_iterator( xcb_get_setup( c ) ).data;
    window = xcb_generate_id( c );
    
    uint32_t mask = XCB_CW_EVENT_MASK;
    uint32_t valuelist[] = {  
	XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
    };
    
    xcb_change_window_attributes( c, screen->root, mask, valuelist );
    xcb_flush( c );
    
    damageReply = xcb_get_extension_data( c, &xcb_damage_id );
    if( !damageReply->present )
    {
	cout << ":: can\'t find damage" << endl;
	return -1;
    }
    
    damExt = xcb_damage_query_version_reply( c, xcb_damage_query_version( c, 1, 1 ), 0 );
    cout << ":: XDamage " << damExt->major_version << "." << damExt->minor_version << endl;
    
    while( ( e = xcb_wait_for_event( c ) ) )
    {
	switch( e->response_type & ~0x80 )
	{
	    case XCB_CREATE_NOTIFY:
	    {
		xcb_create_notify_event_t *ev = reinterpret_cast<xcb_create_notify_event_t*>( e );
		cout << ":: create " << ev->window << endl;
		
		
	    }
	    break;
		
	    case XCB_DESTROY_NOTIFY:
	    {
		xcb_destroy_notify_event_t *ev = reinterpret_cast<xcb_destroy_notify_event_t *>( e );
		cout << ":: destroy " << ev->window << endl;
		
	    }
	    break;
		
	    case XCB_MAP_NOTIFY:
	    {
		xcb_map_notify_event_t *ev = reinterpret_cast<xcb_map_notify_event_t *>( e );
		cout << ":: map " << ev->window << endl;
		xcb_damage_damage_t damage;
		
		damage = xcb_generate_id( c );
		xcb_damage_create( c, damage, ev->window, XCB_DAMAGE_REPORT_LEVEL_RAW_RECTANGLES );
		xcb_flush( c );
	    }
	    break;
		
	    case XCB_UNMAP_NOTIFY:
	    {
		xcb_unmap_notify_event_t *ev = reinterpret_cast<xcb_unmap_notify_event_t *>( e );
		cout << ":: unmap " << ev->window << endl;
		
	    }
	    break;
		
	    default:
		if( e->response_type == damageReply->first_event + XCB_DAMAGE_NOTIFY )
		{
		    xcb_damage_notify_event_t *ev = reinterpret_cast<xcb_damage_notify_event_t *>( e );
		    cout << ">> damage:" << endl;
		    cout << "   area: x = " << ev->area.x << " y = " << ev->area.y 
	         << " width = " << ev->area.width << " height = " << ev->area.height << endl;
		 
	    cout << "   geometry: x = " << ev->geometry.x << " y = " << ev->geometry.y 
	         << " width = " << ev->geometry.width << " height = " << ev->geometry.height << endl;
		 
		 xcb_damage_subtract( c, ev->damage, XCB_NONE, XCB_NONE );
		}
	}
	
	delete e;
    }
    
    xcb_disconnect( c );
    
    
    return 0;
}