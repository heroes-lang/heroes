/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_web_gtk.c: the web view on Linux, WebKitGTK 6.0, one
 * member of the archive the compiler puts on every program's link line
 * (`selfhost/cli/layer.hero`). Its header, `hero_ui_web_gtk.h`, says what
 * each function answers and who calls it; this file says how.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so a definition
 * with another signature than the header's is `conflicting types` where it
 * is compiled.
 *
 * WHY `dlopen`, as `hero_keyring.c` says of libsecret: the layer's archive
 * is compiled for every program a machine builds, so a member that included
 * WebKitGTK's headers would stop every build on a machine without them, and
 * one linked against the library every program on a machine without it. So
 * the member names no header of WebKitGTK, GTK, GLib or libsoup: it declares
 * the functions it calls with their own signatures, as WebKitGTK 6.0's,
 * GLib 2's and libsoup 3's references give them (read 2026-11-22), and the
 * first web view opens `libwebkitgtk-6.0.so.4` once, `RTLD_LOCAL`, and finds
 * every function by `dlsym` on its handle, which searches the library and
 * the libraries it needs (GTK 4, GLib, GIO, libsoup 3, JavaScriptCore). One
 * missing is the library missing: then `create` answers
 * `HERO_UI_UNSUPPORTED` naming it. The library is never closed.
 *
 * WHAT IT ASKS THE LAYER, never decides itself (`platform/hero_ui_web.h`):
 * every request of the app's scheme is answered whole with what the host's
 * `serve` wrote; every navigation of the main frame goes on where `ask`
 * says so; a message is queued where `ask` hears the page's origin. The
 * scheme is registered once, on WebKit's default context, and a request
 * finds its web view by the view WebKit names for it.
 *
 * WHAT WEBKITGTK DOES NOT SAY, AND HOW THIS ARM HOLDS THE LAYER'S RULE
 * ANYWAY. Its message signal names no frame, so a frame inside the page
 * could post to the handler as the page does. The arm gives each web view a
 * seal of 16 random bytes, written as 32 hexadecimal digits and a line
 * feed, which a script run before the bridge, in the main frame alone,
 * hands the bridge as `window.__heroesSeal`; the bridge takes it away and
 * puts it before every text it sends, so a message that does not begin with
 * the seal came from no bridge and is dropped and counted (`ask`'s
 * `SENDER` with no URL). And its policy signal is asked of the main frame's
 * navigations alone: a frame inside the page is held to the page's content
 * security policy, whose default (`HERO_UI_WEB_POLICY_DEFAULT`) frames the
 * app's own origin alone.
 *
 * WHAT THE PAGE MAY NOT DO: a new web view is refused (`create` answers
 * none), a download is cancelled as it starts, a permission (the camera, the
 * microphone, the location, notifications) denied, a file chooser
 * cancelled, and the page's storage kept in an ephemeral network session of
 * its own, in memory alone.
 *
 * MEMORY: a web view's record and the texts kept for its page are the
 * layer's (`hero_layer_alloc`); WebKit's objects are released by GLib's
 * reference counts, each one this arm took given back at `destroy`.
 *
 * Written from the platforms' documentation; no other library's source.
 */

#include "hero_ui_web_gtk.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "hero_layer_alloc.h"
#include "hero_ui.h"

#if defined(__linux__)
#include <dlfcn.h>
#include <pthread.h>
#include <sys/random.h>
#include <sys/types.h>
#endif

/* -- THE LAST FAILURE, PER THREAD ------------------------------------------ */

#define HERO_UI_WEB_GTK_WORDS_MOST 512

static _Thread_local char hero_ui_web_gtk_said_words[HERO_UI_WEB_GTK_WORDS_MOST];

/* Keeps `words` as the last failure's and answers `kind`. */
static int64_t hero_ui_web_gtk_said(int64_t kind, const char *words) {
    size_t length = strlen(words);

    if (length >= sizeof hero_ui_web_gtk_said_words) {
        length = sizeof hero_ui_web_gtk_said_words - 1;
        while (length > 0 && ((unsigned char)words[length] & 0xC0u) == 0x80u) {
            length -= 1;
        }
    }
    memcpy(hero_ui_web_gtk_said_words, words, length);
    hero_ui_web_gtk_said_words[length] = '\0';
    return kind;
}

int64_t hero_ui_web_gtk_words(char *words, int64_t count) {
    size_t length = strlen(hero_ui_web_gtk_said_words);

    if (words == NULL || count <= 0) {
        return 0;
    }
    if (length >= (size_t)count) {
        length = (size_t)count - 1;
        while (length > 0 && ((unsigned char)hero_ui_web_gtk_said_words[length] & 0xC0u) == 0x80u) {
            length -= 1;
        }
    }
    memcpy(words, hero_ui_web_gtk_said_words, length);
    words[length] = '\0';
    return (int64_t)length;
}

#if defined(__linux__)
/* ==========================================================================
 * THE LINUX ARM
 * ========================================================================== */

/* GLib's and WebKit's numbers this arm reads, as their references name them:
 * `WebKitUserContentInjectedFrames`, `WebKitUserScriptInjectionTime`,
 * `WebKitPolicyDecisionType`, `WebKitLoadEvent`, `SoupMessageHeadersType`. */
