/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_ui_appkit.swift: the macOS arm of `hero_ui.c`, the system's
 * own widgets by AppKit (M-rebel-library, 2026-11-22). AppKit is Swift's and
 * Objective-C's, never C's, so this arm is Swift, through the layer's Swift
 * route (`selfhost/cli/layer_swift.hero`), and exports to C by `@c` the
 * functions `hero_ui.c`'s THE ARM calls, each declared again in
 * `hero_ui_appkit.h`, which the layer holds to the header `swiftc` writes.
 *
 * EVERY CALL IS ON THE MAIN THREAD, the main actor's: `hero_ui.c` asks
 * `pthread_main_np` before it calls any function here, so
 * `MainActor.assumeIsolated` never finds another thread. Each function
 * answers a kind of `hero_ui.h`, `HERO_UI_OK` (0) where done; its arguments
 * were checked by `hero_ui.c` (a widget number 1 or more, a count with its
 * bytes behind it, UTF-8 text with no zero byte, sizes 0 or more), and every
 * lookup of a number here answers `HERO_UI_NOT_FOUND` where none stands, so
 * a call never reaches a widget that is not there.
 *
 * EVERY FAILURE IS A VALUE, NEVER A TRAP: no `!`, no `try!`, no index past
 * a list's end, and every pointer read only for the count `hero_ui.c`
 * checked; so a Swift runtime trap, which would end the program with no
 * word of the runtime's, has no place to arise.
 *
 * WHAT THE PERSON DOES is queued in `hero_ui.c` by the function `open`
 * hands over, `push`: a button's action, a field's change, a list's change
 * of selection, a window's close button and its resize. What the program
 * sets (`set_*`) queues nothing: AppKit sends no action for a value set by
 * code, and the table's notification of a selection set by code, and the
 * resize of a size set by code, are muted while the program sets them.
 *
 * THE MENU BAR. macOS shows one menu bar, the application's, at the top of
 * the screen: its first menu is the application's own (its name, and Quit,
 * which asks every window to close), then the menus of the window in front,
 * moved into it each time a window comes to the front.
 *
 * THE WEB VIEW is WebKit's `WKWebView` (`hero_ui.h`, THE WEB VIEW). What it
 * serves, where its page may go and which message is heard are asked of
 * `hero_ui.c` through the functions `open` hands over (`hero_ui_web.h`), so
 * this arm decides none of them: a request of the app's scheme is answered
 * whole, at once, with what `serve` wrote; a navigation goes on where `ask`
 * says so; a message is queued where `ask` hears its sender. Its page keeps
 * nothing on disk (a website data store of its own, in memory), opens no
 * window, downloads nothing, is denied the camera and the microphone, and
 * its file chooser opens nothing. A text the program posts is handed to the
 * page once a page of the app's own origin has loaded, through the bridge's
 * `heroes.deliver` as an argument of the call and never inside a script's
 * text, so no text of the program's is read as a script.
 *
 * Written from Apple's AppKit documentation; no other library's source.
 */

import AppKit
import UniformTypeIdentifiers
import WebKit

// The kinds of failure, `hero_ui.h`'s numbers.
private let answerOk: Int64 = 0
private let answerInvalid: Int64 = 1
private let answerUnsupported: Int64 = 4
private let answerNotFound: Int64 = 6
private let answerLimit: Int64 = 7
private let answerOther: Int64 = 8

// The kinds of widget, `hero_ui.h`'s numbers.
private let kindWindow: Int64 = 1
private let kindLabel: Int64 = 2
private let kindButton: Int64 = 3
private let kindTextField: Int64 = 4
private let kindTextArea: Int64 = 5
private let kindCheckbox: Int64 = 6
private let kindRadioGroup: Int64 = 7
private let kindList: Int64 = 8
private let kindListMultiple: Int64 = 9
private let kindCombo: Int64 = 10
private let kindProgress: Int64 = 11
private let kindSlider: Int64 = 12
private let kindImage: Int64 = 13
private let kindMenu: Int64 = 14
private let kindMenuItem: Int64 = 15
private let kindMenuSeparator: Int64 = 16
private let kindWebView: Int64 = 17

// The kinds of event, `hero_ui.h`'s numbers.
private let eventClick: Int64 = 1
private let eventText: Int64 = 2
private let eventSelect: Int64 = 3
private let eventToggle: Int64 = 4
private let eventValue: Int64 = 5
private let eventMenu: Int64 = 6
private let eventClose: Int64 = 7
private let eventResize: Int64 = 8
private let eventMessage: Int64 = 9

// The keys held with a shortcut, `hero_ui.h`'s numbers.
private let heldPrimary: Int64 = 1
private let heldShift: Int64 = 2
private let heldAlt: Int64 = 4

// `hero_ui.c`'s flags and sizes (THE ARM).
private let flagEnabled: Int64 = 1
private let flagVisible: Int64 = 2
private let flagResizable: Int64 = 3
private let sizeContent: Int64 = 1
private let sizeLeast: Int64 = 2

// A message box's tone and buttons, and the answers, `hero_ui.h`'s numbers.
private let toneWarning: Int64 = 2
private let toneError: Int64 = 3
private let buttonsOkCancel: Int64 = 2
private let buttonsYesNo: Int64 = 3
private let buttonsYesNoCancel: Int64 = 4
private let pressedOk: Int64 = 1
private let pressedCancel: Int64 = 2
private let pressedYes: Int64 = 3
private let pressedNo: Int64 = 4

// The sizes a widget takes where AppKit's has no width of its own to give,
// the headless side's own (`ui/internal/sizes.hero`), so a window looks
// alike on both sides.
private let fieldWidth: CGFloat = 160
private let areaWidth: CGFloat = 240
private let areaHeight: CGFloat = 96
private let viewWidth: CGFloat = 320
private let viewHeight: CGFloat = 240

// `hero_ui_web.h`'s questions, the indices of `serve`'s answer, and the
// app's own scheme.
private let askFrame: Int64 = 1
private let askSubframe: Int64 = 2
private let askSender: Int64 = 3
private let askOurs: Int64 = 4
private let servedStatus = 0
private let servedLength = 1
private let servedTypeCount = 2
private let servedBodyCount = 3
private let servedPolicyCount = 4
private let servedNumbers = 5
private let servedType = 0
private let servedBody = 1
private let servedPolicy = 2
private let servedTexts = 3
private let appScheme = "app"
// The most texts a web view keeps for its page while the page loads,
// `HERO_UI_QUEUE_MOST`.
private let heldMost = 4096

/// The function `hero_ui.c` queues an event by: its kind, its widget, its
/// two values, and a text with its count.
public typealias HeroUiPush = @convention(c) (Int64, Int64, Int64, Int64, UnsafePointer<UInt8>?, Int64) -> Void

/// What a request of the app's scheme is answered: `hero_ui_web.h`'s
/// `hero_ui_web_serve_fn`.
public typealias HeroUiServe = @convention(c) (Int64, UnsafePointer<UInt8>?, Int64, UnsafePointer<UInt8>?, Int64, UnsafeMutablePointer<Int64>?, UnsafeMutablePointer<UnsafePointer<UInt8>?>?) -> Int64

/// Whether a URL is allowed for a question: `hero_ui_web_ask_fn`.
public typealias HeroUiAsk = @convention(c) (Int64, Int64, UnsafePointer<UInt8>?, Int64) -> Int64

/// That a web view is gone: `hero_ui_web_gone_fn`.
public typealias HeroUiGone = @convention(c) (Int64) -> Void

// A window of the program's: AppKit gives the keys to a window with no
// title bar only where it says it can take them, and a window the program
// takes its frame from (`hero_ui_set_framed`) must keep its text fields.
private final class KeyedWindow: NSWindow {
    override var canBecomeKey: Bool {
        return true
    }

    override var canBecomeMain: Bool {
        return true
    }
}

// A window's content: its y axis down, as `hero_ui.h` places widgets.
private final class FlippedView: NSView {
    override var isFlipped: Bool {
        return true
    }
}

