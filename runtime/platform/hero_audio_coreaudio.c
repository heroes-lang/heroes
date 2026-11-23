/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_audio_coreaudio.c — the arm of sound on Apple's systems
 * (`platform/hero_audio_arm.h`), by CoreAudio, in C (M-rebel-library,
 * 2026-11-23). Empty elsewhere.
 *
 * THE DEVICES are the HAL's audio objects (`kAudioHardwarePropertyDevices`):
 * a device is an output where its output scope has channels and an input
 * where its input scope has, so one device may be both and is then listed
 * twice; its identifier is its UID, its name its object name, its rate its
 * nominal rate, its channels those of its streams in that scope, and its
 * encoding `float32`, the HAL's own.
 *
 * A STREAM is an AudioQueue of interleaved 32-bit floats at the stream's rate
 * and channels, on the default device or the one whose UID it names
 * (`kAudioQueueProperty_CurrentDevice`); the queue converts to the device's
 * own format. Its callbacks run on the queue's own thread: an output's pulls
 * frames from the core into the buffer it hands back, three buffers of 20 ms
 * each, and an input's pushes what it was given. A finished output, once the
 * core has handed its last frame, is stopped without dropping what the
 * queue holds (`AudioQueueStop` with `false`), and its end is told when the
 * queue says it no longer runs (`kAudioQueueProperty_IsRunning`); a queue
 * that stops running without having been asked has lost its device.
 *
 * THE MICROPHONE is the person's to allow (macOS 10.14 and later): an input
 * is opened only where `AVCaptureDevice`'s authorization for audio reads
 * authorized, since an input queue the person has not allowed records
 * silence and says nothing. AVFoundation is opened by `dlopen` at the first
 * ask, so a program that never records links none of it, and its class is
 * reached through the Objective-C runtime's C functions; the request's
 * completion only stores the answer, which the waiting call reads again.
 *
 * THE BEEP is `AudioServicesPlayAlertSound` of the person's alert sound.
 */

#include "hero_audio_coreaudio.h"

#include <stdint.h>

#if defined(__APPLE__)
#include <AudioToolbox/AudioToolbox.h>
#include <CoreAudio/CoreAudio.h>
#include <CoreFoundation/CoreFoundation.h>
#include <dlfcn.h>
#include <objc/message.h>
#include <objc/runtime.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "hero_layer_alloc.h"

/* The frameworks the arm calls, named in the object, so that the linker adds
 * them to a program that takes this member and to no other. */
__asm__(".linker_option \"-framework\", \"AudioToolbox\"");
__asm__(".linker_option \"-framework\", \"CoreAudio\"");
__asm__(".linker_option \"-framework\", \"CoreFoundation\"");
__asm__(".linker_option \"-lobjc\"");

/* The buffers a queue cycles, and the frames of each a fiftieth of a second. */
#define HERO_AUDIO_CA_BUFFERS 3
#define HERO_AUDIO_CA_PER_SECOND 50

typedef struct {
    hero_audio_core *st;
    AudioQueueRef queue;
    AudioQueueBufferRef buffers[HERO_AUDIO_CA_BUFFERS];
    int64_t direction;
    uint32_t frame_bytes;
    uint32_t frames;
    /* Set by the program's side before the queue is told, read by the queue's
     * thread: what the next callback and the running listener may do. */
    _Atomic bool closing;
    _Atomic bool draining;
    _Atomic bool stopping;
    _Atomic bool paused;
} hero_audio_ca;

/* The answer the microphone's request completed with, 0 while none came, 1
 * allowed, 2 refused: written by the request's completion on a thread of the
 * system's, read by the call that waits for it. */
static _Atomic int hero_audio_ca_asked;

/* The words of `status`, a CoreAudio call's failure, with what was asked. */
static int64_t hero_audio_ca_fail(int64_t kind, const char *asked, OSStatus status) {
    char words[160];
    uint32_t code = (uint32_t)status;
    char four[5] = {(char)(code >> 24), (char)(code >> 16), (char)(code >> 8), (char)code, 0};
    bool printable = true;

    for (int i = 0; i < 4; i += 1) {
        if (four[i] < 32 || four[i] > 126) {
            printable = false;
        }
    }
    if (printable) {
        (void)snprintf(words, sizeof words, "%s: CoreAudio answered %d ('%s')", asked, (int)status, four);
    } else {
        (void)snprintf(words, sizeof words, "%s: CoreAudio answered %d", asked, (int)status);
    }
    return hero_audio_arm_fail(kind, (int64_t)status, words);
}

