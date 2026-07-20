#include <fsl.h>

#ifndef _FSL_WEB_SERVER

typedef struct {
    string      name;
    string      route;
    string      content;
    handler_t   handler;
} _route;

typedef _route *route_t;

/* Incoming REQ struct */
typedef struct {
	string 	req_type;
	string 	route;
	string 	req_version;
	map_t 	headers;	// Header(s) map
	map_t 	cookies;	// Cookie(s) map
	map_t 	GET;		// GET Params
	map_t 	POST;		// POST Params/Data

	string 	body;		// Request Body (Holding POST data)
	sArr 	lines;
    i32     line_count;
} _req;

typedef _req *req_t;

/* Web server struct */
typedef struct {
    sock_t      con;
    bool        running;
    array       router;
} _web;

typedef _web *web_t;

extern web_t WEB_SERVER;

#define new_router(n) \
    init_array(n)

#define append_route(arr, element) \
    array_append((array)arr, element);

#define router_destruct(arr) \
    pfree_array((array)arr)

web_t init_web_server(string ip, i32 port);
void run(web_t w);
void request_handler(sock_t client);
i32 search_route(web_t w, string q);

#endif