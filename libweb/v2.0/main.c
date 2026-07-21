#include "src/init.h"

void _test(req_t r)
{
    map_t m = init_map();
	map_append(m, "Content-type", "text/html");
	map_append(m, "Content-length", "13");
	send_response(r->con, OK, m, NULL, "Hello World");
}

int entry()
{
	uninit_mem();
	set_heap_sz(_HEAP_PAGE_ * 40);
	init_mem();
    web_t web = init_web_server("127.0.0.1", 80);
    if(!web)
        return 1;
    
    web->router[0] = to_heap(&(_route){
        .name = "Index",
        .route = "/",
        .content = "Hello World",
        .handler = (void *)_test
    }, sizeof(_route));

    run(web);
    println("[ + ] Web Server Started....");
    return 0;
}