/* A property of the HAL object `object` read whole into a block of the
 * layer's, its size in `*size`; NULL where the HAL answered otherwise. */
static void *hero_audio_ca_property(AudioObjectID object, AudioObjectPropertySelector selector, AudioObjectPropertyScope scope, UInt32 *size) {
    AudioObjectPropertyAddress at = {selector, scope, 0};
    UInt32 bytes = 0;

    if (AudioObjectGetPropertyDataSize(object, &at, 0, NULL, &bytes) != noErr || bytes == 0) {
        return NULL;
    }
    void *block = hero_layer_alloc(bytes);

    if (block == NULL) {
        return NULL;
    }
    if (AudioObjectGetPropertyData(object, &at, 0, NULL, &bytes, block) != noErr) {
        hero_layer_free(block);
        return NULL;
    }
    *size = bytes;
    return block;
}

/* The UTF-8 of `text` in a block of the layer's, or NULL. */
static char *hero_audio_ca_utf8(CFStringRef text) {
    if (text == NULL) {
        return NULL;
    }
    CFIndex most = CFStringGetMaximumSizeForEncoding(CFStringGetLength(text), kCFStringEncodingUTF8) + 1;
    char *out = hero_layer_alloc((size_t)most);

    if (out != NULL && !CFStringGetCString(text, out, most, kCFStringEncodingUTF8)) {
        hero_layer_free(out);
        return NULL;
    }
    return out;
}

/* A CFString property of `device` as UTF-8, or NULL. */
static char *hero_audio_ca_text(AudioObjectID device, AudioObjectPropertySelector selector) {
    AudioObjectPropertyAddress at = {selector, kAudioObjectPropertyScopeGlobal, 0};
    CFStringRef text = NULL;
    UInt32 size = sizeof text;

    if (AudioObjectGetPropertyData(device, &at, 0, NULL, &size, &text) != noErr || text == NULL) {
        return NULL;
    }
    char *out = hero_audio_ca_utf8(text);

    CFRelease(text);
    return out;
}

/* The channels `device` has in `scope`, 0 where none. */
static int64_t hero_audio_ca_channels(AudioObjectID device, AudioObjectPropertyScope scope) {
    UInt32 size = 0;
    AudioBufferList *list = hero_audio_ca_property(device, kAudioDevicePropertyStreamConfiguration, scope, &size);
    int64_t channels = 0;

    if (list != NULL) {
        for (UInt32 i = 0; i < list->mNumberBuffers; i += 1) {
            channels += list->mBuffers[i].mNumberChannels;
        }
        hero_layer_free(list);
    }
    return channels;
}

/* The default device of `direction`, `kAudioObjectUnknown` where none. */
static AudioObjectID hero_audio_ca_default(int64_t direction) {
    AudioObjectPropertyAddress at = {
        direction == HERO_AUDIO_INPUT ? kAudioHardwarePropertyDefaultInputDevice : kAudioHardwarePropertyDefaultOutputDevice,
        kAudioObjectPropertyScopeGlobal, 0};
    AudioObjectID device = kAudioObjectUnknown;
    UInt32 size = sizeof device;

    if (AudioObjectGetPropertyData(kAudioObjectSystemObject, &at, 0, NULL, &size, &device) != noErr) {
        return kAudioObjectUnknown;
    }
    return device;
}

int64_t hero_audio_arm_system(char *buffer, size_t count) {
    static const char name[] = "CoreAudio";

    if (sizeof name - 1 <= count) {
        memcpy(buffer, name, sizeof name - 1);
    }
    return (int64_t)(sizeof name - 1);
}

