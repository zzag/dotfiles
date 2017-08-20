#include <iostream>
#include <xcb/xcb.h>

using std::cout;
using std::endl;

int main()
{
    xcb_connection_t *c;
    int screen;
    
    c = xcb_connect( 0, &screen );
    if( c == 0 )
    {
	cout << ":: xcb_connect() failed" << endl;
	return -1;
    }
    
    cout << ":: screen = " << screen << endl;
    
    xcb_disconnect( c );
    
    return 0;
}