// One native widget and what it is made of.
@MainActor
private final class Native {
    let id: Int64
    let kind: Int64
    let parent: Int64
    // The view placed in a window's content: the control itself, or the
    // scroll view around a list or a text area, or a radio group's box.
    var view: NSView?
    var window: NSWindow?
    // A menu's own menu, and the item that holds it in the bar; a menu
    // item's or a separator's item.
    var menu: NSMenu?
    var item: NSMenuItem?
    var table: NSTableView?
    var textView: NSTextView?
    var radios: [NSButton] = []
    var items: [String] = []
    // A window's menus, in order.
    var menus: [Int64] = []
    // A web view's page and what it keeps.
    var page: WebPage?

    init(id: Int64, kind: Int64, parent: Int64) {
        self.id = id
        self.kind = kind
        self.parent = parent
    }
}

// The one native app's widgets and the means to queue what the person does.
@MainActor
private final class Backend: NSObject, NSWindowDelegate, NSTextFieldDelegate, NSTextViewDelegate, NSTableViewDataSource, NSTableViewDelegate {
    static var shared: Backend?
    static var launched = false

    let push: HeroUiPush
    let serve: HeroUiServe
    let ask: HeroUiAsk
    let gone: HeroUiGone
    // The bridge's script, which every web view gives its pages.
    let bridge: String
    var widgets: [Int64: Native] = [:]
    var tables: [ObjectIdentifier: Int64] = [:]
    var words = ""
    var paths: [UInt8] = []
    var pushed = 0
    // Set while the program sets what would otherwise queue an event.
    var muted = false

    init(push: @escaping HeroUiPush, serve: @escaping HeroUiServe, ask: @escaping HeroUiAsk, gone: @escaping HeroUiGone, bridge: String) {
        self.push = push
        self.serve = serve
        self.ask = ask
        self.gone = gone
        self.bridge = bridge
    }

    // Queues a message the page of web view `id` sent, `bytes` its UTF-8.
    func queueMessage(_ id: Int64, _ bytes: [UInt8]) {
        pushed += 1
        bytes.withUnsafeBufferPointer { buffer in
            push(eventMessage, id, 0, 0, buffer.baseAddress, Int64(buffer.count))
        }
    }

    func queue(_ kind: Int64, _ id: Int64, _ value: Int64, _ second: Int64) {
        if muted {
            return
        }
        pushed += 1
        push(kind, id, value, second, nil, 0)
    }

    // MARK: making and destroying

