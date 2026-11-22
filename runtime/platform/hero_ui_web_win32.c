/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_web_win32.c: the web view on Windows, WebView2, one
 * member of the archive the compiler puts on every program's link line
 * (`selfhost/cli/layer.hero`). Its header, `hero_ui_web_win32.h`, says what
 * each function answers and who calls it; this file says how.
 *
 * ITS OWN HEADER FIRST (held by the net's `runtime` suite), so a definition
 * with another signature than the header's is `conflicting types` where it
 * is compiled.
 *
 * WHY THE LOADER AT RUN TIME, AND NO SDK. WebView2's runtime is a part of
 * Windows that updates itself; a program reaches it through
 * `WebView2Loader.dll`, which finds the runtime installed and makes its
 * environment. The member loads that DLL with `LoadLibraryW` at the first
 * web view, from the program's own folder first as Windows searches, and
 * names no header of the SDK: the COM interfaces it calls and the ones it
 * implements are declared here, each method in the order Microsoft's
 * published `WebView2.idl` gives it and each identifier as it gives it, read
 * 2026-11-22 from the SDK `Microsoft.Web.WebView2` 1.0.4370.26 (BSD-3-Clause,
 * its licence read that day; nothing of it is copied but those orders and
 * identifiers). A method this member never calls stands as a `void *` of its
 * name, so the order stays readable and a slot cannot move.
 *
 * WHAT IT ASKS THE LAYER, never decides itself (`platform/hero_ui_web.h`):
 * every request of the app's scheme is answered with what the host's
 * `serve` wrote; every navigation, of the main frame and of a frame inside,
 * is asked of `ask` before it starts and cancelled where refused; a message
 * is queued where `ask` hears its source.
 *
 * THE APP'S SCHEME is registered with the environment, `app` with an
 * authority and treated as secure, so `app://localhost` is an origin of its
 * own; WebView2 hands this member every request of it, and asks nothing of
 * the network for one. A frame inside the page reaches the main frame's
 * `window.chrome.webview` alone through its own frame object, which this
 * member never asks for, so only the main frame is heard.
 *
 * WHAT THE PAGE MAY NOT DO: a new window is refused (handled, with no new
 * web view), a download cancelled, a permission denied, and the controller
 * made in private mode, so the page's cookies and storage live in memory
 * and end with it; the developer tools, host objects and the status bar are
 * off. The browser process still needs a folder of its own for what it
 * keeps outside the page: the user's local application data folder,
 * `%LOCALAPPDATA%\Heroes\WebView2`.
 *
 * MEMORY: a web view's record, the handlers and the texts kept for its page
 * are the layer's (`hero_layer_alloc`); every COM pointer this member holds
 * is released at `destroy`, and every string WebView2 hands out is freed
 * with `CoTaskMemFree`, as its contract says.
 *
 * Written from the platforms' documentation; no other library's source.
 */

#include "hero_ui_web_win32.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "hero_layer_alloc.h"
#include "hero_ui.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <objbase.h>
#include <shlwapi.h>
#if defined(_MSC_VER)
/* What only a program that takes this member links. MSVC's word; a MinGW
 * target links them by `-l`. */
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "user32.lib")
#endif
#endif

/* -- THE LAST FAILURE, PER THREAD ------------------------------------------ */

#define HERO_UI_WEB_WIN32_WORDS_MOST 512

static _Thread_local char hero_ui_web_win32_said_words[HERO_UI_WEB_WIN32_WORDS_MOST];

/* Keeps `words` as the last failure's and answers `kind`. */
static int64_t hero_ui_web_win32_said(int64_t kind, const char *words) {
    size_t length = strlen(words);

    if (length >= sizeof hero_ui_web_win32_said_words) {
        length = sizeof hero_ui_web_win32_said_words - 1;
        while (length > 0 && ((unsigned char)words[length] & 0xC0u) == 0x80u) {
            length -= 1;
        }
    }
    memcpy(hero_ui_web_win32_said_words, words, length);
    hero_ui_web_win32_said_words[length] = '\0';
    return kind;
}

int64_t hero_ui_web_win32_words(char *words, int64_t count) {
    size_t length = strlen(hero_ui_web_win32_said_words);

    if (words == NULL || count <= 0) {
        return 0;
    }
    if (length >= (size_t)count) {
        length = (size_t)count - 1;
        while (length > 0 && ((unsigned char)hero_ui_web_win32_said_words[length] & 0xC0u) == 0x80u) {
            length -= 1;
        }
    }
    memcpy(words, hero_ui_web_win32_said_words, length);
    words[length] = '\0';
    return (int64_t)length;
}

#if defined(_WIN32)
/* ==========================================================================
 * THE WINDOWS ARM
 * ========================================================================== */

/* -- THE INTERFACES, AS `WebView2.idl` ORDERS THEM --------------------------- */

/* WinRT's `EventRegistrationToken`, which every `add_` writes. */
typedef struct hero_wv2_token {
    int64_t value;
} hero_wv2_token;

/* The three methods every COM interface begins with. */
#define HERO_WV2_UNKNOWN(T)                                                  \
    HRESULT(STDMETHODCALLTYPE *QueryInterface)(T * self, REFIID riid, void **out); \
    ULONG(STDMETHODCALLTYPE *AddRef)(T * self);                              \
    ULONG(STDMETHODCALLTYPE *Release)(T * self)

typedef struct hero_wv2_unknown hero_wv2_unknown;
typedef struct hero_wv2_environment hero_wv2_environment;
typedef struct hero_wv2_controller_options hero_wv2_controller_options;
typedef struct hero_wv2_controller hero_wv2_controller;
typedef struct hero_wv2_core hero_wv2_core;
typedef struct hero_wv2_settings hero_wv2_settings;
typedef struct hero_wv2_navigation_args hero_wv2_navigation_args;
typedef struct hero_wv2_resource_args hero_wv2_resource_args;
typedef struct hero_wv2_request hero_wv2_request;
typedef struct hero_wv2_message_args hero_wv2_message_args;
typedef struct hero_wv2_new_window_args hero_wv2_new_window_args;
typedef struct hero_wv2_permission_args hero_wv2_permission_args;
typedef struct hero_wv2_download_args hero_wv2_download_args;
typedef struct hero_wv2_handler hero_wv2_handler;

/* IUnknown, for what this member only releases (a response). */
typedef struct hero_wv2_unknown_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_unknown);
} hero_wv2_unknown_vtbl;
struct hero_wv2_unknown {
    const hero_wv2_unknown_vtbl *lpVtbl;
};

/* ICoreWebView2Environment10, which holds every method of the environments
 * before it in their order. */
typedef struct hero_wv2_environment_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_environment);
    void *CreateCoreWebView2Controller;
    HRESULT(STDMETHODCALLTYPE *CreateWebResourceResponse)(hero_wv2_environment *self, IStream *content, int status, LPCWSTR reason, LPCWSTR headers, hero_wv2_unknown **response);
    void *get_BrowserVersionString;
    void *add_NewBrowserVersionAvailable;
    void *remove_NewBrowserVersionAvailable;
    void *CreateWebResourceRequest;
    void *CreateCoreWebView2CompositionController;
    void *CreateCoreWebView2PointerInfo;
    void *GetAutomationProviderForWindow;
    void *add_BrowserProcessExited;
    void *remove_BrowserProcessExited;
    void *CreatePrintSettings;
    void *get_UserDataFolder;
    void *add_ProcessInfosChanged;
    void *remove_ProcessInfosChanged;
    void *GetProcessInfos;
    void *CreateContextMenuItem;
    HRESULT(STDMETHODCALLTYPE *CreateCoreWebView2ControllerOptions)(hero_wv2_environment *self, hero_wv2_controller_options **options);
    HRESULT(STDMETHODCALLTYPE *CreateCoreWebView2ControllerWithOptions)(hero_wv2_environment *self, HWND parent, hero_wv2_controller_options *options, hero_wv2_handler *handler);
    void *CreateCoreWebView2CompositionControllerWithOptions;
} hero_wv2_environment_vtbl;
struct hero_wv2_environment {
    const hero_wv2_environment_vtbl *lpVtbl;
};

/* ICoreWebView2ControllerOptions. */
typedef struct hero_wv2_controller_options_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_controller_options);
    void *get_ProfileName;
    void *put_ProfileName;
    void *get_IsInPrivateModeEnabled;
    HRESULT(STDMETHODCALLTYPE *put_IsInPrivateModeEnabled)(hero_wv2_controller_options *self, BOOL on);
} hero_wv2_controller_options_vtbl;
struct hero_wv2_controller_options {
    const hero_wv2_controller_options_vtbl *lpVtbl;
};

/* ICoreWebView2Controller. */
typedef struct hero_wv2_controller_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_controller);
    void *get_IsVisible;
    HRESULT(STDMETHODCALLTYPE *put_IsVisible)(hero_wv2_controller *self, BOOL visible);
    void *get_Bounds;
    HRESULT(STDMETHODCALLTYPE *put_Bounds)(hero_wv2_controller *self, RECT bounds);
    void *get_ZoomFactor;
    void *put_ZoomFactor;
    void *add_ZoomFactorChanged;
    void *remove_ZoomFactorChanged;
    void *SetBoundsAndZoomFactor;
    void *MoveFocus;
    void *add_MoveFocusRequested;
    void *remove_MoveFocusRequested;
    void *add_GotFocus;
    void *remove_GotFocus;
    void *add_LostFocus;
    void *remove_LostFocus;
    void *add_AcceleratorKeyPressed;
    void *remove_AcceleratorKeyPressed;
    void *get_ParentWindow;
    void *put_ParentWindow;
    void *NotifyParentWindowPositionChanged;
    HRESULT(STDMETHODCALLTYPE *Close)(hero_wv2_controller *self);
    HRESULT(STDMETHODCALLTYPE *get_CoreWebView2)(hero_wv2_controller *self, hero_wv2_core **core);
} hero_wv2_controller_vtbl;
struct hero_wv2_controller {
    const hero_wv2_controller_vtbl *lpVtbl;
};

