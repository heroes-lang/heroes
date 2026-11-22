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

    init(push: @escaping HeroUiSystemPush) {
        self.push = push
    }

    func applicationShouldTerminate(_ sender: NSApplication) -> NSApplication.TerminateReply {
        push(eventQuit, 0, 0, 0, nil, 0)
        return .terminateCancel
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

// Holds the system's asks to end the app from now, queuing
// `HERO_UI_EVENT_QUIT` by `push` for each; `push` nil answers every ask
// held as ending and lets AppKit's delegate go.
@c
public func hero_ui_appkit_session_watch(_ push: HeroUiSystemPush?) {
    MainActor.assumeIsolated {
        if let open = session {
            open.answer(true)
            if NSApp.delegate === open {
                NSApp.delegate = nil
            }
            session = nil
        }
        guard let push = push else {
            return
        }
        let made = Session(push: push)
        session = made
        NSApp.delegate = made
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
// taken by the same handler as the system's.
@c
public func hero_ui_appkit_ask_quit(_ sessionEnd: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        let target = NSAppleEventDescriptor(processIdentifier: ProcessInfo.processInfo.processIdentifier)
        let event = NSAppleEventDescriptor(eventClass: coreEvents, eventID: quitApplication, targetDescriptor: target, returnID: AEReturnID(kAutoGenerateReturnID), transactionID: AETransactionID(kAnyTransactionID))
        if sessionEnd == 1 {
            event.setAttribute(NSAppleEventDescriptor(enumCode: fourCharacters("rlgo")), forKeyword: quitReason)
        }
        do {
            _ = try event.sendEvent(options: [.noReply], timeout: 5)
        } catch {
            said = "the system refused its own quit event: \(error.localizedDescription)"
            return answerOther
        }
        return answerOk
    }
}