    func create(_ id: Int64, _ kind: Int64, _ parent: Int64) -> Int64 {
        if widgets[id] != nil {
            return answerNotFound
        }
        let made = Native(id: id, kind: kind, parent: parent)
        if kind == kindWindow {
            let window = KeyedWindow(contentRect: NSRect(x: 0, y: 0, width: 100, height: 100), styleMask: [.titled, .closable, .miniaturizable, .resizable], backing: .buffered, defer: false)
            window.isReleasedWhenClosed = false
            window.contentView = FlippedView(frame: NSRect(x: 0, y: 0, width: 100, height: 100))
            window.delegate = self
            made.window = window
            widgets[id] = made
            return answerOk
        }
        if kind == kindMenuItem || kind == kindMenuSeparator {
            // `hero_ui.h`'s two answers: no widget of that number is
            // `NOT_FOUND`, a parent of a kind that holds no item `INVALID`.
            guard let owner = widgets[parent] else {
                return answerNotFound
            }
            guard owner.kind == kindMenu, let menu = owner.menu else {
                return answerInvalid
            }
            let item: NSMenuItem
            if kind == kindMenuSeparator {
                item = NSMenuItem.separator()
            } else {
                item = NSMenuItem(title: "", action: #selector(menuChosen(_:)), keyEquivalent: "")
                item.target = self
                item.tag = Int(id)
            }
            menu.addItem(item)
            made.item = item
            widgets[id] = made
            return answerOk
        }
        guard let owner = widgets[parent] else {
            return answerNotFound
        }
        guard owner.kind == kindWindow, let window = owner.window, let content = window.contentView else {
            return answerInvalid
        }
        if kind == kindWebView {
            guard let page = WebPage.made(id: id, backend: self) else {
                words = "WebKit refused to serve the scheme " + appScheme + " in a web view"
                return answerOther
            }
            page.view.frame = NSRect(x: 0, y: 0, width: 0, height: 0)
            content.addSubview(page.view)
            made.view = page.view
            made.page = page
            widgets[id] = made
            return answerOk
        }
        if kind == kindMenu {
            let menu = NSMenu(title: "")
            menu.autoenablesItems = false
            let item = NSMenuItem(title: "", action: nil, keyEquivalent: "")
            item.submenu = menu
            made.menu = menu
            made.item = item
            owner.menus.append(id)
            widgets[id] = made
            refreshMenuBar()
            return answerOk
        }
        guard let view = viewOf(kind, made) else {
            return answerUnsupported
        }
        view.frame = NSRect(x: 0, y: 0, width: 0, height: 0)
        content.addSubview(view)
        made.view = view
        widgets[id] = made
        return answerOk
    }

    // The view of a widget of `kind`, or nil for a kind this arm does not
    // draw.
    func viewOf(_ kind: Int64, _ widget: Native) -> NSView? {
        switch kind {
        case kindLabel:
            let label = NSTextField(labelWithString: "")
            label.maximumNumberOfLines = 0
            return label
        case kindButton:
            let button = NSButton(title: "", target: self, action: #selector(clicked(_:)))
            button.bezelStyle = .push
            button.tag = Int(widget.id)
            return button
        case kindTextField:
            let field = NSTextField(string: "")
            field.usesSingleLineMode = true
            field.delegate = self
            field.tag = Int(widget.id)
            return field
        case kindTextArea:
            let scroll = NSScrollView(frame: NSRect(x: 0, y: 0, width: areaWidth, height: areaHeight))
            scroll.hasVerticalScroller = true
            scroll.borderType = .bezelBorder
            let text = NSTextView(frame: NSRect(x: 0, y: 0, width: areaWidth, height: areaHeight))
            text.isRichText = false
            // An `NSTextView` made in code keeps no undo of its own until
            // asked, where a text field's field editor keeps one: the Edit
            // menu's Undo and Redo then reach both alike.
            text.allowsUndo = true
            text.autoresizingMask = [.width]
            text.delegate = self
            scroll.documentView = text
            widget.textView = text
            return scroll
        case kindCheckbox:
            let box = NSButton(checkboxWithTitle: "", target: self, action: #selector(toggled(_:)))
            box.tag = Int(widget.id)
            return box
        case kindRadioGroup:
            return FlippedView(frame: NSRect(x: 0, y: 0, width: 0, height: 0))
        case kindList, kindListMultiple:
            let scroll = NSScrollView(frame: NSRect(x: 0, y: 0, width: fieldWidth, height: areaHeight))
            scroll.hasVerticalScroller = true
            scroll.borderType = .bezelBorder
            let table = NSTableView(frame: scroll.bounds)
            let column = NSTableColumn(identifier: NSUserInterfaceItemIdentifier("items"))
            column.resizingMask = .autoresizingMask
            table.addTableColumn(column)
            table.headerView = nil
            table.allowsMultipleSelection = kind == kindListMultiple
            table.allowsEmptySelection = true
            table.dataSource = self
            table.delegate = self
            scroll.documentView = table
            widget.table = table
            tables[ObjectIdentifier(table)] = widget.id
            return scroll
        case kindCombo:
            let popup = NSPopUpButton(frame: NSRect(x: 0, y: 0, width: 0, height: 0), pullsDown: false)
            popup.target = self
            popup.action = #selector(comboChosen(_:))
            popup.tag = Int(widget.id)
            return popup
        case kindProgress:
            let bar = NSProgressIndicator(frame: NSRect(x: 0, y: 0, width: fieldWidth, height: 20))
            bar.style = .bar
            bar.isIndeterminate = false
            bar.minValue = 0
            bar.maxValue = 100
            return bar
        case kindSlider:
            let slider = NSSlider(value: 0, minValue: 0, maxValue: 100, target: self, action: #selector(slid(_:)))
            slider.isContinuous = true
            slider.tag = Int(widget.id)
            return slider
        case kindImage:
            let picture = NSImageView(frame: NSRect(x: 0, y: 0, width: 0, height: 0))
            picture.imageScaling = .scaleNone
            return picture
        default:
            return nil
        }
    }

    func destroy(_ id: Int64) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        for (other, each) in widgets where each.parent == id {
            _ = destroy(other)
        }
        if let window = widget.window {
            window.delegate = nil
            window.orderOut(nil)
            window.close()
        }
        if let table = widget.table {
            tables[ObjectIdentifier(table)] = nil
            table.dataSource = nil
            table.delegate = nil
        }
        if let page = widget.page {
            page.close()
            widget.page = nil
            gone(id)
        }
        widget.view?.removeFromSuperview()
        if let item = widget.item {
            item.menu?.removeItem(item)
        }
        if widget.kind == kindMenu, let owner = widgets[widget.parent] {
            owner.menus.removeAll { $0 == id }
        }
        widgets[id] = nil
        if widget.kind == kindMenu || widget.kind == kindWindow {
            refreshMenuBar()
        }
        return answerOk
    }

    func closeAll() {
        let windows = widgets.values.filter { $0.kind == kindWindow }.map { $0.id }
        for id in windows {
            _ = destroy(id)
        }
        widgets = [:]
        tables = [:]
        NSApp.mainMenu = appMenuBar(menus: [])
    }

    // MARK: the menu bar

    // The application's menu bar: its own menu, then `menus`' items, the
    // standard Edit menu after the first of them (the place Apple's order of
    // a menu bar gives File) or after the application's own where there is
    // none. AppKit hands a key held with Command to the main menu's key
    // equivalents before any widget sees it, so without this menu Command+X,
    // Command+C, Command+V, Command+A and Command+Z reach no text field and
    // no text area, and do nothing.
    func appMenuBar(menus: [NSMenuItem]) -> NSMenu {
        let bar = NSMenu(title: "")
        let appItem = NSMenuItem(title: "", action: nil, keyEquivalent: "")
        let appMenu = NSMenu(title: ProcessInfo.processInfo.processName)
        let quit = NSMenuItem(title: "Quit " + ProcessInfo.processInfo.processName, action: #selector(quitAsked(_:)), keyEquivalent: "q")
        quit.target = self
        appMenu.addItem(quit)
        appItem.submenu = appMenu
        bar.addItem(appItem)
        for item in menus {
            item.menu?.removeItem(item)
            bar.addItem(item)
        }
        bar.insertItem(editMenuItem(), at: menus.isEmpty ? 1 : 2)
        return bar
    }

    // The standard Edit menu, as the item that holds it in the bar. Each
    // item sends AppKit's own action with no target, so it goes to the first
    // responder: the field editor of the text field with the keys, or the
    // text area's `NSTextView`, which cut, copy and paste through the
    // general pasteboard and undo through their window's undo manager. The
    // menu enables its items itself (`autoenablesItems`), so an item no
    // responder takes is greyed, as a person expects.
    func editMenuItem() -> NSMenuItem {
        let edit = NSMenu(title: "Edit")
        // Each item's title, its action (none for a separator), its key and
        // the keys held with it, in the order of every Mac program's menu.
        let entries: [(String, Selector?, String, NSEvent.ModifierFlags)] = [
            ("Undo", Selector(("undo:")), "z", [.command]),
            ("Redo", Selector(("redo:")), "z", [.command, .shift]),
            ("", nil, "", []),
            ("Cut", #selector(NSText.cut(_:)), "x", [.command]),
            ("Copy", #selector(NSText.copy(_:)), "c", [.command]),
            ("Paste", #selector(NSText.paste(_:)), "v", [.command]),
            ("Delete", #selector(NSText.delete(_:)), "", []),
            ("Select All", #selector(NSText.selectAll(_:)), "a", [.command]),
        ]
        for (title, action, key, held) in entries {
            guard let action = action else {
                edit.addItem(NSMenuItem.separator())
                continue
            }
            let item = NSMenuItem(title: title, action: action, keyEquivalent: key)
            item.keyEquivalentModifierMask = held
            edit.addItem(item)
        }
        let holder = NSMenuItem(title: "Edit", action: nil, keyEquivalent: "")
        holder.submenu = edit
        return holder
    }

    // Puts the menus of the window in front into the application's bar.
    func refreshMenuBar() {
        var front: Native?
        for widget in widgets.values where widget.kind == kindWindow {
            if widget.window?.isKeyWindow == true || front == nil {
                front = widget
            }
        }
        var items: [NSMenuItem] = []
        if let window = front {
            for id in window.menus {
                if let item = widgets[id]?.item {
                    items.append(item)
                }
            }
        }
        NSApp.mainMenu = appMenuBar(menus: items)
    }

    // MARK: what the person does

    @objc func clicked(_ sender: NSButton) {
        queue(eventClick, Int64(sender.tag), 0, 0)
    }

    @objc func toggled(_ sender: NSButton) {
        queue(eventToggle, Int64(sender.tag), sender.state == .on ? 1 : 0, 0)
    }

    @objc func radioChosen(_ sender: NSButton) {
        guard let group = widgets[Int64(sender.tag)] else {
            return
        }
        for radio in group.radios {
            radio.state = radio === sender ? .on : .off
        }
        queue(eventSelect, group.id, 0, 0)
    }

    @objc func comboChosen(_ sender: NSPopUpButton) {
        queue(eventSelect, Int64(sender.tag), Int64(sender.indexOfSelectedItem), 0)
    }

    @objc func slid(_ sender: NSSlider) {
        queue(eventValue, Int64(sender.tag), Int64(sender.doubleValue.rounded()), 0)
    }

    @objc func menuChosen(_ sender: NSMenuItem) {
        queue(eventMenu, Int64(sender.tag), 0, 0)
    }

    @objc func quitAsked(_ sender: NSMenuItem) {
        let windows = widgets.values.filter { $0.kind == kindWindow }.map { $0.id }.sorted()
        for id in windows {
            queue(eventClose, id, 0, 0)
        }
    }

    func controlTextDidChange(_ note: Notification) {
        if let field = note.object as? NSTextField {
            queue(eventText, Int64(field.tag), 0, 0)
        }
    }

    func textDidChange(_ note: Notification) {
        guard let text = note.object as? NSTextView else {
            return
        }
        for widget in widgets.values where widget.textView === text {
            queue(eventText, widget.id, 0, 0)
        }
    }

    func windowShouldClose(_ sender: NSWindow) -> Bool {
        if let widget = widgets.values.first(where: { $0.window === sender }) {
            queue(eventClose, widget.id, 0, 0)
        }
        return false
    }

    func windowDidResize(_ note: Notification) {
        guard let window = note.object as? NSWindow, let widget = widgets.values.first(where: { $0.window === window }) else {
            return
        }
        let size = window.contentLayoutRect.size
        queue(eventResize, widget.id, Int64(size.width.rounded()), Int64(size.height.rounded()))
    }

    func windowDidBecomeKey(_ note: Notification) {
        refreshMenuBar()
    }

    func numberOfRows(in table: NSTableView) -> Int {
        guard let id = tables[ObjectIdentifier(table)], let widget = widgets[id] else {
            return 0
        }
        return widget.items.count
    }

    func tableView(_ table: NSTableView, viewFor column: NSTableColumn?, row: Int) -> NSView? {
        guard let id = tables[ObjectIdentifier(table)], let widget = widgets[id], row >= 0, row < widget.items.count else {
            return nil
        }
        let cell = NSTextField(labelWithString: widget.items[row])
        cell.lineBreakMode = .byTruncatingTail
        return cell
    }

    func tableViewSelectionDidChange(_ note: Notification) {
        if let table = note.object as? NSTableView, let id = tables[ObjectIdentifier(table)] {
            queue(eventSelect, id, 0, 0)
        }
    }

    // MARK: what the program sets

    func setText(_ id: Int64, _ text: String) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        switch widget.kind {
        case kindWindow:
            widget.window?.title = text
        case kindLabel, kindTextField:
            (widget.view as? NSTextField)?.stringValue = text
        case kindButton, kindCheckbox:
            (widget.view as? NSButton)?.title = text
        case kindTextArea:
            widget.textView?.string = text
        case kindMenu:
            widget.menu?.title = text
            widget.item?.title = text
        case kindMenuItem:
            widget.item?.title = text
        default:
            return answerInvalid
        }
        return answerOk
    }

    func setItems(_ id: Int64, _ items: [String]) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        widget.items = items
        switch widget.kind {
        case kindList, kindListMultiple:
            muted = true
            widget.table?.reloadData()
            widget.table?.deselectAll(nil)
            muted = false
        case kindCombo:
            guard let popup = widget.view as? NSPopUpButton else {
                return answerInvalid
            }
            popup.removeAllItems()
            for text in items {
                popup.menu?.addItem(NSMenuItem(title: text, action: nil, keyEquivalent: ""))
            }
            popup.selectItem(at: -1)
        case kindRadioGroup:
            for radio in widget.radios {
                radio.removeFromSuperview()
            }
            widget.radios = items.map { text in
                let radio = NSButton(radioButtonWithTitle: text, target: self, action: #selector(radioChosen(_:)))
                radio.tag = Int(id)
                radio.state = .off
                widget.view?.addSubview(radio)
                return radio
            }
        default:
            return answerInvalid
        }
        return answerOk
    }

    func setValue(_ id: Int64, _ value: Int64) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        switch widget.kind {
        case kindCheckbox:
            guard value == 0 || value == 1 else {
                return answerInvalid
            }
            (widget.view as? NSButton)?.state = value == 1 ? .on : .off
        case kindSlider:
            guard let slider = widget.view as? NSSlider, Double(value) >= slider.minValue, Double(value) <= slider.maxValue else {
                return answerInvalid
            }
            slider.doubleValue = Double(value)
        case kindProgress:
            guard let bar = widget.view as? NSProgressIndicator, Double(value) >= bar.minValue, Double(value) <= bar.maxValue else {
                return answerInvalid
            }
            bar.doubleValue = Double(value)
        case kindCombo:
            guard value >= -1, value < Int64(widget.items.count) else {
                return answerInvalid
            }
            (widget.view as? NSPopUpButton)?.selectItem(at: Int(value))
        case kindRadioGroup:
            guard value >= -1, value < Int64(widget.radios.count) else {
                return answerInvalid
            }
            for (index, radio) in widget.radios.enumerated() {
                radio.state = Int64(index) == value ? .on : .off
            }
        default:
            return answerInvalid
        }
        return answerOk
    }

    func setRange(_ id: Int64, _ least: Int64, _ most: Int64) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        if let slider = widget.view as? NSSlider {
            slider.minValue = Double(least)
            slider.maxValue = Double(most)
            return answerOk
        }
        if let bar = widget.view as? NSProgressIndicator {
            bar.minValue = Double(least)
            bar.maxValue = Double(most)
            return answerOk
        }
        return answerInvalid
    }

    func setSelection(_ id: Int64, _ indices: [Int64]) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard let table = widget.table else {
            return answerInvalid
        }
        if indices.count > 1 && widget.kind != kindListMultiple {
            return answerInvalid
        }
        var chosen = IndexSet()
        for index in indices {
            guard index >= 0, index < Int64(widget.items.count) else {
                return answerInvalid
            }
            chosen.insert(Int(index))
        }
        muted = true
        table.selectRowIndexes(chosen, byExtendingSelection: false)
        muted = false
        return answerOk
    }

    func setFlag(_ id: Int64, _ which: Int64, _ on: Bool) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        if which == flagResizable {
            guard let window = widget.window else {
                return answerInvalid
            }
            if on {
                window.styleMask.insert(.resizable)
            } else {
                window.styleMask.remove(.resizable)
            }
            return answerOk
        }
        if which == flagVisible {
            if let window = widget.window {
                if on {
                    window.makeKeyAndOrderFront(nil)
                    NSApp.activate()
                } else {
                    window.orderOut(nil)
                }
            } else if let item = widget.item {
                item.isHidden = !on
            } else {
                widget.view?.isHidden = !on
            }
            return answerOk
        }
        if which == flagEnabled {
            if let item = widget.item {
                item.isEnabled = on
            } else if let table = widget.table {
                table.isEnabled = on
            } else if let text = widget.textView {
                text.isEditable = on
                text.isSelectable = on
            } else if widget.kind == kindRadioGroup {
                for radio in widget.radios {
                    radio.isEnabled = on
                }
            } else if let control = widget.view as? NSControl {
                control.isEnabled = on
            } else if widget.view is NSProgressIndicator {
                // AppKit's progress indicator takes no input and has no
                // greyed state to draw, so there is nothing to change; the
                // call is done, as on every other arm, and the face keeps
                // what the program set.
            } else {
                return answerInvalid
            }
            return answerOk
        }
        return answerInvalid
    }

    func setShortcut(_ id: Int64, _ key: String, _ modifiers: Int64) -> Int64 {
        guard let widget = widgets[id], let item = widget.item, widget.kind == kindMenuItem else {
            return widgets[id] == nil ? answerNotFound : answerInvalid
        }
        guard let equivalent = keyEquivalent(key) else {
            return answerInvalid
        }
        var mask: NSEvent.ModifierFlags = []
        if modifiers & heldPrimary != 0 {
            mask.insert(.command)
        }
        if modifiers & heldShift != 0 {
            mask.insert(.shift)
        }
        if modifiers & heldAlt != 0 {
            mask.insert(.option)
        }
        item.keyEquivalent = equivalent
        item.keyEquivalentModifierMask = mask
        return answerOk
    }

    // The key equivalent AppKit gives the key `hero_ui.h` names, "" for no
    // key, nil for a name it does not list.
    func keyEquivalent(_ key: String) -> String? {
        if key.isEmpty {
            return ""
        }
        if key.utf8.count == 1, let c = key.utf8.first, (c >= 0x61 && c <= 0x7A) || (c >= 0x30 && c <= 0x39) {
            return key
        }
        let named: [String: Int] = [
            "Up": NSUpArrowFunctionKey, "Down": NSDownArrowFunctionKey, "Left": NSLeftArrowFunctionKey, "Right": NSRightArrowFunctionKey,
            "Home": NSHomeFunctionKey, "End": NSEndFunctionKey, "PageUp": NSPageUpFunctionKey, "PageDown": NSPageDownFunctionKey,
            "Delete": NSDeleteFunctionKey, "Enter": 0x0D, "Escape": 0x1B, "Tab": 0x09, "Space": 0x20, "Backspace": 0x08,
        ]
        if let code = named[key], let scalar = UnicodeScalar(code) {
            return String(Character(scalar))
        }
        if key.hasPrefix("F"), let n = Int(key.dropFirst()), n >= 1, n <= 12, let scalar = UnicodeScalar(NSF1FunctionKey + n - 1) {
            return String(Character(scalar))
        }
        return nil
    }

    func setImage(_ id: Int64, _ rgba: UnsafePointer<UInt8>?, _ width: Int64, _ height: Int64) -> Int64 {
        guard let widget = widgets[id], let picture = widget.view as? NSImageView else {
            return widgets[id] == nil ? answerNotFound : answerInvalid
        }
        if width == 0 || height == 0 {
            picture.image = nil
            return answerOk
        }
        guard let rgba = rgba, let rep = NSBitmapImageRep(bitmapDataPlanes: nil, pixelsWide: Int(width), pixelsHigh: Int(height), bitsPerSample: 8, samplesPerPixel: 4, hasAlpha: true, isPlanar: false, colorSpaceName: .deviceRGB, bitmapFormat: [.alphaNonpremultiplied], bytesPerRow: Int(width) * 4, bitsPerPixel: 32), let into = rep.bitmapData else {
            return answerLimit
        }
        into.update(from: rgba, count: Int(width) * Int(height) * 4)
        let image = NSImage(size: NSSize(width: CGFloat(width), height: CGFloat(height)))
        image.addRepresentation(rep)
        picture.image = image
        return answerOk
    }

    func setSize(_ id: Int64, _ which: Int64, _ width: Int64, _ height: Int64) -> Int64 {
        guard let widget = widgets[id], let window = widget.window else {
            return widgets[id] == nil ? answerNotFound : answerInvalid
        }
        let size = NSSize(width: CGFloat(width), height: CGFloat(height))
        if which == sizeLeast {
            window.contentMinSize = size
            return answerOk
        }
        if window.contentLayoutRect.size != size {
            // AppKit keeps a window's bottom-left corner where its size
            // changes; `hero_ui.h` places a window by its top-left corner,
            // so that corner is kept, and a window the program placed stays
            // where it was put when its layout grows it.
            let top = window.frame.maxY
            muted = true
            window.setContentSize(size)
            window.setFrameTopLeftPoint(NSPoint(x: window.frame.minX, y: top))
            muted = false
        }
        return answerOk
    }

    func place(_ id: Int64, _ index: Int64, _ frame: NSRect) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        if index < 0 {
            guard let view = widget.view else {
                return answerInvalid
            }
            view.frame = frame
            if let text = widget.textView, let scroll = view as? NSScrollView {
                text.frame = NSRect(origin: .zero, size: scroll.contentSize)
            }
            return answerOk
        }
        guard widget.kind == kindRadioGroup, index < Int64(widget.radios.count), let group = widget.view else {
            return answerInvalid
        }
        widget.radios[Int(index)].frame = NSRect(x: frame.minX - group.frame.minX, y: frame.minY - group.frame.minY, width: frame.width, height: frame.height)
        return answerOk
    }

    func preferred(_ id: Int64) -> NSSize? {
        guard let widget = widgets[id] else {
            return nil
        }
        switch widget.kind {
        case kindTextField:
            return NSSize(width: fieldWidth, height: (widget.view as? NSTextField)?.intrinsicContentSize.height ?? 22)
        case kindTextArea:
            return NSSize(width: areaWidth, height: areaHeight)
        case kindList, kindListMultiple:
            return NSSize(width: fieldWidth, height: areaHeight)
        case kindProgress, kindSlider:
            let height = widget.view?.intrinsicContentSize.height ?? 20
            return NSSize(width: fieldWidth, height: height > 0 ? height : 20)
        case kindRadioGroup:
            var size = NSSize(width: 0, height: 0)
            for radio in widget.radios {
                let fit = radio.fittingSize
                size.width = max(size.width, fit.width)
                size.height += fit.height
            }
            return size
        case kindImage:
            return (widget.view as? NSImageView)?.image?.size ?? NSSize(width: 0, height: 0)
        case kindWebView:
            return NSSize(width: viewWidth, height: viewHeight)
        default:
            return widget.view?.fittingSize ?? NSSize(width: 0, height: 0)
        }
    }

    // MARK: what the person left

    func text(_ id: Int64) -> String? {
        guard let widget = widgets[id] else {
            return nil
        }
        if let text = widget.textView {
            return text.string
        }
        return (widget.view as? NSTextField)?.stringValue
    }

    func value(_ id: Int64) -> Int64? {
        guard let widget = widgets[id] else {
            return nil
        }
        switch widget.kind {
        case kindCheckbox:
            return (widget.view as? NSButton)?.state == .on ? 1 : 0
        case kindSlider:
            return Int64(((widget.view as? NSSlider)?.doubleValue ?? 0).rounded())
        case kindProgress:
            return Int64(((widget.view as? NSProgressIndicator)?.doubleValue ?? 0).rounded())
        case kindCombo:
            return Int64((widget.view as? NSPopUpButton)?.indexOfSelectedItem ?? -1)
        case kindRadioGroup:
            return Int64(widget.radios.firstIndex { $0.state == .on } ?? -1)
        default:
            return nil
        }
    }

    func selection(_ id: Int64) -> [Int64]? {
        guard let widget = widgets[id] else {
            return nil
        }
        if let table = widget.table {
            return table.selectedRowIndexes.map { Int64($0) }
        }
        if widget.kind == kindCombo || widget.kind == kindRadioGroup {
            let at = value(id) ?? -1
            return at >= 0 ? [at] : []
        }
        return nil
    }

    // MARK: the loop and the person's acts

    func pump(_ ms: Int64) {
        let before = pushed
        let deadline = Date(timeIntervalSinceNow: Double(ms) / 1000)
        while pushed == before {
            guard let event = NSApp.nextEvent(matching: .any, until: deadline, inMode: .default, dequeue: true) else {
                break
            }
            NSApp.sendEvent(event)
        }
        NSApp.updateWindows()
    }

    func inject(_ kind: Int64, _ id: Int64, _ value: Int64, _ second: Int64, _ text: String) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        switch kind {
        case eventClick, eventToggle:
            guard let button = widget.view as? NSButton else {
                return answerInvalid
            }
            button.performClick(nil)
        case eventText:
            if let area = widget.textView {
                if area.isEditable {
                    area.string = text
                    area.didChangeText()
                }
            } else if let field = widget.view as? NSTextField, field.isEnabled {
                field.stringValue = text
                queue(eventText, id, 0, 0)
            } else {
                return answerInvalid
            }
        case eventSelect:
            if let table = widget.table {
                if !table.isEnabled {
                    return answerOk
                }
                if value < 0 {
                    table.deselectAll(nil)
                } else if value < Int64(widget.items.count) {
                    table.selectRowIndexes(IndexSet(integer: Int(value)), byExtendingSelection: second == 1)
                } else {
                    return answerInvalid
                }
            } else if let popup = widget.view as? NSPopUpButton {
                guard value >= 0, value < Int64(widget.items.count) else {
                    return answerInvalid
                }
                if popup.isEnabled {
                    popup.selectItem(at: Int(value))
                    popup.sendAction(popup.action, to: popup.target)
                }
            } else if widget.kind == kindRadioGroup {
                guard value >= 0, value < Int64(widget.radios.count) else {
                    return answerInvalid
                }
                widget.radios[Int(value)].performClick(nil)
            } else {
                return answerInvalid
            }
        case eventValue:
            guard let slider = widget.view as? NSSlider else {
                return answerInvalid
            }
            if slider.isEnabled {
                slider.doubleValue = Double(value)
                slider.sendAction(slider.action, to: slider.target)
            }
        case eventMenu:
            guard let item = widget.item, let menu = item.menu else {
                return answerInvalid
            }
            let index = menu.index(of: item)
            if index >= 0 && item.isEnabled {
                menu.performActionForItem(at: index)
            }
        case eventClose:
            guard let window = widget.window else {
                return answerInvalid
            }
            window.performClose(nil)
        case eventResize:
            guard let window = widget.window else {
                return answerInvalid
            }
            if window.styleMask.contains(.resizable) {
                window.setContentSize(NSSize(width: CGFloat(value), height: CGFloat(second)))
            }
        case eventMessage:
            guard let page = widget.page else {
                return answerInvalid
            }
            if !page.sendAsPage(text) {
                return answerLimit
            }
        default:
            return answerInvalid
        }
        return answerOk
    }

    // MARK: web views

    func webLoad(_ id: Int64, _ url: String) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard let page = widget.page else {
            return answerInvalid
        }
        guard let target = URL(string: url) else {
            words = "WebKit reads no URL in " + url
            return answerOther
        }
        page.load(target)
        return answerOk
    }

    func webPost(_ id: Int64, _ text: String) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard let page = widget.page else {
            return answerInvalid
        }
        return page.post(text) ? answerOk : answerLimit
    }