#define HERO_WEBKIT_INJECT_TOP_FRAME 1
#define HERO_WEBKIT_INJECT_AT_DOCUMENT_START 0
#define HERO_WEBKIT_POLICY_NAVIGATION_ACTION 0
#define HERO_WEBKIT_POLICY_NEW_WINDOW_ACTION 1
#define HERO_WEBKIT_POLICY_RESPONSE 2
#define HERO_WEBKIT_LOAD_STARTED 0
#define HERO_WEBKIT_LOAD_FINISHED 3
#define HERO_SOUP_HEADERS_RESPONSE 1

/* The library opened, and the one every function is found in. */
#define HERO_WEBKIT_LIBRARY "libwebkitgtk-6.0.so.4"

/* GLib's `GCallback` and `GDestroyNotify`. */
typedef void (*hero_gcallback)(void);
typedef void (*hero_gdestroy)(void *data);

/* The functions, found once by `hero_ui_web_gtk_load_library` under
 * `hero_ui_web_gtk_once`, and read only after it. A WebKit, GTK or GLib
 * object is a `void *` here: this arm reads no field of one. */
typedef struct {
    /* GLib and GObject. */
    void *(*object_new)(unsigned long type, const char *first_property, ...);
    void *(*object_ref)(void *object);
    void (*object_unref)(void *object);
    unsigned long (*signal_connect_data)(void *instance, const char *signal, hero_gcallback handler, void *data, hero_gdestroy destroy, int flags);
    void (*signal_handlers_disconnect_by_data)(void *instance, void *data);
    void *(*g_malloc)(size_t size);
    void (*g_free)(void *memory);
    void *(*variant_new_parsed)(const char *format, ...);
    void *(*memory_input_stream_new_from_data)(const void *data, ptrdiff_t length, hero_gdestroy destroy);
    /* WebKitGTK 6.0. */
    unsigned long (*web_view_get_type)(void);
    void *(*web_context_get_default)(void);
    void (*register_uri_scheme)(void *context, const char *scheme, void (*callback)(void *request, void *data), void *data, hero_gdestroy destroy);
    void *(*get_security_manager)(void *context);
    void (*register_scheme_as_secure)(void *manager, const char *scheme);
    void *(*network_session_new_ephemeral)(void);
    void *(*user_content_manager_new)(void);
    void *(*user_script_new)(const char *source, int frames, int time, const char *const *allow, const char *const *block);
    void (*add_script)(void *manager, void *script);
    void (*user_script_unref)(void *script);
    int (*register_message_handler)(void *manager, const char *name, const char *world);
    void (*unregister_message_handler)(void *manager, const char *name, const char *world);
    int (*jsc_value_is_string)(void *value);
    char *(*jsc_value_to_string)(void *value);
    void (*load_uri)(void *view, const char *uri);
    const char *(*get_uri)(void *view);
    void (*stop_loading)(void *view);
    void (*reload)(void *view);
    void (*call_async_javascript_function)(void *view, const char *body, ptrdiff_t length, void *arguments, const char *world, const char *source_uri, void *cancellable, void *callback, void *data);
    void *(*navigation_policy_decision_get_navigation_action)(void *decision);
    void *(*navigation_action_get_request)(void *action);
    const char *(*uri_request_get_uri)(void *request);
    int (*response_policy_decision_is_mime_type_supported)(void *decision);
    void (*policy_decision_use)(void *decision);
    void (*policy_decision_ignore)(void *decision);
    void (*permission_request_deny)(void *request);
    void (*file_chooser_request_cancel)(void *request);
    void (*download_cancel)(void *download);
    void *(*uri_scheme_request_get_web_view)(void *request);
    const char *(*uri_scheme_request_get_uri)(void *request);
    const char *(*uri_scheme_request_get_http_method)(void *request);
    void *(*uri_scheme_response_new)(void *stream, int64_t length);
    void (*uri_scheme_response_set_status)(void *response, unsigned status, const char *reason);
    void (*uri_scheme_response_set_content_type)(void *response, const char *type);
    void (*uri_scheme_response_set_http_headers)(void *response, void *headers);
    void (*uri_scheme_request_finish_with_response)(void *request, void *response);
    /* libsoup 3. */
    void *(*soup_headers_new)(int type);
    void (*soup_headers_append)(void *headers, const char *name, const char *value);
} hero_ui_web_gtk_library;

static hero_ui_web_gtk_library hero_ui_web_gtk_functions;
static bool hero_ui_web_gtk_found;
static pthread_once_t hero_ui_web_gtk_once = PTHREAD_ONCE_INIT;

/* One function by name into `*slot`; false where the library has none. The
 * address is copied by its bytes, which POSIX makes the same for an object
 * and a function pointer (`dlsym`'s own page). */
static bool hero_ui_web_gtk_find(void *library, const char *name, void *slot, size_t slot_size) {
    void *found = dlsym(library, name);

    if (found == NULL || slot_size != sizeof found) {
        return false;
    }
    memcpy(slot, &found, sizeof found);
    return true;
}

