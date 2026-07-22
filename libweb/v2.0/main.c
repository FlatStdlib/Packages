#include "src/init.h"

void _test(req_t r)
{
    string resp = "Hello World";
    string num = int_to_str(str_len(resp) + 2);
    _map m = {
        .fields = (field_t []){
            (field_t)&(_field){ "Content-type", "text/html"},
            (field_t)&(_field){ "Content-length", num }
        },
        .len = 2
    };

    println("Served");
	send_response(r->con, OK, &m, NULL, resp);
    _pfree(num);
}

int entry()
{
    toggle_debug_mode();
	// uninit_mem();
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

    web_run(web);
    // run_thread(run, web);
    println("[ + ] Web Server Started....");

    // while(get_input(_OUTPUT_, 10) != -1 || get_input(_OUTPUT_, 10) != 0)
    // {
    //     if(_OUTPUT_[0] == 'y') {
    //         break;
    //     }
    // }

    return 0;
}

int main() { return entry(); }