    // MARK: dialogs

    func message(_ tone: Int64, _ buttons: Int64, _ title: String, _ text: String) -> Int64 {
        let alert = NSAlert()
        alert.messageText = title
        alert.informativeText = text
        alert.alertStyle = tone == toneError ? .critical : (tone == toneWarning ? .warning : .informational)
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
        let response = alert.runModal().rawValue - NSApplication.ModalResponse.alertFirstButtonReturn.rawValue
        if response >= 0 && response < answers.count {
            return answers[response]
        }
        return pressedCancel
    }

    func fileDialog(_ save: Bool, _ multiple: Bool, _ title: String, _ name: String, _ extensions: [String]) -> Int64 {
        var types: [UTType] = []
        for ext in extensions {
            if let type = UTType(filenameExtension: ext) {
                types.append(type)
            }
        }
        let urls: [URL]
        if save {
            let panel = NSSavePanel()
            panel.title = title
            panel.nameFieldStringValue = name
            if !types.isEmpty {
                panel.allowedContentTypes = types
            }
            guard panel.runModal() == .OK, let url = panel.url else {
                paths = []
                return 0
            }
            urls = [url]
        } else {
            let panel = NSOpenPanel()
            panel.title = title
            panel.allowsMultipleSelection = multiple
            panel.canChooseFiles = true
            panel.canChooseDirectories = false
            if !types.isEmpty {
                panel.allowedContentTypes = types
            }
            guard panel.runModal() == .OK else {
                paths = []
                return 0
            }
            urls = panel.urls
        }
        var joined: [UInt8] = []
        for url in urls {
            let path = Array(url.path.utf8)
            if path.contains(0) {
                continue
            }
            joined.append(contentsOf: path)
            joined.append(0)
        }
        paths = joined
        return Int64(joined.count)
    }
}

