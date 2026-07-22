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
    }
}


void request_handler()
{
    register long v asm("r13");
    sock_t client = (sock_t)v;
    if(!client) return;
    
    req_t r = allocate(sizeof(_req), 1);

    string rdata = sock_read(client);
    if(!rdata) {
        req_destruct(r);
        // sock_close(client);
        return;
    }

    r->lines = split_string(rdata, '\n', &r->line_count);
    int argc = 0;

    sArr args = split_string(r->lines[0], ' ', &argc);
    if(argc < 3 || !args) {
        req_destruct(r);
        _pfree(rdata);
        sock_close(client);
        return;
    }

    r->req_type = str_dup(args[0]);
    r->route = str_dup(args[1]);
    r->req_version = str_dup(args[2]);

    i32 rpos = 0;
    i32 len = str_len(r->route);
    if((rpos = find_char(r->route, '?')) > -1)
    {
        r->route[rpos] = '\0';
        string copy = (string)((string)r->route + rpos + 1);
        _printf("Route: %s | Copy: %s | Pos: %d\n", r->route, copy, &rpos);

        i32 g_argc = 0;
        r->GET = init_map();
        if(find_char(r->route, '&') > -1)
        {
            sArr get_params = split_string(r->route, '&', &g_argc);
            if(g_argc > 0)
            {
                for(i32 i = 0; i < g_argc; i++) {
                    i32 k_argc = 0;
                    sArr key = split_string(get_params[i], '=', &k_argc);
                    if(k_argc != 2) fsl_panic("& malformed request");
                    map_append(r->GET, key[0], key[1]);

                    pfree_array((array)key);
                }
            }

            pfree_array((array)get_params);
        } else {
            sArr key = split_string(copy, '=', &g_argc);
            if(g_argc != 2) fsl_panic("malformed request");
            map_append(r->GET, key[0], key[1]);
            pfree_array((array)key);
        }
    }

    
    int pos = 0;
    if((pos = search_route(WEB_SERVER, r->route)) == -1)
    {
        req_destruct(r);
        _pfree(rdata);
        pfree_array((array)args);
        sock_close(client);
        return;
    }

    r->body = allocate(0, 4096);
	r->headers = init_map();
	bool stop_headers = 0;
	i32 i = 0;
	for(i = 1; i < argc; i++)
	{
		string line = r->lines[i];
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
			map_append(r->headers, key, value);
			continue;
		}

		str_append(r->body, line);
	}
    
    r->con = client;
    _printf("[WEB_SERVER] New request for '%s'\n", r->route);
    ((void *(*)(req_t))((route_t)WEB_SERVER->router[pos])->handler)(r);

    req_destruct(r);
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
    if(r->req_type)
        _pfree(r->req_type);

    if(r->route)
        _pfree(r->route);

    if(r->req_version)
        _pfree(r->req_version);

	if(r->body)
		_pfree(r->body);

    if(r->GET)
        map_destruct(r->GET);

	if(r->headers)
		map_destruct(r->headers);

	if(r->lines)
		pfree_array((array)r->lines);

    _pfree(r);
}