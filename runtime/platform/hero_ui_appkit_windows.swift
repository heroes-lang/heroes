/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_appkit_windows.swift: the macOS arm of `hero_ui.c`'s calls
 * on a window as the system shows it, by AppKit: a key pressed as the
 * person's keyboard presses it. A Swift member of its own, compiled as a
 * module of its own (`selfhost/cli/layer_swift.hero`), so it reads none of
 * `hero_ui_appkit.swift`'s widgets: `hero_ui.c` asks that member for the
 * AppKit object a call acts on (`hero_ui_appkit_object`) and hands it here
 * within the same call, on the main thread, while the widget stands.
 *
 * EVERY CALL IS ON THE MAIN THREAD, as in `hero_ui_appkit.swift`, and every
 * failure is a value: no `!`, no `try!`, no index past an end; each function
 * answers a kind of `hero_ui.h`, and `words` gives the words of the last
 * `HERO_UI_OTHER`.
 *
 * Written from Apple's AppKit documentation; no other library's source.
 */

import AppKit

// The kinds of failure, `hero_ui.h`'s numbers.
private let answerOk: Int64 = 0
private let answerInvalid: Int64 = 1
private let answerNotFound: Int64 = 6
private let answerOther: Int64 = 8

// The keys held, `hero_ui.h`'s numbers.
private let heldPrimary: Int64 = 1
private let heldShift: Int64 = 2
private let heldAlt: Int64 = 4

// The words of the last `HERO_UI_OTHER` this member answered.
@MainActor
private var said = ""

// The longest a key press waits for the system to make its window the key
// window, in seconds: activation is asked of the window server, which
// answers through the event queue.
private let keyWait: Double = 2

// The virtual key code of each named key `hero_ui_set_shortcut` takes:
// HIToolbox's `Events.h`, `kVK_*`, as the macOS 27.2 SDK holds them (read
// 2026-11-22). These keys stand at one code on every layout.
private let namedCodes: [String: UInt16] = [
    "Enter": 0x24, "Tab": 0x30, "Space": 0x31, "Backspace": 0x33, "Escape": 0x35, "Delete": 0x75,
    "Home": 0x73, "End": 0x77, "PageUp": 0x74, "PageDown": 0x79,
    "Left": 0x7B, "Right": 0x7C, "Down": 0x7D, "Up": 0x7E,
    "F1": 0x7A, "F2": 0x78, "F3": 0x63, "F4": 0x76, "F5": 0x60, "F6": 0x61,
    "F7": 0x62, "F8": 0x64, "F9": 0x65, "F10": 0x6D, "F11": 0x67, "F12": 0x6F,
]

// The key event a person's keyboard makes for virtual key `code` going
// down or up with `flags` held: made by Core Graphics and read by AppKit,
// so its characters are the ones the keyboard layout in use types (Shift
// with `1` is `!` on a US layout, and Command with Shift and `z` keeps `z`
// as its characters and `Z` as those ignoring the modifiers), the event
// AppKit's menus and text views read a real key by. Core Graphics leaves a
// made event's time at 0, which no key a person presses has, so it is given
// the time since the system started, as the keyboard's are.
private func keyEvent(_ code: UInt16, down: Bool, flags: CGEventFlags) -> NSEvent? {
    guard let made = CGEvent(keyboardEventSource: nil, virtualKey: CGKeyCode(code), keyDown: down) else {
        return nil
    }
    made.flags = flags
    made.timestamp = CGEventTimestamp(clock_gettime_nsec_np(CLOCK_UPTIME_RAW))
    return NSEvent(cgEvent: made)
}

