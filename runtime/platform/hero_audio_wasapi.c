/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_audio_wasapi.c — the arm of sound (`platform/hero_audio_arm.h`)
 * by WASAPI, on Windows (M-rebel-library, 2026-11-23). Empty elsewhere.
 *
 * COM, IN C. The interfaces are reached by `COBJMACROS`, and the class and
 * interface identifiers this arm needs are written here, as Microsoft's
 * headers define them, so no `initguid` trick is needed; `ole32` is linked by
 * `#pragma comment(lib)`, as the web view's member links it, so only a
 * program that takes this member links it. Each call that reaches COM on the
 * program's thread initializes it for that call (the multithreaded
 * apartment, or the one the thread already has) and gives it back.
 *
 * THE DEVICES are the active render (outputs) and capture (inputs) endpoints,
 * each named by its endpoint identifier and its friendly name, at its shared
 * mode's mix format; the defaults are the console role's.
 *
 * A STREAM is a shared-mode `IAudioClient` of 32-bit floats at the stream's
 * rate and channels, which the audio engine converts to the device's mix
 * format (`AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM`), driven by its event, and one
 * thread of this arm's own that owns every COM object of the stream: it makes
 * them, tells the opener by an event whether it could, then moves frames each
 * time the engine signals and acts on the flags the program's side sets
 * (pause, resume, stop, close). A finished output's end is told once the
 * engine's padding has played to zero; `AUDCLNT_E_DEVICE_INVALIDATED` is a
 * lost device; `E_ACCESSDENIED` on a capture endpoint is the person's
 * privacy setting for the microphone, told `HERO_AUDIO_DENIED`.
 *
 * THE BEEP is `MessageBeep(MB_OK)`, the sound the person chose for it.
 */

#include "hero_audio_wasapi.h"

#if defined(_WIN32)
#define COBJMACROS
#include <windows.h>
#include <audioclient.h>
#include <mmdeviceapi.h>
#include <propidl.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "hero_layer_alloc.h"

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "user32.lib")

static const CLSID hero_wasapi_enumerator_class = {0xBCDE0395, 0xE52F, 0x467C, {0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E}};
static const IID hero_wasapi_enumerator_iid = {0xA95664D2, 0x9614, 0x4F35, {0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6}};
static const IID hero_wasapi_endpoint_iid = {0x1BE09788, 0x6894, 0x4089, {0x85, 0x86, 0x9A, 0x2A, 0x6C, 0x26, 0x5A, 0xC5}};
static const IID hero_wasapi_client_iid = {0x1CB9AD4C, 0xDBFA, 0x4C32, {0xB1, 0x78, 0xC2, 0xF5, 0x68, 0xA7, 0x03, 0xB2}};
static const IID hero_wasapi_render_iid = {0xF294ACFC, 0x3146, 0x4483, {0xA7, 0xBF, 0xAD, 0xDC, 0xA7, 0xC2, 0x60, 0xE2}};
static const IID hero_wasapi_capture_iid = {0xC8ADBD64, 0xE71E, 0x48A0, {0xA4, 0xDE, 0x18, 0x5C, 0x39, 0x5C, 0xD3, 0x17}};
static const GUID hero_wasapi_float = {0x00000003, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71}};
static const PROPERTYKEY hero_wasapi_friendly_name = {{0xA45C254E, 0xDF1C, 0x4EFD, {0x80, 0x20, 0x67, 0xD1, 0x46, 0xA8, 0x50, 0xE0}}, 14};

#define HERO_WASAPI_AUTOCONVERT 0x80000000u
#define HERO_WASAPI_SRC_QUALITY 0x08000000u
#define HERO_WASAPI_INVALIDATED ((HRESULT)0x88890004L)
#define HERO_WASAPI_NOT_FOUND ((HRESULT)0x80070490L)

static int64_t hero_wasapi_fail(int64_t kind, const char *asked, HRESULT status) {
    char words[192];

    (void)snprintf(words, sizeof words, "%s: WASAPI answered 0x%08lX", asked, (unsigned long)status);
    return hero_audio_arm_fail(kind, (int64_t)(uint32_t)status, words);
}

/* COM on the calling thread for one call: true where this call began it and
 * owes `CoUninitialize`. */
static bool hero_wasapi_enter(void) {
    HRESULT status = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    return status == S_OK || status == S_FALSE;
}

static void hero_wasapi_leave(bool began) {
    if (began) {
        CoUninitialize();
    }
}