int64_t hero_audio_arm_devices(hero_audio_list *list) {
    UInt32 size = 0;
    AudioObjectID *devices = hero_audio_ca_property(kAudioObjectSystemObject, kAudioHardwarePropertyDevices, kAudioObjectPropertyScopeGlobal, &size);

    if (devices == NULL) {
        return 0;
    }
    AudioObjectID defaults[2] = {hero_audio_ca_default(HERO_AUDIO_OUTPUT), hero_audio_ca_default(HERO_AUDIO_INPUT)};
    int64_t answer = 0;

    for (UInt32 i = 0; i < size / sizeof *devices && answer == 0; i += 1) {
        char *id = hero_audio_ca_text(devices[i], kAudioDevicePropertyDeviceUID);
        char *name = hero_audio_ca_text(devices[i], kAudioObjectPropertyName);
        Float64 rate = 0;
        AudioObjectPropertyAddress at = {kAudioDevicePropertyNominalSampleRate, kAudioObjectPropertyScopeGlobal, 0};
        UInt32 rate_size = sizeof rate;

        (void)AudioObjectGetPropertyData(devices[i], &at, 0, NULL, &rate_size, &rate);
        for (int d = 0; d < 2 && answer == 0 && id != NULL && name != NULL; d += 1) {
            int64_t direction = d == 0 ? HERO_AUDIO_OUTPUT : HERO_AUDIO_INPUT;
            int64_t channels = hero_audio_ca_channels(devices[i], d == 0 ? kAudioObjectPropertyScopeOutput : kAudioObjectPropertyScopeInput);

            if (channels > 0) {
                answer = hero_audio_list_add(list, direction, devices[i] == defaults[d], id, name, (int64_t)(rate + 0.5), channels, "float32");
            }
        }
        hero_layer_free(id);
        hero_layer_free(name);
    }
    hero_layer_free(devices);
    return answer;
}

/* Whether a device with UID `uid` has channels in `direction`. */
static bool hero_audio_ca_has(const char *uid, int64_t direction) {
    UInt32 size = 0;
    AudioObjectID *devices = hero_audio_ca_property(kAudioObjectSystemObject, kAudioHardwarePropertyDevices, kAudioObjectPropertyScopeGlobal, &size);
    bool found = false;

    for (UInt32 i = 0; devices != NULL && i < size / sizeof *devices && !found; i += 1) {
        char *id = hero_audio_ca_text(devices[i], kAudioDevicePropertyDeviceUID);

        found = id != NULL && strcmp(id, uid) == 0 &&
                hero_audio_ca_channels(devices[i], direction == HERO_AUDIO_INPUT ? kAudioObjectPropertyScopeInput : kAudioObjectPropertyScopeOutput) > 0;
        hero_layer_free(id);
    }
    hero_layer_free(devices);
    return found;
}

/* AVFoundation's class and media type, found once. */
static pthread_once_t hero_audio_ca_once = PTHREAD_ONCE_INIT;
static Class hero_audio_ca_capture;
static id hero_audio_ca_media;

static void hero_audio_ca_find(void) {
    void *framework = dlopen("/System/Library/Frameworks/AVFoundation.framework/AVFoundation", RTLD_NOW | RTLD_LOCAL);

    if (framework == NULL) {
        return;
    }
    id *media = dlsym(framework, "AVMediaTypeAudio");

    hero_audio_ca_media = media == NULL ? NULL : *media;
    hero_audio_ca_capture = objc_getClass("AVCaptureDevice");
}

/* The microphone's authorization: 0 not asked, 1 restricted, 2 refused, 3
 * allowed, -1 where AVFoundation could not be reached. */
static long hero_audio_ca_status(void) {
    (void)pthread_once(&hero_audio_ca_once, hero_audio_ca_find);
    if (hero_audio_ca_capture == NULL || hero_audio_ca_media == NULL) {
        return -1;
    }
    return ((long (*)(Class, SEL, id))objc_msgSend)(hero_audio_ca_capture, sel_registerName("authorizationStatusForMediaType:"), hero_audio_ca_media);
}

/* The failure an input meets where the microphone is not allowed, by `status`. */
static int64_t hero_audio_ca_refused(long status) {
    if (status == 0) {
        return hero_audio_arm_fail(HERO_AUDIO_DENIED, 0, "the person has not been asked for the microphone yet: hero_audio_permission asks them");
    }
    if (status == 1) {
        return hero_audio_arm_fail(HERO_AUDIO_DENIED, 1, "a policy of this Mac forbids this program the microphone");
    }
    if (status == 2) {
        return hero_audio_arm_fail(HERO_AUDIO_DENIED, 2, "the person refused this program the microphone: System Settings, Privacy & Security, Microphone gives it back");
    }
    return hero_audio_arm_fail(HERO_AUDIO_OTHER, status, "AVFoundation, which tells whether the microphone is allowed, could not be reached");
}