#define HERO_WEBKIT_FIND(field, name) ok = ok && hero_ui_web_gtk_find(library, name, &hero_ui_web_gtk_functions.field, sizeof hero_ui_web_gtk_functions.field)

static void hero_ui_web_gtk_load_library(void) {
    bool ok = true;
    void *library = dlopen(HERO_WEBKIT_LIBRARY, RTLD_NOW | RTLD_LOCAL);

    if (library == NULL) {
        return;
    }
    HERO_WEBKIT_FIND(object_new, "g_object_new");
    HERO_WEBKIT_FIND(object_ref, "g_object_ref");
    HERO_WEBKIT_FIND(object_unref, "g_object_unref");
    HERO_WEBKIT_FIND(signal_connect_data, "g_signal_connect_data");
    HERO_WEBKIT_FIND(signal_handlers_disconnect_by_data, "g_signal_handlers_disconnect_by_data");
    HERO_WEBKIT_FIND(g_malloc, "g_malloc");
    HERO_WEBKIT_FIND(g_free, "g_free");
    HERO_WEBKIT_FIND(variant_new_parsed, "g_variant_new_parsed");
    HERO_WEBKIT_FIND(memory_input_stream_new_from_data, "g_memory_input_stream_new_from_data");
    HERO_WEBKIT_FIND(web_view_get_type, "webkit_web_view_get_type");
    HERO_WEBKIT_FIND(web_context_get_default, "webkit_web_context_get_default");
    HERO_WEBKIT_FIND(register_uri_scheme, "webkit_web_context_register_uri_scheme");
    HERO_WEBKIT_FIND(get_security_manager, "webkit_web_context_get_security_manager");
    HERO_WEBKIT_FIND(register_scheme_as_secure, "webkit_security_manager_register_uri_scheme_as_secure");
    HERO_WEBKIT_FIND(network_session_new_ephemeral, "webkit_network_session_new_ephemeral");
    HERO_WEBKIT_FIND(user_content_manager_new, "webkit_user_content_manager_new");
    HERO_WEBKIT_FIND(user_script_new, "webkit_user_script_new");
    HERO_WEBKIT_FIND(add_script, "webkit_user_content_manager_add_script");
    HERO_WEBKIT_FIND(user_script_unref, "webkit_user_script_unref");
    HERO_WEBKIT_FIND(register_message_handler, "webkit_user_content_manager_register_script_message_handler");
    HERO_WEBKIT_FIND(unregister_message_handler, "webkit_user_content_manager_unregister_script_message_handler");
    HERO_WEBKIT_FIND(jsc_value_is_string, "jsc_value_is_string");
    HERO_WEBKIT_FIND(jsc_value_to_string, "jsc_value_to_string");
    HERO_WEBKIT_FIND(load_uri, "webkit_web_view_load_uri");
    HERO_WEBKIT_FIND(get_uri, "webkit_web_view_get_uri");
    HERO_WEBKIT_FIND(stop_loading, "webkit_web_view_stop_loading");
    HERO_WEBKIT_FIND(reload, "webkit_web_view_reload");
    HERO_WEBKIT_FIND(call_async_javascript_function, "webkit_web_view_call_async_javascript_function");
    HERO_WEBKIT_FIND(navigation_policy_decision_get_navigation_action, "webkit_navigation_policy_decision_get_navigation_action");
    HERO_WEBKIT_FIND(navigation_action_get_request, "webkit_navigation_action_get_request");
    HERO_WEBKIT_FIND(uri_request_get_uri, "webkit_uri_request_get_uri");
    HERO_WEBKIT_FIND(response_policy_decision_is_mime_type_supported, "webkit_response_policy_decision_is_mime_type_supported");
    HERO_WEBKIT_FIND(policy_decision_use, "webkit_policy_decision_use");
    HERO_WEBKIT_FIND(policy_decision_ignore, "webkit_policy_decision_ignore");
    HERO_WEBKIT_FIND(permission_request_deny, "webkit_permission_request_deny");
    HERO_WEBKIT_FIND(file_chooser_request_cancel, "webkit_file_chooser_request_cancel");
    HERO_WEBKIT_FIND(download_cancel, "webkit_download_cancel");
    HERO_WEBKIT_FIND(uri_scheme_request_get_web_view, "webkit_uri_scheme_request_get_web_view");
    HERO_WEBKIT_FIND(uri_scheme_request_get_uri, "webkit_uri_scheme_request_get_uri");
    HERO_WEBKIT_FIND(uri_scheme_request_get_http_method, "webkit_uri_scheme_request_get_http_method");
    HERO_WEBKIT_FIND(uri_scheme_response_new, "webkit_uri_scheme_response_new");
    HERO_WEBKIT_FIND(uri_scheme_response_set_status, "webkit_uri_scheme_response_set_status");
    HERO_WEBKIT_FIND(uri_scheme_response_set_content_type, "webkit_uri_scheme_response_set_content_type");
    HERO_WEBKIT_FIND(uri_scheme_response_set_http_headers, "webkit_uri_scheme_response_set_http_headers");
    HERO_WEBKIT_FIND(uri_scheme_request_finish_with_response, "webkit_uri_scheme_request_finish_with_response");
    HERO_WEBKIT_FIND(soup_headers_new, "soup_message_headers_new");
    HERO_WEBKIT_FIND(soup_headers_append, "soup_message_headers_append");
    hero_ui_web_gtk_found = ok;
}

