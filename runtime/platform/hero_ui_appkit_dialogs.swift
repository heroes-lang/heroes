/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_appkit_dialogs.swift: the macOS arm of `hero_ui.h`'s
 * dialogs (DIALOGS ANSWERED THROUGH THE QUEUE): a message box, a box of the
 * program's words, a file dialog and a folder dialog, each shown as a sheet
 * of the window it stands over, `beginSheetModal(for:completionHandler:)`,
 * which answers its call at once and the person later. The answer is queued
 * by the function `hero_ui.c` hands each call, as `HERO_UI_EVENT_DIALOG`
 * naming the dialog's number. A Swift member of its own, compiled as a
 * module of its own (`selfhost/cli/layer_swift.hero`), so it reads none of
 * `hero_ui_appkit.swift`'s widgets: `hero_ui.c` asks that member for the
 * window's `NSWindow` (`hero_ui_appkit_object`) and hands it here within
 * the same call.
 *
 * THE DIALOGS SHOWN are kept by their numbers until answered; a sheet the
 * program ends (its window destroyed, its app closed, `dialogs_end`) is
 * marked ended first, so its completion handler, which AppKit runs as the
 * sheet goes, queues nothing for it. Two dialogs over one window come one
 * after the other, by this member's own queue: AppKit queues an alert
 * behind a sheet but begins no panel while one stands (measured on macOS
 * 27: a folder panel begun behind an open panel was never presented), so
 * the next is begun once the one before has gone.
 *
 * EVERY CALL IS ON THE MAIN THREAD, as in `hero_ui_appkit.swift`, and every
 * failure is a value: no `!`, no `try!`, no index past an end; each function
 * answers a kind of `hero_ui.h`, and `words` gives the words of the last
 * `HERO_UI_OTHER` or `HERO_UI_UNSUPPORTED`.
 *
 * Written from Apple's AppKit documentation; no other library's source.
 */

import AppKit
import UniformTypeIdentifiers

private let answerOk: Int64 = 0
private let answerInvalid: Int64 = 1
private let answerUnsupported: Int64 = 4
private let answerNotFound: Int64 = 6
private let answerOther: Int64 = 8

private let eventDialog: Int64 = 17

// The longest an answer waits for AppKit to present a sheet it queued, in
// seconds: the sheet before it slides away first, which a loaded machine
// takes a while to draw.
private let sheetWait: Double = 5

private let toneWarning: Int64 = 2
private let toneError: Int64 = 3

private let buttonsOkCancel: Int64 = 2
private let buttonsYesNo: Int64 = 3
private let buttonsYesNoCancel: Int64 = 4

private let pressedOk: Int64 = 1
private let pressedCancel: Int64 = 2
private let pressedYes: Int64 = 3
private let pressedNo: Int64 = 4

/// The function `hero_ui.c` queues a dialog's answer by, as
/// `hero_ui_appkit.swift` declares its own.
public typealias HeroUiDialogsPush = @convention(c) (Int64, Int64, Int64, Int64, UnsafePointer<UInt8>?, Int64) -> Void

// One dialog shown and not yet answered: its number, the window it stands
// over, its sheet, and how its sheet's answer reads.
@MainActor
private final class Shown {
    let dialog: Int64
    let window: Int64
    let owner: NSWindow
    let alert: NSAlert?
    let panel: NSSavePanel?
    // A message box's answers, `HERO_UI_ANSWER_*`, one per button in order;
    // empty for a box of the program's words, whose answer is the index.
    let answers: [Int64]
    // A box of the program's words' button pressed by Escape, -1 for none.
    let cancel: Int64
    let push: HeroUiDialogsPush
    var presented = false
    var ended = false

    init(dialog: Int64, window: Int64, owner: NSWindow, alert: NSAlert?, panel: NSSavePanel?, answers: [Int64], cancel: Int64, push: @escaping HeroUiDialogsPush) {
        self.dialog = dialog
        self.window = window
        self.owner = owner
        self.alert = alert
        self.panel = panel
        self.answers = answers
        self.cancel = cancel
        self.push = push
    }

    // The window AppKit presents as the sheet.
    var sheet: NSWindow? {
        if let alert = alert {
            return alert.window
        }
        return panel
    }
}

@MainActor
private var shown: [Int64: Shown] = [:]
// The dialogs shown and not yet presented, each behind another over its
// window, oldest first.
@MainActor
private var queued: [Shown] = []
@MainActor
private var said = ""

// The window an opaque pointer of `hero_ui.c` names.
@MainActor
private func windowOf(_ pointer: UnsafeMutableRawPointer) -> NSWindow {
    return Unmanaged<NSWindow>.fromOpaque(pointer).takeUnretainedValue()
}

