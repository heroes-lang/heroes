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

// How long a window that has just become key is left to the system before
// a key is pressed in it, in seconds.
private let keySettle: Double = 0.15

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
    if window.isKeyWindow && NSApp.isActive {
        return true
    }
    NSApp.activate()
    window.makeKeyAndOrderFront(nil)
    let deadline = Date(timeIntervalSinceNow: keyWait)
    while !window.isKeyWindow && Date() < deadline {
        if let event = NSApp.nextEvent(matching: .any, until: Date(timeIntervalSinceNow: 0.01), inMode: .default, dequeue: true) {
            NSApp.sendEvent(event)
        }
    }
    if !window.isKeyWindow {
        return false
    }
    // A window that has just become key is given its first key view by
    // AppKit in the events that follow, which selects a text field's whole
    // text; those events are run first, so the keys pressed after them go
    // where the call puts them.
    let settled = Date(timeIntervalSinceNow: keySettle)
    while Date() < settled {
        if let event = NSApp.nextEvent(matching: .any, until: settled, inMode: .default, dequeue: true) {
            NSApp.sendEvent(event)
        }
    }
    return true
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

// MARK: windows on the screen

// The kinds of failure and of event, and a window's states and backdrops,
// `hero_ui.h`'s numbers.
private let answerLimit: Int64 = 7
private let eventMove: Int64 = 10
private let eventState: Int64 = 11
private let eventScreen: Int64 = 12
private let stateNormal: Int64 = 1
private let stateMinimized: Int64 = 2
private let stateMaximized: Int64 = 3
private let stateFullScreen: Int64 = 4
private let backdropNone: Int64 = 0
private let backdropTranslucent: Int64 = 1

// The longest a change of a window's state waits for the system to make it,
// in seconds: minimizing, zooming and entering full screen are animated, and
// full screen moves the window to a space of its own, which a loaded machine
// takes seconds to finish.
private let stateWait: Double = 5

/// The function `hero_ui.c` queues an event by, as `hero_ui_appkit.swift`
/// declares it.
public typealias HeroUiWindowsPush = @convention(c) (Int64, Int64, Int64, Int64, UnsafePointer<UInt8>?, Int64) -> Void

// What this member keeps of each window it watches: its number, the
// observers it gave the notification centre, and the state last told.
@MainActor
private final class Watched {
    let id: Int64
    var observers: [NSObjectProtocol] = []
    var state: Int64 = 1
    var screen = 0

    init(id: Int64) {
        self.id = id
    }
}

// The windows watched, by the identity of their `NSWindow`; the function
// events are queued by; and how many program-set changes are under way,
// during which no event of this member is queued: what the program sets is
// never told back to it (`hero_ui.h`).
@MainActor
private var watched: [ObjectIdentifier: Watched] = [:]
@MainActor
private var pushing: HeroUiWindowsPush?
@MainActor
private var quiet = 0

// The window an opaque pointer of `hero_ui.c` names.
@MainActor
private func windowOf(_ pointer: UnsafeMutableRawPointer) -> NSWindow {
    return Unmanaged<NSWindow>.fromOpaque(pointer).takeUnretainedValue()
}

// THE DESKTOP'S POINTS. AppKit places a screen and a window in one space
// whose origin is the bottom-left corner of the first screen, the y axis up;
// `hero_ui.h` gives each position from the top-left corner of the screen it
// is on, the y axis down. The screens are `NSScreen.screens` in order, the
// first the one holding the menu bar.
@MainActor
private func screensNow() -> [NSScreen] {
    return NSScreen.screens
}

// The top of the first screen in AppKit's space, from which every y is
// turned over.
@MainActor
private func desktopTop() -> CGFloat {
    return screensNow().first?.frame.maxY ?? 0
}

// The index of the screen `window` stands on most, 0 where the system names
// none (a window wholly off every screen).
@MainActor
private func screenIndex(_ window: NSWindow) -> Int {
    guard let screen = window.screen else {
        return 0
    }
    return screensNow().firstIndex(of: screen) ?? 0
}

