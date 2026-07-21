#include "init.h"

web_t WEB_SERVER = NULL;

web_t init_web_server(string ip, i32 port)

{
    web_t w = allocate(0, sizeof(_web));

    w->con = listen_tcp(ip, port, 999);
	if(w->con->fd <= 0)
	    fsl_panic("Unable to create socket...");

    WEB_SERVER = w;
    w->router = allocate(sizeof(_req), 1);
    return w;
}

void run(web_t w)
{
    w->running = true;
    while(w->running != false)
    {
        sock_t client = sock_accept(w->con, 1024);
        if(!client)
            return;

        run_thread((void *)request_handler, client);
        // request_handler(client);
    }
}


void request_handler()
{
    register long v asm("r13");
    sock_t client = (sock_t)v;
    if(!client) return;
    
    _req r;
    memzero(&r, sizeof(_req));

    string rdata = sock_read(client);
    if(!rdata) return;

    r.lines = split_string(rdata, '\n', &r.line_count);

    int argc = 0;
    sArr args = split_string(r.lines[0], ' ', &argc);
    if(argc < 3 || !args) {
        pfree_array((array)r.lines);
        return;
    }

    int pos = 0;
    if((pos = search_route(WEB_SERVER, args[1])) == -1)
    {
        pfree_array((array)args);
        pfree_array((array)r.lines);
        return;
    }

    r.body = allocate(0, 4096);
	r.headers = init_map();
	int stop_headers = 0;
	int i = 0;
	for(i = 1; i < argc; i++)
	{
		string line = r.lines[i];
		if(!line) break;

		if(line[1] == '\0') {
			stop_headers = 1;
			continue;
		}

		if(!stop_headers)
		{
			i32 pos = find_char(line, ':');
			if(pos == -1)
				continue;

			string key = line;
			line[pos] ='\0';

			string value = line + pos + 2;
			map_append(r.headers, key, value);
			continue;
		}

		str_append(r.body, line);
	}

    req_t n = to_heap(&r, sizeof(_req));
    n->con = client;
    
    ((void *(*)(req_t))((route_t)WEB_SERVER->router[pos])->handler)(n);

    // Todo; Destruct shit here
    req_destruct(n);
    _pfree(rdata);
    __syscall__(0, -1, -1, -1, -1, -1, _SYS_EXIT);
}

void send_response(sock_t sock, status_code_t c0de, map_t headers, map_t cookies, string body)
{
	string response = allocate(0, 4096);
	str_append(response, "HTTP/1.1 ");

	str_append_int(response, c0de);
	str_append(response, " ");
	str_append(response, code_2_str(c0de));
	str_append(response, "\r\n");

	if(headers->len > 0)
	{
		for(int i = 0; i < headers->len; i++)
		{
			str_append(response, headers->fields[i]->key);
			str_append(response, ": ");
			str_append(response, headers->fields[i]->value);
			str_append(response, "\r\n");
		}

		str_append(response, "\r\n");
	}

	if(body)
	{
		str_append(response, body);
		str_append(response, "\r\n");
	}

	if(__syscall__(sock->fd, (long)response, _str_len(response), -1, -1, -1, _SYS_WRITE) <= 0)
		println("Error, Failed to send data to client request!");
}

i32 search_route(web_t w, string q)
{
    printi(__get_meta__(w->router)->length), println(NULL);
    for(int i = 0; i < __get_meta__(w->router)->length; i++)
    {
        if(!w->router[i]) break;
        if(str_cmp(((route_t)w->router[i])->route, q))
            return i;
    }

    return -1;
}

void req_destruct(req_t r)
{
	if(r->body)
		_pfree(r->body);

	if(r->headers)
		map_destruct(r->headers);

	if(r->lines)
		pfree_array((array)r->lines);

    _pfree(r);
}