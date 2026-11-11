/* Defect 2467: a variadic, and a macro so that a call passes exactly two, as curl/curl.h writes curl_easy_setopt. */
int v2467(int a, ...);
#define v2467(a, b) v2467(a, b)