/* -- THE WEB VIEWS ---------------------------------------------------------- */

/* A text kept for a page that loads: posted to it, or sent as it. */
typedef struct hero_ui_web_gtk_held {
    bool as_page;
    char *text;
} hero_ui_web_gtk_held;

/* One web view this arm made. */
typedef struct hero_ui_web_gtk_view {
    int64_t id;
    hero_ui_web_host host;
    /* The `WebKitWebView`, its `WebKitUserContentManager` and its
     * `WebKitNetworkSession`, each a reference this arm holds. */
    void *view;
    void *manager;
    void *session;
    /* Whether a page of the app's own origin has loaded. */
    bool ready;
    /* The seal its bridge puts before every text, with its zero. */
    char seal[34];
    /* What waits for the page, `held` of them in a block of `room`. */
    hero_ui_web_gtk_held *waiting;
    int64_t held;
    int64_t room;
} hero_ui_web_gtk_view;

/* The UI thread's web views, `hero_ui_web_gtk_count` of them. */
#define HERO_UI_WEB_GTK_VIEWS_MOST 64
static _Thread_local hero_ui_web_gtk_view *hero_ui_web_gtk_views[HERO_UI_WEB_GTK_VIEWS_MOST];
static _Thread_local int64_t hero_ui_web_gtk_count;
/* Whether the app's scheme is registered on WebKit's default context, which
 * takes a scheme once in a process. */
static _Thread_local bool hero_ui_web_gtk_scheme_registered;

static hero_ui_web_gtk_view *hero_ui_web_gtk_of(int64_t id) {
    for (int64_t i = 0; i < hero_ui_web_gtk_count; i += 1) {
        if (hero_ui_web_gtk_views[i]->id == id) {
            return hero_ui_web_gtk_views[i];
        }
    }
    return NULL;
}

static hero_ui_web_gtk_view *hero_ui_web_gtk_of_view(void *view) {
    for (int64_t i = 0; i < hero_ui_web_gtk_count; i += 1) {
        if (hero_ui_web_gtk_views[i]->view == view) {
            return hero_ui_web_gtk_views[i];
        }
    }
    return NULL;
}

/* `ask` of `url` for `what`, `url` a zero-ended text or NULL for none. */
static bool hero_ui_web_gtk_allowed(const hero_ui_web_gtk_view *v, int64_t what, const char *url) {
    size_t count = url == NULL ? 0 : strlen(url);

    return v->host.ask(what, v->id, (const uint8_t *)url, (int64_t)count) == 1;
}

/* Runs `body` in the page's world with `text` as its argument `t`, never
 * inside the script's own text. */
static void hero_ui_web_gtk_run(hero_ui_web_gtk_view *v, const char *body, const char *text) {
    const hero_ui_web_gtk_library *f = &hero_ui_web_gtk_functions;
    /* A dictionary of one string, `t`; `%s` reads `text` as a value, never
     * as part of the format. */
    void *arguments = f->variant_new_parsed("{'t': <%s>}", text);

    f->call_async_javascript_function(v->view, body, -1, arguments, NULL, NULL, NULL, NULL, NULL);
}

/* Hands what waits to a page of the app's own origin, in order. */
static void hero_ui_web_gtk_flush(hero_ui_web_gtk_view *v) {
    for (int64_t i = 0; i < v->held; i += 1) {
        hero_ui_web_gtk_run(v, v->waiting[i].as_page ? "window.heroes.send(t)" : "window.heroes.deliver(t)", v->waiting[i].text);
        hero_layer_free(v->waiting[i].text);
    }
    v->held = 0;
}

/* Forgets what waits, a page of another origin having loaded. */
static void hero_ui_web_gtk_drop_waiting(hero_ui_web_gtk_view *v) {
    for (int64_t i = 0; i < v->held; i += 1) {
        hero_layer_free(v->waiting[i].text);
    }
    v->held = 0;
}

/* `text` handed to the page now, or kept for it. */
static int64_t hero_ui_web_gtk_given(int64_t id, bool as_page, const uint8_t *text, int64_t count) {
    hero_ui_web_gtk_view *v = hero_ui_web_gtk_of(id);
    char *copy;

    if (v == NULL) {
        return hero_ui_web_gtk_said(HERO_UI_NOT_FOUND, "WebKitGTK holds no such web view");
    }
    if (v->ready) {
        copy = hero_layer_alloc((size_t)count + 1);
        if (copy == NULL) {
            return hero_ui_web_gtk_said(HERO_UI_LIMIT, "no memory for the text");
        }
        memcpy(copy, text, (size_t)count);
        copy[count] = '\0';
        hero_ui_web_gtk_run(v, as_page ? "window.heroes.send(t)" : "window.heroes.deliver(t)", copy);
        hero_layer_free(copy);
        return HERO_UI_OK;
    }
    if (v->held == HERO_UI_QUEUE_MOST) {
        return hero_ui_web_gtk_said(HERO_UI_LIMIT, "as many texts wait for the page as it keeps");
    }
    if (v->held == v->room) {
        int64_t room = v->room == 0 ? 16 : v->room * 2;
        hero_ui_web_gtk_held *grown = hero_layer_grow(v->waiting, (size_t)room * sizeof *grown);

        if (grown == NULL) {
            return hero_ui_web_gtk_said(HERO_UI_LIMIT, "no memory for the texts waiting");
        }
        v->waiting = grown;
        v->room = room;
    }
    copy = hero_layer_alloc((size_t)count + 1);
    if (copy == NULL) {
        return hero_ui_web_gtk_said(HERO_UI_LIMIT, "no memory for the text");
    }
    memcpy(copy, text, (size_t)count);
    copy[count] = '\0';
    v->waiting[v->held].as_page = as_page;
    v->waiting[v->held].text = copy;
    v->held += 1;
    return HERO_UI_OK;
}

