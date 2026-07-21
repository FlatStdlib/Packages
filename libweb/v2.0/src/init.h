#include <fsl.h>

#ifndef _FSL_WEB_SERVER

typedef enum {
    get_params = 1,
    post_params,

    headers,
    cookies
} ParseFlags;


typedef enum status_code_t {
    CONTINUEE                       = 100,
    SWITCH_PROTOCOL                 = 101,
    PROCESSING                      = 102,
    EARLY_HINT                      = 103,

    OK                              = 200,
    CREATED                         = 201,
    ACCEPTED                        = 202,
    NON_AUTHORIZED_INFO             = 203,
    NO_CONTENT                      = 204,
    RESET_CONTENT                   = 205,
    PARTIAL_CONTENT                 = 206,
    MULTI_STATUS                    = 207,
    ALREADY_REPRORTED               = 208,
    IM_USED                         = 226,

    MULTIPLE_CHOICES                = 300,
    MOVED_PERMANENTLY               = 301,
    FOUND                           = 302,
    SEE_OTHER                       = 303,
    NOT_MODIFIED                    = 304,
    USE_PROXY                       = 305,
    SWITCH_PROXY                    = 306,
    TEMP_REDIRECT                   = 307,
    PERM_REDIRECT                   = 308,

    BAD_REQUEST                     = 400,
    UNAUTHORIZED                    = 401,
    PAYMENT_REQUIRED                = 402,
    FORBIDDEN                       = 403,
    NOT_FOUND                       = 404,
    METHOD_NOT_ALLOWED              = 405,
    NOT_ACCEPTABLE                  = 406,
    PROXY_AUTH_REQUIRED             = 407,
    REQUIRE_TIMEOUT                 = 408,
    CONFLICT                        = 409,
    GONE                            = 410,
    LENGTH_REQUIRED                 = 411,
    PRECONDITION_FAILED             = 412,
    PAYLOAD_TOO_LARGE               = 413,
    URI_TOO_LONG                    = 414,
    UNSUPPORTED_MEDIA_TYPE          = 415,
    RANGE_NOT_SATISFIABLE           = 416,
    EXPECTATION_FAILED              = 417,
    IM_A_TEAPOT                     = 418,
    MISDIRECTED_REQUEST             = 421,
    UNPROCESSABLE_ENTITY            = 422,
    LOCKED                          = 423,
    FAILED_DEPENDENCY               = 424,
    TOO_EARLY                       = 425,
    UPGRADE_REQUIRED                = 426,
    PROCONDITION_REQUIRED           = 428,
    TOO_MANY_REQUEST                = 429,
    REQ_HEADER_FIELD_TOO_LARGE      = 431,
    UNAVAILABLE_FOR_LEGAL_REASON    = 451,
    
    INTERNAL_SERVER_ERR             = 500,
    NOT_IMPLEMENTED                 = 501,
    BAD_GATEWAY                     = 502,
    SERVER_UNAVAILABLE              = 503,
    GATEWAY_TIMEOUT                 = 504,
    HTTP_VERSION_NOT_SUPPORTED      = 505,
    VARIANT_ALSO_NEGOTIATES         = 506,
    INSUFFICIENT_STORAGE            = 507,
    LOOP_DETECTED                   = 508,
    NOT_EXTENDED                    = 510,
    NETWORK_AUTHENTICATION_REQUIRED = 511
} status_code_t;

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
    sock_t  con;
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

/* init.c */
web_t init_web_server(string ip, i32 port);
void run(web_t w);
void request_handler();
void send_response(sock_t sock, status_code_t c0de, map_t headers, map_t cookies, string body);
i32 search_route(web_t w, string q);
void req_destruct(req_t r);

/* parsers.c */
status_code_t find_code_t(string c0de);
string code_2_str(status_code_t c0de);

#endif