// What WebKit keeps a strong hold on, its script message handler and its
// scheme handler: a stand-in that reaches the page weakly, so the page and
// its `WKWebView` are given back when the widget is destroyed although the
// view's configuration keeps the stand-in.
@MainActor
private final class WebProxy: NSObject, WKScriptMessageHandler, WKURLSchemeHandler {
    weak var page: WebPage?

    func userContentController(_ controller: WKUserContentController, didReceive message: WKScriptMessage) {
        page?.received(message)
    }

    func webView(_ view: WKWebView, start task: any WKURLSchemeTask) {
        if let page = page {
            page.answer(task)
        } else {
            task.didFailWithError(URLError(.cancelled))
        }
    }

    func webView(_ view: WKWebView, stop task: any WKURLSchemeTask) {
        // Every request is answered whole inside `start`, so none is left
        // to stop.
    }
}

// One web view: its `WKWebView`, whether a page of the app's own origin has
// loaded, and what waits for one: the texts posted, and those `inject` has
// the page send, in the order given, each `asPage` where the page sends it.
@MainActor
private final class WebPage: NSObject, WKNavigationDelegate, WKUIDelegate {
    let id: Int64
    unowned let backend: Backend
    let view: WKWebView
    let proxy: WebProxy
    var ready = false
    var held: [(asPage: Bool, text: String)] = []