/* The UTF-8 of `wide` in a block of the layer's, or NULL. */
static char *hero_wasapi_utf8(const wchar_t *wide) {
    int n = wide == NULL ? 0 : WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wide, -1, NULL, 0, NULL, NULL);
    char *out = n <= 0 ? NULL : hero_layer_alloc((size_t)n);

    if (out != NULL && WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wide, -1, out, n, NULL, NULL) != n) {
        hero_layer_free(out);
        return NULL;
    }
    return out;
}

/* The UTF-16 of `text`, or NULL. */
static wchar_t *hero_wasapi_wide(const char *text) {
    int n = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, NULL, 0);
    wchar_t *out = n <= 0 ? NULL : hero_layer_alloc((size_t)n * sizeof(wchar_t));

    if (out != NULL && MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, out, n) != n) {
        hero_layer_free(out);
        return NULL;
    }
    return out;
}

static IMMDeviceEnumerator *hero_wasapi_enumerator(void) {
    IMMDeviceEnumerator *e = NULL;
    HRESULT status = CoCreateInstance(&hero_wasapi_enumerator_class, NULL, CLSCTX_ALL, &hero_wasapi_enumerator_iid, (void **)&e);

    if (FAILED(status)) {
        (void)hero_wasapi_fail(HERO_AUDIO_UNSUPPORTED, "reaching the audio endpoints", status);
        return NULL;
    }
    return e;
}

/* The identifier of `device` as UTF-8, or NULL. */
static char *hero_wasapi_id(IMMDevice *device) {
    LPWSTR wide = NULL;

    if (FAILED(IMMDevice_GetId(device, &wide))) {
        return NULL;
    }
    char *id = hero_wasapi_utf8(wide);

    CoTaskMemFree(wide);
    return id;
}

/* The encoding name of a mix format. */
static const char *hero_wasapi_encoding(const WAVEFORMATEX *w) {
    bool floats = w->wFormatTag == WAVE_FORMAT_IEEE_FLOAT ||
                  (w->wFormatTag == WAVE_FORMAT_EXTENSIBLE && w->cbSize >= 22 && IsEqualGUID(&((const WAVEFORMATEXTENSIBLE *)w)->SubFormat, &hero_wasapi_float));

    if (floats) {
        return w->wBitsPerSample == 64 ? "float64" : "float32";
    }
    return w->wBitsPerSample == 8 ? "pcm8" : w->wBitsPerSample == 16 ? "pcm16" : w->wBitsPerSample == 24 ? "pcm24" : "pcm32";
}

int64_t hero_audio_arm_system(char *buffer, size_t count) {
    static const char name[] = "WASAPI";

    if (sizeof name - 1 <= count) {
        memcpy(buffer, name, sizeof name - 1);
    }
    return (int64_t)(sizeof name - 1);
}

int64_t hero_audio_arm_devices(hero_audio_list *list) {
    bool began = hero_wasapi_enter();
    IMMDeviceEnumerator *e = hero_wasapi_enumerator();
    int64_t answer = e == NULL ? -1 : 0;

    for (int d = 0; d < 2 && answer == 0; d += 1) {
        EDataFlow flow = d == 0 ? eRender : eCapture;
        IMMDeviceCollection *all = NULL;
        IMMDevice *chosen = NULL;
        char *chosen_id = NULL;
        UINT count = 0;

        if (SUCCEEDED(IMMDeviceEnumerator_GetDefaultAudioEndpoint(e, flow, eConsole, &chosen))) {
            chosen_id = hero_wasapi_id(chosen);
            IMMDevice_Release(chosen);
        }
        HRESULT status = IMMDeviceEnumerator_EnumAudioEndpoints(e, flow, DEVICE_STATE_ACTIVE, &all);

        if (FAILED(status)) {
            answer = hero_wasapi_fail(HERO_AUDIO_OTHER, "listing the audio endpoints", status);
        } else {
            (void)IMMDeviceCollection_GetCount(all, &count);
        }
        for (UINT i = 0; i < count && answer == 0; i += 1) {
            IMMDevice *device = NULL;

            if (FAILED(IMMDeviceCollection_Item(all, i, &device))) {
                continue;
            }
            char *id = hero_wasapi_id(device);
            char *name = NULL;
            IPropertyStore *store = NULL;
            IAudioClient *client = NULL;
            WAVEFORMATEX *mix = NULL;

            if (SUCCEEDED(IMMDevice_OpenPropertyStore(device, STGM_READ, &store))) {
                PROPVARIANT value;

                PropVariantInit(&value);
                if (SUCCEEDED(IPropertyStore_GetValue(store, &hero_wasapi_friendly_name, &value)) && value.vt == VT_LPWSTR) {
                    name = hero_wasapi_utf8(value.pwszVal);
                }
                (void)PropVariantClear(&value);
                IPropertyStore_Release(store);
            }
            if (SUCCEEDED(IMMDevice_Activate(device, &hero_wasapi_client_iid, CLSCTX_ALL, NULL, (void **)&client))) {
                (void)IAudioClient_GetMixFormat(client, &mix);
            }
            if (id != NULL && mix != NULL) {
                answer = hero_audio_list_add(list, d == 0 ? HERO_AUDIO_OUTPUT : HERO_AUDIO_INPUT, chosen_id != NULL && strcmp(chosen_id, id) == 0, id,
                                             name == NULL ? id : name, (int64_t)mix->nSamplesPerSec, (int64_t)mix->nChannels, hero_wasapi_encoding(mix));
            }
            if (mix != NULL) {
                CoTaskMemFree(mix);
            }
            if (client != NULL) {
                IAudioClient_Release(client);
            }
            hero_layer_free(id);
            hero_layer_free(name);
            IMMDevice_Release(device);
        }
        if (all != NULL) {
            IMMDeviceCollection_Release(all);
        }
        hero_layer_free(chosen_id);
    }
    if (e != NULL) {
        IMMDeviceEnumerator_Release(e);
    }
    hero_wasapi_leave(began);
    return answer;
}

