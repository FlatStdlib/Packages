#include "init.h"

web_t WEB_SERVER = NULL;

web_t init_web_server(string ip, i32 port)
{
    web_t w = allocate(0, sizeof(_web));

    w->con = listen_tcp(ip, port, 999);
	if(w->con->fd > 0) {
        WEB_SERVER = w;
        _printf("Server Started @ http://%s/:%d", ip, (ptr)&port);
        return w;
    }

	fsl_panic("Unable to create socket...");
    return NULL;
}

void run(web_t w)
{
    while(w->running != false)
    {
        sock_t client = sock_accept(w->con, 1024);

        _start_thread((void *)request_handler, client);
    }
}


void request_handler(sock_t client)
{
    if(!client) return;
    
    _req r;
    memzero(&r, sizeof(_req));

    string rdata = sock_read(client);
    r.lines = split_string(rdata, '\n', &r.line_count);

    int argc = 0;
    sArr args = split_string(r.lines[0], ' ', &argc);
    if(argc < 3 || !args) {
        pfree_array((array)r.lines);
        return;
    }
    
    
    // Parse Req
    // Validate route in router
}

i32 search_route(web_t w, string q)
{
    for(int i = 0; i < __get_meta__(w->router)->length; i++)
    {
        if(str_cmp(((route_t)w->router[i])->route, q))
            return i;
    }

    return -1;
}