/* -- WHAT WEBKIT TELLS ------------------------------------------------------ */

/* "script-message-received::heroes": a text of the bridge, heard where it
 * carries the seal and the main frame shows the app's own origin. */
static void hero_ui_web_gtk_message(void *manager, void *value, void *data) {
    hero_ui_web_gtk_view *v = data;
    const hero_ui_web_gtk_library *f = &hero_ui_web_gtk_functions;
    char *text;
    size_t sealed = strlen(v->seal);

    (void)manager;
    if (!f->jsc_value_is_string(value)) {
        (void)hero_ui_web_gtk_allowed(v, HERO_UI_WEB_ASK_SENDER, NULL);
        return;
    }
    text = f->jsc_value_to_string(value);
    if (text == NULL || strncmp(text, v->seal, sealed) != 0) {
        (void)hero_ui_web_gtk_allowed(v, HERO_UI_WEB_ASK_SENDER, NULL);
    } else if (hero_ui_web_gtk_allowed(v, HERO_UI_WEB_ASK_SENDER, f->get_uri(v->view))) {
        v->host.push(HERO_UI_EVENT_MESSAGE, v->id, 0, 0, (const uint8_t *)text + sealed, (int64_t)(strlen(text) - sealed));
    }
    f->g_free(text);
}

/* "decide-policy": the main frame's navigations asked of the layer, a new
 * window refused, and a response no page shows (a download) refused. */
static int hero_ui_web_gtk_decide(void *view, void *decision, int type, void *data) {
    hero_ui_web_gtk_view *v = data;
    const hero_ui_web_gtk_library *f = &hero_ui_web_gtk_functions;

    (void)view;
    if (type == HERO_WEBKIT_POLICY_NAVIGATION_ACTION) {
        void *action = f->navigation_policy_decision_get_navigation_action(decision);
        const char *url = action == NULL ? NULL : f->uri_request_get_uri(f->navigation_action_get_request(action));

        if (url != NULL && hero_ui_web_gtk_allowed(v, HERO_UI_WEB_ASK_FRAME, url)) {
            f->policy_decision_use(decision);
        } else {
            f->policy_decision_ignore(decision);
        }
        return 1;
    }
    if (type == HERO_WEBKIT_POLICY_RESPONSE) {
        if (f->response_policy_decision_is_mime_type_supported(decision)) {
            f->policy_decision_use(decision);
        } else {
            f->policy_decision_ignore(decision);
        }
        return 1;
    }
    f->policy_decision_ignore(decision);
    return 1;
}

/* "load-changed": a page that starts is not ready; one that finished at the
 * app's own origin is handed what waits, one of another origin nothing. */
static void hero_ui_web_gtk_load_changed(void *view, int event, void *data) {
    hero_ui_web_gtk_view *v = data;

    if (event == HERO_WEBKIT_LOAD_STARTED) {
        v->ready = false;
        return;
    }
    if (event != HERO_WEBKIT_LOAD_FINISHED) {
        return;
    }
    if (hero_ui_web_gtk_allowed(v, HERO_UI_WEB_ASK_OURS, hero_ui_web_gtk_functions.get_uri(view))) {
        v->ready = true;
        hero_ui_web_gtk_flush(v);
    } else {
        v->ready = false;
        hero_ui_web_gtk_drop_waiting(v);
    }
}

/* "create": no new web view, so no new window. */
static void *hero_ui_web_gtk_create_view(void *view, void *action, void *data) {
    (void)view;
    (void)action;
    (void)data;
    return NULL;
}

/* "permission-request": denied, whatever is asked. */
static int hero_ui_web_gtk_permission(void *view, void *request, void *data) {
    (void)view;
    (void)data;
    hero_ui_web_gtk_functions.permission_request_deny(request);
    return 1;
}

/* "run-file-chooser": cancelled, so the page reaches no file. */
static int hero_ui_web_gtk_file_chooser(void *view, void *request, void *data) {
    (void)view;
    (void)data;
    hero_ui_web_gtk_functions.file_chooser_request_cancel(request);
    return 1;
}

/* "download-started" of the web view's session: cancelled. */
static void hero_ui_web_gtk_download(void *session, void *download, void *data) {
    (void)session;
    (void)data;
    hero_ui_web_gtk_functions.download_cancel(download);
}

