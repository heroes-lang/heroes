/* The libcurl this program binds: `curl/curl.h`, with `curl_easy_setopt` the
 * function `curl/easy.h` declares rather than the macro `curl.h` puts over it
 * (defect 2229, 2026-10-26), and the one door this program calls it through
 * (defect 2421, 2026-11-10).
 *
 * Under a GNU C compiler, `curl.h` turns `curl_easy_setopt` into a macro that
 * checks `value`'s type against `option` (`typecheck-gcc.h`). libcurl 8.18,
 * the release Ubuntu 26.04 carries, wraps that check in
 * `CURL_IGNORE_DEPRECATION`, whose `_Pragma("GCC diagnostic push")` changes how
 * clang judges every line after the call, this program's binding checks among
 * them, and the compiler refuses the binding (`ffi_macro_name`). The remedy is
 * the one its note names: include the header, then take the macro off, so the
 * name is the function again and clang holds this program's declaration to
 * its prototype like any other. libcurl 8.7.1 on macOS defines the macro with
 * no pragma, and the `#undef` takes it off there too. */

/* `curl_easy_setopt(CURL *, CURLoption, ...)` takes its value past `...`,
 * where nothing holds an argument to a type: libcurl reads it as the option
 * says, a `long`, a string or a pointer, so a binding naming the value held
 * it to nothing (panel 243's R1: a binding declares no parameter past a
 * header's `...`). This header defines the door instead, a function of a new
 * name with a fixed prototype for the one option this program sets: its
 * `const char *` holds the URL to a string, and the option is written here,
 * so no call can hand the string to an option that reads a `long`. libcurl
 * copies every string option but `CURLOPT_POSTFIELDS`, so the URL is lent. */
#include <curl/curl.h>
#undef curl_easy_setopt

static inline CURLcode net_set_url(CURL *handle, const char *url) {
    return curl_easy_setopt(handle, CURLOPT_URL, url);
}