/* The stream: what the opener hands the thread, what the thread tells back,
 * and the flags the program's side sets. */
typedef struct {
    hero_audio_core *st;
    wchar_t *device;
    int64_t direction;
    int64_t rate;
    int64_t channels;
    HANDLE thread;
    HANDLE ready;
    HANDLE wake;
    /* What the thread could do, told by `ready`: 0, or the kind it failed with,
     * the system's number and its words. */
    int64_t kind;
    int64_t os;
    char words[192];
    _Atomic bool closing;
    _Atomic bool stopping;
    _Atomic bool paused;
} hero_wasapi_stream;

static void hero_wasapi_tell(hero_wasapi_stream *ws, int64_t kind, const char *asked, HRESULT status) {
    ws->kind = kind;
    ws->os = (int64_t)(uint32_t)status;
    (void)snprintf(ws->words, sizeof ws->words, "%s: WASAPI answered 0x%08lX", asked, (unsigned long)status);
}

/* The thread's work once its objects stand: frames moved at each event, the
 * flags acted on. */
static void hero_wasapi_serve(hero_wasapi_stream *ws, IAudioClient *client, IAudioRenderClient *render, IAudioCaptureClient *capture, HANDLE event, UINT32 size) {
    bool running = true;
    bool held = false;
    bool ended = false;
    bool draining = false;

    while (!atomic_load(&ws->closing)) {
        HANDLE both[2] = {event, ws->wake};

        (void)WaitForMultipleObjects(2, both, FALSE, 100);
        if (atomic_load(&ws->closing)) {
            break;
        }
        if (atomic_load(&ws->stopping) && !ended) {
            (void)IAudioClient_Stop(client);
            (void)IAudioClient_Reset(client);
            ended = true;
        }
        bool pause = atomic_load(&ws->paused);

        if (!ended && pause != held) {
            (void)(pause ? IAudioClient_Stop(client) : IAudioClient_Start(client));
            held = pause;
        }
        if (ended || held || !running) {
            continue;
        }
        HRESULT status = S_OK;

        if (render != NULL) {
            UINT32 padding = 0;

            status = IAudioClient_GetCurrentPadding(client, &padding);
            if (SUCCEEDED(status) && draining) {
                if (padding == 0) {
                    hero_audio_played_out(ws->st);
                    ended = true;
                }
                continue;
            }
            UINT32 room = size - padding;
            BYTE *data = NULL;

            if (SUCCEEDED(status) && room > 0) {
                status = IAudioRenderClient_GetBuffer(render, room, &data);
                if (SUCCEEDED(status)) {
                    (void)hero_audio_pull(ws->st, (float *)data, room);
                    status = IAudioRenderClient_ReleaseBuffer(render, room, 0);
                }
            }
            if (SUCCEEDED(status) && hero_audio_drained(ws->st)) {
                draining = true;
            }
        } else {
            UINT32 next = 0;

            status = IAudioCaptureClient_GetNextPacketSize(capture, &next);
            while (SUCCEEDED(status) && next > 0) {
                BYTE *data = NULL;
                UINT32 frames = 0;
                DWORD flags = 0;

                status = IAudioCaptureClient_GetBuffer(capture, &data, &frames, &flags, NULL, NULL);
                if (FAILED(status)) {
                    break;
                }
                if ((flags & AUDCLNT_BUFFERFLAGS_SILENT) != 0) {
                    memset(data, 0, (size_t)frames * (size_t)ws->channels * sizeof(float));
                }
                (void)hero_audio_push(ws->st, (const float *)data, frames);
                status = IAudioCaptureClient_ReleaseBuffer(capture, frames);
                if (SUCCEEDED(status)) {
                    status = IAudioCaptureClient_GetNextPacketSize(capture, &next);
                }
            }
        }
        if (FAILED(status)) {
            char words[96];

            (void)snprintf(words, sizeof words, "WASAPI answered 0x%08lX: %s", (unsigned long)status,
                           status == HERO_WASAPI_INVALIDATED ? "the device went away" : "the stream stopped");
            hero_audio_lost(ws->st, (int64_t)(uint32_t)status, words);
            running = false;
        }
    }
    (void)IAudioClient_Stop(client);
}