// The text of `count` bytes at `bytes`.
private func textOf(_ bytes: UnsafePointer<UInt8>?, _ count: Int64) -> String {
    guard let bytes = bytes, count > 0 else {
        return ""
    }
    return String(decoding: UnsafeBufferPointer(start: bytes, count: Int(count)), as: UTF8.self)
}

// The texts of `count` bytes at `bytes`, each followed by one zero byte.
private func textsOf(_ bytes: UnsafePointer<UInt8>?, _ count: Int64) -> [String] {
    guard let bytes = bytes, count > 0 else {
        return []
    }
    let buffer = UnsafeBufferPointer(start: bytes, count: Int(count))
    return buffer.split(separator: 0, omittingEmptySubsequences: false).dropLast().map { String(decoding: $0, as: UTF8.self) }
}

// Queues dialog `entry`'s answer `value`, `paths` its text, unless the
// program ended it; the dialog is then no longer shown.
@MainActor
private func answered(_ entry: Shown, _ value: Int64, _ paths: [UInt8]) {
    if entry.ended {
        return
    }
    entry.ended = true
    if shown[entry.dialog] === entry {
        shown[entry.dialog] = nil
    }
    paths.withUnsafeBufferPointer { buffer in
        entry.push(eventDialog, entry.dialog, value, 0, buffer.baseAddress, Int64(buffer.count))
    }
    presentNext(entry.window)
}

// Begins the oldest dialog queued over window `window`, on the system's next
// turn, once the sheet that ended has gone from it.
@MainActor
private func presentNext(_ window: Int64) {
    DispatchQueue.main.async {
        MainActor.assumeIsolated {
            guard let at = queued.firstIndex(where: { $0.window == window }) else {
                return
            }
            let next = queued.remove(at: at)
            if !next.ended {
                present(next)
            }
        }
    }
}

// Shows `entry`: its sheet begun at once, or queued where another dialog
// over its window stands.
@MainActor
private func present(_ entry: Shown) {
    shown[entry.dialog] = entry
    if shown.values.contains(where: { $0 !== entry && $0.window == entry.window && $0.presented && !$0.ended }) {
        queued.append(entry)
        return
    }
    entry.presented = true
    if entry.alert != nil {
        beginAlert(entry)
    } else {
        beginPanel(entry)
    }
}

// The paths of `urls`, each followed by one zero byte, and how many: a path
// holding a zero byte, which no text of `hero_ui.h` carries, is left out.
private func pathsOf(_ urls: [URL]) -> ([UInt8], Int64) {
    var joined: [UInt8] = []
    var count: Int64 = 0
    for url in urls {
        let path = Array(url.path.utf8)
        if path.isEmpty || path.contains(0) {
            continue
        }
        joined.append(contentsOf: path)
        joined.append(0)
        count += 1
    }
    return (joined, count)
}

// An alert of `tone` saying `title` and `text`.
@MainActor
private func alertOf(_ tone: Int64, _ title: String, _ text: String) -> NSAlert {
    let alert = NSAlert()
    alert.messageText = title
    alert.informativeText = text
    alert.alertStyle = tone == toneError ? .critical : (tone == toneWarning ? .warning : .informational)
    return alert
}

// Whether dialog `dialog` may be shown: no dialog of its number is shown
// already.
@MainActor
private func unshown(_ dialog: Int64) -> Bool {
    return shown[dialog] == nil
}

// Begins `entry`'s alert as a sheet of its window, its answer read from
// the button pressed: a message box's `HERO_UI_ANSWER_*`, or a box of the
// program's words' index, its Escape button's where none.
@MainActor
private func beginAlert(_ entry: Shown) {
    guard let alert = entry.alert else {
        return
    }
    alert.beginSheetModal(for: entry.owner) { response in
        let index = response.rawValue - NSApplication.ModalResponse.alertFirstButtonReturn.rawValue
        MainActor.assumeIsolated {
            var value: Int64
            if !entry.answers.isEmpty {
                value = index >= 0 && index < entry.answers.count ? entry.answers[index] : pressedCancel
            } else if index >= 0 && index < alert.buttons.count {
                value = Int64(index)
            } else {
                value = entry.cancel >= 0 ? entry.cancel : 0
            }
            answered(entry, value, [])
        }
    }
}

// Begins `entry`'s panel as a sheet of its window, its paths read once the
// person chooses or cancels.
@MainActor
private func beginPanel(_ entry: Shown) {
    guard let panel = entry.panel else {
        return
    }
    panel.beginSheetModal(for: entry.owner) { response in
        MainActor.assumeIsolated {
            var urls: [URL] = []
            if response == .OK {
                if let open = panel as? NSOpenPanel {
                    urls = open.urls
                } else if let url = panel.url {
                    urls = [url]
                }
            }
            let (paths, count) = pathsOf(urls)
            answered(entry, count, paths)
        }
    }
}