int64_t hero_audio_arm_permission(int64_t limit_ns) {
    long status = hero_audio_ca_status();

    if (status == 3) {
        return 1;
    }
    if (status != 0 || limit_ns == 0) {
        return hero_audio_ca_refused(status);
    }
    atomic_store(&hero_audio_ca_asked, 0);
    /* A block with no capture is a global one: nothing it holds outlives the
     * call that made it. */
    void (^answered)(signed char) = ^(signed char granted) {
      atomic_store(&hero_audio_ca_asked, granted ? 1 : 2);
    };
    ((void (*)(Class, SEL, id, void (^)(signed char)))objc_msgSend)(hero_audio_ca_capture, sel_registerName("requestAccessForMediaType:completionHandler:"), hero_audio_ca_media, answered);
    struct timespec step = {0, 10000000};
    int64_t waited = 0;

    while (atomic_load(&hero_audio_ca_asked) == 0 && waited < limit_ns) {
        if (hero_net_stop_asked() != 0) {
            return hero_audio_arm_fail(HERO_AUDIO_STOPPED, 0, "the program was asked to stop while the person was asked for the microphone");
        }
        (void)nanosleep(&step, NULL);
        waited += 10000000;
    }
    status = hero_audio_ca_status();
    if (status == 3) {
        return 1;
    }
    if (status == 0) {
        return hero_audio_arm_fail(HERO_AUDIO_TIMEOUT, 0, "the person was asked for the microphone and had not answered when the limit passed");
    }
    return hero_audio_ca_refused(status);
}

int64_t hero_audio_arm_beep(void) {
    AudioServicesPlayAlertSound(kSystemSoundID_UserPreferredAlert);
    return 0;
}

/* The queue's thread: an output's buffer handed back, filled again. */
static void hero_audio_ca_out(void *given, AudioQueueRef queue, AudioQueueBufferRef buffer) {
    hero_audio_ca *ca = given;

    if (atomic_load(&ca->closing) || atomic_load(&ca->stopping) || atomic_load(&ca->draining)) {
        return;
    }
    (void)hero_audio_pull(ca->st, buffer->mAudioData, ca->frames);
    buffer->mAudioDataByteSize = ca->frames * ca->frame_bytes;
    (void)AudioQueueEnqueueBuffer(queue, buffer, 0, NULL);
    if (hero_audio_drained(ca->st)) {
        atomic_store(&ca->draining, true);
        (void)AudioQueueStop(queue, false);
    }
}

/* The queue's thread: an input's buffer of recorded frames, kept and handed
 * back. */
static void hero_audio_ca_in(void *given, AudioQueueRef queue, AudioQueueBufferRef buffer, const AudioTimeStamp *start, UInt32 packets, const AudioStreamPacketDescription *described) {
    hero_audio_ca *ca = given;

    (void)start;
    (void)packets;
    (void)described;
    if (atomic_load(&ca->closing) || atomic_load(&ca->stopping)) {
        return;
    }
    (void)hero_audio_push(ca->st, buffer->mAudioData, buffer->mAudioDataByteSize / ca->frame_bytes);
    (void)AudioQueueEnqueueBuffer(queue, buffer, 0, NULL);
}

/* The queue started or stopped running. */
static void hero_audio_ca_running(void *given, AudioQueueRef queue, AudioQueuePropertyID property) {
    hero_audio_ca *ca = given;
    UInt32 running = 1;
    UInt32 size = sizeof running;

    (void)property;
    if (atomic_load(&ca->closing) || AudioQueueGetProperty(queue, kAudioQueueProperty_IsRunning, &running, &size) != noErr || running != 0) {
        return;
    }
    if (atomic_load(&ca->draining)) {
        hero_audio_played_out(ca->st);
    } else if (!atomic_load(&ca->stopping) && !atomic_load(&ca->paused)) {
        hero_audio_lost(ca->st, 0, "the queue stopped running: its device went away");
    }
}

