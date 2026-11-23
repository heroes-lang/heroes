/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_appkit_print.swift: the macOS arm of the print and
 * page-setup dialogs (`platform/hero_ui_print_win32.h`, THE SETTINGS' TEXT): the
 * system's print panel, `NSPrintPanel`, and its page layout, `NSPageLayout`,
 * each begun as a sheet of the window it stands over, which answers its call
 * at once and the person later. The answer is queued by the function
 * `hero_ui.c` hands each call, as `HERO_UI_EVENT_DIALOG` naming the dialog's
 * number, its value 1 with the settings chosen in its text, or 0 where the
 * person cancelled. A Swift member of its own, compiled as a module of its
 * own (`selfhost/cli/layer_swift.hero`): `hero_ui.c` asks
 * `hero_ui_appkit.swift` for the window's `NSWindow` and hands it here.
 *
 * THE SETTINGS cross as `hero_ui_print_win32.h`'s text and are set on the panel's
 * `NSPrintInfo` through Core Printing: the printer by its CUPS queue's name
 * (`PMPrinterGetID`), the copies, the collation, the sides, the first page
 * range (the panel holds one range), the orientation and the paper, a PWG
 * name turned into the PPD name the panel takes and back by CUPS's own
 * table (`pwgMediaForPWG`, `pwgMediaForPPD`, `pwgMediaForSize`). The colour
 * mode, which the panel sets through each printer's own pane, crosses back
 * as it was given. Measured on macOS 27 (2026-11-23): a sheet of either
 * panel ended with OK reaches its `didEnd` with OK; the queue set by its
 * name is read back; copies and collation survive the panel; the panel sets
 * one side where the printer has one; the print panel starts at every page
 * whatever range it is given and answers the range the person sets there,
 * where the page layout keeps the range it was given.
 *
 * A WEB VIEW'S PAGE PRINTED (`hero_ui_appkit_print_web`): WebKit's own print
 * operation of the `WKWebView`, `printOperation(with:)`, run as a sheet of
 * its window with the print panel (the person chooses) or without it (the
 * settings given print as they are), its view given the web view's frame,
 * since WebKit's print view has none of its own. The answer is the same
 * event: value 1 with the settings printed with once the page is handed to
 * the system, 0 where the person cancelled or WebKit could not print.
 *
 * TWO SHEETS OVER ONE WINDOW come one after the other: a panel is begun once
 * the sheet standing over its window has gone (`NSWindow.didEndSheetNotification`),
 * whoever's sheet it was.
 *
 * EVERY CALL IS ON THE MAIN THREAD, as in `hero_ui_appkit.swift`, and every
 * failure is a value: no `!`, no `try!`, no index past an end.
 *
 * Written from Apple's AppKit and Core Printing documentation and CUPS's
 * `pwg.h`; no other library's source.
 */

import AppKit
import WebKit

private let answerOk: Int64 = 0
private let answerInvalid: Int64 = 1
private let answerNotFound: Int64 = 6
private let answerOther: Int64 = 8
private let eventDialog: Int64 = 17

/* The function an answer is queued by: kind, dialog, value, second, text. */
public typealias HeroUiPrintPush = @convention(c) (Int64, Int64, Int64, Int64, UnsafePointer<UInt8>?, Int64) -> Void

/* The words of the last refusal, which `words` hands to `hero_ui.c`. */
@MainActor private var lastWords = ""

/* The settings of a job, as `hero_ui_print_win32.h`'s text writes them. */
private struct Settings {
    var printer = ""
    var copies: Int64 = 1
    var paper = ""
    var orientation: Int64 = 3
    var sides: Int64 = 1
    var color: Int64 = 0
    var collate: Int64 = 1
    var pages: [(Int64, Int64)] = []

    /* Read from the text; nil where a line is not one of the eight, a key
     * comes twice, or a line is empty but the last, as
     * `printing.parse_settings` refuses them. */
    init?(text: String) {
        var seen = Set<String>()
        var lines = text.split(separator: "\n", omittingEmptySubsequences: false)
        if lines.last == "" {
            lines.removeLast()
        }
        for line in lines {
            guard let equals = line.firstIndex(of: "=") else {
                return nil
            }
            let key = String(line[line.startIndex..<equals])
            guard seen.insert(key).inserted else {
                return nil
            }
            let value = String(line[line.index(after: equals)...])
            switch key {
            case "printer": printer = value
            case "media": paper = value
            case "copies":
                guard let n = Int64(value), n >= 1, n <= 999 else { return nil }
                copies = n
            case "orientation-requested":
                guard let n = Int64(value), n >= 3, n <= 6 else { return nil }
                orientation = n
            case "sides":
                guard let n = Settings.sideWords.firstIndex(of: value), n > 0 else { return nil }
                sides = Int64(n)
            case "print-color-mode":
                guard let n = Settings.colorWords.firstIndex(of: value) else { return nil }
                color = Int64(n)
            case "multiple-document-handling":
                guard let n = Settings.collationWords.firstIndex(of: value) else { return nil }
                collate = Int64(n)
            case "page-ranges":
                pages = []
                for range in value.split(separator: ",") {
                    let ends = range.split(separator: "-")
                    guard ends.count == 2, let first = Int64(ends[0]), let last = Int64(ends[1]) else {
                        return nil
                    }
                    pages.append((first, last))
                }
            default: return nil
            }
        }
    }

    /* IPP's keywords at the header's numbers (`hero_printing.h`). */
    static let sideWords = ["", "one-sided", "two-sided-long-edge", "two-sided-short-edge"]
    static let colorWords = ["auto", "color", "monochrome"]
    static let collationWords = ["separate-documents-uncollated-copies", "separate-documents-collated-copies"]

    /* The text, in `hero_ui_print_win32.h`'s order. */
    var text: String {
        let ranges = pages.map { "\($0.0)-\($0.1)" }.joined(separator: ",")
        let side = Settings.sideWords[Int(max(1, min(3, sides)))]
        let color = Settings.colorWords[Int(max(0, min(2, self.color)))]
        let collation = Settings.collationWords[Int(max(0, min(1, collate)))]
        return "printer=\(printer)\ncopies=\(copies)\nmedia=\(paper)\norientation-requested=\(orientation)\nsides=\(side)\nprint-color-mode=\(color)\nmultiple-document-handling=\(collation)\npage-ranges=\(ranges)\n"
    }
}

/* CUPS's table of papers (`pwg.h`), found once in libcups, which every macOS
 * ships; its `pwg_media_t` read by its fields' places: three pointers, then
 * the width and the length in 2540ths of an inch. */
private typealias PwgByName = @convention(c) (UnsafePointer<CChar>?) -> UnsafeRawPointer?
private typealias PwgBySize = @convention(c) (Int32, Int32) -> UnsafeRawPointer?

private struct PwgTable {
    let byPwg: PwgByName?
    let byPpd: PwgByName?
    let bySize: PwgBySize?

    init() {
        let library = dlopen("/usr/lib/libcups.2.dylib", RTLD_NOW | RTLD_LOCAL)
        byPwg = library.flatMap { dlsym($0, "pwgMediaForPWG") }.map { unsafeBitCast($0, to: PwgByName.self) }
        byPpd = library.flatMap { dlsym($0, "pwgMediaForPPD") }.map { unsafeBitCast($0, to: PwgByName.self) }
        bySize = library.flatMap { dlsym($0, "pwgMediaForSize") }.map { unsafeBitCast($0, to: PwgBySize.self) }
    }

    /* The field at `place` of a `pwg_media_t`, a C string. */
    private func name(_ media: UnsafeRawPointer?, _ place: Int) -> String? {
        guard let media = media else {
            return nil
        }
        let field = media.load(fromByteOffset: place * MemoryLayout<UnsafeRawPointer>.size, as: UnsafePointer<CChar>?.self)
        return field.map { String(cString: $0) }
    }

    func ppd(ofPwg pwg: String) -> String? {
        return pwg.withCString { name(byPwg?($0), 2) }
    }

    func pwg(ofPpd ppd: String) -> String? {
        return ppd.withCString { name(byPpd?($0), 0) }
    }

    func pwg(width points: Double, height: Double) -> String? {
        return name(bySize?(Int32((points * 2540 / 72).rounded()), Int32((height * 2540 / 72).rounded())), 0)
    }
}

@MainActor private let pwgTable = PwgTable()

/* A dialog shown, kept by its number until answered or ended. */
@MainActor private final class Shown: NSObject {
    let dialog: Int64
    let owner: NSWindow
    let page: Bool
    let info: NSPrintInfo
    let push: HeroUiPrintPush
    var given: Settings
    /* The settings an injected answer chose, answered as they are: a panel
     * already up writes its own fields over its `NSPrintInfo` as it ends
     * (measured: copies, paper and collation set after it began were
     * answered as the panel showed them). */
    var injected: Settings?
    var ended = false
    var begun = false
    var waiting: NSObjectProtocol?

    init(dialog: Int64, owner: NSWindow, page: Bool, info: NSPrintInfo, given: Settings, push: @escaping HeroUiPrintPush) {
        self.dialog = dialog
        self.owner = owner
        self.page = page
        self.info = info
        self.given = given
        self.push = push
    }

    @objc func printPanelDidEnd(_ panel: NSPrintPanel, returnCode: Int, contextInfo: UnsafeMutableRawPointer?) {
        finish(returnCode)
    }

    @objc func pageLayoutDidEnd(_ layout: NSPageLayout, returnCode: Int, contextInfo: UnsafeMutableRawPointer?) {
        finish(returnCode)
    }

    /* Queues the answer, once, unless the dialog was ended first. */
    func finish(_ code: Int) {
        if ended {
            return
        }
        ended = true
        shown[dialog] = nil
        if code == NSApplication.ModalResponse.OK.rawValue {
            var text = Array((injected ?? settings(of: info, given: given)).text.utf8)
            text.withUnsafeMutableBufferPointer { buffer in
                push(eventDialog, dialog, 1, 0, UnsafePointer(buffer.baseAddress), Int64(buffer.count))
            }
        } else {
            push(eventDialog, dialog, 0, 0, nil, 0)
        }
    }

    /* Begins the sheet, or waits for the one over the window to go. */
    func begin() {
        if ended || begun {
            return
        }
        if owner.attachedSheet != nil {
            if waiting == nil {
                waiting = NotificationCenter.default.addObserver(forName: NSWindow.didEndSheetNotification, object: owner, queue: .main) { [weak self] _ in
                    MainActor.assumeIsolated {
                        self?.begin()
                    }
                }
            }
            return
        }
        if let waiting = waiting {
            NotificationCenter.default.removeObserver(waiting)
            self.waiting = nil
        }
        begun = true
        if page {
            NSPageLayout().beginSheet(with: info, modalFor: owner, delegate: self, didEnd: #selector(pageLayoutDidEnd(_:returnCode:contextInfo:)), contextInfo: nil)
        } else {
            NSPrintPanel().beginSheet(with: info, modalFor: owner, delegate: self, didEnd: #selector(printPanelDidEnd(_:returnCode:contextInfo:)), contextInfo: nil)
        }
    }

    /* Ends the dialog unanswered: its sheet, where begun, is taken down. */
    func end() {
        ended = true
        shown[dialog] = nil
        if let waiting = waiting {
            NotificationCenter.default.removeObserver(waiting)
            self.waiting = nil
        }
        if begun, let sheet = owner.attachedSheet {
            owner.endSheet(sheet, returnCode: .abort)
        }
    }
}

@MainActor private var shown: [Int64: Shown] = [:]

/* A web view's page being printed, kept by its number until the operation
 * has run. */
@MainActor private final class WebPrint: NSObject {
    let dialog: Int64
    let owner: NSWindow
    let given: Settings
    let push: HeroUiPrintPush
    var ended = false

    init(dialog: Int64, owner: NSWindow, given: Settings, push: @escaping HeroUiPrintPush) {
        self.dialog = dialog
        self.owner = owner
        self.given = given
        self.push = push
    }

    @objc func printOperationDidRun(_ operation: NSPrintOperation, success: Bool, contextInfo: UnsafeMutableRawPointer?) {
        webPrints[dialog] = nil
        if ended {
            return
        }
        if success {
            var text = Array(settings(of: operation.printInfo, given: given).text.utf8)
            text.withUnsafeMutableBufferPointer { buffer in
                push(eventDialog, dialog, 1, 0, UnsafePointer(buffer.baseAddress), Int64(buffer.count))
            }
        } else {
            push(eventDialog, dialog, 0, 0, nil, 0)
        }
    }
}

@MainActor private var webPrints: [Int64: WebPrint] = [:]

/* The printer queue named `name`, from Core Printing's list. */
private func printer(named name: String) -> PMPrinter? {
    var list: Unmanaged<CFArray>?
    guard PMServerCreatePrinterList(nil, &list) == noErr, let array = list?.takeRetainedValue() else {
        return nil
    }
    for i in 0..<CFArrayGetCount(array) {
        guard let raw = CFArrayGetValueAtIndex(array, i) else {
            continue
        }
        let candidate = OpaquePointer(raw)
        if (PMPrinterGetID(candidate) as String?) == name {
            PMRetain(PMObject(candidate))
            return candidate
        }
    }
    return nil
}

/* Core Printing's orientation of IPP's number, and back. */
private func pmOrientation(_ ipp: Int64) -> PMOrientation {
    switch ipp {
    case 4: return PMOrientation(kPMLandscape)
    case 5: return PMOrientation(kPMReverseLandscape)
    case 6: return PMOrientation(kPMReversePortrait)
    default: return PMOrientation(kPMPortrait)
    }
}

private func ippOrientation(_ pm: PMOrientation) -> Int64 {
    switch Int(pm) {
    case Int(kPMLandscape): return 4
    case Int(kPMReverseLandscape): return 5
    case Int(kPMReversePortrait): return 6
    default: return 3
    }
}

/* `settings` set on `info`; NOT_FOUND where no queue is so named. */
@MainActor private func apply(_ settings: Settings, to info: NSPrintInfo) -> Int64 {
    let session = OpaquePointer(info.pmPrintSession())
    let print = OpaquePointer(info.pmPrintSettings())
    if !settings.printer.isEmpty {
        guard let queue = printer(named: settings.printer) else {
            lastWords = "no printer named \"\(settings.printer)\""
            return answerNotFound
        }
        defer {
            PMRelease(PMObject(queue))
        }
        guard PMSessionSetCurrentPMPrinter(session, queue) == noErr else {
            lastWords = "the print panel could not be set to printer \"\(settings.printer)\""
            return answerOther
        }
    }
    _ = PMSetCopies(print, UInt32(clamping: settings.copies), false)
    _ = PMSetCollate(print, settings.collate == 1)
    let duplex = settings.sides == 2 ? kPMDuplexNoTumble : settings.sides == 3 ? kPMDuplexTumble : kPMDuplexNone
    _ = PMSetDuplex(print, PMDuplexMode(duplex))
    info.updateFromPMPrintSettings()
    /* The paper before the orientation: naming a paper sets the page format
     * upright again. */
    if !settings.paper.isEmpty, let ppd = pwgTable.ppd(ofPwg: settings.paper) {
        info.paperName = NSPrinter.PaperName(ppd)
    }
    _ = PMSetOrientation(OpaquePointer(info.pmPageFormat()), pmOrientation(settings.orientation), false)
    info.updateFromPMPageFormat()
    /* The panel reads the pages from the print info's own keys. */
    let attributes = info.dictionary()
    attributes[NSPrintInfo.AttributeKey.allPages] = NSNumber(value: settings.pages.isEmpty)
    if let first = settings.pages.first {
        attributes[NSPrintInfo.AttributeKey.firstPage] = NSNumber(value: first.0)
        attributes[NSPrintInfo.AttributeKey.lastPage] = NSNumber(value: first.1)
    }
    return answerOk
}

/* The settings `info` holds once the person has answered, the colour mode
 * as it was given. */
@MainActor private func settings(of info: NSPrintInfo, given: Settings) -> Settings {
    var out = given
    let session = OpaquePointer(info.pmPrintSession())
    let print = OpaquePointer(info.pmPrintSettings())
    let format = OpaquePointer(info.pmPageFormat())
    var current: PMPrinter?
    if PMSessionGetCurrentPrinter(session, &current) == noErr, let current = current, let id = PMPrinterGetID(current) as String? {
        out.printer = id
    }
    var copies: UInt32 = 1
    if PMGetCopies(print, &copies) == noErr {
        out.copies = Int64(copies)
    }
    var collate = DarwinBoolean(true)
    if PMGetCollate(print, &collate) == noErr {
        out.collate = collate.boolValue ? 1 : 0
    }
    var duplex = PMDuplexMode(kPMDuplexNone)
    if PMGetDuplex(print, &duplex) == noErr {
        out.sides = Int(duplex) == Int(kPMDuplexNoTumble) ? 2 : Int(duplex) == Int(kPMDuplexTumble) ? 3 : 1
    }
    let attributes = info.dictionary()
    let all = (attributes[NSPrintInfo.AttributeKey.allPages] as? NSNumber)?.boolValue ?? true
    let first = (attributes[NSPrintInfo.AttributeKey.firstPage] as? NSNumber)?.int64Value ?? 1
    let last = (attributes[NSPrintInfo.AttributeKey.lastPage] as? NSNumber)?.int64Value ?? Int64(Int32.max)
    out.pages = !all && first >= 1 && last >= first && last < Int64(Int32.max) ? [(first, last)] : []
    var orientation = PMOrientation(kPMPortrait)
    if PMGetOrientation(format, &orientation) == noErr {
        out.orientation = ippOrientation(orientation)
    }
    var paper: PMPaper?
    if PMGetPageFormatPaper(format, &paper) == noErr, let paper = paper {
        var ppd: Unmanaged<CFString>?
        if PMPaperGetPPDPaperName(paper, &ppd) == noErr, let name = ppd?.takeUnretainedValue() as String?, let pwg = pwgTable.pwg(ofPpd: name) {
            out.paper = pwg
        } else {
            var width = 0.0
            var height = 0.0
            if PMPaperGetWidth(paper, &width) == noErr, PMPaperGetHeight(paper, &height) == noErr, let pwg = pwgTable.pwg(width: width, height: height) {
                out.paper = pwg
            }
        }
    }
    return out
}

/* The `count` bytes at `bytes` as text, or nil where they are no UTF-8. */
private func text(_ bytes: UnsafePointer<UInt8>?, _ count: Int64) -> String? {
    guard count > 0 else {
        return ""
    }
    guard let bytes = bytes else {
        return nil
    }
    return String(validating: UnsafeBufferPointer(start: bytes, count: Int(count)), as: UTF8.self)
}

@c
public func hero_ui_appkit_print_words(_ words: UnsafeMutablePointer<CChar>?, _ count: Int64) -> Int64 {
    guard let words = words, count > 0 else {
        return 0
    }
    return MainActor.assumeIsolated {
        let bytes = Array(lastWords.utf8.prefix(Int(count) - 1))
        for (i, byte) in bytes.enumerated() {
            words[i] = CChar(bitPattern: byte)
        }
        words[bytes.count] = 0
        return Int64(bytes.count)
    }
}

// Shows print dialog `dialog` (`page` 0) or page-setup dialog `dialog`
// (`page` 1) over the `NSWindow` `window`, its fields first set from the
// `count` bytes of `settings`.
@c
public func hero_ui_appkit_print_show(_ window: UnsafeMutableRawPointer?, _ dialog: Int64, _ page: Int64, _ settings: UnsafePointer<UInt8>?, _ count: Int64, _ push: HeroUiPrintPush?) -> Int64 {
    guard let window = window, let push = push, page == 0 || page == 1 else {
        return answerInvalid
    }
    return MainActor.assumeIsolated {
        guard shown[dialog] == nil else {
            lastWords = "dialog \(dialog) is shown already"
            return answerInvalid
        }
        guard let given = text(settings, count).flatMap({ Settings(text: $0) }) else {
            lastWords = "the dialog's settings are not the text hero_ui_print_win32.h names"
            return answerInvalid
        }
        let owner = Unmanaged<NSWindow>.fromOpaque(window).takeUnretainedValue()
        let info = NSPrintInfo()
        let kind = apply(given, to: info)
        if kind != answerOk {
            return kind
        }
        let entry = Shown(dialog: dialog, owner: owner, page: page == 1, info: info, given: given, push: push)
        shown[dialog] = entry
        entry.begin()
        return answerOk
    }
}

// Prints the page the `WKWebView` `view` shows, over the `NSWindow`
// `window`: through the print panel where `ask` is 1, straight to the printer
// of the `count` bytes of `settings` where it is 0; the answer queued as
// dialog `dialog`'s.
@c
public func hero_ui_appkit_print_web(_ view: UnsafeMutableRawPointer?, _ window: UnsafeMutableRawPointer?, _ dialog: Int64, _ ask: Int64, _ settings: UnsafePointer<UInt8>?, _ count: Int64, _ push: HeroUiPrintPush?) -> Int64 {
    guard let view = view, let window = window, let push = push, ask == 0 || ask == 1 else {
        return answerInvalid
    }
    return MainActor.assumeIsolated {
        guard shown[dialog] == nil, webPrints[dialog] == nil else {
            lastWords = "dialog \(dialog) is shown already"
            return answerInvalid
        }
        guard let web = Unmanaged<NSView>.fromOpaque(view).takeUnretainedValue() as? WKWebView else {
            lastWords = "the widget is no web view"
            return answerInvalid
        }
        guard let given = text(settings, count).flatMap({ Settings(text: $0) }) else {
            lastWords = "the print's settings are not the text hero_ui_print_win32.h names"
            return answerInvalid
        }
        let owner = Unmanaged<NSWindow>.fromOpaque(window).takeUnretainedValue()
        guard owner.attachedSheet == nil else {
            lastWords = "a sheet stands over the web view's window"
            return answerOther
        }
        let info = NSPrintInfo()
        let kind = apply(given, to: info)
        if kind != answerOk {
            return kind
        }
        let operation = web.printOperation(with: info)
        operation.showsPrintPanel = ask == 1
        operation.showsProgressPanel = false
        operation.view?.frame = web.bounds
        let entry = WebPrint(dialog: dialog, owner: owner, given: given, push: push)
        webPrints[dialog] = entry
        operation.runModal(for: owner, delegate: entry, didRun: #selector(WebPrint.printOperationDidRun(_:success:contextInfo:)), contextInfo: nil)
        return answerOk
    }
}

// Answers dialog `dialog` as its person would: OK with the `count` bytes of
// `settings` set first where `value` is 1, Cancel where it is 0.
@c
public func hero_ui_appkit_print_answer(_ dialog: Int64, _ value: Int64, _ settings: UnsafePointer<UInt8>?, _ count: Int64) -> Int64 {
    return MainActor.assumeIsolated {
        if let printing = webPrints[dialog] {
            guard value == 0 || value == 1, let sheet = printing.owner.attachedSheet else {
                lastWords = "web view print \(dialog) shows no panel to answer"
                return answerNotFound
            }
            printing.owner.endSheet(sheet, returnCode: value == 1 ? .OK : .cancel)
            return answerOk
        }
        guard let entry = shown[dialog] else {
            lastWords = "no print dialog \(dialog) is shown"
            return answerNotFound
        }
        guard value == 0 || value == 1 else {
            return answerInvalid
        }
        if value == 1 {
            guard let chosen = text(settings, count).flatMap({ Settings(text: $0) }) else {
                lastWords = "the answer's settings are not the text hero_ui_print_win32.h names"
                return answerInvalid
            }
            if !chosen.printer.isEmpty && printer(named: chosen.printer).map({ PMRelease(PMObject($0)); return true }) != true {
                lastWords = "no printer named \"\(chosen.printer)\""
                return answerNotFound
            }
            entry.injected = chosen
        }
        if !entry.begun {
            entry.finish(value == 1 ? NSApplication.ModalResponse.OK.rawValue : NSApplication.ModalResponse.cancel.rawValue)
            entry.end()
            return answerOk
        }
        guard let sheet = entry.owner.attachedSheet else {
            lastWords = "print dialog \(dialog) shows no sheet over its window"
            return answerOther
        }
        entry.owner.endSheet(sheet, returnCode: value == 1 ? .OK : .cancel)
        return answerOk
    }
}

// Ends unanswered every print dialog over the `NSWindow` `window`, or every
// one where `window` is NULL, before the window or the app goes.
@c
public func hero_ui_appkit_print_end(_ window: UnsafeMutableRawPointer?) {
    MainActor.assumeIsolated {
        let owner = window.map { Unmanaged<NSWindow>.fromOpaque($0).takeUnretainedValue() }
        for entry in shown.values where owner == nil || entry.owner === owner {
            entry.end()
        }
        for printing in webPrints.values where owner == nil || printing.owner === owner {
            printing.ended = true
            if let sheet = printing.owner.attachedSheet {
                printing.owner.endSheet(sheet, returnCode: .abort)
            }
        }
    }
}
