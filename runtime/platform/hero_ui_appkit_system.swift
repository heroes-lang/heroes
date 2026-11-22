/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_appkit_system.swift: the macOS arm of `hero_ui.h`'s calls
 * on the system around the program rather than on a widget: a
 * notification of the system's. A Swift member of its own, compiled as a
 * module of its own (`selfhost/cli/layer_swift.hero`), reading none of
 * `hero_ui_appkit.swift`'s widgets.
 *
 * A NOTIFICATION NEEDS AN APPLICATION BUNDLE. `UNUserNotificationCenter`
 * serves an application by its bundle identifier, and its `current()` traps
 * in a process that has none (a program `heroes build` writes, run from a
 * shell), so this member asks `Bundle.main.bundleIdentifier` first and
 * answers `HERO_UI_UNSUPPORTED` with words where there is none. With one,
 * the person's leave is asked once, as the system asks it, and a
 * notification the person has not allowed is held back by the system,
 * which tells the program nothing.
 *
 * EVERY CALL IS ON THE MAIN THREAD, and every failure is a value: no `!`,
 * no `try!`; each function answers a kind of `hero_ui.h`, and `words` gives
 * the words of the last `HERO_UI_OTHER` or `HERO_UI_UNSUPPORTED`.
 *
 * Written from Apple's documentation; no other library's source.
 */

import AppKit
import UserNotifications

private let answerOk: Int64 = 0
private let answerUnsupported: Int64 = 4

@MainActor
private var said = ""

// The text of `count` bytes at `bytes`.
private func textOf(_ bytes: UnsafePointer<UInt8>?, _ count: Int64) -> String {
    guard let bytes = bytes, count > 0 else {
        return ""
    }
    return String(decoding: UnsafeBufferPointer(start: bytes, count: Int(count)), as: UTF8.self)
}

@c
public func hero_ui_appkit_system_words(_ words: UnsafeMutablePointer<CChar>?, _ count: Int64) -> Int64 {
    guard let words = words, count > 0 else {
        return 0
    }
    let bytes = MainActor.assumeIsolated { () -> [UInt8] in
        return Array(said.utf8)
    }
    var length = min(bytes.count, Int(count) - 1)
    while length > 0 && length < bytes.count && (bytes[length] & 0xC0) == 0x80 {
        length -= 1
    }
    for at in 0..<length {
        words[at] = CChar(bitPattern: bytes[at])
    }
    words[length] = 0
    return Int64(length)
}

// Shows a notification of the system's, `title` and `text`: answered at
// once, the system showing it once the person's leave is known.
@c
public func hero_ui_appkit_notify(_ title: UnsafePointer<UInt8>?, _ titleCount: Int64, _ text: UnsafePointer<UInt8>?, _ textCount: Int64) -> Int64 {
    let heading = textOf(title, titleCount)
    let words = textOf(text, textCount)
    return MainActor.assumeIsolated { () -> Int64 in
        guard Bundle.main.bundleIdentifier != nil else {
            said = "macOS shows a notification only for an application bundle, and this program runs outside one, with no bundle identifier"
            return answerUnsupported
        }
        UNUserNotificationCenter.current().requestAuthorization(options: [.alert, .sound]) { granted, _ in
            guard granted else {
                return
            }
            let content = UNMutableNotificationContent()
            content.title = heading
            content.body = words
            let request = UNNotificationRequest(identifier: UUID().uuidString, content: content, trigger: nil)
            UNUserNotificationCenter.current().add(request, withCompletionHandler: nil)
        }
        return answerOk
    }
}