/* ICoreWebView2_4, which holds every method of ICoreWebView2 to _3 before
 * its own, in their order. */
typedef struct hero_wv2_core_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_core);
    HRESULT(STDMETHODCALLTYPE *get_Settings)(hero_wv2_core *self, hero_wv2_settings **settings);
    HRESULT(STDMETHODCALLTYPE *get_Source)(hero_wv2_core *self, LPWSTR *uri);
    HRESULT(STDMETHODCALLTYPE *Navigate)(hero_wv2_core *self, LPCWSTR uri);
    void *NavigateToString;
    HRESULT(STDMETHODCALLTYPE *add_NavigationStarting)(hero_wv2_core *self, hero_wv2_handler *handler, hero_wv2_token *token);
    void *remove_NavigationStarting;
    void *add_ContentLoading;
    void *remove_ContentLoading;
    void *add_SourceChanged;
    void *remove_SourceChanged;
    void *add_HistoryChanged;
    void *remove_HistoryChanged;
    HRESULT(STDMETHODCALLTYPE *add_NavigationCompleted)(hero_wv2_core *self, hero_wv2_handler *handler, hero_wv2_token *token);
    void *remove_NavigationCompleted;
    HRESULT(STDMETHODCALLTYPE *add_FrameNavigationStarting)(hero_wv2_core *self, hero_wv2_handler *handler, hero_wv2_token *token);
    void *remove_FrameNavigationStarting;
    void *add_FrameNavigationCompleted;
    void *remove_FrameNavigationCompleted;
    void *add_ScriptDialogOpening;
    void *remove_ScriptDialogOpening;
    HRESULT(STDMETHODCALLTYPE *add_PermissionRequested)(hero_wv2_core *self, hero_wv2_handler *handler, hero_wv2_token *token);
    void *remove_PermissionRequested;
    HRESULT(STDMETHODCALLTYPE *add_ProcessFailed)(hero_wv2_core *self, hero_wv2_handler *handler, hero_wv2_token *token);
    void *remove_ProcessFailed;
    HRESULT(STDMETHODCALLTYPE *AddScriptToExecuteOnDocumentCreated)(hero_wv2_core *self, LPCWSTR script, hero_wv2_handler *handler);
    void *RemoveScriptToExecuteOnDocumentCreated;
    HRESULT(STDMETHODCALLTYPE *ExecuteScript)(hero_wv2_core *self, LPCWSTR script, hero_wv2_handler *handler);
    void *CapturePreview;
    HRESULT(STDMETHODCALLTYPE *Reload)(hero_wv2_core *self);
    void *PostWebMessageAsJson;
    HRESULT(STDMETHODCALLTYPE *PostWebMessageAsString)(hero_wv2_core *self, LPCWSTR text);
    HRESULT(STDMETHODCALLTYPE *add_WebMessageReceived)(hero_wv2_core *self, hero_wv2_handler *handler, hero_wv2_token *token);
    void *remove_WebMessageReceived;
    void *CallDevToolsProtocolMethod;
    void *get_BrowserProcessId;
    void *get_CanGoBack;
    void *get_CanGoForward;
    void *GoBack;
    void *GoForward;
    void *GetDevToolsProtocolEventReceiver;
    HRESULT(STDMETHODCALLTYPE *Stop)(hero_wv2_core *self);
    HRESULT(STDMETHODCALLTYPE *add_NewWindowRequested)(hero_wv2_core *self, hero_wv2_handler *handler, hero_wv2_token *token);
    void *remove_NewWindowRequested;
    void *add_DocumentTitleChanged;
    void *remove_DocumentTitleChanged;
    void *get_DocumentTitle;
    void *AddHostObjectToScript;
    void *RemoveHostObjectFromScript;
    void *OpenDevToolsWindow;
    void *add_ContainsFullScreenElementChanged;
    void *remove_ContainsFullScreenElementChanged;
    void *get_ContainsFullScreenElement;
    HRESULT(STDMETHODCALLTYPE *add_WebResourceRequested)(hero_wv2_core *self, hero_wv2_handler *handler, hero_wv2_token *token);
    void *remove_WebResourceRequested;
    HRESULT(STDMETHODCALLTYPE *AddWebResourceRequestedFilter)(hero_wv2_core *self, LPCWSTR uri, int context);
    void *RemoveWebResourceRequestedFilter;
    void *add_WindowCloseRequested;
    void *remove_WindowCloseRequested;
    void *add_WebResourceResponseReceived;
    void *remove_WebResourceResponseReceived;
    void *NavigateWithWebResourceRequest;
    void *add_DOMContentLoaded;
    void *remove_DOMContentLoaded;
    void *get_CookieManager;
    void *get_Environment;
    void *TrySuspend;
    void *Resume;
    void *get_IsSuspended;
    void *SetVirtualHostNameToFolderMapping;
    void *ClearVirtualHostNameToFolderMapping;
    void *add_FrameCreated;
    void *remove_FrameCreated;
    HRESULT(STDMETHODCALLTYPE *add_DownloadStarting)(hero_wv2_core *self, hero_wv2_handler *handler, hero_wv2_token *token);
    void *remove_DownloadStarting;
} hero_wv2_core_vtbl;
struct hero_wv2_core {
    const hero_wv2_core_vtbl *lpVtbl;
};

/* ICoreWebView2Settings. */
typedef struct hero_wv2_settings_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_settings);
    void *get_IsScriptEnabled;
    void *put_IsScriptEnabled;
    void *get_IsWebMessageEnabled;
    HRESULT(STDMETHODCALLTYPE *put_IsWebMessageEnabled)(hero_wv2_settings *self, BOOL on);
    void *get_AreDefaultScriptDialogsEnabled;
    void *put_AreDefaultScriptDialogsEnabled;
    void *get_IsStatusBarEnabled;
    HRESULT(STDMETHODCALLTYPE *put_IsStatusBarEnabled)(hero_wv2_settings *self, BOOL on);
    void *get_AreDevToolsEnabled;
    HRESULT(STDMETHODCALLTYPE *put_AreDevToolsEnabled)(hero_wv2_settings *self, BOOL on);
    void *get_AreDefaultContextMenusEnabled;
    void *put_AreDefaultContextMenusEnabled;
    void *get_AreHostObjectsAllowed;
    HRESULT(STDMETHODCALLTYPE *put_AreHostObjectsAllowed)(hero_wv2_settings *self, BOOL on);
    void *get_IsZoomControlEnabled;
    void *put_IsZoomControlEnabled;
    void *get_IsBuiltInErrorPageEnabled;
    void *put_IsBuiltInErrorPageEnabled;
} hero_wv2_settings_vtbl;
struct hero_wv2_settings {
    const hero_wv2_settings_vtbl *lpVtbl;
};

/* ICoreWebView2NavigationStartingEventArgs. */
typedef struct hero_wv2_navigation_args_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_navigation_args);
    HRESULT(STDMETHODCALLTYPE *get_Uri)(hero_wv2_navigation_args *self, LPWSTR *uri);
    void *get_IsUserInitiated;
    void *get_IsRedirected;
    void *get_RequestHeaders;
    void *get_Cancel;
    HRESULT(STDMETHODCALLTYPE *put_Cancel)(hero_wv2_navigation_args *self, BOOL cancel);
    void *get_NavigationId;
} hero_wv2_navigation_args_vtbl;
struct hero_wv2_navigation_args {
    const hero_wv2_navigation_args_vtbl *lpVtbl;
};

/* ICoreWebView2WebResourceRequestedEventArgs. */
typedef struct hero_wv2_resource_args_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_resource_args);
    HRESULT(STDMETHODCALLTYPE *get_Request)(hero_wv2_resource_args *self, hero_wv2_request **request);
    void *get_Response;
    HRESULT(STDMETHODCALLTYPE *put_Response)(hero_wv2_resource_args *self, hero_wv2_unknown *response);
    void *GetDeferral;
    void *get_ResourceContext;
} hero_wv2_resource_args_vtbl;
struct hero_wv2_resource_args {
    const hero_wv2_resource_args_vtbl *lpVtbl;
};

/* ICoreWebView2WebResourceRequest. */
typedef struct hero_wv2_request_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_request);
    HRESULT(STDMETHODCALLTYPE *get_Uri)(hero_wv2_request *self, LPWSTR *uri);
    void *put_Uri;
    HRESULT(STDMETHODCALLTYPE *get_Method)(hero_wv2_request *self, LPWSTR *method);
    void *put_Method;
    void *get_Content;
    void *put_Content;
    void *get_Headers;
} hero_wv2_request_vtbl;
struct hero_wv2_request {
    const hero_wv2_request_vtbl *lpVtbl;
};

/* ICoreWebView2WebMessageReceivedEventArgs. */
typedef struct hero_wv2_message_args_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_message_args);
    HRESULT(STDMETHODCALLTYPE *get_Source)(hero_wv2_message_args *self, LPWSTR *source);
    void *get_WebMessageAsJson;
    HRESULT(STDMETHODCALLTYPE *TryGetWebMessageAsString)(hero_wv2_message_args *self, LPWSTR *text);
} hero_wv2_message_args_vtbl;
struct hero_wv2_message_args {
    const hero_wv2_message_args_vtbl *lpVtbl;
};