    private init(id: Int64, backend: Backend, view: WKWebView, proxy: WebProxy) {
        self.id = id
        self.backend = backend
        self.view = view
        self.proxy = proxy
    }

    // A web view of number `id`, or nil where WebKit already serves the
    // app's scheme itself, which `setURLSchemeHandler` would end the
    // process for.
    static func made(id: Int64, backend: Backend) -> WebPage? {
        if WKWebView.handlesURLScheme(appScheme) {
            return nil
        }
        let proxy = WebProxy()
        let configuration = WKWebViewConfiguration()
        configuration.websiteDataStore = WKWebsiteDataStore.nonPersistent()
        configuration.setURLSchemeHandler(proxy, forURLScheme: appScheme)
        configuration.preferences.javaScriptCanOpenWindowsAutomatically = false
        configuration.preferences.inactiveSchedulingPolicy = .none
        let controller = WKUserContentController()
        controller.addUserScript(WKUserScript(source: backend.bridge, injectionTime: .atDocumentStart, forMainFrameOnly: true))
        controller.add(proxy, name: "heroes")
        configuration.userContentController = controller
        let view = WKWebView(frame: NSRect(x: 0, y: 0, width: 0, height: 0), configuration: configuration)
        view.allowsBackForwardNavigationGestures = false
        view.allowsMagnification = false
        let page = WebPage(id: id, backend: backend, view: view, proxy: proxy)
        proxy.page = page
        view.navigationDelegate = page
        view.uiDelegate = page
        return page
    }

    // Lets go of WebKit before the view is removed: no delegate is called
    // for this page again, and its message handler is taken away.
    func close() {
        view.stopLoading()
        view.navigationDelegate = nil
        view.uiDelegate = nil
        view.configuration.userContentController.removeScriptMessageHandler(forName: "heroes")
        proxy.page = nil
        held = []
        ready = false
    }

    func load(_ url: URL) {
        ready = false
        view.load(URLRequest(url: url))
    }

    // Hands `text` to the page now, or keeps it for the next page of the
    // app's own origin; false where `heldMost` wait already.
    func post(_ text: String) -> Bool {
        return given(asPage: false, text)
    }

    // Has the page's own bridge send `text`, as its script would (`inject`),
    // now or once a page of the app's own origin has loaded.
    func sendAsPage(_ text: String) -> Bool {
        return given(asPage: true, text)
    }

    func given(asPage: Bool, _ text: String) -> Bool {
        if ready {
            run(asPage: asPage, text)
            return true
        }
        if held.count >= heldMost {
            return false
        }
        held.append((asPage: asPage, text: text))
        return true
    }

    // The text handed to the bridge as an argument of a call, never inside
    // the script's own text.
    func run(asPage: Bool, _ text: String) {
        let script = asPage ? "window.heroes.send(t)" : "window.heroes.deliver(t)"
        view.callAsyncJavaScript(script, arguments: ["t": text], in: nil, in: .page, completionHandler: nil)
    }

    // `ask` of `url` for `what`.
    func allowed(_ what: Int64, _ url: String) -> Bool {
        let bytes = Array(url.utf8)
        return bytes.withUnsafeBufferPointer { buffer in
            backend.ask(what, id, buffer.baseAddress, Int64(buffer.count)) == 1
        }
    }

    // MARK: the bridge's messages

    func received(_ message: WKScriptMessage) {
        let origin = message.frameInfo.securityOrigin
        var from = origin.protocol + "://" + origin.host
        if origin.port != 0 {
            from += ":" + String(origin.port)
        }
        guard message.frameInfo.isMainFrame, let text = message.body as? String else {
            _ = allowed(askSender, "")
            return
        }
        if allowed(askSender, from) {
            backend.queueMessage(id, Array(text.utf8))
        }
    }

    // MARK: the app's scheme

    func answer(_ task: any WKURLSchemeTask) {
        guard let url = task.request.url else {
            task.didFailWithError(URLError(.badURL))
            return
        }
        let method = Array((task.request.httpMethod ?? "GET").utf8)
        let whole = Array(url.absoluteString.utf8)
        var numbers = [Int64](repeating: 0, count: servedNumbers)
        var texts = [UnsafePointer<UInt8>?](repeating: nil, count: servedTexts)
        var body = Data()
        var type = ""
        var policy = ""
        method.withUnsafeBufferPointer { methodBytes in
            whole.withUnsafeBufferPointer { urlBytes in
                numbers.withUnsafeMutableBufferPointer { numbersOut in
                    texts.withUnsafeMutableBufferPointer { textsOut in
                        _ = backend.serve(id, methodBytes.baseAddress, Int64(methodBytes.count), urlBytes.baseAddress, Int64(urlBytes.count), numbersOut.baseAddress, textsOut.baseAddress)
                        // Copied now: the layer's bytes live only while
                        // this call runs.
                        type = copied(textsOut[servedType], numbersOut[servedTypeCount])
                        policy = copied(textsOut[servedPolicy], numbersOut[servedPolicyCount])
                        if let bytes = textsOut[servedBody], numbersOut[servedBodyCount] > 0 {
                            body = Data(bytes: bytes, count: Int(numbersOut[servedBodyCount]))
                        }
                    }
                }
            }
        }
        var headers = [
            "Content-Type": type,
            "Content-Length": String(numbers[servedLength]),
            "Content-Security-Policy": policy,
            "X-Content-Type-Options": "nosniff",
            "Cache-Control": "no-store",
            "Referrer-Policy": "no-referrer",
        ]
        if numbers[servedStatus] == 405 {
            headers["Allow"] = "GET, HEAD"
        }
        guard let response = HTTPURLResponse(url: url, statusCode: Int(numbers[servedStatus]), httpVersion: "HTTP/1.1", headerFields: headers) else {
            task.didFailWithError(URLError(.cannotParseResponse))
            return
        }
        task.didReceive(response)
        if !body.isEmpty {
            task.didReceive(body)
        }
        task.didFinish()
    }

