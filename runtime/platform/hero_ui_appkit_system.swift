/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_appkit_system.swift: the macOS arm of `hero_ui.h`'s calls
 * on the system around the program rather than on a widget: a
 * notification of the system's, the dark or light appearance, asked
 * and watched (`NSApplication.effectiveAppearance`, observed for its change
 * from the app's open to its close), and the system's ask to end the app,
 * held for the program's answer. A Swift member of its own, compiled as
 * a module of its own (`selfhost/cli/layer_swift.hero`), reading none of
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
 * THE SYSTEM'S ASK TO END THE APP is its quit Apple event
 * (`kAEQuitApplication`), which the session's end sends every application
 * with its reason (`kAEQuitReason`), as the Dock's Quit and another
 * program's request send it without one. AppKit's own handler of it ends
 * the process from inside the loop's pump (`terminate(_:)`, whose
 * `applicationShouldTerminate(_:)` answering later runs a loop of its own
 * that the program, waiting in `hero_ui_wait`, never returns from); so
 * this member takes the event in a handler of its own, suspends it
 * (`suspendCurrentAppleEvent()`), queues `HERO_UI_EVENT_QUIT`, and resumes
 * it once the program answers, with no error to go on and `userCanceledErr`
 * to stay, the reply that cancels the session's end. The handler stays from
 * the first open to the process's end: with no app open it answers at once
 * with no error, the program having closed its windows. A `terminate(_:)`
 * of any other sender asks the application delegate, which queues the same
 * ask and cancels it. The app menu's Quit is the program's and comes here
 * not at all (`hero_ui_appkit.swift`).
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
private let answerInvalid: Int64 = 1
private let answerUnsupported: Int64 = 4

private let eventAppearance: Int64 = 18
private let eventQuit: Int64 = 19
private let eventOpen: Int64 = 21
private let openFiles: Int64 = 1
private let openAddresses: Int64 = 2
private let answerOther: Int64 = 8
private let appearanceLight: Int64 = 1
private let appearanceDark: Int64 = 2

/// The function `hero_ui.c` queues an event by, as `hero_ui_appkit.swift`
/// declares its own.
public typealias HeroUiSystemPush = @convention(c) (Int64, Int64, Int64, Int64, UnsafePointer<UInt8>?, Int64) -> Void

@MainActor
private var said = ""
// The observation of the app's appearance, and the appearance last told.
@MainActor
private var watching: NSKeyValueObservation?
@MainActor
private var told: Int64 = 0

// The appearance the app's windows are drawn in now.
@MainActor
private func appearanceNow() -> Int64 {
    let match = NSApp.effectiveAppearance.bestMatch(from: [.aqua, .darkAqua])
    return match == .darkAqua ? appearanceDark : appearanceLight
}

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

// Writes the appearance the app's windows are drawn in now through
// `appearance`, `HERO_UI_APPEARANCE_LIGHT` or `_DARK`.
@c
public func hero_ui_appkit_appearance(_ appearance: UnsafeMutablePointer<Int64>?) -> Int64 {
    guard let appearance = appearance else {
        return answerInvalid
    }
    appearance.pointee = MainActor.assumeIsolated { () -> Int64 in
        return appearanceNow()
    }
    return answerOk
}

// Watches the app's appearance from now, queuing `HERO_UI_EVENT_APPEARANCE`
// by `push` each time it turns, whether the system's or the app's own
// changed it; `push` nil stops watching.
@c
public func hero_ui_appkit_appearance_watch(_ push: HeroUiSystemPush?) {
    MainActor.assumeIsolated {
        watching?.invalidate()
        watching = nil
        guard let push = push else {
            return
        }
        told = appearanceNow()
        watching = NSApp.observe(\.effectiveAppearance, options: [.new]) { _, _ in
            MainActor.assumeIsolated {
                let now = appearanceNow()
                if now != told {
                    told = now
                    push(eventAppearance, 0, now, 0, nil, 0)
                }
            }
        }
    }
}

// Draws the app in `appearance`, as the system's change to it would turn
// it, or in the system's own again where `appearance` is 0: what a case
// injects, the app's own appearance standing over the system's.
@c
public func hero_ui_appkit_appearance_set(_ appearance: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        switch appearance {
        case appearanceLight:
            NSApp.appearance = NSAppearance(named: .aqua)
        case appearanceDark:
            NSApp.appearance = NSAppearance(named: .darkAqua)
        default:
            NSApp.appearance = nil
        }
        return answerOk
    }
}