/* ICoreWebView2NewWindowRequestedEventArgs. */
typedef struct hero_wv2_new_window_args_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_new_window_args);
    void *get_Uri;
    void *put_NewWindow;
    void *get_NewWindow;
    HRESULT(STDMETHODCALLTYPE *put_Handled)(hero_wv2_new_window_args *self, BOOL handled);
    void *get_Handled;
    void *get_IsUserInitiated;
    void *GetDeferral;
    void *get_WindowFeatures;
} hero_wv2_new_window_args_vtbl;
struct hero_wv2_new_window_args {
    const hero_wv2_new_window_args_vtbl *lpVtbl;
};

/* ICoreWebView2PermissionRequestedEventArgs. */
typedef struct hero_wv2_permission_args_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_permission_args);
    void *get_Uri;
    void *get_PermissionKind;
    void *get_IsUserInitiated;
    void *get_State;
    HRESULT(STDMETHODCALLTYPE *put_State)(hero_wv2_permission_args *self, int state);
    void *GetDeferral;
} hero_wv2_permission_args_vtbl;
struct hero_wv2_permission_args {
    const hero_wv2_permission_args_vtbl *lpVtbl;
};

/* ICoreWebView2DownloadStartingEventArgs. */
typedef struct hero_wv2_download_args_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_download_args);
    void *get_DownloadOperation;
    void *get_Cancel;
    HRESULT(STDMETHODCALLTYPE *put_Cancel)(hero_wv2_download_args *self, BOOL cancel);
    void *get_ResultFilePath;
    void *put_ResultFilePath;
    void *get_Handled;
    HRESULT(STDMETHODCALLTYPE *put_Handled)(hero_wv2_download_args *self, BOOL handled);
    void *GetDeferral;
} hero_wv2_download_args_vtbl;
struct hero_wv2_download_args {
    const hero_wv2_download_args_vtbl *lpVtbl;
};

/* The numbers of `WebView2.idl` this member writes. */
#define HERO_WV2_PERMISSION_DENY 2
#define HERO_WV2_RESOURCE_CONTEXT_ALL 0

/* The identifiers, as `WebView2.idl` gives them. */
static const IID hero_wv2_iid_environment10 = { 0xee0eb9df, 0x6f12, 0x46ce, { 0xb5, 0x3f, 0x3f, 0x47, 0xb9, 0xc9, 0x28, 0xe0 } };
static const IID hero_wv2_iid_core4 = { 0x20d02d59, 0x6df2, 0x42dc, { 0xbd, 0x06, 0xf9, 0x8a, 0x69, 0x4b, 0x13, 0x02 } };
static const IID hero_wv2_iid_options = { 0x2fde08a8, 0x1e9a, 0x4766, { 0x8c, 0x05, 0x95, 0xa9, 0xce, 0xb9, 0xd1, 0xc5 } };
static const IID hero_wv2_iid_options4 = { 0xac52d13f, 0x0d38, 0x475a, { 0x9d, 0xca, 0x87, 0x65, 0x80, 0xd6, 0x79, 0x3e } };
static const IID hero_wv2_iid_scheme = { 0xd60ac92c, 0x37a6, 0x4b26, { 0xa3, 0x9e, 0x95, 0xcf, 0xe5, 0x90, 0x47, 0xbb } };
static const IID hero_wv2_iid_environment_done = { 0x4e8a3389, 0xc9d8, 0x4bd2, { 0xb6, 0xb5, 0x12, 0x4f, 0xee, 0x6c, 0xc1, 0x4d } };
static const IID hero_wv2_iid_controller_done = { 0x6c4819f3, 0xc9b7, 0x4260, { 0x81, 0x27, 0xc9, 0xf5, 0xbd, 0xe7, 0xf6, 0x8c } };
static const IID hero_wv2_iid_navigation_starting = { 0x9adbe429, 0xf36d, 0x432b, { 0x9d, 0xdc, 0xf8, 0x88, 0x1f, 0xbd, 0x76, 0xe3 } };
static const IID hero_wv2_iid_resource_requested = { 0xab00b74c, 0x15f1, 0x4646, { 0x80, 0xe8, 0xe7, 0x63, 0x41, 0xd2, 0x5d, 0x71 } };
static const IID hero_wv2_iid_message_received = { 0x57213f19, 0x00e6, 0x49fa, { 0x8e, 0x07, 0x89, 0x8e, 0xa0, 0x1e, 0xcb, 0xd2 } };
static const IID hero_wv2_iid_new_window = { 0xd4c185fe, 0xc81c, 0x4989, { 0x97, 0xaf, 0x2d, 0x3f, 0xa7, 0xab, 0x56, 0x51 } };
static const IID hero_wv2_iid_permission = { 0x15e1c6a3, 0xc72a, 0x4df3, { 0x91, 0xd7, 0xd0, 0x97, 0xfb, 0xec, 0x6b, 0xfd } };
static const IID hero_wv2_iid_navigation_completed = { 0xd33a35bf, 0x1c49, 0x4f98, { 0x93, 0xab, 0x00, 0x6e, 0x05, 0x33, 0xfe, 0x1c } };
static const IID hero_wv2_iid_process_failed = { 0x79e0aea4, 0x990b, 0x42d9, { 0xaa, 0x1d, 0x0f, 0xcc, 0x2e, 0x5b, 0xc7, 0xf1 } };
static const IID hero_wv2_iid_download_starting = { 0xefedc989, 0xc396, 0x41ca, { 0x83, 0xf7, 0x07, 0xf8, 0x45, 0xa5, 0x57, 0x24 } };

/* The runtime version the environment asks for at least: the one the SDK
 * read above names its own (its `CORE_WEBVIEW_TARGET_PRODUCT_VERSION`). */
#define HERO_WV2_TARGET_VERSION L"158.0.4370.26"

/* The loader's entry point, `CreateCoreWebView2EnvironmentWithOptions`. */
typedef HRESULT(STDAPICALLTYPE *hero_wv2_create_environment_fn)(PCWSTR browser_folder, PCWSTR data_folder, void *options, hero_wv2_handler *handler);

/* -- THE WEB VIEWS ----------------------------------------------------------- */

/* A text kept for a page that loads: posted to it, or sent as it. */
typedef struct hero_ui_web_win32_held {
    bool as_page;
    char *text;
    size_t count;
} hero_ui_web_win32_held;

/* One web view this member made. */
typedef struct hero_ui_web_win32_view {
    int64_t id;
    hero_ui_web_host host;
    hero_wv2_controller *controller;
    hero_wv2_core *core;
    /* The handlers it registered, which it tells it is gone. */
    hero_wv2_handler *handlers[9];
    int handler_count;
    bool ready;
    hero_ui_web_win32_held *waiting;
    int64_t held;
    int64_t room;
} hero_ui_web_win32_view;

#define HERO_UI_WEB_WIN32_VIEWS_MOST 64
static _Thread_local hero_ui_web_win32_view *hero_ui_web_win32_views[HERO_UI_WEB_WIN32_VIEWS_MOST];
static _Thread_local int64_t hero_ui_web_win32_count;
/* The process's environment, made at the first web view and kept, as
 * WebView2 wants one per process and folder. */
static _Thread_local hero_wv2_environment *hero_ui_web_win32_environment;
static _Thread_local hero_wv2_create_environment_fn hero_ui_web_win32_create_environment;

static hero_ui_web_win32_view *hero_ui_web_win32_of(int64_t id) {
    for (int64_t i = 0; i < hero_ui_web_win32_count; i += 1) {
        if (hero_ui_web_win32_views[i]->id == id) {
            return hero_ui_web_win32_views[i];
        }
    }
    return NULL;
}

/* -- TEXTS BETWEEN UTF-8 AND UTF-16 ------------------------------------------ */

/* `count` bytes of UTF-8 at `text` as a zero-ended UTF-16 copy, the layer's
 * to free, or NULL. */
static wchar_t *hero_ui_web_win32_wide(const uint8_t *text, size_t count) {
    int need;
    wchar_t *out;

    if (count > (size_t)INT32_MAX) {
        return NULL;
    }
    need = count == 0 ? 0 : MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (const char *)text, (int)count, NULL, 0);
    if (count > 0 && need <= 0) {
        return NULL;
    }
    out = hero_layer_alloc(((size_t)need + 1) * sizeof *out);
    if (out == NULL) {
        return NULL;
    }
    if (need > 0) {
        (void)MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (const char *)text, (int)count, out, need);
    }
    out[need] = L'\0';
    return out;
}

/* The zero-ended UTF-16 `text` as a zero-ended UTF-8 copy, the layer's to
 * free, a lone surrogate becoming U+FFFD; NULL for none or no memory. */
static char *hero_ui_web_win32_narrow(const wchar_t *text, size_t *count) {
    int need;
    char *out;

    if (text == NULL) {
        return NULL;
    }
    need = WideCharToMultiByte(CP_UTF8, 0, text, -1, NULL, 0, NULL, NULL);
    if (need <= 0) {
        return NULL;
    }
    out = hero_layer_alloc((size_t)need);
    if (out == NULL) {
        return NULL;
    }
    (void)WideCharToMultiByte(CP_UTF8, 0, text, -1, out, need, NULL, NULL);
    *count = (size_t)need - 1;
    return out;
}