    // MARK: where the page may go

    func webView(_ view: WKWebView, decidePolicyFor action: WKNavigationAction, decisionHandler: @escaping @MainActor (WKNavigationActionPolicy) -> Void) {
        guard let frame = action.targetFrame, !action.shouldPerformDownload, let url = action.request.url else {
            decisionHandler(.cancel)
            return
        }
        decisionHandler(allowed(frame.isMainFrame ? askFrame : askSubframe, url.absoluteString) ? .allow : .cancel)
    }

    func webView(_ view: WKWebView, decidePolicyFor response: WKNavigationResponse, decisionHandler: @escaping @MainActor (WKNavigationResponsePolicy) -> Void) {
        decisionHandler(response.canShowMIMEType ? .allow : .cancel)
    }

    func webView(_ view: WKWebView, didStartProvisionalNavigation navigation: WKNavigation!) {
        ready = false
    }

    func webView(_ view: WKWebView, didFinish navigation: WKNavigation!) {
        guard let url = view.url, allowed(askOurs, url.absoluteString) else {
            // A page of another origin receives nothing the program posted.
            ready = false
            held = []
            return
        }
        ready = true
        let waiting = held
        held = []
        for each in waiting {
            run(asPage: each.asPage, each.text)
        }
    }

    func webViewWebContentProcessDidTerminate(_ view: WKWebView) {
        ready = false
        view.reload()
    }

    // MARK: what the page may not do

    func webView(_ view: WKWebView, createWebViewWith configuration: WKWebViewConfiguration, for action: WKNavigationAction, windowFeatures: WKWindowFeatures) -> WKWebView? {
        return nil
    }

    func webView(_ view: WKWebView, requestMediaCapturePermissionFor origin: WKSecurityOrigin, initiatedByFrame frame: WKFrameInfo, type: WKMediaCaptureType, decisionHandler: @escaping @MainActor (WKPermissionDecision) -> Void) {
        decisionHandler(.deny)
    }

    func webView(_ view: WKWebView, runOpenPanelWith parameters: WKOpenPanelParameters, initiatedByFrame frame: WKFrameInfo, completionHandler: @escaping @MainActor ([URL]?) -> Void) {
        completionHandler(nil)
    }
}

// The text of `count` bytes at `bytes`, read as UTF-8.
private func copied(_ bytes: UnsafePointer<UInt8>?, _ count: Int64) -> String {
    guard let bytes = bytes, count > 0 else {
        return ""
    }
    return String(decoding: UnsafeBufferPointer(start: bytes, count: Int(count)), as: UTF8.self)
}

// The texts of `count` bytes at `bytes`, each followed by one zero byte.
private func texts(_ bytes: UnsafePointer<UInt8>?, _ count: Int64) -> [String] {
    guard let bytes = bytes, count > 0 else {
        return []
    }
    let buffer = UnsafeBufferPointer(start: bytes, count: Int(count))
    return buffer.split(separator: 0, omittingEmptySubsequences: false).dropLast().map { String(decoding: $0, as: UTF8.self) }
}

// The text of `count` bytes at `bytes`.
private func text(_ bytes: UnsafePointer<UInt8>?, _ count: Int64) -> String {
    guard let bytes = bytes, count > 0 else {
        return ""
    }
    return String(decoding: UnsafeBufferPointer(start: bytes, count: Int(count)), as: UTF8.self)
}

// The one backend, or nil before `open` and after `close`.
@MainActor
private func backend() -> Backend? {
    return Backend.shared
}

@c
public func hero_ui_appkit_here() -> Int64 {
    return CGSessionCopyCurrentDictionary() == nil ? 0 : 1
}

@c
public func hero_ui_appkit_open(_ push: HeroUiPush?, _ serve: HeroUiServe?, _ ask: HeroUiAsk?, _ gone: HeroUiGone?, _ bridge: UnsafePointer<UInt8>?, _ bridgeCount: Int64) -> Int64 {
    guard let push = push, let serve = serve, let ask = ask, let gone = gone else {
        return answerInvalid
    }
    let script = text(bridge, bridgeCount)
    return MainActor.assumeIsolated { () -> Int64 in
        let app = NSApplication.shared
        _ = app.setActivationPolicy(.regular)
        if !Backend.launched {
            app.finishLaunching()
            Backend.launched = true
        }
        let made = Backend(push: push, serve: serve, ask: ask, gone: gone, bridge: script)
        Backend.shared = made
        app.mainMenu = made.appMenuBar(menus: [])
        return answerOk
    }
}

@c
public func hero_ui_appkit_close() {
    MainActor.assumeIsolated {
        backend()?.closeAll()
        Backend.shared = nil
    }
}

@c
public func hero_ui_appkit_words(_ words: UnsafeMutablePointer<CChar>?, _ count: Int64) -> Int64 {
    guard let words = words, count > 0 else {
        return 0
    }
    let said = MainActor.assumeIsolated { () -> [UInt8] in
        return Array((backend()?.words ?? "").utf8)
    }
    var length = min(said.count, Int(count) - 1)
    while length > 0 && length < said.count && (said[length] & 0xC0) == 0x80 {
        length -= 1
    }
    for at in 0..<length {
        words[at] = CChar(bitPattern: said[at])
    }
    words[length] = 0
    return Int64(length)
}

// `hero_ui_appkit.h`'s `HERO_UI_APPKIT_OBJECT_*`: which object of a widget
// `hero_ui_appkit_object` answers.
private let objectWindow: Int64 = 1
private let objectView: Int64 = 2
private let objectTyped: Int64 = 3
private let objectMenu: Int64 = 4
private let objectItem: Int64 = 5
private let objectOwnWindow: Int64 = 6

// The AppKit object widget `id` stands on, `hero_ui_appkit.h`'s
// `HERO_UI_APPKIT_OBJECT_*`, unretained: what the arm's other Swift
// members act on, each compiled as a module of its own and so unable to
// read `Backend`, which `hero_ui.c` asks here and hands them within one
// call on the main thread. The object lives while `widgets` holds the
// widget, which no call between the asking and the handing changes; nil
// where the widget is none, or holds no such object.
@c
public func hero_ui_appkit_object(_ id: Int64, _ which: Int64) -> UnsafeMutableRawPointer? {
    let found = MainActor.assumeIsolated { () -> AnyObject? in
        guard let widgets = backend()?.widgets, let widget = widgets[id] else {
            return nil
        }
        switch which {
        case objectWindow:
            var at: Native? = widget
            while let each = at {
                if let window = each.window ?? each.view?.window {
                    return window
                }
                at = widgets[each.parent]
            }
            return nil
        case objectView:
            return widget.view
        case objectTyped:
            return widget.textView ?? widget.table ?? widget.view
        case objectMenu:
            return widget.menu
        case objectItem:
            return widget.item
        case objectOwnWindow:
            return widget.window
        default:
            return nil
        }
    }
    guard let object = found else {
        return nil
    }
    return Unmanaged.passUnretained(object).toOpaque()
}

