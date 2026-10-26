/* The libcurl this program binds: `curl/curl.h`, with `curl_easy_setopt` the
 * function `curl/easy.h` declares rather than the macro `curl.h` puts over it
 * (defect 2229, 2026-10-26).
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
#include <curl/curl.h>
#undef curl_easy_setopt