// `window`'s frame on the screen it stands on: that screen's index, and the
// frame's top-left corner from the screen's top-left corner, with its size.
@MainActor
private func frameOn(_ window: NSWindow) -> (Int64, Int64, Int64, Int64, Int64) {
    let index = screenIndex(window)
    let screens = screensNow()
    let frame = window.frame
    guard index < screens.count else {
        return (0, Int64(frame.minX.rounded()), Int64((desktopTop() - frame.maxY).rounded()), Int64(frame.width.rounded()), Int64(frame.height.rounded()))
    }
    let screen = screens[index].frame
    return (Int64(index), Int64((frame.minX - screen.minX).rounded()), Int64((screen.maxY - frame.maxY).rounded()), Int64(frame.width.rounded()), Int64(frame.height.rounded()))
}

// The state `window` stands in now, `hero_ui.h`'s `HERO_UI_STATE_*`.
@MainActor
private func stateOf(_ window: NSWindow) -> Int64 {
    if window.isMiniaturized {
        return stateMinimized
    }
    if window.styleMask.contains(.fullScreen) {
        return stateFullScreen
    }
    if window.isZoomed {
        return stateMaximized
    }
    return stateNormal
}

// Queues an event of this member where no program-set change is under way.
@MainActor
private func told(_ kind: Int64, _ id: Int64, _ value: Int64, _ second: Int64) {
    if quiet > 0 {
        return
    }
    pushing?(kind, id, value, second, nil, 0)
}

// What a watched window's notification tells: a move, its screen, its
// state. A state is told once each time it changes, a zoom being read on
// every resize since AppKit names no notification of its own for it.
@MainActor
private func noticed(_ window: NSWindow, _ name: Notification.Name) {
    guard let entry = watched[ObjectIdentifier(window)] else {
        return
    }
    // A new screen is told before the move that took the window there, so
    // the position a move carries is read on the screen last told.
    let index = screenIndex(window)
    if index != entry.screen {
        entry.screen = index
        told(eventScreen, entry.id, Int64(index), 0)
    }
    if name == NSWindow.didMoveNotification {
        let (_, x, y, _, _) = frameOn(window)
        told(eventMove, entry.id, x, y)
        return
    }
    if name == NSWindow.didChangeScreenNotification {
        return
    }
    let now = stateOf(window)
    if now != entry.state {
        entry.state = now
        told(eventState, entry.id, now, 0)
    }
}

// The notifications a watched window is observed by.
private let watchedNames: [Notification.Name] = [
    NSWindow.didMoveNotification,
    NSWindow.didChangeScreenNotification,
    NSWindow.didMiniaturizeNotification,
    NSWindow.didDeminiaturizeNotification,
    NSWindow.didEnterFullScreenNotification,
    NSWindow.didExitFullScreenNotification,
    NSWindow.didResizeNotification,
]

// Runs the system's loop until `done` holds or `stateWait` passes, sending
// on every event it takes as `pump` does; answers whether `done` holds.
@MainActor
private func waited(_ done: () -> Bool) -> Bool {
    let deadline = Date(timeIntervalSinceNow: stateWait)
    while !done() && Date() < deadline {
        if let event = NSApp.nextEvent(matching: .any, until: Date(timeIntervalSinceNow: 0.02), inMode: .default, dequeue: true) {
            NSApp.sendEvent(event)
        }
    }
    return done()
}

// Whether a notification came, set on the main thread by the observer that
// waits for it.
@MainActor
private final class Came {
    var done = false
}

// Takes `window` into full screen, `entering`, or out of it, and waits for
// the system to finish: AppKit marks the window full screen as its
// transition begins and moves it to a space of its own over the next second,
// and a second toggle asked during the transition is not made, so the end is
// read from the notification AppKit posts when it is done.
@MainActor
private func toggledFullScreen(_ window: NSWindow, entering: Bool) -> Bool {
    let came = Came()
    let name = entering ? NSWindow.didEnterFullScreenNotification : NSWindow.didExitFullScreenNotification
    let token = NotificationCenter.default.addObserver(forName: name, object: window, queue: nil) { _ in
        MainActor.assumeIsolated {
            came.done = true
        }
    }
    window.toggleFullScreen(nil)
    let reached = waited({ came.done })
    NotificationCenter.default.removeObserver(token)
    return reached
}

