/* Route I's stand-in for `extern "curl/curl.h"` in examples/curl/main.hero. */
typedef struct hero_stand_in_CURL CURL;
enum { CURLOPT_URL = 0, CURLE_OK = 0, CURLE_UNSUPPORTED_PROTOCOL = 0 };
const char *curl_version(void);
CURL *curl_easy_init(void);
int64_t curl_easy_setopt(CURL *a0, uint32_t a1, const char *a2);
const char *curl_easy_strerror(uint32_t a0);
void curl_easy_cleanup(CURL *a0);