int64_t hero_audio_arm_open(hero_audio_core *st, const char *device, int64_t direction, int64_t rate, int64_t channels, void **arm) {
    if (direction == HERO_AUDIO_INPUT) {
        long status = hero_audio_ca_status();

        if (status != 3) {
            return hero_audio_ca_refused(status);
        }
    }
    if (device == NULL ? hero_audio_ca_default(direction) == kAudioObjectUnknown : !hero_audio_ca_has(device, direction)) {
        return hero_audio_arm_fail(HERO_AUDIO_NOT_FOUND, 0, device == NULL ? "this Mac has no default device of that direction" : "no device of that identifier has channels of that direction");
    }
    hero_audio_ca *ca = hero_layer_alloc_zero(1, sizeof *ca);

    if (ca == NULL) {
        return hero_audio_arm_fail(HERO_AUDIO_LIMIT, 0, "no memory for the queue");
    }
    ca->st = st;
    ca->direction = direction;
    ca->frame_bytes = (uint32_t)(4 * channels);
    ca->frames = (uint32_t)((rate + HERO_AUDIO_CA_PER_SECOND - 1) / HERO_AUDIO_CA_PER_SECOND);
    AudioStreamBasicDescription format = {0};

    format.mSampleRate = (Float64)rate;
    format.mFormatID = kAudioFormatLinearPCM;
    format.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
    format.mBytesPerPacket = ca->frame_bytes;
    format.mFramesPerPacket = 1;
    format.mBytesPerFrame = ca->frame_bytes;
    format.mChannelsPerFrame = (UInt32)channels;
    format.mBitsPerChannel = 32;
    OSStatus status = direction == HERO_AUDIO_OUTPUT ? AudioQueueNewOutput(&format, hero_audio_ca_out, ca, NULL, NULL, 0, &ca->queue)
                                                     : AudioQueueNewInput(&format, hero_audio_ca_in, ca, NULL, NULL, 0, &ca->queue);

    if (status != noErr) {
        hero_layer_free(ca);
        return hero_audio_ca_fail(HERO_AUDIO_OTHER, "making the audio queue", status);
    }
    const char *asked = "choosing the queue's device";

    if (device != NULL) {
        CFStringRef uid = CFStringCreateWithCString(NULL, device, kCFStringEncodingUTF8);

        status = uid == NULL ? kAudio_ParamError : AudioQueueSetProperty(ca->queue, kAudioQueueProperty_CurrentDevice, &uid, sizeof uid);
        if (uid != NULL) {
            CFRelease(uid);
        }
    }
    if (status == noErr) {
        asked = "listening to the queue's running";
        status = AudioQueueAddPropertyListener(ca->queue, kAudioQueueProperty_IsRunning, hero_audio_ca_running, ca);
    }
    for (int i = 0; i < HERO_AUDIO_CA_BUFFERS && status == noErr; i += 1) {
        asked = "making the queue's buffers";
        status = AudioQueueAllocateBuffer(ca->queue, ca->frames * ca->frame_bytes, &ca->buffers[i]);
        if (status == noErr && direction == HERO_AUDIO_OUTPUT) {
            memset(ca->buffers[i]->mAudioData, 0, ca->frames * ca->frame_bytes);
            ca->buffers[i]->mAudioDataByteSize = ca->frames * ca->frame_bytes;
        }
        if (status == noErr) {
            status = AudioQueueEnqueueBuffer(ca->queue, ca->buffers[i], 0, NULL);
        }
    }
    if (status == noErr) {
        asked = "starting the queue";
        status = AudioQueueStart(ca->queue, NULL);
    }
    if (status != noErr) {
        atomic_store(&ca->closing, true);
        (void)AudioQueueDispose(ca->queue, true);
        hero_layer_free(ca);
        return hero_audio_ca_fail(HERO_AUDIO_OTHER, asked, status);
    }
    *arm = ca;
    return 0;
}

int64_t hero_audio_arm_pause(void *arm) {
    hero_audio_ca *ca = arm;

    atomic_store(&ca->paused, true);
    OSStatus status = AudioQueuePause(ca->queue);

    return status == noErr ? 0 : hero_audio_ca_fail(HERO_AUDIO_OTHER, "pausing the queue", status);
}

int64_t hero_audio_arm_resume(void *arm) {
    hero_audio_ca *ca = arm;
    OSStatus status = AudioQueueStart(ca->queue, NULL);

    if (status == noErr) {
        atomic_store(&ca->paused, false);
    }
    return status == noErr ? 0 : hero_audio_ca_fail(HERO_AUDIO_OTHER, "resuming the queue", status);
}

void hero_audio_arm_stop(void *arm) {
    hero_audio_ca *ca = arm;

    atomic_store(&ca->stopping, true);
    (void)AudioQueueStop(ca->queue, true);
}

void hero_audio_arm_close(void *arm) {
    hero_audio_ca *ca = arm;

    atomic_store(&ca->closing, true);
    (void)AudioQueueStop(ca->queue, true);
    /* Synchronous: no callback runs once it returns. */
    (void)AudioQueueDispose(ca->queue, true);
    hero_layer_free(ca);
}

#endif