// Brings `window` to `target`, one of the states, leaving the state it
// stands in first: restored from the Dock, out of full screen, unzoomed.
// Each step waits for the system to finish it. Answers whether `window`
// stands in `target` at the end.
@MainActor
private func brought(_ window: NSWindow, _ target: Int64) -> Bool {
    if window.isMiniaturized && target != stateMinimized {
        window.deminiaturize(nil)
        if !waited({ !window.isMiniaturized }) {
            return false
        }
    }
    if window.styleMask.contains(.fullScreen) && target != stateFullScreen {
        if !toggledFullScreen(window, entering: false) {
            return false
        }
    }
    if window.isZoomed && target != stateMaximized && target != stateMinimized {
        window.zoom(nil)
        if !waited({ !window.isZoomed }) {
            return false
        }
    }
    switch target {
    case stateMinimized:
        if !window.isMiniaturized {
            window.miniaturize(nil)
        }
    case stateMaximized:
        if !window.isZoomed {
            window.zoom(nil)
        }
    case stateFullScreen:
        if !window.styleMask.contains(.fullScreen) {
            window.collectionBehavior.insert(.fullScreenPrimary)
            if !toggledFullScreen(window, entering: true) {
                return false
            }
        }
    default:
        break
    }
    if !waited({ stateOf(window) == target }) {
        return false
    }
    // The system animates the frame on after the state is reached; it is
    // waited for too, so the moves it makes belong to this call.
    var last = window.frame
    var still = 0
    let deadline = Date(timeIntervalSinceNow: stateWait)
    while still < 3 && Date() < deadline {
        if let event = NSApp.nextEvent(matching: .any, until: Date(timeIntervalSinceNow: 0.02), inMode: .default, dequeue: true) {
            NSApp.sendEvent(event)
        }
        if window.frame == last {
            still += 1
        } else {
            still = 0
            last = window.frame
        }
    }
    return true
}

// Watches `window`, widget `id`, so what the person does to it among the
// screens is queued by `push`: its moves, its screen, its state. Once for
// each window, from the time it is first shown; the watch ends when the
// window closes, as a destroyed window is.
@c
public func hero_ui_appkit_windows_watch(_ window: UnsafeMutableRawPointer?, _ id: Int64, _ push: HeroUiWindowsPush?) -> Int64 {
    guard let window = window, let push = push else {
        return answerInvalid
    }
    return MainActor.assumeIsolated { () -> Int64 in
        let shown = windowOf(window)
        pushing = push
        let key = ObjectIdentifier(shown)
        if watched[key] != nil {
            return answerOk
        }
        let entry = Watched(id: id)
        entry.state = stateOf(shown)
        entry.screen = screenIndex(shown)
        let centre = NotificationCenter.default
        for name in watchedNames {
            entry.observers.append(centre.addObserver(forName: name, object: shown, queue: nil) { note in
                MainActor.assumeIsolated {
                    if let window = note.object as? NSWindow {
                        noticed(window, note.name)
                    }
                }
            })
        }
        entry.observers.append(centre.addObserver(forName: NSWindow.willCloseNotification, object: shown, queue: nil) { note in
            MainActor.assumeIsolated {
                guard let window = note.object as? NSWindow, let gone = watched.removeValue(forKey: ObjectIdentifier(window)) else {
                    return
                }
                for observer in gone.observers {
                    NotificationCenter.default.removeObserver(observer)
                }
                if modalWindow === window {
                    endModal()
                }
            }
        })
        watched[key] = entry
        return answerOk
    }
}