/* "web-process-terminated": the page reloaded, not ready until it loads. */
static void hero_ui_web_gtk_terminated(void *view, int reason, void *data) {
    hero_ui_web_gtk_view *v = data;

    (void)reason;
    v->ready = false;
    hero_ui_web_gtk_functions.reload(view);
}

/* `text`, `count` bytes, as a zero-ended copy GLib frees, or NULL. */
static char *hero_ui_web_gtk_copy(const uint8_t *text, int64_t count) {
    char *copy = hero_ui_web_gtk_functions.g_malloc((size_t)count + 1);

    if (copy != NULL) {
        if (count > 0) {
            memcpy(copy, text, (size_t)count);
        }
        copy[count] = '\0';
    }
    return copy;
}

/* The app's scheme: a request answered whole with what the layer's `serve`
 * wrote, its headers those `hero_ui.h` names. */
static void hero_ui_web_gtk_serve(void *request, void *data) {
    const hero_ui_web_gtk_library *f = &hero_ui_web_gtk_functions;
    hero_ui_web_gtk_view *v = hero_ui_web_gtk_of_view(f->uri_scheme_request_get_web_view(request));
    const char *url = f->uri_scheme_request_get_uri(request);
    const char *method = f->uri_scheme_request_get_http_method(request);
    int64_t numbers[HERO_UI_WEB_NUMBERS] = { 0 };
    const uint8_t *texts[HERO_UI_WEB_TEXTS] = { NULL };
    char *type;
    char *policy;
    char *body;
    char length[24];
    void *stream;
    void *response;
    void *headers;

    (void)data;
    if (method == NULL) {
        method = "GET";
    }
    if (v == NULL || url == NULL) {
        /* A request of a web view this arm does not hold: answered as a
         * path nobody holds, as the layer answers a web view of no table. */
        numbers[HERO_UI_WEB_STATUS] = 404;
        texts[HERO_UI_WEB_TYPE] = (const uint8_t *)"text/plain; charset=utf-8";
        numbers[HERO_UI_WEB_TYPE_COUNT] = 25;
        texts[HERO_UI_WEB_BODY] = (const uint8_t *)"not found\n";
        numbers[HERO_UI_WEB_LENGTH] = 10;
        numbers[HERO_UI_WEB_BODY_COUNT] = 10;
        texts[HERO_UI_WEB_POLICY] = (const uint8_t *)HERO_UI_WEB_POLICY_DEFAULT;
        numbers[HERO_UI_WEB_POLICY_COUNT] = (int64_t)strlen(HERO_UI_WEB_POLICY_DEFAULT);
    } else {
        (void)v->host.serve(v->id, (const uint8_t *)method, (int64_t)strlen(method), (const uint8_t *)url, (int64_t)strlen(url), numbers, texts);
    }
    type = hero_ui_web_gtk_copy(texts[HERO_UI_WEB_TYPE], numbers[HERO_UI_WEB_TYPE_COUNT]);
    policy = hero_ui_web_gtk_copy(texts[HERO_UI_WEB_POLICY], numbers[HERO_UI_WEB_POLICY_COUNT]);
    /* The stream frees its bytes with GLib's `g_free` when WebKit is done. */
    body = hero_ui_web_gtk_copy(texts[HERO_UI_WEB_BODY], numbers[HERO_UI_WEB_BODY_COUNT]);
    if (type == NULL || policy == NULL || body == NULL) {
        f->g_free(type);
        f->g_free(policy);
        f->g_free(body);
        return;
    }
    stream = f->memory_input_stream_new_from_data(body, (ptrdiff_t)numbers[HERO_UI_WEB_BODY_COUNT], f->g_free);
    response = f->uri_scheme_response_new(stream, numbers[HERO_UI_WEB_BODY_COUNT]);
    f->uri_scheme_response_set_status(response, (unsigned)numbers[HERO_UI_WEB_STATUS], numbers[HERO_UI_WEB_STATUS] == 200 ? "OK" : (numbers[HERO_UI_WEB_STATUS] == 404 ? "Not Found" : "Method Not Allowed"));
    f->uri_scheme_response_set_content_type(response, type);
    headers = f->soup_headers_new(HERO_SOUP_HEADERS_RESPONSE);
    (void)snprintf(length, sizeof length, "%lld", (long long)numbers[HERO_UI_WEB_LENGTH]);
    f->soup_headers_append(headers, "Content-Type", type);
    f->soup_headers_append(headers, "Content-Length", length);
    f->soup_headers_append(headers, "Content-Security-Policy", policy);
    f->soup_headers_append(headers, "X-Content-Type-Options", "nosniff");
    f->soup_headers_append(headers, "Cache-Control", "no-store");
    f->soup_headers_append(headers, "Referrer-Policy", "no-referrer");
    if (numbers[HERO_UI_WEB_STATUS] == 405) {
        f->soup_headers_append(headers, "Allow", "GET, HEAD");
    }
    /* The response takes the headers. */
    f->uri_scheme_response_set_http_headers(response, headers);
    f->uri_scheme_request_finish_with_response(request, response);
    f->object_unref(response);
    f->object_unref(stream);
    f->g_free(type);
    f->g_free(policy);
}

