/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_audio_coreaudio.h: the header of `platform/hero_audio_coreaudio.c`,
 * the arm of sound by CoreAudio on Apple's systems. Its functions are THE
 * ARM of `platform/hero_audio_arm.h`, which every system's arm defines and
 * `hero_audio.c` alone calls; this header names them for the member, so it
 * is compiled against their one prototype. No program includes it.
 */

#ifndef HERO_AUDIO_COREAUDIO_H
#define HERO_AUDIO_COREAUDIO_H

#include "platform/hero_audio_arm.h"

#endif