// Writes the screens through `values`, `HERO_UI_SCREEN_FIELDS` numbers each
// where `count` holds them all, and answers how many there are.
@c
public func hero_ui_appkit_screens(_ values: UnsafeMutablePointer<Int64>?, _ count: Int64, _ screens: UnsafeMutablePointer<Int64>?) -> Int64 {
    guard let screens = screens else {
        return answerInvalid
    }
    let read = MainActor.assumeIsolated { () -> [Int64] in
        let top = desktopTop()
        var out: [Int64] = []
        for screen in screensNow() {
            let whole = screen.frame
            let work = screen.visibleFrame
            out.append(contentsOf: [
                Int64(whole.minX.rounded()), Int64((top - whole.maxY).rounded()), Int64(whole.width.rounded()), Int64(whole.height.rounded()),
                Int64(work.minX.rounded()), Int64((top - work.maxY).rounded()), Int64(work.width.rounded()), Int64(work.height.rounded()),
                Int64((screen.backingScaleFactor * 100).rounded()),
            ])
        }
        return out
    }
    screens.pointee = Int64(read.count / 9)
    if let values = values, read.count <= Int(count) {
        for at in 0..<read.count {
            values[at] = read[at]
        }
    }
    return answerOk
}

// Writes `window`'s frame on the screen it stands on through the five
// places.
@c
public func hero_ui_appkit_frame_of(_ window: UnsafeMutableRawPointer?, _ screen: UnsafeMutablePointer<Int64>?, _ x: UnsafeMutablePointer<Int64>?, _ y: UnsafeMutablePointer<Int64>?, _ width: UnsafeMutablePointer<Int64>?, _ height: UnsafeMutablePointer<Int64>?) -> Int64 {
    guard let window = window, let screen = screen, let x = x, let y = y, let width = width, let height = height else {
        return answerInvalid
    }
    let frame = MainActor.assumeIsolated { () -> (Int64, Int64, Int64, Int64, Int64) in
        return frameOn(windowOf(window))
    }
    screen.pointee = frame.0
    x.pointee = frame.1
    y.pointee = frame.2
    width.pointee = frame.3
    height.pointee = frame.4
    return answerOk
}

// Moves `window`'s frame's top-left corner to `x`, `y` from screen
// `screen`'s top-left corner; `person` 1 where the person moves it, so its
// events are queued, 0 where the program does.
@c
public func hero_ui_appkit_set_position(_ window: UnsafeMutableRawPointer?, _ screen: Int64, _ x: Int64, _ y: Int64, _ person: Int64) -> Int64 {
    guard let window = window else {
        return answerInvalid
    }
    return MainActor.assumeIsolated { () -> Int64 in
        let screens = screensNow()
        guard screen >= 0, screen < Int64(screens.count) else {
            return answerInvalid
        }
        let area = screens[Int(screen)].frame
        let moved = windowOf(window)
        if person == 0 {
            quiet += 1
        }
        moved.setFrameTopLeftPoint(NSPoint(x: area.minX + CGFloat(x), y: area.maxY - CGFloat(y)))
        if person == 0 {
            quiet -= 1
        }
        return answerOk
    }
}

// Centres `window`'s frame in the work area of the screen it stands on, or
// over `over`'s frame where it is not nil, and keeps it within that screen's
// work area as far as its size allows.
@c
public func hero_ui_appkit_center(_ window: UnsafeMutableRawPointer?, _ over: UnsafeMutableRawPointer?) -> Int64 {
    guard let window = window else {
        return answerInvalid
    }
    return MainActor.assumeIsolated { () -> Int64 in
        let moved = windowOf(window)
        let anchor = over.map { windowOf($0) }
        let screen = (anchor ?? moved).screen ?? NSScreen.main ?? screensNow().first
        guard let work = screen?.visibleFrame else {
            return answerOk
        }
        let around = anchor?.frame ?? work
        let size = moved.frame.size
        var x = around.midX - size.width / 2
        var y = around.midY - size.height / 2
        x = max(work.minX, min(x, work.maxX - size.width))
        y = max(work.minY, min(y, work.maxY - size.height))
        quiet += 1
        moved.setFrameOrigin(NSPoint(x: x.rounded(), y: y.rounded()))
        quiet -= 1
        return answerOk
    }
}