// MARK: the system's ask to end the app

// The four characters' code `code` names, as Apple events key their
// classes, their identifiers and their parameters.
private func fourCharacters(_ code: String) -> UInt32 {
    var value: UInt32 = 0
    for byte in code.utf8.prefix(4) {
        value = (value << 8) | UInt32(byte)
    }
    return value
}

private let coreEvents = fourCharacters("aevt")
private let quitApplication = fourCharacters("quit")
private let quitReason = fourCharacters("why?")
private let errorNumber = fourCharacters("errn")
// The reasons the session's end gives its quit: a logout, a restart or a
// shutdown, each asked first or done (`kAELogOut` to `kAEShutDown`).
private let sessionReasons: Set<UInt32> = Set(["logo", "rlgo", "rrst", "rest", "rsdn", "shut"].map(fourCharacters))
// The reply that tells the asker the person cancelled, `userCanceledErr`.
private let userCancelled: Int32 = -128

// The open app's asks to end it, held until the program answers, and its
// application delegate, which `terminate(_:)` of any other sender asks.
@MainActor
private final class Session: NSObject, NSApplicationDelegate {
    let push: HeroUiSystemPush
    // The quit events suspended, oldest first.
    var held: [NSAppleEventManager.SuspensionID] = []
    // How many asks to end it were told.
    var asks = 0
    // How many requests to open documents or addresses came.
    var opens = 0

    init(push: @escaping HeroUiSystemPush) {
        self.push = push
    }

    func applicationShouldTerminate(_ sender: NSApplication) -> NSApplication.TerminateReply {
        asks += 1
        push(eventQuit, 0, 0, 0, nil, 0)
        return .terminateCancel
    }

    // The documents and addresses the system asks the app to open (its
    // open and get-address Apple events): files as their paths, the rest as
    // the addresses, each kind queued as one `HERO_UI_EVENT_OPEN`.
    func application(_ application: NSApplication, open urls: [URL]) {
        opens += 1
        let files = urls.filter { $0.isFileURL }.map { $0.path }.filter { !$0.isEmpty }
        let addresses = urls.filter { !$0.isFileURL }.map { $0.absoluteString }.filter { !$0.isEmpty }
        tell(openFiles, files)
        tell(openAddresses, addresses)
    }

    func tell(_ what: Int64, _ items: [String]) {
        if items.isEmpty {
            return
        }
        var joined: [UInt8] = []
        for item in items {
            joined.append(contentsOf: Array(item.utf8).filter { $0 != 0 })
            joined.append(0)
        }
        joined.withUnsafeBufferPointer { bytes in
            push(eventOpen, 0, Int64(items.count), what, bytes.baseAddress, Int64(bytes.count))
        }
    }

    // Resumes every quit event held: with no error where the app ends, and
    // with the person's cancel where it stays.
    func answer(_ go: Bool) {
        let manager = NSAppleEventManager.shared()
        for id in held {
            if !go {
                let reply = manager.replyAppleEvent(forSuspensionID: id)
                if reply.descriptorType == coreEvents {
                    reply.setParam(NSAppleEventDescriptor(int32: userCancelled), forKeyword: errorNumber)
                }
            }
            manager.resume(withSuspensionID: id)
        }
        held.removeAll()
    }
}

// What the quit event's handler reaches: an object of the process's life,
// since Apple events hold their handler for as long.
private final class QuitHandler: NSObject {
    @objc func quitAsked(_ event: NSAppleEventDescriptor, withReplyEvent reply: NSAppleEventDescriptor) {
        MainActor.assumeIsolated {
            guard let open = session else {
                return
            }
            let reason = event.attributeDescriptor(forKeyword: quitReason) ?? event.paramDescriptor(forKeyword: quitReason)
            var ending: Int64 = 0
            if let reason = reason, sessionReasons.contains(reason.enumCodeValue) || sessionReasons.contains(reason.typeCodeValue) {
                ending = 1
            }
            if let id = NSAppleEventManager.shared().suspendCurrentAppleEvent() {
                open.held.append(id)
            }
            open.asks += 1
            open.push(eventQuit, 0, ending, 0, nil, 0)
        }
    }
}

@MainActor
private var session: Session?
@MainActor
private let quitHandler = QuitHandler()
@MainActor
private var handling = false