static DWORD WINAPI hero_wasapi_run(LPVOID given) {
    hero_wasapi_stream *ws = given;
    bool began = hero_wasapi_enter();
    IMMDeviceEnumerator *e = hero_wasapi_enumerator();
    IMMDevice *device = NULL;
    IAudioClient *client = NULL;
    IAudioRenderClient *render = NULL;
    IAudioCaptureClient *capture = NULL;
    HANDLE event = CreateEventW(NULL, FALSE, FALSE, NULL);
    EDataFlow flow = ws->direction == HERO_AUDIO_OUTPUT ? eRender : eCapture;
    HRESULT status = e == NULL ? E_FAIL : ws->device == NULL ? IMMDeviceEnumerator_GetDefaultAudioEndpoint(e, flow, eConsole, &device)
                                                              : IMMDeviceEnumerator_GetDevice(e, ws->device, &device);
    UINT32 size = 0;

    if (e == NULL) {
        ws->kind = HERO_AUDIO_UNSUPPORTED;
        (void)snprintf(ws->words, sizeof ws->words, "this Windows has no audio endpoints this layer reaches");
    } else if (FAILED(status)) {
        hero_wasapi_tell(ws, status == HERO_WASAPI_NOT_FOUND || ws->device == NULL ? HERO_AUDIO_NOT_FOUND : HERO_AUDIO_OTHER, "finding the device", status);
    } else {
        IMMEndpoint *endpoint = NULL;
        EDataFlow its = flow;

        if (SUCCEEDED(IMMDevice_QueryInterface(device, &hero_wasapi_endpoint_iid, (void **)&endpoint))) {
            (void)IMMEndpoint_GetDataFlow(endpoint, &its);
            IMMEndpoint_Release(endpoint);
        }
        status = its != flow ? HERO_WASAPI_NOT_FOUND : IMMDevice_Activate(device, &hero_wasapi_client_iid, CLSCTX_ALL, NULL, (void **)&client);
        if (FAILED(status)) {
            hero_wasapi_tell(ws, its != flow ? HERO_AUDIO_NOT_FOUND : HERO_AUDIO_OTHER, "activating the device", status);
        }
    }
    if (ws->kind == 0) {
        WAVEFORMATEXTENSIBLE format;

        memset(&format, 0, sizeof format);
        format.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
        format.Format.nChannels = (WORD)ws->channels;
        format.Format.nSamplesPerSec = (DWORD)ws->rate;
        format.Format.wBitsPerSample = 32;
        format.Format.nBlockAlign = (WORD)(4 * ws->channels);
        format.Format.nAvgBytesPerSec = format.Format.nSamplesPerSec * format.Format.nBlockAlign;
        format.Format.cbSize = 22;
        format.Samples.wValidBitsPerSample = 32;
        format.dwChannelMask = 0;
        format.SubFormat = hero_wasapi_float;
        status = IAudioClient_Initialize(client, AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK | HERO_WASAPI_AUTOCONVERT | HERO_WASAPI_SRC_QUALITY,
                                         1000000, 0, (WAVEFORMATEX *)&format, NULL);
        if (SUCCEEDED(status)) {
            status = IAudioClient_SetEventHandle(client, event);
        }
        if (SUCCEEDED(status)) {
            status = IAudioClient_GetBufferSize(client, &size);
        }
        if (SUCCEEDED(status)) {
            status = ws->direction == HERO_AUDIO_OUTPUT ? IAudioClient_GetService(client, &hero_wasapi_render_iid, (void **)&render)
                                                        : IAudioClient_GetService(client, &hero_wasapi_capture_iid, (void **)&capture);
        }
        if (SUCCEEDED(status)) {
            status = IAudioClient_Start(client);
        }
        if (FAILED(status)) {
            hero_wasapi_tell(ws, status == E_ACCESSDENIED && ws->direction == HERO_AUDIO_INPUT ? HERO_AUDIO_DENIED : HERO_AUDIO_OTHER,
                             status == E_ACCESSDENIED && ws->direction == HERO_AUDIO_INPUT
                                 ? "starting the capture (Settings, Privacy, Microphone may refuse it to desktop programs)"
                                 : "starting the stream",
                             status);
        }
    }
    bool serve = ws->kind == 0;

    (void)SetEvent(ws->ready);
    if (serve) {
        hero_wasapi_serve(ws, client, render, capture, event, size);
    }
    if (render != NULL) {
        IAudioRenderClient_Release(render);
    }
    if (capture != NULL) {
        IAudioCaptureClient_Release(capture);
    }
    if (client != NULL) {
        IAudioClient_Release(client);
    }
    if (device != NULL) {
        IMMDevice_Release(device);
    }
    if (e != NULL) {
        IMMDeviceEnumerator_Release(e);
    }
    if (event != NULL) {
        (void)CloseHandle(event);
    }
    hero_wasapi_leave(began);
    return 0;
}