// Brings `window` to `state`, waiting for the system to make it; `person`
// as `set_position`'s.
@c
public func hero_ui_appkit_set_state(_ window: UnsafeMutableRawPointer?, _ state: Int64, _ person: Int64) -> Int64 {
    guard let window = window else {
        return answerInvalid
    }
    return MainActor.assumeIsolated { () -> Int64 in
        let changed = windowOf(window)
        if person == 0 {
            quiet += 1
        }
        let reached = brought(changed, state)
        if person == 0 {
            quiet -= 1
            watched[ObjectIdentifier(changed)]?.state = stateOf(changed)
        }
        if !reached {
            said = "the system did not bring the window to the state asked within \(Int(stateWait)) seconds"
            return answerOther
        }
        return answerOk
    }
}

// Writes the state `window` stands in through `state`.
@c
public func hero_ui_appkit_state(_ window: UnsafeMutableRawPointer?, _ state: UnsafeMutablePointer<Int64>?) -> Int64 {
    guard let window = window, let state = state else {
        return answerInvalid
    }
    state.pointee = MainActor.assumeIsolated { () -> Int64 in
        return stateOf(windowOf(window))
    }
    return answerOk
}

// Keeps `window` above every other application's windows, `on` 1, or at
// the level of a document's window, 0.
@c
public func hero_ui_appkit_set_on_top(_ window: UnsafeMutableRawPointer?, _ on: Int64) -> Int64 {
    guard let window = window else {
        return answerInvalid
    }
    MainActor.assumeIsolated {
        windowOf(window).level = on == 1 ? .floating : .normal
    }
    return answerOk
}

// Gives `window` its frame, a title bar and its buttons, `on` 1, or takes
// them away, 0; its content keeps its size. A window with no frame is moved
// by a drag on its background, as it has no title bar to be dragged by.
@c
public func hero_ui_appkit_set_framed(_ window: UnsafeMutableRawPointer?, _ on: Int64) -> Int64 {
    guard let window = window else {
        return answerInvalid
    }
    return MainActor.assumeIsolated { () -> Int64 in
        let changed = windowOf(window)
        if changed.styleMask.contains(.fullScreen) {
            said = "a window in full screen keeps its frame until it leaves full screen"
            return answerOther
        }
        let content = changed.contentLayoutRect.size
        let resizable = changed.styleMask.contains(.resizable)
        var mask: NSWindow.StyleMask = on == 1 ? [.titled, .closable, .miniaturizable] : [.borderless, .miniaturizable]
        if resizable {
            mask.insert(.resizable)
        }
        quiet += 1
        changed.styleMask = mask
        changed.isMovableByWindowBackground = on == 0
        changed.setContentSize(content)
        quiet -= 1
        return answerOk
    }
}

// Sets `window`'s opacity, `alpha` from 0, wholly see-through, to 255,
// wholly opaque.
@c
public func hero_ui_appkit_set_opacity(_ window: UnsafeMutableRawPointer?, _ alpha: Int64) -> Int64 {
    guard let window = window else {
        return answerInvalid
    }
    MainActor.assumeIsolated {
        windowOf(window).alphaValue = CGFloat(alpha) / 255
    }
    return answerOk
}

// The identifier the backdrop this member puts under a window's content
// carries, by which it is found again, so no table of them is kept.
private let backdropName = NSUserInterfaceItemIdentifier("hero_ui.backdrop")

// Puts under `window`'s content the system's translucent material, which
// blurs what stands behind the window (`NSVisualEffectView`, blending
// behind the window), or takes it away.
@c
public func hero_ui_appkit_set_backdrop(_ window: UnsafeMutableRawPointer?, _ backdrop: Int64) -> Int64 {
    guard let window = window else {
        return answerInvalid
    }
    return MainActor.assumeIsolated { () -> Int64 in
        guard let content = windowOf(window).contentView else {
            said = "the window has no content view to put a backdrop under"
            return answerOther
        }
        for view in content.subviews where view.identifier == backdropName {
            view.removeFromSuperview()
        }
        if backdrop == backdropTranslucent {
            let effect = NSVisualEffectView(frame: content.bounds)
            effect.identifier = backdropName
            effect.material = .underWindowBackground
            effect.blendingMode = .behindWindow
            effect.state = .active
            effect.autoresizingMask = [.width, .height]
            content.addSubview(effect, positioned: .below, relativeTo: nil)
        }
        return answerOk
    }
}