@c
public func hero_ui_appkit_create(_ id: Int64, _ kind: Int64, _ parent: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.create(id, kind, parent) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_destroy(_ id: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.destroy(id) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_set_text(_ id: Int64, _ bytes: UnsafePointer<UInt8>?, _ count: Int64) -> Int64 {
    let said = text(bytes, count)
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setText(id, said) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_set_items(_ id: Int64, _ bytes: UnsafePointer<UInt8>?, _ count: Int64) -> Int64 {
    let items = texts(bytes, count)
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setItems(id, items) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_set_value(_ id: Int64, _ value: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setValue(id, value) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_set_range(_ id: Int64, _ least: Int64, _ most: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setRange(id, least, most) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_set_selection(_ id: Int64, _ indices: UnsafePointer<Int64>?, _ count: Int64) -> Int64 {
    var chosen: [Int64] = []
    if let indices = indices, count > 0 {
        chosen = Array(UnsafeBufferPointer(start: indices, count: Int(count)))
    }
    let given = chosen
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setSelection(id, given) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_set_flag(_ id: Int64, _ which: Int64, _ on: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setFlag(id, which, on == 1) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_set_shortcut(_ id: Int64, _ key: UnsafePointer<UInt8>?, _ count: Int64, _ modifiers: Int64) -> Int64 {
    let named = text(key, count)
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setShortcut(id, named, modifiers) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_set_image(_ id: Int64, _ rgba: UnsafePointer<UInt8>?, _ width: Int64, _ height: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setImage(id, rgba, width, height) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_set_size(_ id: Int64, _ which: Int64, _ width: Int64, _ height: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setSize(id, which, width, height) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_place(_ id: Int64, _ index: Int64, _ x: Int64, _ y: Int64, _ width: Int64, _ height: Int64) -> Int64 {
    let frame = NSRect(x: CGFloat(x), y: CGFloat(y), width: CGFloat(width), height: CGFloat(height))
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.place(id, index, frame) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_preferred(_ id: Int64, _ width: UnsafeMutablePointer<Int64>?, _ height: UnsafeMutablePointer<Int64>?) -> Int64 {
    guard let width = width, let height = height else {
        return answerInvalid
    }
    let size = MainActor.assumeIsolated { () -> NSSize? in
        return backend()?.preferred(id)
    }
    guard let found = size else {
        return answerNotFound
    }
    width.pointee = Int64(found.width.rounded(.up))
    height.pointee = Int64(found.height.rounded(.up))
    return answerOk
}

@c
public func hero_ui_appkit_text(_ id: Int64, _ buffer: UnsafeMutablePointer<UInt8>?, _ count: Int64, _ length: UnsafeMutablePointer<Int64>?) -> Int64 {
    guard let length = length else {
        return answerInvalid
    }
    let (held, read) = MainActor.assumeIsolated { () -> (Bool, [UInt8]?) in
        guard let backend = backend(), backend.widgets[id] != nil else {
            return (false, nil)
        }
        return (true, backend.text(id).map { Array($0.utf8) })
    }
    // A widget the native side does not hold is `NOT_FOUND`, one of a kind
    // that holds no text `INVALID` (`hero_ui.h`).
    guard held else {
        return answerNotFound
    }
    guard let bytes = read else {
        return answerInvalid
    }
    length.pointee = Int64(bytes.count)
    if let buffer = buffer, bytes.count <= Int(count) {
        for at in 0..<bytes.count {
            buffer[at] = bytes[at]
        }
    }
    return answerOk
}

@c
public func hero_ui_appkit_value(_ id: Int64, _ value: UnsafeMutablePointer<Int64>?) -> Int64 {
    guard let value = value else {
        return answerInvalid
    }
    let (held, read) = MainActor.assumeIsolated { () -> (Bool, Int64?) in
        guard let backend = backend(), backend.widgets[id] != nil else {
            return (false, nil)
        }
        return (true, backend.value(id))
    }
    // A widget the native side does not hold is `NOT_FOUND`, one of a kind
    // that holds no value `INVALID` (`hero_ui.h`).
    guard held else {
        return answerNotFound
    }
    guard let found = read else {
        return answerInvalid
    }
    value.pointee = found
    return answerOk
}

@c
public func hero_ui_appkit_selection(_ id: Int64, _ indices: UnsafeMutablePointer<Int64>?, _ count: Int64, _ chosen: UnsafeMutablePointer<Int64>?) -> Int64 {
    guard let chosen = chosen else {
        return answerInvalid
    }
    let (held, read) = MainActor.assumeIsolated { () -> (Bool, [Int64]?) in
        guard let backend = backend(), backend.widgets[id] != nil else {
            return (false, nil)
        }
        return (true, backend.selection(id))
    }
    // A widget the native side does not hold is `NOT_FOUND`, one of a kind
    // that chooses nothing `INVALID` (`hero_ui.h`).
    guard held else {
        return answerNotFound
    }
    guard let found = read else {
        return answerInvalid
    }
    chosen.pointee = Int64(found.count)
    if let indices = indices, found.count <= Int(count) {
        for at in 0..<found.count {
            indices[at] = found[at]
        }
    }
    return answerOk
}

@c
public func hero_ui_appkit_pump(_ ms: Int64) -> Int64 {
    MainActor.assumeIsolated {
        backend()?.pump(ms)
    }
    return answerOk
}

@c
public func hero_ui_appkit_inject(_ kind: Int64, _ id: Int64, _ value: Int64, _ second: Int64, _ bytes: UnsafePointer<UInt8>?, _ count: Int64) -> Int64 {
    let typed = text(bytes, count)
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.inject(kind, id, value, second, typed) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_message(_ window: Int64, _ tone: Int64, _ buttons: Int64, _ title: UnsafePointer<UInt8>?, _ titleCount: Int64, _ bytes: UnsafePointer<UInt8>?, _ count: Int64, _ answer: UnsafeMutablePointer<Int64>?) -> Int64 {
    guard let answer = answer else {
        return answerInvalid
    }
    let heading = text(title, titleCount)
    let said = text(bytes, count)
    let pressed = MainActor.assumeIsolated { () -> Int64? in
        _ = window
        return backend()?.message(tone, buttons, heading, said)
    }
    guard let found = pressed else {
        return answerNotFound
    }
    answer.pointee = found
    return answerOk
}

@c
public func hero_ui_appkit_file_dialog(_ window: Int64, _ save: Int64, _ multiple: Int64, _ title: UnsafePointer<UInt8>?, _ titleCount: Int64, _ name: UnsafePointer<UInt8>?, _ nameCount: Int64, _ filters: UnsafePointer<UInt8>?, _ filtersCount: Int64, _ bytes: UnsafeMutablePointer<Int64>?) -> Int64 {
    guard let bytes = bytes else {
        return answerInvalid
    }
    let heading = text(title, titleCount)
    let proposed = text(name, nameCount)
    let spelled = texts(filters, filtersCount)
    var extensions: [String] = []
    var at = 1
    while at < spelled.count {
        extensions.append(contentsOf: spelled[at].split(separator: ";").map { String($0) })
        at += 2
    }
    let found = extensions
    let got = MainActor.assumeIsolated { () -> Int64? in
        _ = window
        return backend()?.fileDialog(save == 1, multiple == 1, heading, proposed, found)
    }
    guard let length = got else {
        return answerNotFound
    }
    bytes.pointee = length
    return answerOk
}

@c
public func hero_ui_appkit_dialog_paths(_ buffer: UnsafeMutablePointer<UInt8>?, _ count: Int64) -> Int64 {
    let kept = MainActor.assumeIsolated { () -> [UInt8] in
        return backend()?.paths ?? []
    }
    if let buffer = buffer, kept.count <= Int(count) {
        for at in 0..<kept.count {
            buffer[at] = kept[at]
        }
    }
    return Int64(kept.count)
}

@c
public func hero_ui_appkit_web_load(_ id: Int64, _ url: UnsafePointer<UInt8>?, _ count: Int64) -> Int64 {
    let target = text(url, count)
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.webLoad(id, target) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_web_post(_ id: Int64, _ bytes: UnsafePointer<UInt8>?, _ count: Int64) -> Int64 {
    let posted = text(bytes, count)
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.webPost(id, posted) ?? answerNotFound
    }
}
