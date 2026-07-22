#include "init.h"

#define ENCODED_SYMBOL_COUNT 9
void *EncodedSymbols[][2] = {
    {(void *)' ',      "+"},
    {(void *)'@',      "%40"},
    {(void *)';',      "%3B"},
    {(void *)':',      "%3A"},
    {(void *)'#',      "%23"},
    {(void *)'"',      "\\"},
    {(void *)'/',      "%2F"},
    {(void *)'?',      "%3F"},
    {(void *)'=',      "%3D"},
    NULL
};

#define STATUS_CODE_COUNT 63
void *StatusCodeDef[][2] = {
    {(void *)CONTINUEE,                         "Continue" },
    {(void *)SWITCH_PROTOCOL,                   "Switching Protocols" },
    {(void *)PROCESSING,                        "Processing" },
    {(void *)EARLY_HINT,                        "Early Hints" },
    {(void *)OK,                                "OK" },
    {(void *)CREATED,                           "Created" },
    {(void *)ACCEPTED,                          "Accepted" },
    {(void *)NON_AUTHORIZED_INFO,               "Non-Authoritative Information" },
    {(void *)NO_CONTENT,                        "No Content" },
    {(void *)RESET_CONTENT,                     "Reset Content" },
    {(void *)PARTIAL_CONTENT,                   "Partial Content" },
    {(void *)MULTI_STATUS,                      "Multi-Status" },
    {(void *)ALREADY_REPRORTED,                 "Already Reported" },
    {(void *)IM_USED,                           "IM Used" },
    {(void *)MULTIPLE_CHOICES,                  "Multiple Choices" },
    {(void *)MOVED_PERMANENTLY,                 "Moved Permanently" },
    {(void *)FOUND,                             "Found" },
    {(void *)SEE_OTHER,                         "See Other" },
    {(void *)NOT_MODIFIED,                      "Not Modified" },
    {(void *)USE_PROXY,                         "Use Proxy" },
    {(void *)SWITCH_PROXY,                      "Switch Proxy" },
    {(void *)TEMP_REDIRECT,                     "Temporary Redirect" },
    {(void *)PERM_REDIRECT,                     "Permanent Redirect" },
    {(void *)BAD_REQUEST,                       "Bad Request" },
    {(void *)UNAUTHORIZED,                      "Unauthorized" },
    {(void *)PAYMENT_REQUIRED,                  "Payment Required" },
    {(void *)FORBIDDEN,                         "Forbidden" },
    {(void *)NOT_FOUND,                         "Not Found" },
    {(void *)METHOD_NOT_ALLOWED,                "Method Not Allowed" },
    {(void *)NOT_ACCEPTABLE,                    "Not Acceptable" },
    {(void *)PROXY_AUTH_REQUIRED,               "Proxy Authentication Required" },
    {(void *)REQUIRE_TIMEOUT,                   "Request Timeout" },
    {(void *)CONFLICT,                          "Conflict" },
    {(void *)GONE,                              "Gone" },
    {(void *)LENGTH_REQUIRED,                   "Length Required" },
    {(void *)PRECONDITION_FAILED,               "Precondition Failed" },
    {(void *)PAYLOAD_TOO_LARGE,                 "Content Too Large" },
    {(void *)URI_TOO_LONG,                      "URI Too Long" },
    {(void *)UNSUPPORTED_MEDIA_TYPE,            "Unsupported Media Type" },
    {(void *)RANGE_NOT_SATISFIABLE,             "Range Not Satisfiable" },
    {(void *)EXPECTATION_FAILED,                "Expectation Failed" },
    {(void *)IM_A_TEAPOT,                       "I'm a teapot" },
    {(void *)MISDIRECTED_REQUEST,               "Misdirected Request" },
    {(void *)UNPROCESSABLE_ENTITY,              "Unprocessable Content" },
    {(void *)LOCKED,                            "Locked" },
    {(void *)FAILED_DEPENDENCY,                 "Failed Dependency" },
    {(void *)TOO_EARLY,                         "Too Early" },
    {(void *)UPGRADE_REQUIRED,                  "Upgrade Required" },
    {(void *)PROCONDITION_REQUIRED,             "Precondition Required" },
    {(void *)TOO_MANY_REQUEST,                  "Too Many Requests" },
    {(void *)REQ_HEADER_FIELD_TOO_LARGE,        "Request Header Fields Too Large" },
    {(void *)UNAVAILABLE_FOR_LEGAL_REASON,      "Unavailable For Legal Reasons" },

    {(void *)INTERNAL_SERVER_ERR,               "Internal Server Error" },
    {(void *)NOT_IMPLEMENTED,                   "Not Implemented" },
    {(void *)BAD_GATEWAY,                       "Bad Gateway" },
    {(void *)SERVER_UNAVAILABLE,                "Service Unavailable" },
    {(void *)GATEWAY_TIMEOUT,                   "Gateway Timeout" },
    {(void *)HTTP_VERSION_NOT_SUPPORTED,        "HTTP Version Not Supported" },
    {(void *)VARIANT_ALSO_NEGOTIATES,           "Variant Also Negotiates" },
    {(void *)INSUFFICIENT_STORAGE,              "Insufficient Storage" },
    {(void *)LOOP_DETECTED,                     "Loop Detected" },
    {(void *)NOT_EXTENDED,                      "Not Extended" },
    {(void *)NETWORK_AUTHENTICATION_REQUIRED,   "Network Authentication Required" },
    NULL
};

status_code_t find_code_t(string c0de)
{
	for(int i = 0; i < STATUS_CODE_COUNT; i++)
	{
		void **n = (void **)StatusCodeDef[i];
		if(str_cmp(n[1], c0de))
			return (status_code_t)n[0];
	}

	return -1;
}

string code_2_str(status_code_t c0de)
{
	for(int i = 0; i < STATUS_CODE_COUNT; i++)
	{
		void **n = (void **)StatusCodeDef[i];
		if((status_code_t)n[0] == c0de)
			return n[1];
	}

	return NULL;
}

string decode_input_symbols(const string data) {
    if(!data)
        return NULL;

    string buff = allocate(0, str_len(data));
    str_append(buff, data);

    for(int i = 0; i < ENCODED_SYMBOL_COUNT; i++) {
        if(!EncodedSymbols[i])
            break;

        int pos = -1;
        while((pos = find_string(buff, EncodedSymbols[i][1])) != -1) {
            int rm_len = str_len(EncodedSymbols[i][1]);
            for(int b = 0; b < rm_len; b++)
                trim_char_idx(buff, pos);
                
            // buff[pos - 1] = ((char)((char *)EncodedSymbols[i])[0]);
            pos = -1;
        }
    }

    if(str_len(buff) > 0)
        return buff;

    _pfree(buff);
    return NULL;
}