/* -- THE ENTRY POINTS ------------------------------------------------------ */

/* A seal of 16 random bytes as 32 hexadecimal digits and a line feed, into
 * `seal`; false where the system gives no random bytes. */
static bool hero_ui_web_gtk_sealed(char seal[34]) {
    static const char digits[] = "0123456789abcdef";
    uint8_t bytes[16];
    size_t got = 0;

    while (got < sizeof bytes) {
        ssize_t read = getrandom(bytes + got, sizeof bytes - got, 0);

        if (read <= 0) {
            return false;
        }
        got += (size_t)read;
    }
    for (size_t i = 0; i < sizeof bytes; i += 1) {
        seal[2 * i] = digits[bytes[i] >> 4];
        seal[2 * i + 1] = digits[bytes[i] & 15u];
    }
    seal[32] = '\n';
    seal[33] = '\0';
    return true;
}

int64_t hero_ui_web_gtk_create(int64_t id, const hero_ui_web_host *host, void **widget) {
    const hero_ui_web_gtk_library *f = &hero_ui_web_gtk_functions;
    hero_ui_web_gtk_view *v;
    char prelude[96];
    void *script;
    char *bridge;

    if (host == NULL || widget == NULL) {
        return hero_ui_web_gtk_said(HERO_UI_INVALID, "no host or no place for the widget was given");
    }
    (void)pthread_once(&hero_ui_web_gtk_once, hero_ui_web_gtk_load_library);
    if (!hero_ui_web_gtk_found) {
        return hero_ui_web_gtk_said(HERO_UI_UNSUPPORTED, "this machine has no WebKitGTK 6.0 (" HERO_WEBKIT_LIBRARY ", the GTK 4 build), which a web view needs on Linux");
    }
    if (hero_ui_web_gtk_count == HERO_UI_WEB_GTK_VIEWS_MOST) {
        return hero_ui_web_gtk_said(HERO_UI_LIMIT, "an app holds 64 web views at most");
    }
    v = hero_layer_alloc_zero(1, sizeof *v);
    if (v == NULL) {
        return hero_ui_web_gtk_said(HERO_UI_LIMIT, "no memory for the web view");
    }
    v->id = id;
    v->host = *host;
    if (!hero_ui_web_gtk_sealed(v->seal)) {
        hero_layer_free(v);
        return hero_ui_web_gtk_said(HERO_UI_OTHER, "the system gave no random bytes for the bridge's seal");
    }
    if (!hero_ui_web_gtk_scheme_registered) {
        void *context = f->web_context_get_default();

        f->register_uri_scheme(context, HERO_UI_WEB_SCHEME, hero_ui_web_gtk_serve, NULL, NULL);
        f->register_scheme_as_secure(f->get_security_manager(context), HERO_UI_WEB_SCHEME);
        hero_ui_web_gtk_scheme_registered = true;
    }
    v->session = f->network_session_new_ephemeral();
    v->manager = f->user_content_manager_new();
    /* The seal first, in the main frame alone, then the bridge, which takes
     * it away; 32 hexadecimal digits need no escaping in a script. */
    (void)snprintf(prelude, sizeof prelude, "Object.defineProperty(window, \"__heroesSeal\", { value: \"%.32s\\n\", configurable: true });", v->seal);
    script = f->user_script_new(prelude, HERO_WEBKIT_INJECT_TOP_FRAME, HERO_WEBKIT_INJECT_AT_DOCUMENT_START, NULL, NULL);
    f->add_script(v->manager, script);
    f->user_script_unref(script);
    bridge = hero_ui_web_gtk_copy(host->bridge, host->bridge_count);
    if (bridge == NULL) {
        f->object_unref(v->manager);
        f->object_unref(v->session);
        hero_layer_free(v);
        return hero_ui_web_gtk_said(HERO_UI_LIMIT, "no memory for the bridge's script");
    }
    script = f->user_script_new(bridge, HERO_WEBKIT_INJECT_TOP_FRAME, HERO_WEBKIT_INJECT_AT_DOCUMENT_START, NULL, NULL);
    f->g_free(bridge);
    f->add_script(v->manager, script);
    f->user_script_unref(script);
    (void)f->register_message_handler(v->manager, "heroes", NULL);
    v->view = f->object_new(f->web_view_get_type(), "web-context", f->web_context_get_default(), "network-session", v->session, "user-content-manager", v->manager, NULL);
    if (v->view == NULL) {
        f->object_unref(v->manager);
        f->object_unref(v->session);
        hero_layer_free(v);
        return hero_ui_web_gtk_said(HERO_UI_OTHER, "WebKitGTK made no web view");
    }
    /* The view is the arm's to place, and this arm's to let go of. */
    (void)f->object_ref(v->view);
    (void)f->signal_connect_data(v->manager, "script-message-received::heroes", (hero_gcallback)(void (*)(void))hero_ui_web_gtk_message, v, NULL, 0);
    (void)f->signal_connect_data(v->view, "decide-policy", (hero_gcallback)(void (*)(void))hero_ui_web_gtk_decide, v, NULL, 0);
    (void)f->signal_connect_data(v->view, "load-changed", (hero_gcallback)(void (*)(void))hero_ui_web_gtk_load_changed, v, NULL, 0);
    (void)f->signal_connect_data(v->view, "create", (hero_gcallback)(void (*)(void))hero_ui_web_gtk_create_view, v, NULL, 0);
    (void)f->signal_connect_data(v->view, "permission-request", (hero_gcallback)(void (*)(void))hero_ui_web_gtk_permission, v, NULL, 0);
    (void)f->signal_connect_data(v->view, "run-file-chooser", (hero_gcallback)(void (*)(void))hero_ui_web_gtk_file_chooser, v, NULL, 0);
    (void)f->signal_connect_data(v->view, "web-process-terminated", (hero_gcallback)(void (*)(void))hero_ui_web_gtk_terminated, v, NULL, 0);
    (void)f->signal_connect_data(v->session, "download-started", (hero_gcallback)(void (*)(void))hero_ui_web_gtk_download, v, NULL, 0);
    hero_ui_web_gtk_views[hero_ui_web_gtk_count] = v;
    hero_ui_web_gtk_count += 1;
    *widget = v->view;
    return HERO_UI_OK;
}