// Stands the app's delegate from now, before AppKit's launch so the
// documents the system opened the app with reach it, queuing by `push`
// the system's asks to end the app and its documents; `push` nil answers
// every ask held as ending and lets the delegate go.
@c
public func hero_ui_appkit_session_watch(_ push: HeroUiSystemPush?) {
    MainActor.assumeIsolated {
        let app = NSApplication.shared
        if let open = session {
            open.answer(true)
            if app.delegate === open {
                app.delegate = nil
            }
            session = nil
        }
        guard let push = push else {
            return
        }
        let made = Session(push: push)
        session = made
        app.delegate = made
    }
}

// Takes the quit Apple event in this member's handler, after AppKit's
// launch, which installs its own.
@c
public func hero_ui_appkit_quit_watch() {
    MainActor.assumeIsolated {
        if !handling {
            NSAppleEventManager.shared().setEventHandler(quitHandler, andSelector: #selector(QuitHandler.quitAsked(_:withReplyEvent:)), forEventClass: coreEvents, andEventID: quitApplication)
            handling = true
        }
    }
}

// Answers every ask to end the app held: `go` 1 the app ends, 0 it stays.
@c
public func hero_ui_appkit_answer_quit(_ go: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        session?.answer(go == 1)
        return answerOk
    }
}

// Sends this process the quit Apple event the system sends, with the
// session's end as its reason where `sessionEnd` is 1: what a case injects,
// taken by the same handler as the system's. An event a process sends
// itself is handed to its handler at once, and the handler suspends it, so
// the send waits for the reply until its deadline (measured 2026-11-23:
// errAETimeout, -1712, after the five seconds asked, the event taken all
// the same): the deadline is short, and a send whose event the handler
// took is the system's path taken, its timeout no failure. Sending it from
// another queue instead trips Swift's check that the main actor's code
// runs on the main queue.
@c
public func hero_ui_appkit_ask_quit(_ sessionEnd: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        let target = NSAppleEventDescriptor(processIdentifier: ProcessInfo.processInfo.processIdentifier)
        let event = NSAppleEventDescriptor(eventClass: coreEvents, eventID: quitApplication, targetDescriptor: target, returnID: AEReturnID(kAutoGenerateReturnID), transactionID: AETransactionID(kAnyTransactionID))
        if sessionEnd == 1 {
            event.setAttribute(NSAppleEventDescriptor(enumCode: fourCharacters("rlgo")), forKeyword: quitReason)
        }
        let before = session?.asks ?? 0
        do {
            _ = try event.sendEvent(options: [.noReply], timeout: 0.05)
        } catch {
            if (session?.asks ?? 0) == before {
                said = "the system refused its own quit event: \(error.localizedDescription)"
                return answerOther
            }
        }
        return answerOk
    }
}

// MARK: documents from the system

// Sends this process the Apple events the system sends to have an app open
// `count` bytes of items, each followed by one zero byte: one open event
// for files (`what` 1), their paths as file addresses, or one get-address
// event per address (2); taken by AppKit and its delegate as the system's.
@c
public func hero_ui_appkit_open_items(_ what: Int64, _ items: UnsafePointer<UInt8>?, _ count: Int64) -> Int64 {
    let list = textOf(items, count).split(separator: "\u{0}", omittingEmptySubsequences: true).map(String.init)
    return MainActor.assumeIsolated { () -> Int64 in
        let target = NSAppleEventDescriptor(processIdentifier: ProcessInfo.processInfo.processIdentifier)
        var events: [NSAppleEventDescriptor] = []
        if what == openFiles {
            let event = NSAppleEventDescriptor(eventClass: coreEvents, eventID: fourCharacters("odoc"), targetDescriptor: target, returnID: AEReturnID(kAutoGenerateReturnID), transactionID: AETransactionID(kAnyTransactionID))
            let files = NSAppleEventDescriptor.list()
            for (at, path) in list.enumerated() {
                files.insert(NSAppleEventDescriptor(fileURL: URL(fileURLWithPath: path)), at: at + 1)
            }
            event.setParam(files, forKeyword: fourCharacters("----"))
            events.append(event)
        } else {
            for address in list {
                let event = NSAppleEventDescriptor(eventClass: fourCharacters("GURL"), eventID: fourCharacters("GURL"), targetDescriptor: target, returnID: AEReturnID(kAutoGenerateReturnID), transactionID: AETransactionID(kAnyTransactionID))
                event.setParam(NSAppleEventDescriptor(string: address), forKeyword: fourCharacters("----"))
                events.append(event)
            }
        }
        // As the quit event: one the process sends itself is handed to
        // AppKit's handler at once, which holds its reply past the
        // delegate (measured 2026-11-23: errAETimeout after the deadline,
        // the documents told all the same), so the deadline is short and a
        // send the delegate took is no failure.
        for event in events {
            let before = session?.opens ?? 0
            do {
                _ = try event.sendEvent(options: [.noReply], timeout: 0.05)
            } catch {
                if (session?.opens ?? 0) == before {
                    said = "the system refused its own open event: \(error.localizedDescription)"
                    return answerOther
                }
            }
        }
        return answerOk
    }
}