// The virtual key code of the key `name` names: a named key's own, or the
// code of the key that types the letter or the digit `name` on the layout
// in use with nothing held, asked of the layout key by key, so a letter is
// found where the layout puts it (an AZERTY layout types `a` where QWERTY
// types `q`). Nil for a name `hero_ui_set_shortcut` does not take, or a
// character no key of the layout types.
private func codeOf(_ name: String) -> UInt16? {
    if let code = namedCodes[name] {
        return code
    }
    guard name.utf8.count == 1, let c = name.utf8.first, (c >= 0x61 && c <= 0x7A) || (c >= 0x30 && c <= 0x39) else {
        return nil
    }
    for code in UInt16(0)..<UInt16(128) {
        if keyEvent(code, down: true, flags: [])?.charactersIgnoringModifiers == name {
            return code
        }
    }
    return nil
}

// The text of `count` bytes at `bytes`, which `hero_ui.c` checked as UTF-8.
private func text(_ bytes: UnsafePointer<UInt8>?, _ count: Int64) -> String {
    guard let bytes = bytes, count > 0 else {
        return ""
    }
    return String(decoding: UnsafeBufferPointer(start: bytes, count: Int(count)), as: UTF8.self)
}

// Makes `window` the key window of the active application, running the
// system's loop until it is or `keyWait` passes: the system hands a key to
// the key window alone. Events the loop takes are sent on as `pump` sends
// them, so none is lost.
@MainActor
private func madeKey(_ window: NSWindow) -> Bool {
    if !window.isVisible {
        return false
    }
    NSApp.activate()
    window.makeKeyAndOrderFront(nil)
    let deadline = Date(timeIntervalSinceNow: keyWait)
    while !window.isKeyWindow && Date() < deadline {
        if let event = NSApp.nextEvent(matching: .any, until: Date(timeIntervalSinceNow: 0.01), inMode: .default, dequeue: true) {
            NSApp.sendEvent(event)
        }
    }
    return window.isKeyWindow
}

@c
public func hero_ui_appkit_windows_words(_ words: UnsafeMutablePointer<CChar>?, _ count: Int64) -> Int64 {
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

// Presses the key `key` with `modifiers` held in `window`, `typed` first
// given the keys where it is not nil: the key goes down and up through
// `NSApp.sendEvent`, so the main menu's key equivalents, the Edit menu's
// among them, and then the first responder take it as a person's.
@c
public func hero_ui_appkit_press_key(_ window: UnsafeMutableRawPointer?, _ typed: UnsafeMutableRawPointer?, _ key: UnsafePointer<UInt8>?, _ count: Int64, _ modifiers: Int64) -> Int64 {
    guard let window = window else {
        return answerNotFound
    }
    let name = text(key, count)
    return MainActor.assumeIsolated { () -> Int64 in
        let shown = Unmanaged<NSWindow>.fromOpaque(window).takeUnretainedValue()
        guard let code = codeOf(name) else {
            return answerInvalid
        }
        var flags: CGEventFlags = []
        if modifiers & heldPrimary != 0 {
            flags.insert(.maskCommand)
        }
        if modifiers & heldShift != 0 {
            flags.insert(.maskShift)
        }
        if modifiers & heldAlt != 0 {
            flags.insert(.maskAlternate)
        }
        if !madeKey(shown) {
            if !shown.isVisible {
                said = "the window is hidden, and the system gives keys to a window it shows alone"
            } else {
                said = "the system kept \(NSWorkspace.shared.frontmostApplication?.localizedName ?? "another application") in front and would not make this window the key window"
            }
            return answerOther
        }
        if let typed = typed {
            let responder = Unmanaged<NSResponder>.fromOpaque(typed).takeUnretainedValue()
            if shown.firstResponder !== responder && !(shown.firstResponder is NSTextView && (shown.firstResponder as? NSTextView)?.delegate === responder) {
                guard responder.acceptsFirstResponder, shown.makeFirstResponder(responder) else {
                    return answerInvalid
                }
            }
        }
        for down in [true, false] {
            guard let event = keyEvent(code, down: down, flags: flags) else {
                said = "the system made no key event for \"\(name)\""
                return answerOther
            }
            NSApp.sendEvent(event)
        }
        return answerOk
    }
}