@c
public func hero_ui_appkit_dialogs_words(_ words: UnsafeMutablePointer<CChar>?, _ count: Int64) -> Int64 {
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

// Shows message box `dialog` over window `id`, whose `NSWindow` is `window`.
@c
public func hero_ui_appkit_show_message(_ window: UnsafeMutableRawPointer?, _ id: Int64, _ dialog: Int64, _ tone: Int64, _ buttons: Int64, _ title: UnsafePointer<UInt8>?, _ titleCount: Int64, _ text: UnsafePointer<UInt8>?, _ textCount: Int64, _ push: HeroUiDialogsPush?) -> Int64 {
    guard let window = window, let push = push else {
        return answerInvalid
    }
    let heading = textOf(title, titleCount)
    let words = textOf(text, textCount)
    return MainActor.assumeIsolated { () -> Int64 in
        guard unshown(dialog) else {
            return answerNotFound
        }
        let alert = alertOf(tone, heading, words)
        var answers: [Int64]
        switch buttons {
        case buttonsOkCancel:
            answers = [pressedOk, pressedCancel]
        case buttonsYesNo:
            answers = [pressedYes, pressedNo]
        case buttonsYesNoCancel:
            answers = [pressedYes, pressedNo, pressedCancel]
        default:
            answers = [pressedOk]
        }
        for answer in answers {
            switch answer {
            case pressedYes:
                alert.addButton(withTitle: "Yes")
            case pressedNo:
                alert.addButton(withTitle: "No")
            case pressedCancel:
                alert.addButton(withTitle: "Cancel")
            default:
                alert.addButton(withTitle: "OK")
            }
        }
        present(Shown(dialog: dialog, window: id, owner: windowOf(window), alert: alert, panel: nil, answers: answers, cancel: -1, push: push))
        return answerOk
    }
}

// Shows box `dialog` of the program's words over window `id`.
@c
public func hero_ui_appkit_show_ask(_ window: UnsafeMutableRawPointer?, _ id: Int64, _ dialog: Int64, _ tone: Int64, _ title: UnsafePointer<UInt8>?, _ titleCount: Int64, _ text: UnsafePointer<UInt8>?, _ textCount: Int64, _ buttons: UnsafePointer<UInt8>?, _ buttonsCount: Int64, _ cancel: Int64, _ push: HeroUiDialogsPush?) -> Int64 {
    guard let window = window, let push = push else {
        return answerInvalid
    }
    let heading = textOf(title, titleCount)
    let words = textOf(text, textCount)
    let labels = textsOf(buttons, buttonsCount)
    return MainActor.assumeIsolated { () -> Int64 in
        guard unshown(dialog) else {
            return answerNotFound
        }
        let alert = alertOf(tone, heading, words)
        for label in labels {
            alert.addButton(withTitle: label)
        }
        // The first button is Return's already; Escape presses `cancel`.
        if cancel > 0 && cancel < Int64(alert.buttons.count) {
            alert.buttons[Int(cancel)].keyEquivalent = "\u{1b}"
        }
        present(Shown(dialog: dialog, window: id, owner: windowOf(window), alert: alert, panel: nil, answers: [], cancel: cancel, push: push))
        return answerOk
    }
}

// Shows file dialog `dialog` over window `id`: an open panel, or a save
// panel where `save` is 1.
@c
public func hero_ui_appkit_show_files(_ window: UnsafeMutableRawPointer?, _ id: Int64, _ dialog: Int64, _ save: Int64, _ multiple: Int64, _ title: UnsafePointer<UInt8>?, _ titleCount: Int64, _ name: UnsafePointer<UInt8>?, _ nameCount: Int64, _ filters: UnsafePointer<UInt8>?, _ filtersCount: Int64, _ push: HeroUiDialogsPush?) -> Int64 {
    guard let window = window, let push = push else {
        return answerInvalid
    }
    let heading = textOf(title, titleCount)
    let proposed = textOf(name, nameCount)
    let spelled = textsOf(filters, filtersCount)
    return MainActor.assumeIsolated { () -> Int64 in
        guard unshown(dialog) else {
            return answerNotFound
        }
        // Each filter is its name and then its extensions joined by `;`.
        var types: [UTType] = []
        var at = 1
        while at < spelled.count {
            for ext in spelled[at].split(separator: ";") {
                if let type = UTType(filenameExtension: String(ext)) {
                    types.append(type)
                }
            }
            at += 2
        }
        let panel: NSSavePanel
        if save == 1 {
            panel = NSSavePanel()
            panel.nameFieldStringValue = proposed
        } else {
            let open = NSOpenPanel()
            open.allowsMultipleSelection = multiple == 1
            open.canChooseFiles = true
            open.canChooseDirectories = false
            panel = open
        }
        panel.title = heading
        panel.message = heading
        if !types.isEmpty {
            panel.allowedContentTypes = types
        }
        present(Shown(dialog: dialog, window: id, owner: windowOf(window), alert: nil, panel: panel, answers: [], cancel: -1, push: push))
        return answerOk
    }
}

// Shows folder dialog `dialog` over window `id`.
@c
public func hero_ui_appkit_show_folders(_ window: UnsafeMutableRawPointer?, _ id: Int64, _ dialog: Int64, _ multiple: Int64, _ title: UnsafePointer<UInt8>?, _ titleCount: Int64, _ push: HeroUiDialogsPush?) -> Int64 {
    guard let window = window, let push = push else {
        return answerInvalid
    }
    let heading = textOf(title, titleCount)
    return MainActor.assumeIsolated { () -> Int64 in
        guard unshown(dialog) else {
            return answerNotFound
        }
        let panel = NSOpenPanel()
        panel.title = heading
        panel.message = heading
        panel.canChooseFiles = false
        panel.canChooseDirectories = true
        panel.canCreateDirectories = true
        panel.allowsMultipleSelection = multiple == 1
        present(Shown(dialog: dialog, window: id, owner: windowOf(window), alert: nil, panel: panel, answers: [], cancel: -1, push: push))
        return answerOk
    }
}

// Does what the person does to dialog `dialog`: presses a message box's
// button whose answer is `value` (a box of the program's words' button at
// index `value`), or cancels a file or a folder dialog, `value` 0; a choice
// of `value` paths there, `paths` holding them each followed by one zero
// byte, is `HERO_UI_UNSUPPORTED`. The sheet answers through its own
// completion handler, as the person's click would have it answer.
@c
public func hero_ui_appkit_answer_dialog(_ dialog: Int64, _ value: Int64, _ paths: UnsafePointer<UInt8>?, _ pathsCount: Int64) -> Int64 {
    let chosen = textsOf(paths, pathsCount)
    return MainActor.assumeIsolated { () -> Int64 in
        guard let entry = shown[dialog], !entry.ended else {
            return answerNotFound
        }
        if let sheet = entry.sheet, entry.owner.attachedSheet !== sheet {
            // AppKit presents a sheet queued behind one that ended a
            // moment after it, animated: the system's loop runs until it
            // stands, at most `sheetWait` seconds.
            let deadline = Date(timeIntervalSinceNow: sheetWait)
            while entry.owner.attachedSheet !== sheet && !entry.ended && Date() < deadline {
                if let event = NSApp.nextEvent(matching: .any, until: Date(timeIntervalSinceNow: 0.02), inMode: .default, dequeue: true) {
                    NSApp.sendEvent(event)
                }
            }
            if entry.ended {
                return answerNotFound
            }
            if entry.owner.attachedSheet !== sheet {
                said = "dialog \(dialog) waits behind another dialog over its window, which is answered first"
                return answerOther
            }
        }
        if let alert = entry.alert {
            var index = -1
            if entry.answers.isEmpty {
                index = value >= 0 && value < Int64(alert.buttons.count) ? Int(value) : -1
            } else if let found = entry.answers.firstIndex(of: value) {
                index = found
            }
            guard index >= 0 && index < alert.buttons.count else {
                return answerInvalid
            }
            alert.buttons[index].performClick(nil)
            return answerOk
        }
        guard let panel = entry.panel else {
            return answerInvalid
        }
        // A panel runs out of the process, and its `ok:` and `cancel:` raise
        // "not implemented" (measured on macOS 27): ending its sheet is the
        // one path the program has, and it can only cancel.
        if value == 0 {
            entry.owner.endSheet(panel, returnCode: .cancel)
            return answerOk
        }
        guard Int64(chosen.count) == value else {
            return answerInvalid
        }
        said = "choosing in a file or a folder dialog as the person: AppKit's panels take no choice from the program, only from the person's clicks"
        return answerUnsupported
    }
}

// Ends every dialog shown over window `window`, or every dialog where
// `window` is 0, unanswered: no event comes for any of them.
@c
public func hero_ui_appkit_dialogs_end(_ window: Int64) {
    MainActor.assumeIsolated {
        let ending = shown.values.filter { window == 0 || $0.window == window }
        for entry in ending {
            entry.ended = true
            shown[entry.dialog] = nil
        }
        queued.removeAll { $0.ended }
        // A queued dialog was never presented; a presented one's sheet ends.
        for entry in ending where entry.presented {
            if let sheet = entry.sheet {
                entry.owner.endSheet(sheet, returnCode: .abort)
            }
        }
    }
}
