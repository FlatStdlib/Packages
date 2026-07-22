#include "src/init.h"

void _test(req_t r)
{
    string resp = "Hello World";
    _map m = {
        .fields = (field_t []){
            (field_t)&(_field){ "Content-type", "text/html"},
            (field_t)&(_field){ "Content-length", int_to_str(str_len(resp)) }
        },
        .len = 2
    };

	send_response(r->con, OK, &m, NULL, resp);
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