// Gives the application the icon of `width` by `height` pixels of 8-bit
// RGBA at `rgba`, the Dock showing it, or the system's own where `width` is
// 0.
@c
public func hero_ui_appkit_set_icon(_ rgba: UnsafePointer<UInt8>?, _ width: Int64, _ height: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        if width == 0 || height == 0 {
            NSApp.applicationIconImage = nil
            return answerOk
        }
        guard let rgba = rgba, let rep = NSBitmapImageRep(bitmapDataPlanes: nil, pixelsWide: Int(width), pixelsHigh: Int(height), bitsPerSample: 8, samplesPerPixel: 4, hasAlpha: true, isPlanar: false, colorSpaceName: .deviceRGB, bitmapFormat: [.alphaNonpremultiplied], bytesPerRow: Int(width) * 4, bitsPerPixel: 32), let into = rep.bitmapData else {
            return answerLimit
        }
        into.update(from: rgba, count: Int(width) * Int(height) * 4)
        let image = NSImage(size: NSSize(width: CGFloat(width), height: CGFloat(height)))
        image.addRepresentation(rep)
        NSApp.applicationIconImage = image
        return answerOk
    }
}

// THE MODAL WINDOW. A window made modal takes the person's every act while
// the app's other windows take none, as AppKit's modal session gives:
// `NSApp.beginModalSession`, and the session run by `runModalSession` in
// place of the system's loop, which `hero_ui.c`'s `wait` asks here while a
// session stands. One window at a time; the session ends when the program
// ends it or the window closes.
@MainActor
private var modalWindow: NSWindow?
@MainActor
private var modalSession: NSApplication.ModalSession?

@MainActor
private func endModal() {
    if let session = modalSession {
        NSApp.endModalSession(session)
    }
    modalSession = nil
    modalWindow = nil
}

// Makes `window` the app's modal window, `on` 1, or ends its session, 0.
@c
public func hero_ui_appkit_set_modal(_ window: UnsafeMutableRawPointer?, _ on: Int64) -> Int64 {
    guard let window = window else {
        return answerInvalid
    }
    return MainActor.assumeIsolated { () -> Int64 in
        let asked = windowOf(window)
        if on == 0 {
            if modalWindow === asked {
                endModal()
            }
            return answerOk
        }
        if modalWindow === asked {
            return answerOk
        }
        if modalWindow != nil {
            said = "another window of the app is modal already; end its session first"
            return answerOther
        }
        modalWindow = asked
        modalSession = NSApp.beginModalSession(for: asked)
        return answerOk
    }
}

// 1 while a window is modal, 0 where none is.
@c
public func hero_ui_appkit_modal_here() -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return modalSession == nil ? 0 : 1
    }
}

// Runs the modal session for at most `ms` milliseconds, or until `held`
// answers that an event is queued: the modal window's events are sent on,
// every other window's refused, as AppKit's session does.
@c
public func hero_ui_appkit_modal_pump(_ ms: Int64, _ held: (@convention(c) () -> Int64)?) -> Int64 {
    guard let held = held else {
        return answerInvalid
    }
    MainActor.assumeIsolated {
        let deadline = Date(timeIntervalSinceNow: Double(ms) / 1000)
        repeat {
            guard let session = modalSession, NSApp.runModalSession(session) == .continue else {
                break
            }
            if held() > 0 {
                break
            }
            _ = NSApp.nextEvent(matching: .any, until: min(deadline, Date(timeIntervalSinceNow: 0.05)), inMode: .modalPanel, dequeue: false)
        } while held() == 0 && Date() < deadline
        NSApp.updateWindows()
    }
    return answerOk
}