/* `ask` of the zero-ended UTF-16 `url` for `what`; NULL asks of no URL. */
static bool hero_ui_web_win32_allowed(const hero_ui_web_win32_view *v, int64_t what, const wchar_t *url) {
    size_t count = 0;
    char *narrow = hero_ui_web_win32_narrow(url, &count);
    bool answer = v->host.ask(what, v->id, (const uint8_t *)narrow, (int64_t)count) == 1;

    hero_layer_free(narrow);
    return answer;
}

/* -- THE HANDLERS THIS MEMBER IMPLEMENTS ------------------------------------- */

/* What a handler answers. */
enum {
    HERO_WV2_ENVIRONMENT_DONE,
    HERO_WV2_CONTROLLER_DONE,
    HERO_WV2_NAVIGATION_STARTING,
    HERO_WV2_FRAME_NAVIGATION_STARTING,
    HERO_WV2_NAVIGATION_COMPLETED,
    HERO_WV2_RESOURCE_REQUESTED,
    HERO_WV2_MESSAGE_RECEIVED,
    HERO_WV2_NEW_WINDOW,
    HERO_WV2_PERMISSION,
    HERO_WV2_PROCESS_FAILED,
    HERO_WV2_DOWNLOAD_STARTING
};

/* The two shapes of a handler's vtable: a completion, `Invoke(HRESULT,
 * result)`, and an event, `Invoke(sender, args)`. */
typedef struct hero_wv2_completed_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_handler);
    HRESULT(STDMETHODCALLTYPE *Invoke)(hero_wv2_handler *self, HRESULT code, hero_wv2_unknown *result);
} hero_wv2_completed_vtbl;

typedef struct hero_wv2_event_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_handler);
    HRESULT(STDMETHODCALLTYPE *Invoke)(hero_wv2_handler *self, hero_wv2_core *sender, void *args);
} hero_wv2_event_vtbl;

/* A handler: its vtable first, as COM reads it; what it answers; the web
 * view it answers for, NULL once that is gone; and a completion's result. */
struct hero_wv2_handler {
    const void *lpVtbl;
    LONG references;
    int kind;
    const IID *iid;
    hero_ui_web_win32_view *view;
    bool done;
    HRESULT code;
    hero_wv2_unknown *result;
};

static HRESULT STDMETHODCALLTYPE hero_wv2_handler_query(hero_wv2_handler *self, REFIID riid, void **out) {
    if (out == NULL) {
        return E_POINTER;
    }
    if (IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid, self->iid)) {
        *out = self;
        InterlockedIncrement(&self->references);
        return S_OK;
    }
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG STDMETHODCALLTYPE hero_wv2_handler_add(hero_wv2_handler *self) {
    return (ULONG)InterlockedIncrement(&self->references);
}

static ULONG STDMETHODCALLTYPE hero_wv2_handler_release(hero_wv2_handler *self) {
    LONG left = InterlockedDecrement(&self->references);

    if (left == 0) {
        if (self->result != NULL) {
            self->result->lpVtbl->Release(self->result);
        }
        hero_layer_free(self);
    }
    return (ULONG)left;
}

static HRESULT STDMETHODCALLTYPE hero_wv2_completed_invoke(hero_wv2_handler *self, HRESULT code, hero_wv2_unknown *result) {
    self->code = code;
    if (result != NULL) {
        result->lpVtbl->AddRef(result);
        self->result = result;
    }
    self->done = true;
    return S_OK;
}

static void hero_ui_web_win32_flush(hero_ui_web_win32_view *v);
static void hero_ui_web_win32_drop_waiting(hero_ui_web_win32_view *v);
static void hero_ui_web_win32_serve(hero_ui_web_win32_view *v, hero_wv2_resource_args *args);

static HRESULT STDMETHODCALLTYPE hero_wv2_event_invoke(hero_wv2_handler *self, hero_wv2_core *sender, void *args) {
    hero_ui_web_win32_view *v = self->view;
    LPWSTR text = NULL;
    LPWSTR source = NULL;

    (void)sender;
    if (v == NULL || args == NULL) {
        return S_OK;
    }
    switch (self->kind) {
    case HERO_WV2_NAVIGATION_STARTING:
    case HERO_WV2_FRAME_NAVIGATION_STARTING: {
        hero_wv2_navigation_args *a = args;

        if (FAILED(a->lpVtbl->get_Uri(a, &text)) || !hero_ui_web_win32_allowed(v, self->kind == HERO_WV2_NAVIGATION_STARTING ? HERO_UI_WEB_ASK_FRAME : HERO_UI_WEB_ASK_SUBFRAME, text)) {
            (void)a->lpVtbl->put_Cancel(a, TRUE);
        } else if (self->kind == HERO_WV2_NAVIGATION_STARTING) {
            v->ready = false;
        }
        break;
    }
    case HERO_WV2_NAVIGATION_COMPLETED:
        if (SUCCEEDED(v->core->lpVtbl->get_Source(v->core, &source)) && hero_ui_web_win32_allowed(v, HERO_UI_WEB_ASK_OURS, source)) {
            v->ready = true;
            hero_ui_web_win32_flush(v);
        } else {
            v->ready = false;
            hero_ui_web_win32_drop_waiting(v);
        }
        break;
    case HERO_WV2_RESOURCE_REQUESTED:
        hero_ui_web_win32_serve(v, args);
        break;
    case HERO_WV2_MESSAGE_RECEIVED: {
        hero_wv2_message_args *a = args;
        size_t count = 0;
        char *narrow;

        if (FAILED(a->lpVtbl->get_Source(a, &source)) || FAILED(a->lpVtbl->TryGetWebMessageAsString(a, &text))) {
            (void)hero_ui_web_win32_allowed(v, HERO_UI_WEB_ASK_SENDER, NULL);
            break;
        }
        if (!hero_ui_web_win32_allowed(v, HERO_UI_WEB_ASK_SENDER, source)) {
            break;
        }
        narrow = hero_ui_web_win32_narrow(text, &count);
        if (narrow != NULL) {
            v->host.push(HERO_UI_EVENT_MESSAGE, v->id, 0, 0, (const uint8_t *)narrow, (int64_t)count);
            hero_layer_free(narrow);
        }
        break;
    }
    case HERO_WV2_NEW_WINDOW: {
        hero_wv2_new_window_args *a = args;

        /* Handled with no new web view: no window opens. */
        (void)a->lpVtbl->put_Handled(a, TRUE);
        break;
    }
    case HERO_WV2_PERMISSION: {
        hero_wv2_permission_args *a = args;

        (void)a->lpVtbl->put_State(a, HERO_WV2_PERMISSION_DENY);
        break;
    }
    case HERO_WV2_PROCESS_FAILED:
        v->ready = false;
        (void)v->core->lpVtbl->Reload(v->core);
        break;
    case HERO_WV2_DOWNLOAD_STARTING: {
        hero_wv2_download_args *a = args;

        (void)a->lpVtbl->put_Cancel(a, TRUE);
        (void)a->lpVtbl->put_Handled(a, TRUE);
        break;
    }
    default:
        break;
    }
    CoTaskMemFree(text);
    CoTaskMemFree(source);
    return S_OK;
}

static const hero_wv2_completed_vtbl hero_wv2_completed_table = {
    hero_wv2_handler_query,
    hero_wv2_handler_add,
    hero_wv2_handler_release,
    hero_wv2_completed_invoke,
};

static const hero_wv2_event_vtbl hero_wv2_event_table = {
    hero_wv2_handler_query,
    hero_wv2_handler_add,
    hero_wv2_handler_release,
    hero_wv2_event_invoke,
};

/* A new handler of `kind` answering for `v` (NULL for a completion), one
 * reference held by the caller; NULL with no memory. */
static hero_wv2_handler *hero_wv2_handler_made(int kind, const IID *iid, hero_ui_web_win32_view *v) {
    hero_wv2_handler *made = hero_layer_alloc_zero(1, sizeof *made);

    if (made == NULL) {
        return NULL;
    }
    made->lpVtbl = kind == HERO_WV2_ENVIRONMENT_DONE || kind == HERO_WV2_CONTROLLER_DONE ? (const void *)&hero_wv2_completed_table : (const void *)&hero_wv2_event_table;
    made->references = 1;
    made->kind = kind;
    made->iid = iid;
    made->view = v;
    return made;
}

/* -- THE ENVIRONMENT'S OPTIONS, WHICH THIS MEMBER IMPLEMENTS ------------------ */

typedef struct hero_wv2_scheme hero_wv2_scheme;

/* ICoreWebView2CustomSchemeRegistration: the app's scheme, with an
 * authority, treated as secure, reached by its own origin alone. */
typedef struct hero_wv2_scheme_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_scheme);
    HRESULT(STDMETHODCALLTYPE *get_SchemeName)(hero_wv2_scheme *self, LPWSTR *name);
    HRESULT(STDMETHODCALLTYPE *get_TreatAsSecure)(hero_wv2_scheme *self, BOOL *secure);
    HRESULT(STDMETHODCALLTYPE *put_TreatAsSecure)(hero_wv2_scheme *self, BOOL secure);
    HRESULT(STDMETHODCALLTYPE *GetAllowedOrigins)(hero_wv2_scheme *self, UINT32 *count, LPWSTR **origins);
    HRESULT(STDMETHODCALLTYPE *SetAllowedOrigins)(hero_wv2_scheme *self, UINT32 count, LPCWSTR *origins);
    HRESULT(STDMETHODCALLTYPE *get_HasAuthorityComponent)(hero_wv2_scheme *self, BOOL *has);
    HRESULT(STDMETHODCALLTYPE *put_HasAuthorityComponent)(hero_wv2_scheme *self, BOOL has);
} hero_wv2_scheme_vtbl;