int64_t hero_ui_web_gtk_destroy(int64_t id) {
    const hero_ui_web_gtk_library *f = &hero_ui_web_gtk_functions;

    for (int64_t i = 0; i < hero_ui_web_gtk_count; i += 1) {
        hero_ui_web_gtk_view *v = hero_ui_web_gtk_views[i];

        if (v->id != id) {
            continue;
        }
        f->stop_loading(v->view);
        f->signal_handlers_disconnect_by_data(v->view, v);
        f->signal_handlers_disconnect_by_data(v->manager, v);
        f->signal_handlers_disconnect_by_data(v->session, v);
        f->unregister_message_handler(v->manager, "heroes", NULL);
        f->object_unref(v->view);
        f->object_unref(v->manager);
        f->object_unref(v->session);
        hero_ui_web_gtk_drop_waiting(v);
        hero_layer_free(v->waiting);
        v->host.gone(id);
        hero_layer_free(v);
        hero_ui_web_gtk_views[i] = hero_ui_web_gtk_views[hero_ui_web_gtk_count - 1];
        hero_ui_web_gtk_count -= 1;
        return HERO_UI_OK;
    }
    return hero_ui_web_gtk_said(HERO_UI_NOT_FOUND, "WebKitGTK holds no such web view");
}

int64_t hero_ui_web_gtk_load(int64_t id, const uint8_t *url, int64_t count) {
    hero_ui_web_gtk_view *v = hero_ui_web_gtk_of(id);
    char *copy;

    if (v == NULL) {
        return hero_ui_web_gtk_said(HERO_UI_NOT_FOUND, "WebKitGTK holds no such web view");
    }
    copy = hero_ui_web_gtk_copy(url, count);
    if (copy == NULL) {
        return hero_ui_web_gtk_said(HERO_UI_LIMIT, "no memory for the URL");
    }
    v->ready = false;
    hero_ui_web_gtk_functions.load_uri(v->view, copy);
    hero_ui_web_gtk_functions.g_free(copy);
    return HERO_UI_OK;
}

int64_t hero_ui_web_gtk_post(int64_t id, const uint8_t *text, int64_t count) {
    return hero_ui_web_gtk_given(id, false, text, count);
}

int64_t hero_ui_web_gtk_send(int64_t id, const uint8_t *text, int64_t count) {
    return hero_ui_web_gtk_given(id, true, text, count);
}

#else
/* ==========================================================================
 * EVERY OTHER SYSTEM: no WebKitGTK; the web view is another arm's. Each
 * function answers `HERO_UI_UNSUPPORTED`, so a call that did reach one is a
 * value.
 * ========================================================================== */

int64_t hero_ui_web_gtk_create(int64_t id, const hero_ui_web_host *host, void **widget) {
    (void)id;
    (void)host;
    (void)widget;
    return hero_ui_web_gtk_said(HERO_UI_UNSUPPORTED, "WebKitGTK is Linux's web view");
}

int64_t hero_ui_web_gtk_destroy(int64_t id) {
    (void)id;
    return hero_ui_web_gtk_said(HERO_UI_UNSUPPORTED, "WebKitGTK is Linux's web view");
}

int64_t hero_ui_web_gtk_load(int64_t id, const uint8_t *url, int64_t count) {
    (void)id;
    (void)url;
    (void)count;
    return hero_ui_web_gtk_said(HERO_UI_UNSUPPORTED, "WebKitGTK is Linux's web view");
}

int64_t hero_ui_web_gtk_post(int64_t id, const uint8_t *text, int64_t count) {
    (void)id;
    (void)text;
    (void)count;
    return hero_ui_web_gtk_said(HERO_UI_UNSUPPORTED, "WebKitGTK is Linux's web view");
}

int64_t hero_ui_web_gtk_send(int64_t id, const uint8_t *text, int64_t count) {
    (void)id;
    (void)text;
    (void)count;
    return hero_ui_web_gtk_said(HERO_UI_UNSUPPORTED, "WebKitGTK is Linux's web view");
}

#endif