static void hero_wasapi_free(hero_wasapi_stream *ws) {
    if (ws->ready != NULL) {
        (void)CloseHandle(ws->ready);
    }
    if (ws->wake != NULL) {
        (void)CloseHandle(ws->wake);
    }
    hero_layer_free(ws->device);
    hero_layer_free(ws);
}

int64_t hero_audio_arm_open(hero_audio_core *st, const char *device, int64_t direction, int64_t rate, int64_t channels, void **arm) {
    hero_wasapi_stream *ws = hero_layer_alloc_zero(1, sizeof *ws);

    if (ws == NULL) {
        return hero_audio_arm_fail(HERO_AUDIO_LIMIT, 0, "no memory for the stream");
    }
    ws->st = st;
    ws->direction = direction;
    ws->rate = rate;
    ws->channels = channels;
    ws->device = device == NULL ? NULL : hero_wasapi_wide(device);
    ws->ready = CreateEventW(NULL, TRUE, FALSE, NULL);
    ws->wake = CreateEventW(NULL, FALSE, FALSE, NULL);
    if ((device != NULL && ws->device == NULL) || ws->ready == NULL || ws->wake == NULL) {
        hero_wasapi_free(ws);
        return hero_audio_arm_fail(device != NULL ? HERO_AUDIO_NOT_FOUND : HERO_AUDIO_LIMIT, 0, "no device of that identifier, or no event for the stream");
    }
    ws->thread = CreateThread(NULL, 0, hero_wasapi_run, ws, 0, NULL);
    if (ws->thread == NULL) {
        hero_wasapi_free(ws);
        return hero_audio_arm_fail(HERO_AUDIO_LIMIT, (int64_t)GetLastError(), "no thread for the stream");
    }
    (void)WaitForSingleObject(ws->ready, INFINITE);
    if (ws->kind != 0) {
        int64_t kind = ws->kind;
        int64_t os = ws->os;
        char words[192];

        memcpy(words, ws->words, sizeof words);
        (void)WaitForSingleObject(ws->thread, INFINITE);
        (void)CloseHandle(ws->thread);
        hero_wasapi_free(ws);
        return hero_audio_arm_fail(kind, os, words);
    }
    *arm = ws;
    return 0;
}

int64_t hero_audio_arm_pause(void *arm) {
    hero_wasapi_stream *ws = arm;

    atomic_store(&ws->paused, true);
    (void)SetEvent(ws->wake);
    return 0;
}

int64_t hero_audio_arm_resume(void *arm) {
    hero_wasapi_stream *ws = arm;

    atomic_store(&ws->paused, false);
    (void)SetEvent(ws->wake);
    return 0;
}

void hero_audio_arm_stop(void *arm) {
    hero_wasapi_stream *ws = arm;

    atomic_store(&ws->stopping, true);
    (void)SetEvent(ws->wake);
}

void hero_audio_arm_close(void *arm) {
    hero_wasapi_stream *ws = arm;

    atomic_store(&ws->closing, true);
    (void)SetEvent(ws->wake);
    (void)WaitForSingleObject(ws->thread, INFINITE);
    (void)CloseHandle(ws->thread);
    hero_wasapi_free(ws);
}

int64_t hero_audio_arm_permission(int64_t limit_ns) {
    (void)limit_ns;
    return 1;
}

int64_t hero_audio_arm_beep(void) {
    if (!MessageBeep(MB_OK)) {
        return hero_audio_arm_fail(HERO_AUDIO_OTHER, (int64_t)GetLastError(), "MessageBeep refused");
    }
    return 0;
}

#endif