struct hero_wv2_scheme {
    const hero_wv2_scheme_vtbl *lpVtbl;
    LONG references;
};

/* `text` as a string WebView2 frees with `CoTaskMemFree`. */
static HRESULT hero_wv2_handed(const wchar_t *text, LPWSTR *out) {
    size_t bytes = (wcslen(text) + 1) * sizeof *text;

    if (out == NULL) {
        return E_POINTER;
    }
    *out = CoTaskMemAlloc(bytes);
    if (*out == NULL) {
        return E_OUTOFMEMORY;
    }
    memcpy(*out, text, bytes);
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE hero_wv2_scheme_query(hero_wv2_scheme *self, REFIID riid, void **out) {
    if (out == NULL) {
        return E_POINTER;
    }
    if (IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid, &hero_wv2_iid_scheme)) {
        *out = self;
        InterlockedIncrement(&self->references);
        return S_OK;
    }
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG STDMETHODCALLTYPE hero_wv2_scheme_add(hero_wv2_scheme *self) {
    return (ULONG)InterlockedIncrement(&self->references);
}

static ULONG STDMETHODCALLTYPE hero_wv2_scheme_release(hero_wv2_scheme *self) {
    LONG left = InterlockedDecrement(&self->references);

    if (left == 0) {
        hero_layer_free(self);
    }
    return (ULONG)left;
}

static HRESULT STDMETHODCALLTYPE hero_wv2_scheme_name(hero_wv2_scheme *self, LPWSTR *name) {
    (void)self;
    return hero_wv2_handed(L"" HERO_UI_WEB_SCHEME, name);
}

static HRESULT STDMETHODCALLTYPE hero_wv2_scheme_secure(hero_wv2_scheme *self, BOOL *secure) {
    (void)self;
    if (secure == NULL) {
        return E_POINTER;
    }
    *secure = TRUE;
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE hero_wv2_scheme_put_secure(hero_wv2_scheme *self, BOOL secure) {
    (void)self;
    (void)secure;
    return S_OK;
}

/* None: a request of the scheme comes from its own origin alone. */
static HRESULT STDMETHODCALLTYPE hero_wv2_scheme_origins(hero_wv2_scheme *self, UINT32 *count, LPWSTR **origins) {
    (void)self;
    if (count == NULL || origins == NULL) {
        return E_POINTER;
    }
    *count = 0;
    *origins = NULL;
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE hero_wv2_scheme_put_origins(hero_wv2_scheme *self, UINT32 count, LPCWSTR *origins) {
    (void)self;
    (void)count;
    (void)origins;
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE hero_wv2_scheme_authority(hero_wv2_scheme *self, BOOL *has) {
    (void)self;
    if (has == NULL) {
        return E_POINTER;
    }
    *has = TRUE;
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE hero_wv2_scheme_put_authority(hero_wv2_scheme *self, BOOL has) {
    (void)self;
    (void)has;
    return S_OK;
}

static const hero_wv2_scheme_vtbl hero_wv2_scheme_table = {
    hero_wv2_scheme_query,
    hero_wv2_scheme_add,
    hero_wv2_scheme_release,
    hero_wv2_scheme_name,
    hero_wv2_scheme_secure,
    hero_wv2_scheme_put_secure,
    hero_wv2_scheme_origins,
    hero_wv2_scheme_put_origins,
    hero_wv2_scheme_authority,
    hero_wv2_scheme_put_authority,
};

typedef struct hero_wv2_options hero_wv2_options;
typedef struct hero_wv2_options4 hero_wv2_options4;

/* ICoreWebView2EnvironmentOptions, every value the default but the target
 * version. */
typedef struct hero_wv2_options_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_options);
    HRESULT(STDMETHODCALLTYPE *get_AdditionalBrowserArguments)(hero_wv2_options *self, LPWSTR *value);
    HRESULT(STDMETHODCALLTYPE *put_AdditionalBrowserArguments)(hero_wv2_options *self, LPCWSTR value);
    HRESULT(STDMETHODCALLTYPE *get_Language)(hero_wv2_options *self, LPWSTR *value);
    HRESULT(STDMETHODCALLTYPE *put_Language)(hero_wv2_options *self, LPCWSTR value);
    HRESULT(STDMETHODCALLTYPE *get_TargetCompatibleBrowserVersion)(hero_wv2_options *self, LPWSTR *value);
    HRESULT(STDMETHODCALLTYPE *put_TargetCompatibleBrowserVersion)(hero_wv2_options *self, LPCWSTR value);
    HRESULT(STDMETHODCALLTYPE *get_AllowSingleSignOnUsingOSPrimaryAccount)(hero_wv2_options *self, BOOL *allow);
    HRESULT(STDMETHODCALLTYPE *put_AllowSingleSignOnUsingOSPrimaryAccount)(hero_wv2_options *self, BOOL allow);
} hero_wv2_options_vtbl;

/* ICoreWebView2EnvironmentOptions4: the custom schemes. */
typedef struct hero_wv2_options4_vtbl {
    HERO_WV2_UNKNOWN(hero_wv2_options4);
    HRESULT(STDMETHODCALLTYPE *GetCustomSchemeRegistrations)(hero_wv2_options4 *self, UINT32 *count, hero_wv2_scheme ***schemes);
    HRESULT(STDMETHODCALLTYPE *SetCustomSchemeRegistrations)(hero_wv2_options4 *self, UINT32 count, hero_wv2_scheme **schemes);
} hero_wv2_options4_vtbl;

struct hero_wv2_options4 {
    const hero_wv2_options4_vtbl *lpVtbl;
};

/* One object, two interfaces: each pointer COM hands out is the address of
 * its vtable's member, and `hero_wv2_options_of` finds the object again. */
struct hero_wv2_options {
    const hero_wv2_options_vtbl *lpVtbl;
    hero_wv2_options4 fourth;
    LONG references;
    hero_wv2_scheme *scheme;
};

static hero_wv2_options *hero_wv2_options_of(hero_wv2_options4 *fourth) {
    return (hero_wv2_options *)(void *)((char *)fourth - offsetof(hero_wv2_options, fourth));
}

static HRESULT STDMETHODCALLTYPE hero_wv2_options_query(hero_wv2_options *self, REFIID riid, void **out) {
    if (out == NULL) {
        return E_POINTER;
    }
    if (IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid, &hero_wv2_iid_options)) {
        *out = self;
    } else if (IsEqualIID(riid, &hero_wv2_iid_options4)) {
        *out = &self->fourth;
    } else {
        *out = NULL;
        return E_NOINTERFACE;
    }
    InterlockedIncrement(&self->references);
    return S_OK;
}

static ULONG STDMETHODCALLTYPE hero_wv2_options_add(hero_wv2_options *self) {
    return (ULONG)InterlockedIncrement(&self->references);
}

static ULONG STDMETHODCALLTYPE hero_wv2_options_release(hero_wv2_options *self) {
    LONG left = InterlockedDecrement(&self->references);

    if (left == 0) {
        self->scheme->lpVtbl->Release(self->scheme);
        hero_layer_free(self);
    }
    return (ULONG)left;
}

static HRESULT STDMETHODCALLTYPE hero_wv2_options_empty(hero_wv2_options *self, LPWSTR *value) {
    (void)self;
    return hero_wv2_handed(L"", value);
}

static HRESULT STDMETHODCALLTYPE hero_wv2_options_ignored(hero_wv2_options *self, LPCWSTR value) {
    (void)self;
    (void)value;
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE hero_wv2_options_target(hero_wv2_options *self, LPWSTR *value) {
    (void)self;
    return hero_wv2_handed(HERO_WV2_TARGET_VERSION, value);
}

static HRESULT STDMETHODCALLTYPE hero_wv2_options_sso(hero_wv2_options *self, BOOL *allow) {
    (void)self;
    if (allow == NULL) {
        return E_POINTER;
    }
    *allow = FALSE;
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE hero_wv2_options_put_sso(hero_wv2_options *self, BOOL allow) {
    (void)self;
    (void)allow;
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE hero_wv2_options4_query(hero_wv2_options4 *self, REFIID riid, void **out) {
    return hero_wv2_options_query(hero_wv2_options_of(self), riid, out);
}

static ULONG STDMETHODCALLTYPE hero_wv2_options4_add(hero_wv2_options4 *self) {
    return hero_wv2_options_add(hero_wv2_options_of(self));
}

static ULONG STDMETHODCALLTYPE hero_wv2_options4_release(hero_wv2_options4 *self) {
    return hero_wv2_options_release(hero_wv2_options_of(self));
}

/* The one scheme, in an array WebView2 frees with `CoTaskMemFree`, its
 * element a reference it releases. */
static HRESULT STDMETHODCALLTYPE hero_wv2_options4_schemes(hero_wv2_options4 *self, UINT32 *count, hero_wv2_scheme ***schemes) {
    hero_wv2_options *options = hero_wv2_options_of(self);

    if (count == NULL || schemes == NULL) {
        return E_POINTER;
    }
    *schemes = CoTaskMemAlloc(sizeof **schemes);
    if (*schemes == NULL) {
        return E_OUTOFMEMORY;
    }
    options->scheme->lpVtbl->AddRef(options->scheme);
    (*schemes)[0] = options->scheme;
    *count = 1;
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE hero_wv2_options4_put_schemes(hero_wv2_options4 *self, UINT32 count, hero_wv2_scheme **schemes) {
    (void)self;
    (void)count;
    (void)schemes;
    return S_OK;
}

static const hero_wv2_options_vtbl hero_wv2_options_table = {
    hero_wv2_options_query,
    hero_wv2_options_add,
    hero_wv2_options_release,
    hero_wv2_options_empty,
    hero_wv2_options_ignored,
    hero_wv2_options_empty,
    hero_wv2_options_ignored,
    hero_wv2_options_target,
    hero_wv2_options_ignored,
    hero_wv2_options_sso,
    hero_wv2_options_put_sso,
};

static const hero_wv2_options4_vtbl hero_wv2_options4_table = {
    hero_wv2_options4_query,
    hero_wv2_options4_add,
    hero_wv2_options4_release,
    hero_wv2_options4_schemes,
    hero_wv2_options4_put_schemes,
};

/* -- RUNNING THE THREAD'S MESSAGES WHILE WEBVIEW2 WORKS ---------------------- */

/* Runs the thread's messages until `handler` is done or `ms` milliseconds
 * have passed; false where they passed, or where the thread was asked to
 * quit, which is posted again for the arm's own loop. */
static bool hero_ui_web_win32_waited(hero_wv2_handler *handler, DWORD ms) {
    ULONGLONG until = GetTickCount64() + ms;
    MSG message;

    while (!handler->done) {
        ULONGLONG now = GetTickCount64();

        if (now >= until) {
            return false;
        }
        (void)MsgWaitForMultipleObjectsEx(0, NULL, (DWORD)(until - now), QS_ALLINPUT, MWMO_INPUTAVAILABLE);
        while (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT) {
                PostQuitMessage((int)message.wParam);
                return false;
            }
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }
    return true;
}

/* -- THE ENVIRONMENT --------------------------------------------------------- */

/* The process's environment, made at the first call; `HERO_UI_OK`, or the
 * failure said. */
static int64_t hero_ui_web_win32_environment_ready(void) {
    wchar_t folder[MAX_PATH];
    DWORD got;
    HRESULT com;
    hero_wv2_options *options;
    hero_wv2_handler *done;
    HRESULT asked;
    void *env10 = NULL;
    char words[HERO_UI_WEB_WIN32_WORDS_MOST];

    if (hero_ui_web_win32_environment != NULL) {
        return HERO_UI_OK;
    }
    com = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (com == RPC_E_CHANGED_MODE) {
        return hero_ui_web_win32_said(HERO_UI_UNSUPPORTED, "this thread's COM is multithreaded, and WebView2 lives in a single-threaded apartment");
    }
    if (hero_ui_web_win32_create_environment == NULL) {
        HMODULE loader = LoadLibraryW(L"WebView2Loader.dll");
        FARPROC found;

        if (loader == NULL) {
            return hero_ui_web_win32_said(HERO_UI_UNSUPPORTED, "WebView2Loader.dll was not found beside the program nor on the system's path; it ships with Microsoft's WebView2 SDK");
        }
        found = GetProcAddress(loader, "CreateCoreWebView2EnvironmentWithOptions");
        if (found == NULL) {
            return hero_ui_web_win32_said(HERO_UI_UNSUPPORTED, "WebView2Loader.dll has no CreateCoreWebView2EnvironmentWithOptions");
        }
        memcpy(&hero_ui_web_win32_create_environment, &found, sizeof found);
    }
    got = GetEnvironmentVariableW(L"LOCALAPPDATA", folder, MAX_PATH);
    if (got == 0 || got >= MAX_PATH - 32) {
        return hero_ui_web_win32_said(HERO_UI_OTHER, "the user's local application data folder (LOCALAPPDATA) is not named, or too long");
    }
    wcscat_s(folder, MAX_PATH, L"\\Heroes\\WebView2");
    options = hero_layer_alloc_zero(1, sizeof *options);
    done = hero_wv2_handler_made(HERO_WV2_ENVIRONMENT_DONE, &hero_wv2_iid_environment_done, NULL);
    if (options == NULL || done == NULL) {
        hero_layer_free(options);
        if (done != NULL) {
            (void)hero_wv2_handler_release(done);
        }
        return hero_ui_web_win32_said(HERO_UI_LIMIT, "no memory for WebView2's environment");
    }
    options->scheme = hero_layer_alloc_zero(1, sizeof *options->scheme);
    if (options->scheme == NULL) {
        hero_layer_free(options);
        (void)hero_wv2_handler_release(done);
        return hero_ui_web_win32_said(HERO_UI_LIMIT, "no memory for WebView2's environment");
    }
    options->scheme->lpVtbl = &hero_wv2_scheme_table;
    options->scheme->references = 1;
    options->lpVtbl = &hero_wv2_options_table;
    options->fourth.lpVtbl = &hero_wv2_options4_table;
    options->references = 1;
    asked = hero_ui_web_win32_create_environment(NULL, folder, options, done);
    if (FAILED(asked) || !hero_ui_web_win32_waited(done, HERO_UI_WEB_WIN32_WAIT) || FAILED(done->code) || done->result == NULL) {
        HRESULT code = FAILED(asked) ? asked : done->code;

        (void)snprintf(words, sizeof words, "WebView2 made no environment (HRESULT 0x%08lx); where it is 0x80070002 no WebView2 runtime is installed", (unsigned long)code);
        (void)hero_wv2_options_release(options);
        (void)hero_wv2_handler_release(done);
        return hero_ui_web_win32_said(HERO_UI_UNSUPPORTED, words);
    }
    (void)hero_wv2_options_release(options);
    if (FAILED(done->result->lpVtbl->QueryInterface(done->result, &hero_wv2_iid_environment10, &env10)) || env10 == NULL) {
        (void)hero_wv2_handler_release(done);
        return hero_ui_web_win32_said(HERO_UI_UNSUPPORTED, "the WebView2 runtime installed is too old: it has no private mode for a web view (ICoreWebView2Environment10)");
    }
    (void)hero_wv2_handler_release(done);
    hero_ui_web_win32_environment = env10;
    return HERO_UI_OK;
}

/* -- WHAT WAITS FOR A PAGE --------------------------------------------------- */

/* `count` bytes of UTF-8 at `text` as a JSON string, quotes and all, the
 * layer's to free, or NULL: every `"`, `\` and control character escaped,
 * and U+2028 and U+2029, which a script of an older engine reads as line
 * ends, written as escapes. */
static char *hero_ui_web_win32_json(const char *text, size_t count, size_t *length) {
    char *out = hero_layer_alloc(count * 6 + 3);
    size_t at = 0;

    if (out == NULL) {
        return NULL;
    }
    out[at] = '"';
    at += 1;
    for (size_t i = 0; i < count; i += 1) {
        unsigned char c = (unsigned char)text[i];

        if (c == '"' || c == '\\') {
            out[at] = '\\';
            out[at + 1] = (char)c;
            at += 2;
        } else if (c < 0x20) {
            (void)snprintf(out + at, 7, "\\u%04x", (unsigned)c);
            at += 6;
        } else if (c == 0xE2 && i + 2 < count && (unsigned char)text[i + 1] == 0x80 && ((unsigned char)text[i + 2] == 0xA8 || (unsigned char)text[i + 2] == 0xA9)) {
            (void)snprintf(out + at, 7, "\\u%04x", (unsigned char)text[i + 2] == 0xA8 ? 0x2028u : 0x2029u);
            at += 6;
            i += 2;
        } else {
            out[at] = (char)c;
            at += 1;
        }
    }
    out[at] = '"';
    at += 1;
    out[at] = '\0';
    *length = at;
    return out;
}

/* Hands `text` to the page: posted to its `receive`, or sent as it by its
 * own bridge, the text a JSON string in the script, written by
 * `hero_ui_web_win32_json`. */
static void hero_ui_web_win32_run(hero_ui_web_win32_view *v, bool as_page, const char *text, size_t count) {
    if (!as_page) {
        wchar_t *wide = hero_ui_web_win32_wide((const uint8_t *)text, count);

        if (wide != NULL) {
            (void)v->core->lpVtbl->PostWebMessageAsString(v->core, wide);
            hero_layer_free(wide);
        }
        return;
    }
    {
        static const char head[] = "window.heroes.send(";
        size_t length = 0;
        char *quoted = hero_ui_web_win32_json(text, count, &length);
        char *script;
        wchar_t *wide;

        if (quoted == NULL) {
            return;
        }
        script = hero_layer_alloc(sizeof head - 1 + length + 2);
        if (script == NULL) {
            hero_layer_free(quoted);
            return;
        }
        memcpy(script, head, sizeof head - 1);
        memcpy(script + sizeof head - 1, quoted, length);
        script[sizeof head - 1 + length] = ')';
        wide = hero_ui_web_win32_wide((const uint8_t *)script, sizeof head + length);
        if (wide != NULL) {
            (void)v->core->lpVtbl->ExecuteScript(v->core, wide, NULL);
            hero_layer_free(wide);
        }
        hero_layer_free(script);
        hero_layer_free(quoted);
    }
}

static void hero_ui_web_win32_flush(hero_ui_web_win32_view *v) {
    for (int64_t i = 0; i < v->held; i += 1) {
        hero_ui_web_win32_run(v, v->waiting[i].as_page, v->waiting[i].text, v->waiting[i].count);
        hero_layer_free(v->waiting[i].text);
    }
    v->held = 0;
}

static void hero_ui_web_win32_drop_waiting(hero_ui_web_win32_view *v) {
    for (int64_t i = 0; i < v->held; i += 1) {
        hero_layer_free(v->waiting[i].text);
    }
    v->held = 0;
}

static int64_t hero_ui_web_win32_given(int64_t id, bool as_page, const uint8_t *text, int64_t count) {
    hero_ui_web_win32_view *v = hero_ui_web_win32_of(id);
    char *copy;

    if (v == NULL) {
        return hero_ui_web_win32_said(HERO_UI_NOT_FOUND, "WebView2 holds no such web view");
    }
    if (v->ready) {
        hero_ui_web_win32_run(v, as_page, (const char *)text, (size_t)count);
        return HERO_UI_OK;
    }
    if (v->held == HERO_UI_QUEUE_MOST) {
        return hero_ui_web_win32_said(HERO_UI_LIMIT, "as many texts wait for the page as it keeps");
    }
    if (v->held == v->room) {
        int64_t room = v->room == 0 ? 16 : v->room * 2;
        hero_ui_web_win32_held *grown = hero_layer_grow(v->waiting, (size_t)room * sizeof *grown);

        if (grown == NULL) {
            return hero_ui_web_win32_said(HERO_UI_LIMIT, "no memory for the texts waiting");
        }
        v->waiting = grown;
        v->room = room;
    }
    copy = hero_layer_alloc((size_t)count + 1);
    if (copy == NULL) {
        return hero_ui_web_win32_said(HERO_UI_LIMIT, "no memory for the text");
    }
    memcpy(copy, text, (size_t)count);
    copy[count] = '\0';
    v->waiting[v->held].as_page = as_page;
    v->waiting[v->held].text = copy;
    v->waiting[v->held].count = (size_t)count;
    v->held += 1;
    return HERO_UI_OK;
}

/* -- THE APP'S SCHEME -------------------------------------------------------- */

/* A request of the app's scheme answered whole with what the layer's
 * `serve` wrote, its headers those `hero_ui.h` names. */
static void hero_ui_web_win32_serve(hero_ui_web_win32_view *v, hero_wv2_resource_args *args) {
    hero_wv2_request *request = NULL;
    LPWSTR uri = NULL;
    LPWSTR method = NULL;
    char *url = NULL;
    char *verb = NULL;
    size_t url_count = 0;
    size_t verb_count = 0;
    int64_t numbers[HERO_UI_WEB_NUMBERS] = { 0 };
    const uint8_t *texts[HERO_UI_WEB_TEXTS] = { NULL };
    char *headers = NULL;
    wchar_t *wide_headers = NULL;
    IStream *stream = NULL;
    hero_wv2_unknown *response = NULL;
    size_t room;
    int written;

    if (FAILED(args->lpVtbl->get_Request(args, &request)) || request == NULL || FAILED(request->lpVtbl->get_Uri(request, &uri)) || FAILED(request->lpVtbl->get_Method(request, &method))) {
        goto done;
    }
    url = hero_ui_web_win32_narrow(uri, &url_count);
    verb = hero_ui_web_win32_narrow(method, &verb_count);
    if (url == NULL || verb == NULL) {
        goto done;
    }
    (void)v->host.serve(v->id, (const uint8_t *)verb, (int64_t)verb_count, (const uint8_t *)url, (int64_t)url_count, numbers, texts);
    room = (size_t)numbers[HERO_UI_WEB_TYPE_COUNT] + (size_t)numbers[HERO_UI_WEB_POLICY_COUNT] + 256;
    headers = hero_layer_alloc(room);
    if (headers == NULL) {
        goto done;
    }
    written = snprintf(headers, room, "Content-Type: %.*s\r\nContent-Length: %lld\r\nContent-Security-Policy: %.*s\r\nX-Content-Type-Options: nosniff\r\nCache-Control: no-store\r\nReferrer-Policy: no-referrer%s", (int)numbers[HERO_UI_WEB_TYPE_COUNT], (const char *)texts[HERO_UI_WEB_TYPE], (long long)numbers[HERO_UI_WEB_LENGTH], (int)numbers[HERO_UI_WEB_POLICY_COUNT], (const char *)texts[HERO_UI_WEB_POLICY], numbers[HERO_UI_WEB_STATUS] == 405 ? "\r\nAllow: GET, HEAD" : "");
    if (written < 0 || (size_t)written >= room) {
        goto done;
    }
    wide_headers = hero_ui_web_win32_wide((const uint8_t *)headers, (size_t)written);
    stream = SHCreateMemStream(numbers[HERO_UI_WEB_BODY_COUNT] > 0 ? texts[HERO_UI_WEB_BODY] : NULL, (UINT)numbers[HERO_UI_WEB_BODY_COUNT]);
    if (wide_headers == NULL || stream == NULL) {
        goto done;
    }
    if (SUCCEEDED(hero_ui_web_win32_environment->lpVtbl->CreateWebResourceResponse(hero_ui_web_win32_environment, stream, (int)numbers[HERO_UI_WEB_STATUS], numbers[HERO_UI_WEB_STATUS] == 200 ? L"OK" : (numbers[HERO_UI_WEB_STATUS] == 404 ? L"Not Found" : L"Method Not Allowed"), wide_headers, &response)) && response != NULL) {
        (void)args->lpVtbl->put_Response(args, response);
    }
done:
    if (response != NULL) {
        response->lpVtbl->Release(response);
    }
    if (stream != NULL) {
        stream->lpVtbl->Release(stream);
    }
    if (request != NULL) {
        request->lpVtbl->Release(request);
    }
    hero_layer_free(wide_headers);
    hero_layer_free(headers);
    hero_layer_free(url);
    hero_layer_free(verb);
    CoTaskMemFree(uri);
    CoTaskMemFree(method);
}

/* -- THE ENTRY POINTS -------------------------------------------------------- */

/* Registers `handler` by `add`, keeping it in `v`'s list; false where
 * WebView2 refused. */
#define HERO_WV2_REGISTER(add, kind, iid)                                                     \
    do {                                                                                      \
        hero_wv2_handler *h = hero_wv2_handler_made(kind, iid, v);                            \
        hero_wv2_token token;                                                                 \
        if (h == NULL || FAILED(v->core->lpVtbl->add(v->core, h, &token))) {                  \
            if (h != NULL) {                                                                  \
                (void)hero_wv2_handler_release(h);                                            \
            }                                                                                 \
            registered = false;                                                               \
        } else {                                                                              \
            v->handlers[v->handler_count] = h;                                                \
            v->handler_count += 1;                                                            \
        }                                                                                     \
    } while (0)

/* Lets go of everything `v` holds, the controller closed first so WebView2
 * calls none of its handlers again. */
static void hero_ui_web_win32_let_go(hero_ui_web_win32_view *v) {
    if (v->controller != NULL) {
        (void)v->controller->lpVtbl->Close(v->controller);
    }
    for (int i = 0; i < v->handler_count; i += 1) {
        v->handlers[i]->view = NULL;
        (void)hero_wv2_handler_release(v->handlers[i]);
    }
    if (v->core != NULL) {
        v->core->lpVtbl->Release(v->core);
    }
    if (v->controller != NULL) {
        v->controller->lpVtbl->Release(v->controller);
    }
    hero_ui_web_win32_drop_waiting(v);
    hero_layer_free(v->waiting);
    hero_layer_free(v);
}

int64_t hero_ui_web_win32_create(int64_t id, const hero_ui_web_host *host, void *parent) {
    hero_ui_web_win32_view *v;
    hero_wv2_controller_options *options = NULL;
    hero_wv2_handler *done;
    hero_wv2_core *first = NULL;
    hero_wv2_settings *settings = NULL;
    wchar_t *bridge;
    bool registered = true;
    int64_t kind;

    if (host == NULL || parent == NULL) {
        return hero_ui_web_win32_said(HERO_UI_INVALID, "no host or no parent window was given");
    }
    if (hero_ui_web_win32_count == HERO_UI_WEB_WIN32_VIEWS_MOST) {
        return hero_ui_web_win32_said(HERO_UI_LIMIT, "an app holds 64 web views at most");
    }
    kind = hero_ui_web_win32_environment_ready();
    if (kind != HERO_UI_OK) {
        return kind;
    }
    v = hero_layer_alloc_zero(1, sizeof *v);
    done = hero_wv2_handler_made(HERO_WV2_CONTROLLER_DONE, &hero_wv2_iid_controller_done, NULL);
    if (v == NULL || done == NULL) {
        hero_layer_free(v);
        if (done != NULL) {
            (void)hero_wv2_handler_release(done);
        }
        return hero_ui_web_win32_said(HERO_UI_LIMIT, "no memory for the web view");
    }
    v->id = id;
    v->host = *host;
    if (FAILED(hero_ui_web_win32_environment->lpVtbl->CreateCoreWebView2ControllerOptions(hero_ui_web_win32_environment, &options)) || options == NULL || FAILED(options->lpVtbl->put_IsInPrivateModeEnabled(options, TRUE)) || FAILED(hero_ui_web_win32_environment->lpVtbl->CreateCoreWebView2ControllerWithOptions(hero_ui_web_win32_environment, (HWND)parent, options, done)) || !hero_ui_web_win32_waited(done, HERO_UI_WEB_WIN32_WAIT) || FAILED(done->code) || done->result == NULL) {
        if (options != NULL) {
            options->lpVtbl->Release(options);
        }
        (void)hero_wv2_handler_release(done);
        hero_layer_free(v);
        return hero_ui_web_win32_said(HERO_UI_OTHER, "WebView2 made no web view in the window");
    }
    options->lpVtbl->Release(options);
    v->controller = (hero_wv2_controller *)done->result;
    v->controller->lpVtbl->AddRef(v->controller);
    (void)hero_wv2_handler_release(done);
    if (FAILED(v->controller->lpVtbl->get_CoreWebView2(v->controller, &first)) || first == NULL || FAILED(first->lpVtbl->QueryInterface(first, &hero_wv2_iid_core4, (void **)&v->core)) || v->core == NULL) {
        if (first != NULL) {
            first->lpVtbl->Release(first);
        }
        hero_ui_web_win32_let_go(v);
        return hero_ui_web_win32_said(HERO_UI_UNSUPPORTED, "the WebView2 runtime installed is too old: it cannot cancel a download (ICoreWebView2_4)");
    }
    first->lpVtbl->Release(first);
    (void)v->controller->lpVtbl->put_IsVisible(v->controller, FALSE);
    if (SUCCEEDED(v->core->lpVtbl->get_Settings(v->core, &settings)) && settings != NULL) {
        (void)settings->lpVtbl->put_IsWebMessageEnabled(settings, TRUE);
        (void)settings->lpVtbl->put_AreDevToolsEnabled(settings, FALSE);
        (void)settings->lpVtbl->put_AreHostObjectsAllowed(settings, FALSE);
        (void)settings->lpVtbl->put_IsStatusBarEnabled(settings, FALSE);
        settings->lpVtbl->Release(settings);
    }
    HERO_WV2_REGISTER(add_NavigationStarting, HERO_WV2_NAVIGATION_STARTING, &hero_wv2_iid_navigation_starting);
    HERO_WV2_REGISTER(add_FrameNavigationStarting, HERO_WV2_FRAME_NAVIGATION_STARTING, &hero_wv2_iid_navigation_starting);
    HERO_WV2_REGISTER(add_NavigationCompleted, HERO_WV2_NAVIGATION_COMPLETED, &hero_wv2_iid_navigation_completed);
    HERO_WV2_REGISTER(add_WebResourceRequested, HERO_WV2_RESOURCE_REQUESTED, &hero_wv2_iid_resource_requested);
    HERO_WV2_REGISTER(add_WebMessageReceived, HERO_WV2_MESSAGE_RECEIVED, &hero_wv2_iid_message_received);
    HERO_WV2_REGISTER(add_NewWindowRequested, HERO_WV2_NEW_WINDOW, &hero_wv2_iid_new_window);
    HERO_WV2_REGISTER(add_PermissionRequested, HERO_WV2_PERMISSION, &hero_wv2_iid_permission);
    HERO_WV2_REGISTER(add_ProcessFailed, HERO_WV2_PROCESS_FAILED, &hero_wv2_iid_process_failed);
    HERO_WV2_REGISTER(add_DownloadStarting, HERO_WV2_DOWNLOAD_STARTING, &hero_wv2_iid_download_starting);
    bridge = hero_ui_web_win32_wide(host->bridge, (size_t)host->bridge_count);
    if (!registered || bridge == NULL || FAILED(v->core->lpVtbl->AddScriptToExecuteOnDocumentCreated(v->core, bridge, NULL)) || FAILED(v->core->lpVtbl->AddWebResourceRequestedFilter(v->core, L"" HERO_UI_WEB_SCHEME L"://" HERO_UI_WEB_HOST L"/*", HERO_WV2_RESOURCE_CONTEXT_ALL))) {
        hero_layer_free(bridge);
        hero_ui_web_win32_let_go(v);
        return hero_ui_web_win32_said(HERO_UI_OTHER, "WebView2 refused the web view's handlers, its bridge or its scheme");
    }
    hero_layer_free(bridge);
    hero_ui_web_win32_views[hero_ui_web_win32_count] = v;
    hero_ui_web_win32_count += 1;
    return HERO_UI_OK;
}

int64_t hero_ui_web_win32_place(int64_t id, int64_t left, int64_t top, int64_t width, int64_t height) {
    hero_ui_web_win32_view *v = hero_ui_web_win32_of(id);
    RECT bounds;

    if (v == NULL) {
        return hero_ui_web_win32_said(HERO_UI_NOT_FOUND, "WebView2 holds no such web view");
    }
    if (left < INT32_MIN || top < INT32_MIN || width < 0 || height < 0 || left > INT32_MAX - width || top > INT32_MAX - height) {
        return hero_ui_web_win32_said(HERO_UI_INVALID, "the web view's place does not fit a window's coordinates");
    }
    bounds.left = (LONG)left;
    bounds.top = (LONG)top;
    bounds.right = (LONG)(left + width);
    bounds.bottom = (LONG)(top + height);
    return FAILED(v->controller->lpVtbl->put_Bounds(v->controller, bounds)) ? hero_ui_web_win32_said(HERO_UI_OTHER, "WebView2 refused the web view's place") : HERO_UI_OK;
}

int64_t hero_ui_web_win32_show(int64_t id, int64_t on) {
    hero_ui_web_win32_view *v = hero_ui_web_win32_of(id);

    if (v == NULL) {
        return hero_ui_web_win32_said(HERO_UI_NOT_FOUND, "WebView2 holds no such web view");
    }
    return FAILED(v->controller->lpVtbl->put_IsVisible(v->controller, on == 1 ? TRUE : FALSE)) ? hero_ui_web_win32_said(HERO_UI_OTHER, "WebView2 refused to show or hide the web view") : HERO_UI_OK;
}

int64_t hero_ui_web_win32_destroy(int64_t id) {
    for (int64_t i = 0; i < hero_ui_web_win32_count; i += 1) {
        hero_ui_web_win32_view *v = hero_ui_web_win32_views[i];
        hero_ui_web_gone_fn gone;

        if (v->id != id) {
            continue;
        }
        gone = v->host.gone;
        hero_ui_web_win32_views[i] = hero_ui_web_win32_views[hero_ui_web_win32_count - 1];
        hero_ui_web_win32_count -= 1;
        hero_ui_web_win32_let_go(v);
        gone(id);
        return HERO_UI_OK;
    }
    return hero_ui_web_win32_said(HERO_UI_NOT_FOUND, "WebView2 holds no such web view");
}

int64_t hero_ui_web_win32_load(int64_t id, const uint8_t *url, int64_t count) {
    hero_ui_web_win32_view *v = hero_ui_web_win32_of(id);
    wchar_t *wide;
    HRESULT asked;

    if (v == NULL) {
        return hero_ui_web_win32_said(HERO_UI_NOT_FOUND, "WebView2 holds no such web view");
    }
    wide = hero_ui_web_win32_wide(url, (size_t)count);
    if (wide == NULL) {
        return hero_ui_web_win32_said(HERO_UI_LIMIT, "no memory for the URL");
    }
    v->ready = false;
    asked = v->core->lpVtbl->Navigate(v->core, wide);
    hero_layer_free(wide);
    return FAILED(asked) ? hero_ui_web_win32_said(HERO_UI_OTHER, "WebView2 refused to show the page") : HERO_UI_OK;
}

int64_t hero_ui_web_win32_post(int64_t id, const uint8_t *text, int64_t count) {
    return hero_ui_web_win32_given(id, false, text, count);
}

int64_t hero_ui_web_win32_send(int64_t id, const uint8_t *text, int64_t count) {
    return hero_ui_web_win32_given(id, true, text, count);
}

#else
/* ==========================================================================
 * EVERY OTHER SYSTEM: no WebView2; the web view is another arm's. Each
 * function answers `HERO_UI_UNSUPPORTED`, so a call that did reach one is a
 * value.
 * ========================================================================== */

int64_t hero_ui_web_win32_create(int64_t id, const hero_ui_web_host *host, void *parent) {
    (void)id;
    (void)host;
    (void)parent;
    return hero_ui_web_win32_said(HERO_UI_UNSUPPORTED, "WebView2 is Windows' web view");
}

int64_t hero_ui_web_win32_place(int64_t id, int64_t left, int64_t top, int64_t width, int64_t height) {
    (void)id;
    (void)left;
    (void)top;
    (void)width;
    (void)height;
    return hero_ui_web_win32_said(HERO_UI_UNSUPPORTED, "WebView2 is Windows' web view");
}

int64_t hero_ui_web_win32_show(int64_t id, int64_t on) {
    (void)id;
    (void)on;
    return hero_ui_web_win32_said(HERO_UI_UNSUPPORTED, "WebView2 is Windows' web view");
}

int64_t hero_ui_web_win32_destroy(int64_t id) {
    (void)id;
    return hero_ui_web_win32_said(HERO_UI_UNSUPPORTED, "WebView2 is Windows' web view");
}

int64_t hero_ui_web_win32_load(int64_t id, const uint8_t *url, int64_t count) {
    (void)id;
    (void)url;
    (void)count;
    return hero_ui_web_win32_said(HERO_UI_UNSUPPORTED, "WebView2 is Windows' web view");
}

int64_t hero_ui_web_win32_post(int64_t id, const uint8_t *text, int64_t count) {
    (void)id;
    (void)text;
    (void)count;
    return hero_ui_web_win32_said(HERO_UI_UNSUPPORTED, "WebView2 is Windows' web view");
}

int64_t hero_ui_web_win32_send(int64_t id, const uint8_t *text, int64_t count) {
    (void)id;
    (void)text;
    (void)count;
    return hero_ui_web_win32_said(HERO_UI_UNSUPPORTED, "WebView2 is Windows' web view");
}

#endif