// -- THE DESCRIPTORS A WAIT WATCHES (`hero_ui.h`'s `hero_ui_add_set`) ---------
//
// A poll set's descriptor (its `kqueue`) watched by the main run loop as a
// `CFFileDescriptor` source in the common modes: the loop `pump` runs wakes
// when the descriptor is readable, and `push` is told the watch's number, the
// event kind `eventReady`. A `CFFileDescriptor` callback is disabled once it
// has fired, so a set that stays ready is told once and again only where
// `hero_ui_appkit_watch_fd` is called for it again, which `hero_ui.c` does
// at each wait. The descriptor is the set's own: never closed here.

private let eventReady: Int64 = 22

private final class Watched {
    let fd: Int32
    let file: CFFileDescriptor
    let source: CFRunLoopSource

    init(fd: Int32, file: CFFileDescriptor, source: CFRunLoopSource) {
        self.fd = fd
        self.file = file
        self.source = source
    }
}

@MainActor
private var watched: [Int64: Watched] = [:]
@MainActor
private var readyPush: HeroUiSystemPush?

// The callback of every watch, on the main thread inside the run loop; its
// `info` is the watch's number.
private let fdReadable: CFFileDescriptorCallBack = { _, _, info in
    guard let info = info else {
        return
    }
    let id = Int64(Int(bitPattern: info))
    MainActor.assumeIsolated {
        readyPush?(eventReady, id, 0, 0, nil, 0)
    }
}

// Watches descriptor `fd` for readability as watch `id` (1 or more), or
// enables its callback again where `id` is watched already: told once
// through `push` while it is readable, then not until this is called again.
// `HERO_UI_OK`; `HERO_UI_INVALID` for a number that is none, or one watched
// on another descriptor; `HERO_UI_OTHER` where Core Foundation made none.
@c
public func hero_ui_appkit_watch_fd(_ fd: Int64, _ id: Int64, _ push: HeroUiSystemPush?) -> Int64 {
    guard fd >= 0, fd <= Int64(Int32.max), id >= 1, let push = push else {
        MainActor.assumeIsolated {
            said = "a descriptor watched is 0 or more and a watch's number 1 or more"
        }
        return answerInvalid
    }
    return MainActor.assumeIsolated { () -> Int64 in
        readyPush = push
        if let held = watched[id] {
            guard held.fd == Int32(fd) else {
                said = "watch \(id) stands on descriptor \(held.fd), not \(fd)"
                return answerInvalid
            }
            CFFileDescriptorEnableCallBacks(held.file, kCFFileDescriptorReadCallBack)
            return answerOk
        }
        var context = CFFileDescriptorContext(version: 0, info: UnsafeMutableRawPointer(bitPattern: Int(id)), retain: nil, release: nil, copyDescription: nil)
        guard let file = CFFileDescriptorCreate(kCFAllocatorDefault, Int32(fd), false, fdReadable, &context), let source = CFFileDescriptorCreateRunLoopSource(kCFAllocatorDefault, file, 0) else {
            said = "Core Foundation made no watch on descriptor \(fd)"
            return answerOther
        }
        CFRunLoopAddSource(CFRunLoopGetMain(), source, .commonModes)
        CFFileDescriptorEnableCallBacks(file, kCFFileDescriptorReadCallBack)
        watched[id] = Watched(fd: Int32(fd), file: file, source: source)
        return answerOk
    }
}

// Takes watch `id` away, or every watch where `id` is 0, the descriptors
// themselves left open.
@c
public func hero_ui_appkit_unwatch_fd(_ id: Int64) {
    MainActor.assumeIsolated {
        for (key, held) in watched where id == 0 || key == id {
            CFRunLoopRemoveSource(CFRunLoopGetMain(), held.source, .commonModes)
            CFFileDescriptorInvalidate(held.file)
            watched[key] = nil
        }
        if watched.isEmpty {
            readyPush = nil
        }
    }
}
