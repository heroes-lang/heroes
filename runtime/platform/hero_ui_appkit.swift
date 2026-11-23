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
private let eventActivate: Int64 = 13
private let eventSubmit: Int64 = 14
private let eventDrop: Int64 = 20

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

// A list's table, which tells the Return key pressed while it has the keys
// and a row chosen as that row opened, as a double click on it is: AppKit's
// table sends its `doubleAction` for the click alone.
private final class ActivatingTable: NSTableView {
    var returned: ((Int) -> Void)?

    override func keyDown(with event: NSEvent) {
        // The Return key and the keypad's Enter, HIToolbox's `kVK_Return`
        // and `kVK_ANSI_KeypadEnter`.
        if (event.keyCode == 0x24 || event.keyCode == 0x4C), let first = selectedRowIndexes.first, let returned = returned {
            returned(first)
            return
        }
        super.keyDown(with: event)
    }
}

// A window's content: its y axis down, as `hero_ui.h` places widgets.
// A text area's text view, which hands a drag of files to its window where
// the window takes them (`hero_ui_set_drop`), so a drop anywhere in an
// editor's window opens its files rather than typing their paths; any
// other drag is the text view's own.
@MainActor
private final class AreaText: NSTextView {
    override func draggingEntered(_ sender: NSDraggingInfo) -> NSDragOperation {
        if let shared = Backend.shared, shared.takesDrop(sender) {
            return .copy
        }
        return super.draggingEntered(sender)
    }

    override func draggingUpdated(_ sender: NSDraggingInfo) -> NSDragOperation {
        if let shared = Backend.shared, shared.takesDrop(sender) {
            return .copy
        }
        return super.draggingUpdated(sender)
    }

    override func prepareForDragOperation(_ sender: NSDraggingInfo) -> Bool {
        if let shared = Backend.shared, shared.takesDrop(sender) {
            return true
        }
        return super.prepareForDragOperation(sender)
    }

    override func performDragOperation(_ sender: NSDraggingInfo) -> Bool {
        if let shared = Backend.shared, shared.takesDrop(sender) {
            return shared.dropped(sender)
        }
        return super.performDragOperation(sender)
    }

    override func concludeDragOperation(_ sender: NSDraggingInfo?) {
        if let sender = sender, let shared = Backend.shared, shared.takesDrop(sender) {
            return
        }
        super.concludeDragOperation(sender)
    }
}

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
    // A menu item's role, `hero_ui.h`'s `HERO_UI_ROLE_*`: 0 for none.
    var role: Int64 = 0
    // A menu item's check, `hero_ui.h`'s `HERO_UI_CHECK_*`: 0 for none.
    var check: Int64 = 0
    // A web view's page and what it keeps.
    var page: WebPage?
    // A tray icon's status item in the menu bar.
    var status: NSStatusItem?
    // Whether a window takes files dropped on it (`hero_ui_set_drop`).
    var dropping = false
    // A text area's own undo, so its edits are undone apart from every
    // other widget's; made at its first edit.
    var undo: UndoManager?
    // A text field's selection, in bytes of its text, kept when it last
    // gave the keys up; nil for its text's end.
    var fieldRange: (Int64, Int64)?

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
        if kind == kindTray {
            return makeTray(made)
        }
        if kind == kindWindow {
            let window = KeyedWindow(contentRect: NSRect(x: 0, y: 0, width: 100, height: 100), styleMask: [.titled, .closable, .miniaturizable, .resizable], backing: .buffered, defer: false)
            window.isReleasedWhenClosed = false
            // Tab and Shift+Tab go through the widgets in the order they
            // stand, left to right and top to bottom, which is the order the
            // layout reads them in, so a keyboard reaches each in turn.
            window.autorecalculatesKeyViewLoop = true
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
            guard owner.kind == kindMenu || owner.kind == kindContextMenu, let menu = owner.menu else {
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
            // A menu the bar left out, every item of it standing in the
            // application's menu by its role, has an item to show again.
            if owner.item?.isHidden == true {
                refreshMenuBar()
            }
            return answerOk
        }
        guard let owner = widgets[parent] else {
            return answerNotFound
        }
        // A menu inside a menu: its item holds it among the outer menu's
        // items, in the order made.
        if kind == kindMenu && (owner.kind == kindMenu || owner.kind == kindContextMenu), let outer = owner.menu {
            let menu = NSMenu(title: "")
            menu.autoenablesItems = false
            let item = NSMenuItem(title: "", action: nil, keyEquivalent: "")
            item.submenu = menu
            outer.addItem(item)
            made.menu = menu
            made.item = item
            widgets[id] = made
            return answerOk
        }
        if let answer = createBox(id, kind, owner, made) {
            return answer
        }
        guard let content = contentOf(owner) else {
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
            // A text field sends its action when the person presses Return
            // in it (`HERO_UI_EVENT_SUBMIT`), and not when its editing ends
            // another way: AppKit's own default sends it too when the field
            // leaves the window while it is edited, its page hidden by a
            // tab chosen, as measured on this Mac.
            field.cell?.sendsActionOnEndEditing = false
            field.target = self
            field.action = #selector(submitted(_:))
            return field
        case kindTextArea:
            let scroll = NSScrollView(frame: NSRect(x: 0, y: 0, width: areaWidth, height: areaHeight))
            scroll.hasVerticalScroller = true
            scroll.borderType = .bezelBorder
            let text = AreaText(frame: NSRect(x: 0, y: 0, width: areaWidth, height: areaHeight))
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
            let table = ActivatingTable(frame: scroll.bounds)
            let column = NSTableColumn(identifier: NSUserInterfaceItemIdentifier("items"))
            column.resizingMask = .autoresizingMask
            table.addTableColumn(column)
            table.headerView = nil
            table.allowsMultipleSelection = kind == kindListMultiple
            table.allowsEmptySelection = true
            table.dataSource = self
            table.delegate = self
            table.target = self
            table.doubleAction = #selector(rowDoubleClicked(_:))
            table.returned = { [weak self, weak table] row in
                if let table = table {
                    self?.activated(table, row)
                }
            }
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
        if widget.kind == kindPage {
            removePage(widget)
        }
        if widget.kind == kindTree {
            forests[id] = nil
        }
        if widget.kind == kindContextMenu, let owner = widgets[widget.parent], let target = contextTarget(owner), target.menu === widget.menu {
            target.menu = nil
        }
        if widget.kind == kindContextMenu, let status = widgets[widget.parent]?.status, status.menu === widget.menu {
            status.menu = nil
        }
        if let status = widget.status {
            NSStatusBar.system.removeStatusItem(status)
            widget.status = nil
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
    func appMenuBar(menus: [NSMenuItem], roles: [Int64: Native] = [:]) -> NSMenu {
        let bar = NSMenu(title: "")
        let appItem = NSMenuItem(title: "", action: nil, keyEquivalent: "")
        appItem.submenu = appMenu(roles)
        bar.addItem(appItem)
        for item in menus {
            item.menu?.removeItem(item)
            // A menu whose every item a role took to the application's menu
            // stands empty; it is left out of the bar.
            if let menu = item.submenu {
                item.isHidden = !menu.items.isEmpty && menu.items.allSatisfy { $0.isHidden }
            }
            bar.addItem(item)
        }
        bar.insertItem(editMenuItem(), at: menus.isEmpty ? 1 : 2)
        return bar
    }

    // The application's menu, as every Mac program has it: About, Settings
    // where the program gave an item that role, Services, Hide, Hide Others,
    // Show All and Quit (Apple's Human Interface Guidelines, The menu bar,
    // the app menu). An item of the program's own with a role
    // (`HERO_UI_ROLE_*`) stands here in the place of the system's, its
    // choosing told as the item's `HERO_UI_EVENT_MENU`, and is hidden in the
    // menu the program put it in; without one, About shows the system's own
    // panel and Quit asks every window to close. Hide, Hide Others, Show All
    // and Services are the system's, and tell the program nothing.
    func appMenu(_ roles: [Int64: Native]) -> NSMenu {
        let name = ProcessInfo.processInfo.processName
        let menu = NSMenu(title: name)
        menu.autoenablesItems = false
        if let about = roles[roleAbout], let item = about.item {
            menu.addItem(proxy(item, about.id, item.keyEquivalent))
        } else {
            menu.addItem(withTitle: "About " + name, action: #selector(NSApplication.orderFrontStandardAboutPanel(_:)), keyEquivalent: "").target = NSApp
        }
        if let settings = roles[roleSettings], let item = settings.item {
            menu.addItem(NSMenuItem.separator())
            // Command+, is every Mac program's key for its settings, given
            // where the program gave the item none of its own.
            let made = proxy(item, settings.id, item.keyEquivalent.isEmpty ? "," : item.keyEquivalent)
            if item.keyEquivalent.isEmpty {
                made.keyEquivalentModifierMask = [.command]
            }
            menu.addItem(made)
        }
        menu.addItem(NSMenuItem.separator())
        let services = NSMenuItem(title: "Services", action: nil, keyEquivalent: "")
        let servicesMenu = NSMenu(title: "Services")
        services.submenu = servicesMenu
        NSApp.servicesMenu = servicesMenu
        menu.addItem(services)
        menu.addItem(NSMenuItem.separator())
        menu.addItem(withTitle: "Hide " + name, action: #selector(NSApplication.hide(_:)), keyEquivalent: "h").target = NSApp
        let others = menu.addItem(withTitle: "Hide Others", action: #selector(NSApplication.hideOtherApplications(_:)), keyEquivalent: "h")
        others.keyEquivalentModifierMask = [.command, .option]
        others.target = NSApp
        menu.addItem(withTitle: "Show All", action: #selector(NSApplication.unhideAllApplications(_:)), keyEquivalent: "").target = NSApp
        menu.addItem(NSMenuItem.separator())
        if let quit = roles[roleQuit], let item = quit.item {
            menu.addItem(proxy(item, quit.id, item.keyEquivalent.isEmpty ? "q" : item.keyEquivalent))
        } else {
            let made = NSMenuItem(title: "Quit " + name, action: #selector(quitAsked(_:)), keyEquivalent: "q")
            made.target = self
            menu.addItem(made)
        }
        return menu
    }

    // An item of the application's menu standing for the program's `item`,
    // widget `id`: its title, its key, its state; choosing it is choosing
    // the program's item.
    func proxy(_ item: NSMenuItem, _ id: Int64, _ key: String) -> NSMenuItem {
        let made = NSMenuItem(title: item.title, action: #selector(menuChosen(_:)), keyEquivalent: key)
        made.keyEquivalentModifierMask = item.keyEquivalentModifierMask
        made.target = self
        made.tag = Int(id)
        made.isEnabled = item.isEnabled
        return made
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
        var roles: [Int64: Native] = [:]
        if let window = front {
            for id in window.menus {
                if let item = widgets[id]?.item {
                    items.append(item)
                }
            }
            // The first item of each role, by its number, in the window's
            // menus.
            for widget in widgets.values.sorted(by: { $0.id < $1.id }) where widget.role != 0 && roles[widget.role] == nil {
                if let menu = widgets[widget.parent], window.menus.contains(menu.id) {
                    roles[widget.role] = widget
                }
            }
        }
        NSApp.mainMenu = appMenuBar(menus: items, roles: roles)
    }

    // MARK: what the person does

    @objc func clicked(_ sender: NSButton) {
        queue(eventClick, Int64(sender.tag), 0, 0)
    }

    @objc func submitted(_ sender: NSTextField) {
        queue(eventSubmit, Int64(sender.tag), 0, 0)
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

    // A row double-clicked: the row under the click, nothing where the
    // click fell past the last row.
    @objc func rowDoubleClicked(_ sender: NSTableView) {
        if sender.clickedRow >= 0 {
            activated(sender, sender.clickedRow)
        }
    }

    // Row `row` of `table` opened by the person (`HERO_UI_EVENT_ACTIVATE`).
    func activated(_ table: NSTableView, _ row: Int) {
        if let id = tables[ObjectIdentifier(table)], table.isEnabled {
            queue(eventActivate, id, Int64(row), 0)
        }
    }

    // A menu item chosen: a check item turns over, a radio item is checked
    // and the other radio items of its group unchecked, before its event,
    // whose value is the item's state after it.
    @objc func menuChosen(_ sender: NSMenuItem) {
        let id = Int64(sender.tag)
        guard let widget = widgets[id] else {
            return
        }
        if widget.check == checkBox, let item = widget.item {
            item.state = item.state == .on ? .off : .on
        } else if widget.check == checkRadio {
            radioChecked(widget)
        }
        queue(eventMenu, id, widget.item?.state == .on ? 1 : 0, 0)
    }

    @objc func quitAsked(_ sender: NSMenuItem) {
        let windows = widgets.values.filter { $0.kind == kindWindow }.map { $0.id }.sorted()
        for id in windows {
            queue(eventClose, id, 0, 0)
        }
    }

    // A text field giving the keys up keeps its selection, which the
    // system's field shows no longer.
    func controlTextDidEndEditing(_ note: Notification) {
        guard let field = note.object as? NSTextField, let widget = widgets[Int64(field.tag)], widget.kind == kindTextField, let editor = note.userInfo?["NSFieldEditor"] as? NSText else {
            return
        }
        widget.fieldRange = bytesOf(editor.string, editor.selectedRange)
    }

    // A text area's own undo, made at its first edit.
    func undoManager(for view: NSTextView) -> UndoManager? {
        guard let widget = widgets.values.first(where: { $0.textView === view }) else {
            return nil
        }
        return areaUndo(widget)
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
        return rowsOf(widget)
    }

    func tableView(_ table: NSTableView, viewFor column: NSTableColumn?, row: Int) -> NSView? {
        guard let id = tables[ObjectIdentifier(table)], let widget = widgets[id], row >= 0, row < rowsOf(widget) else {
            return nil
        }
        let at = cellAt(widget, table, column, row)
        guard at >= 0, at < widget.items.count else {
            return nil
        }
        let cell = NSTextField(labelWithString: widget.items[at])
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
        case kindTray:
            setTrayText(widget, text)
        case kindWindow:
            widget.window?.title = text
        case kindPage:
            if let tabs = widgets[widget.parent]?.view as? NSTabView {
                for item in tabs.tabViewItems where item.view === widget.view {
                    item.label = text
                }
            }
        case kindLabel, kindTextField:
            (widget.view as? NSTextField)?.stringValue = text
            if widget.kind == kindTextField {
                widget.fieldRange = nil
                if let field = widget.view as? NSTextField, let editor = field.currentEditor() {
                    editor.undoManager?.removeAllActions()
                    editor.selectedRange = NSRange(location: (editor.string as NSString).length, length: 0)
                }
            }
        case kindButton, kindCheckbox:
            (widget.view as? NSButton)?.title = text
        case kindTextArea:
            if let area = widget.textView {
                area.string = text
                // The program's text is where undo stops, its caret at
                // its end.
                widget.undo?.removeAllActions()
                area.setSelectedRange(NSRange(location: (text as NSString).length, length: 0))
            }
        case kindMenu:
            widget.menu?.title = text
            widget.item?.title = text
        case kindMenuItem:
            widget.item?.title = text
            if widget.role != roleNone {
                refreshMenuBar()
            }
        default:
            return answerInvalid
        }
        return answerOk
    }

    func setItems(_ id: Int64, _ items: [String]) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        if widget.kind == kindTable, let table = widget.table, items.count % max(table.tableColumns.count, 1) != 0 {
            return answerInvalid
        }
        widget.items = items
        switch widget.kind {
        case kindList, kindListMultiple, kindTable:
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
        if widget.kind == kindSplitRow || widget.kind == kindSplitColumn {
            return setDivider(widget, value)
        }
        if widget.kind == kindMenuItem {
            guard widget.check != checkNone, value == 0 || value == 1, let item = widget.item else {
                return answerInvalid
            }
            if widget.check == checkRadio && value == 1 {
                radioChecked(widget)
            } else {
                item.state = value == 1 ? .on : .off
            }
            return answerOk
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
        if widget.kind == kindSplitRow || widget.kind == kindSplitColumn {
            return setDividerRange(widget, least, most)
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
        if widget.kind == kindTabs {
            return setPage(widget, indices)
        }
        if widget.kind == kindTree {
            return treeChoose(widget, indices, true)
        }
        guard let table = widget.table else {
            return answerInvalid
        }
        if indices.count > 1 && widget.kind != kindListMultiple && widget.kind != kindTable {
            return answerInvalid
        }
        var chosen = IndexSet()
        for index in indices {
            guard index >= 0, index < Int64(rowsOf(widget)) else {
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
                if widget.role != roleNone {
                    refreshMenuBar()
                }
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
        if widget.role != roleNone {
            refreshMenuBar()
        }
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
        if let tray = widgets[id], tray.kind == kindTray {
            return setTrayImage(tray, rgba, width, height)
        }
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
        // A scrolled box's size is its content's.
        if let widget = widgets[id], widget.kind == kindScroll {
            guard which != sizeLeast, let document = (widget.view as? NSScrollView)?.documentView else {
                return answerInvalid
            }
            document.setFrameSize(NSSize(width: CGFloat(width), height: CGFloat(height)))
            return answerOk
        }
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
        // The system sizes a page and a pane itself.
        if widget.kind == kindPage || widget.kind == kindPane {
            return answerInvalid
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
        if let size = boxPreferred(widget) ?? tablePreferred(widget) {
            return size
        }
        if widget.kind == kindTree {
            return NSSize(width: fieldWidth, height: areaHeight)
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
        if let split = widget.view as? NSSplitView, widget.kind == kindSplitRow || widget.kind == kindSplitColumn {
            return dividerAt(split)
        }
        if widget.kind == kindMenuItem && widget.check != checkNone {
            return widget.item?.state == .on ? 1 : 0
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
        if widget.kind == kindTabs {
            return pageShown(widget)
        }
        if widget.kind == kindTree {
            return treeChosen(widget)
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
        if let answer = injectBox(kind, widget, value) {
            return answer
        }
        if kind == eventSort {
            return widget.kind == kindTable ? injectSort(widget, value, second == 1) : answerInvalid
        }
        if let answer = injectTree(kind, widget, value, second) {
            return answer
        }
        switch kind {
        case eventClick, eventToggle:
            guard let button = widget.view as? NSButton else {
                return answerInvalid
            }
            button.performClick(nil)
        case eventText:
            if let area = widget.textView {
                // Typed as the person's edit: one undo of its own, the
                // text replaced whole through the text view's input.
                if area.isEditable {
                    area.breakUndoCoalescing()
                    area.insertText(text, replacementRange: NSRange(location: 0, length: (area.string as NSString).length))
                    area.breakUndoCoalescing()
                    // The undo groups what is registered in one pass of the
                    // loop, which a person's keys each have and injected
                    // edits given at one wait share: a pass of its own
                    // closes this edit's group (measured 2026-11-22, a
                    // zero-time pass brings the grouping level to 0; ending
                    // the group by hand raises).
                    if let manager = widget.undo, manager.groupingLevel > 0 {
                        _ = CFRunLoopRunInMode(.defaultMode, 0, true)
                    }
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
                } else if value < Int64(rowsOf(widget)) {
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
        case eventSubmit:
            guard widget.kind == kindTextField, let field = widget.view as? NSTextField else {
                return answerInvalid
            }
            // Return goes to the window's default button before the field,
            // as AppKit gives the person's Return.
            if field.isEnabled {
                if let button = field.window?.defaultButtonCell?.controlView as? NSButton, button.isEnabled, !button.isHiddenOrHasHiddenAncestor {
                    button.performClick(nil)
                } else {
                    field.sendAction(field.action, to: field.target)
                }
            }
        case eventActivate:
            guard let table = widget.table, value >= 0, value < Int64(rowsOf(widget)) else {
                return answerInvalid
            }
            if table.isEnabled {
                // The first click chooses the row, as the person's does.
                if !table.selectedRowIndexes.contains(Int(value)) {
                    table.selectRowIndexes(IndexSet(integer: Int(value)), byExtendingSelection: false)
                }
                activated(table, Int(value))
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

// The keys a button may be pressed by, `hero_ui.h`'s `HERO_UI_BUTTON_KEY_*`,
// and the key equivalent AppKit gives each.
private let buttonKeyNone: Int64 = 0
private let buttonKeyEnter: Int64 = 1
private let buttonKeyEscape: Int64 = 2
private let enterEquivalent = "\r"
private let escapeEquivalent = "\u{1b}"

extension Backend {
    // Lets `key` press button `id` wherever its window has the keys: Return
    // as the window's default button, which AppKit draws as such, or Escape
    // as its Cancel. A window has one button of each; the one that had the
    // key before loses it.
    func setButtonKey(_ id: Int64, _ key: Int64) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard widget.kind == kindButton, let button = widget.view as? NSButton, key >= buttonKeyNone, key <= buttonKeyEscape else {
            return answerInvalid
        }
        let window = windowOf(widget)?.window
        let equivalent = key == buttonKeyEnter ? enterEquivalent : (key == buttonKeyEscape ? escapeEquivalent : "")
        if !equivalent.isEmpty {
            for other in widgets.values where other.kind == kindButton && other.id != id && windowOf(other)?.id == windowOf(widget)?.id {
                if let held = other.view as? NSButton, held.keyEquivalent == equivalent {
                    held.keyEquivalent = ""
                }
            }
        }
        if button.keyEquivalent == enterEquivalent && window?.defaultButtonCell === button.cell {
            window?.defaultButtonCell = nil
        }
        button.keyEquivalent = equivalent
        button.keyEquivalentModifierMask = []
        if key == buttonKeyEnter, let cell = button.cell as? NSButtonCell {
            window?.defaultButtonCell = cell
        }
        return answerOk
    }
}

@c
public func hero_ui_appkit_set_button_key(_ id: Int64, _ key: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setButtonKey(id, key) ?? answerNotFound
    }
}

extension Backend {
    // Gives widget `id` the keys in its window: the view the person types
    // into or chooses in. AppKit gives it the keys when its window is key.
    func setFocus(_ id: Int64) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        let typed: NSView? = widget.textView ?? widget.table ?? (widget.kind == kindTree ? outlineOf(widget) : nil) ?? widget.view
        guard [kindTextField, kindTextArea, kindList, kindListMultiple, kindTable, kindTree].contains(widget.kind), let view = typed, let window = view.window, view.acceptsFirstResponder else {
            return answerInvalid
        }
        return window.makeFirstResponder(view) ? answerOk : answerInvalid
    }

    // The widget of window `id` that has the keys, `id` itself where none
    // of its widgets has them: a text field's keys are its window's field
    // editor's, whose delegate is the field.
    func focus(_ id: Int64) -> Int64? {
        guard let widget = widgets[id], let window = widget.window else {
            return nil
        }
        guard let responder = window.firstResponder else {
            return id
        }
        let field = (responder as? NSTextView)?.delegate as? NSTextField
        for other in widgets.values where other.kind != kindWindow && windowOf(other)?.id == id {
            if other.view === responder || other.textView === responder || other.table === responder || (other.kind == kindTree && outlineOf(other) === responder) || (field != nil && other.view === field) {
                return other.id
            }
        }
        return id
    }
}

@c
public func hero_ui_appkit_set_focus(_ id: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setFocus(id) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_focus(_ id: Int64, _ focus: UnsafeMutablePointer<Int64>?) -> Int64 {
    guard let focus = focus else {
        return answerInvalid
    }
    let (held, found) = MainActor.assumeIsolated { () -> (Bool, Int64?) in
        guard let backend = backend(), backend.widgets[id] != nil else {
            return (false, nil)
        }
        return (true, backend.focus(id))
    }
    guard held else {
        return answerNotFound
    }
    guard let answer = found else {
        return answerInvalid
    }
    focus.pointee = answer
    return answerOk
}

extension Backend {
    // Masks text field `id`'s text, `on`, or shows it: AppKit masks a field
    // of its own class, `NSSecureTextField`, so the field's view is
    // replaced by one of the other class in its place, with its text, its
    // frame, its state and the program's hooks kept.
    func setMasked(_ id: Int64, _ on: Bool) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard widget.kind == kindTextField, let old = widget.view as? NSTextField, let parent = old.superview else {
            return answerInvalid
        }
        if (old is NSSecureTextField) == on {
            return answerOk
        }
        let made: NSTextField = on ? NSSecureTextField(string: old.stringValue) : NSTextField(string: old.stringValue)
        made.usesSingleLineMode = true
        made.frame = old.frame
        made.isEnabled = old.isEnabled
        made.isHidden = old.isHidden
        made.delegate = self
        made.tag = old.tag
        made.cell?.sendsActionOnEndEditing = false
        made.target = self
        made.action = #selector(submitted(_:))
        parent.replaceSubview(old, with: made)
        widget.view = made
        return answerOk
    }

    // Lets progress bar `id` move with no value, `on`, while the work's end
    // is unknown, or shows its value again.
    func setMoving(_ id: Int64, _ on: Bool) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard widget.kind == kindProgress, let bar = widget.view as? NSProgressIndicator else {
            return answerInvalid
        }
        bar.isIndeterminate = on
        if on {
            bar.startAnimation(nil)
        } else {
            bar.stopAnimation(nil)
        }
        return answerOk
    }
}

@c
public func hero_ui_appkit_set_masked(_ id: Int64, _ on: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setMasked(id, on == 1) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_set_moving(_ id: Int64, _ on: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setMoving(id, on == 1) ?? answerNotFound
    }
}

// A menu item's checks, `hero_ui.h`'s `HERO_UI_CHECK_*`.
private let checkNone: Int64 = 0
private let checkBox: Int64 = 1
private let checkRadio: Int64 = 2

extension Backend {
    // Makes menu item `id` a check item, a radio item or neither, unchecked.
    func setCheck(_ id: Int64, _ mode: Int64) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard widget.kind == kindMenuItem, let item = widget.item, mode >= checkNone, mode <= checkRadio else {
            return answerInvalid
        }
        widget.check = mode
        item.state = .off
        return answerOk
    }

    // Checks radio item `widget` and unchecks every other radio item of its
    // group: the radio items of its menu that no separator and no other item
    // stands between, as a menu shows a group.
    func radioChecked(_ widget: Native) {
        guard let item = widget.item, let menu = item.menu else {
            return
        }
        let at = menu.index(of: item)
        var first = at
        while first > 0, let other = native(menu.item(at: first - 1)), other.check == checkRadio {
            first -= 1
        }
        var last = at
        while last + 1 < menu.numberOfItems, let other = native(menu.item(at: last + 1)), other.check == checkRadio {
            last += 1
        }
        for index in first...last {
            menu.item(at: index)?.state = index == at ? .on : .off
        }
    }

    // The widget whose menu item `item` is.
    func native(_ item: NSMenuItem?) -> Native? {
        guard let item = item, !item.isSeparatorItem, let widget = widgets[Int64(item.tag)], widget.item === item else {
            return nil
        }
        return widget
    }
}

@c
public func hero_ui_appkit_set_check(_ id: Int64, _ mode: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setCheck(id, mode) ?? answerNotFound
    }
}

// A menu item's roles, `hero_ui.h`'s `HERO_UI_ROLE_*`.
private let roleNone: Int64 = 0
private let roleAbout: Int64 = 1
private let roleSettings: Int64 = 2
private let roleQuit: Int64 = 3

extension Backend {
    // Gives menu item `id` the role `role`: it stands in the application's
    // menu in the system's place and is hidden in its own (`appMenu`).
    func setRole(_ id: Int64, _ role: Int64) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard widget.kind == kindMenuItem, let item = widget.item, role >= roleNone, role <= roleQuit else {
            return answerInvalid
        }
        widget.role = role
        item.isHidden = role != roleNone
        refreshMenuBar()
        return answerOk
    }
}

@c
public func hero_ui_appkit_set_role(_ id: Int64, _ role: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setRole(id, role) ?? answerNotFound
    }
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
public func hero_ui_appkit_set_docked(_ on: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setDocked(on == 1) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_set_accessible_name(_ id: Int64, _ name: UnsafePointer<UInt8>?, _ count: Int64) -> Int64 {
    let said = text(name, count)
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setAccessibleName(id, said) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_accessible_name(_ id: Int64, _ buffer: UnsafeMutablePointer<UInt8>?, _ count: Int64, _ length: UnsafeMutablePointer<Int64>?) -> Int64 {
    guard let length = length else {
        return answerInvalid
    }
    let got = MainActor.assumeIsolated { () -> String? in
        return backend()?.accessibleName(id)
    }
    guard let name = got else {
        return answerNotFound
    }
    let bytes = Array(name.utf8)
    if let buffer = buffer, bytes.count <= Int(count) {
        for at in 0..<bytes.count {
            buffer[at] = bytes[at]
        }
    }
    length.pointee = Int64(bytes.count)
    return answerOk
}

@c
public func hero_ui_appkit_set_label_for(_ id: Int64, _ label: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setLabelFor(id, label) ?? answerNotFound
    }
}

// Ends the wait of the loop `pump` runs, once another member queued an
// event (a sheet's answer, which AppKit hands its completion handler outside
// any event): the event is counted as this member's, and an event of the
// application's own is posted so `nextEvent` returns to see it.
@c
public func hero_ui_appkit_wake() {
    MainActor.assumeIsolated {
        guard let shared = backend() else {
            return
        }
        shared.pushed += 1
        if let nudge = NSEvent.otherEvent(with: .applicationDefined, location: .zero, modifierFlags: [], timestamp: 0, windowNumber: 0, context: nil, subtype: 0, data1: 0, data2: 0) {
            NSApp.postEvent(nudge, atStart: false)
        }
    }
}

@c
public func hero_ui_appkit_inject(_ kind: Int64, _ id: Int64, _ value: Int64, _ second: Int64, _ bytes: UnsafePointer<UInt8>?, _ count: Int64) -> Int64 {
    let typed = text(bytes, count)
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.inject(kind, id, value, second, typed) ?? answerNotFound
    }
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

// MARK: boxes the system draws: pages, a scrolled box, two panes

// The kinds of `hero_ui.h`'s BOXES THE SYSTEM DRAWS.
private let kindTabs: Int64 = 22
private let kindPage: Int64 = 23
private let kindScroll: Int64 = 24
private let kindSplitRow: Int64 = 25
private let kindSplitColumn: Int64 = 26
private let kindPane: Int64 = 27

// The key AppKit's notification of a divider dragged by the person carries,
// and a resize of the split's own place does not.
private let dividerDragged = "NSSplitViewDividerIndex"

// A split view whose divider the person drags within the range the program
// set, its panes' least sizes.
private final class BoundedSplitView: NSSplitView {
    var least: CGFloat = 0
    var most: CGFloat = .greatestFiniteMagnitude
}

extension Backend: NSTabViewDelegate, NSSplitViewDelegate {
    // The view a widget whose parent is `owner` stands in: a window's
    // content, a page's view, a scrolled box's content, a pane's view; nil
    // for any other parent.
    func contentOf(_ owner: Native) -> NSView? {
        switch owner.kind {
        case kindWindow:
            return owner.window?.contentView
        case kindPage, kindPane:
            return owner.view
        case kindScroll:
            return (owner.view as? NSScrollView)?.documentView
        default:
            return nil
        }
    }

    // The window `widget` stands in, its parents walked up to it.
    func windowOf(_ widget: Native) -> Native? {
        var at: Native? = widget
        while let each = at {
            if each.kind == kindWindow {
                return each
            }
            at = widgets[each.parent]
        }
        return nil
    }

    // Makes `made`, of a kind of BOXES THE SYSTEM DRAWS, in `owner`; nil
    // for every other kind, which `create` makes itself.
    func createBox(_ id: Int64, _ kind: Int64, _ owner: Native, _ made: Native) -> Int64? {
        switch kind {
        case kindPage:
            guard owner.kind == kindTabs, let tabs = owner.view as? NSTabView else {
                return answerInvalid
            }
            let page = FlippedView(frame: tabs.contentRect)
            page.clipsToBounds = true
            let item = NSTabViewItem(identifier: NSNumber(value: id))
            item.view = page
            muted = true
            tabs.addTabViewItem(item)
            muted = false
            made.view = page
        case kindPane:
            guard owner.kind == kindSplitRow || owner.kind == kindSplitColumn, let split = owner.view as? NSSplitView else {
                return answerInvalid
            }
            if split.arrangedSubviews.count >= 2 {
                return answerInvalid
            }
            let pane = FlippedView(frame: NSRect(x: 0, y: 0, width: 0, height: 0))
            pane.clipsToBounds = true
            split.addArrangedSubview(pane)
            made.view = pane
        case kindContextMenu:
            if widgets.values.contains(where: { $0.kind == kindContextMenu && $0.parent == owner.id }) {
                return answerInvalid
            }
            let menu = NSMenu(title: "")
            menu.autoenablesItems = false
            // A tray icon's menu opens at its click, the status item's own.
            if let status = owner.status {
                status.menu = menu
            } else if let target = contextTarget(owner) {
                target.menu = menu
            } else {
                return answerInvalid
            }
            made.menu = menu
        case kindTabs, kindScroll, kindSplitRow, kindSplitColumn, kindTable, kindTree:
            guard let content = contentOf(owner) else {
                return answerInvalid
            }
            let view: NSView
            if kind == kindTree {
                view = makeTree(made)
            } else if kind == kindTable {
                view = makeTable(made)
            } else if kind == kindTabs {
                let tabs = NSTabView(frame: NSRect(x: 0, y: 0, width: 400, height: 300))
                tabs.delegate = self
                view = tabs
            } else if kind == kindScroll {
                let scroll = NSScrollView(frame: NSRect(x: 0, y: 0, width: 400, height: 300))
                let document = FlippedView(frame: NSRect(x: 0, y: 0, width: 0, height: 0))
                scroll.documentView = document
                scroll.hasVerticalScroller = true
                scroll.hasHorizontalScroller = false
                scroll.autohidesScrollers = true
                scroll.drawsBackground = false
                view = scroll
            } else {
                let split = BoundedSplitView(frame: NSRect(x: 0, y: 0, width: 400, height: 300))
                split.isVertical = kind == kindSplitRow
                split.dividerStyle = .thin
                split.delegate = self
                view = split
            }
            content.addSubview(view)
            made.view = view
        default:
            return nil
        }
        widgets[id] = made
        return answerOk
    }

    // The room tabs or a scrolled box draws around its content.
    func insets(_ id: Int64) -> (Int64, Int64, Int64, Int64)? {
        guard let widget = widgets[id] else {
            return nil
        }
        if let tabs = widget.view as? NSTabView, widget.kind == kindTabs {
            let saved = tabs.frame
            tabs.frame = NSRect(x: saved.minX, y: saved.minY, width: 400, height: 300)
            let inner = tabs.contentRect
            tabs.frame = saved
            let left = Int64(inner.minX.rounded(.up))
            let right = Int64((400 - inner.maxX).rounded(.up))
            let top = tabs.isFlipped ? Int64(inner.minY.rounded(.up)) : Int64((300 - inner.maxY).rounded(.up))
            let bottom = tabs.isFlipped ? Int64((300 - inner.maxY).rounded(.up)) : Int64(inner.minY.rounded(.up))
            return (left, top, right, bottom)
        }
        if let scroll = widget.view as? NSScrollView, widget.kind == kindScroll {
            let frame = NSSize(width: 400, height: 300)
            let inner = NSScrollView.contentSize(forFrameSize: frame, horizontalScrollerClass: nil, verticalScrollerClass: NSScroller.self, borderType: scroll.borderType, controlSize: .regular, scrollerStyle: NSScroller.preferredScrollerStyle)
            return (0, 0, Int64((400 - inner.width).rounded(.up)), Int64((300 - inner.height).rounded(.up)))
        }
        return (-1, -1, -1, -1)
    }

    // What `preferred` answers for a kind of BOXES THE SYSTEM DRAWS: the
    // divider's thickness on both axes for a split, nothing for the others,
    // whose room the layout finds from what they hold.
    func boxPreferred(_ widget: Native) -> NSSize? {
        switch widget.kind {
        case kindSplitRow, kindSplitColumn:
            let thickness = (widget.view as? NSSplitView)?.dividerThickness ?? 1
            return NSSize(width: thickness, height: thickness)
        case kindTabs, kindPage, kindScroll, kindPane:
            return NSSize(width: 0, height: 0)
        default:
            return nil
        }
    }

    // The divider's place: the first pane's width or height.
    func dividerAt(_ split: NSSplitView) -> Int64 {
        guard let first = split.arrangedSubviews.first else {
            return 0
        }
        return Int64((split.isVertical ? first.frame.width : first.frame.height).rounded())
    }

    // Sets the divider of split `widget` at `value` points, its panes laid
    // out at once; nothing is queued.
    func setDivider(_ widget: Native, _ value: Int64) -> Int64 {
        guard let split = widget.view as? NSSplitView, value >= 0 else {
            return answerInvalid
        }
        if split.arrangedSubviews.count == 2 {
            muted = true
            split.setPosition(CGFloat(value), ofDividerAt: 0)
            split.adjustSubviews()
            split.setPosition(CGFloat(value), ofDividerAt: 0)
            muted = false
        }
        return answerOk
    }

    // Sets the range the person may drag split `widget`'s divider within.
    func setDividerRange(_ widget: Native, _ least: Int64, _ most: Int64) -> Int64 {
        guard let split = widget.view as? BoundedSplitView, least >= 0, least <= most else {
            return answerInvalid
        }
        split.least = CGFloat(least)
        split.most = CGFloat(most)
        return answerOk
    }

    // Shows page `index` of tabs `widget`; nothing is queued.
    func setPage(_ widget: Native, _ indices: [Int64]) -> Int64 {
        guard let tabs = widget.view as? NSTabView, indices.count == 1, indices[0] >= 0, indices[0] < Int64(tabs.numberOfTabViewItems) else {
            return answerInvalid
        }
        muted = true
        tabs.selectTabViewItem(at: Int(indices[0]))
        muted = false
        return answerOk
    }

    // The index of the page tabs `widget` shows, as a selection; none where
    // it holds no page.
    func pageShown(_ widget: Native) -> [Int64] {
        guard let tabs = widget.view as? NSTabView, let item = tabs.selectedTabViewItem else {
            return []
        }
        return [Int64(tabs.indexOfTabViewItem(item))]
    }

    // Takes page `widget` out of its tabs.
    func removePage(_ widget: Native) {
        guard let tabs = widgets[widget.parent]?.view as? NSTabView else {
            return
        }
        for item in tabs.tabViewItems where item.view === widget.view {
            muted = true
            tabs.removeTabViewItem(item)
            muted = false
        }
    }

    // What the person does to tabs and a split, through the system's own
    // path: a tab chosen, a divider dragged to `value`. Nil for every other
    // widget.
    func injectBox(_ kind: Int64, _ widget: Native, _ value: Int64) -> Int64? {
        if widget.kind == kindTabs {
            guard kind == eventSelect, let tabs = widget.view as? NSTabView else {
                return answerInvalid
            }
            guard value >= 0, value < Int64(tabs.numberOfTabViewItems) else {
                return answerInvalid
            }
            tabs.selectTabViewItem(at: Int(value))
            return answerOk
        }
        if widget.kind == kindSplitRow || widget.kind == kindSplitColumn {
            guard kind == eventValue, let split = widget.view as? BoundedSplitView, split.arrangedSubviews.count == 2 else {
                return answerInvalid
            }
            let at = min(max(CGFloat(value), split.least), split.most)
            split.setPosition(at, ofDividerAt: 0)
            queue(eventValue, widget.id, dividerAt(split), 0)
            return answerOk
        }
        return nil
    }

    // The person chose a tab.
    func tabView(_ tabView: NSTabView, didSelect tabViewItem: NSTabViewItem?) {
        guard let item = tabViewItem, let id = widgets.values.first(where: { $0.view === tabView })?.id else {
            return
        }
        queue(eventSelect, id, Int64(tabView.indexOfTabViewItem(item)), 0)
    }

    // The person dragged a divider: queued with its place. AppKit names the
    // divider in its notification for its own adjustments too, made while
    // the window is laid out, so the person's drag is the one made with the
    // mouse's button held; a resize of the split's place names none.
    func splitViewDidResizeSubviews(_ notification: Notification) {
        guard notification.userInfo?[dividerDragged] != nil, NSEvent.pressedMouseButtons & 1 == 1, let split = notification.object as? NSSplitView, let id = widgets.values.first(where: { $0.view === split })?.id else {
            return
        }
        queue(eventValue, id, dividerAt(split), 0)
    }

    func splitView(_ splitView: NSSplitView, constrainMinCoordinate proposedMinimumPosition: CGFloat, ofSubviewAt dividerIndex: Int) -> CGFloat {
        return max(proposedMinimumPosition, (splitView as? BoundedSplitView)?.least ?? 0)
    }

    func splitView(_ splitView: NSSplitView, constrainMaxCoordinate proposedMaximumPosition: CGFloat, ofSubviewAt dividerIndex: Int) -> CGFloat {
        return min(proposedMaximumPosition, (splitView as? BoundedSplitView)?.most ?? proposedMaximumPosition)
    }
}

@c
public func hero_ui_appkit_insets(_ id: Int64, _ left: UnsafeMutablePointer<Int64>?, _ top: UnsafeMutablePointer<Int64>?, _ right: UnsafeMutablePointer<Int64>?, _ bottom: UnsafeMutablePointer<Int64>?) -> Int64 {
    guard let left = left, let top = top, let right = right, let bottom = bottom else {
        return answerInvalid
    }
    let found = MainActor.assumeIsolated { () -> (Int64, Int64, Int64, Int64)? in
        return backend()?.insets(id)
    }
    guard let (l, t, r, b) = found else {
        return answerNotFound
    }
    if l < 0 {
        return answerInvalid
    }
    left.pointee = l
    top.pointee = t
    right.pointee = r
    bottom.pointee = b
    return answerOk
}

// MARK: a table: rows of cells in columns

// `hero_ui.h`'s `HERO_UI_TABLE` and `HERO_UI_EVENT_SORT`.
private let kindTable: Int64 = 28
private let eventSort: Int64 = 15

// The width a table's column takes where nothing else decides it.
private let columnWidth: CGFloat = 100

extension Backend {
    // The rows list or table `widget` holds: a list's items, a table's
    // cells over its columns.
    func rowsOf(_ widget: Native) -> Int {
        guard widget.kind == kindTable, let table = widget.table else {
            return widget.items.count
        }
        return widget.items.count / max(table.tableColumns.count, 1)
    }

    // The index among `widget`'s items of the cell at `row` in `column` of
    // `table`: a list's one column holds its items, a table's cells run
    // row after row.
    func cellAt(_ widget: Native, _ table: NSTableView, _ column: NSTableColumn?, _ row: Int) -> Int {
        guard widget.kind == kindTable else {
            return row
        }
        guard let column = column, let at = table.tableColumns.firstIndex(of: column) else {
            return -1
        }
        return row * table.tableColumns.count + at
    }

    // A table's view: a list's, with headers the person clicks to sort by
    // and drags to widen, and several rows chosen.
    func makeTable(_ widget: Native) -> NSView {
        let scroll = NSScrollView(frame: NSRect(x: 0, y: 0, width: fieldWidth, height: areaHeight))
        scroll.hasVerticalScroller = true
        scroll.hasHorizontalScroller = true
        scroll.autohidesScrollers = true
        scroll.borderType = .bezelBorder
        let table = ActivatingTable(frame: scroll.bounds)
        table.allowsMultipleSelection = true
        table.allowsEmptySelection = true
        table.allowsColumnResizing = true
        table.allowsColumnReordering = false
        table.columnAutoresizingStyle = .lastColumnOnlyAutoresizingStyle
        table.usesAlternatingRowBackgroundColors = true
        table.dataSource = self
        table.delegate = self
        table.target = self
        table.doubleAction = #selector(rowDoubleClicked(_:))
        table.returned = { [weak self, weak table] row in
            if let table = table {
                self?.activated(table, row)
            }
        }
        scroll.documentView = table
        widget.table = table
        tables[ObjectIdentifier(table)] = widget.id
        return scroll
    }

    // Gives table `id` the columns `titles`, its cells and its sort gone.
    func setColumns(_ id: Int64, _ titles: [String]) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard widget.kind == kindTable, let table = widget.table, !titles.isEmpty else {
            return answerInvalid
        }
        muted = true
        table.sortDescriptors = []
        for column in table.tableColumns {
            table.removeTableColumn(column)
        }
        for (at, title) in titles.enumerated() {
            let column = NSTableColumn(identifier: NSUserInterfaceItemIdentifier("column\(at)"))
            column.title = title
            column.width = columnWidth
            column.minWidth = 24
            column.resizingMask = .userResizingMask
            column.sortDescriptorPrototype = NSSortDescriptor(key: "\(at)", ascending: true)
            table.addTableColumn(column)
        }
        widget.items = []
        table.reloadData()
        muted = false
        return answerOk
    }

    // Shows table `id` sorted by `column`, by none where it is -1; nothing is
    // queued.
    func setSort(_ id: Int64, _ column: Int64, _ ascending: Bool) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard widget.kind == kindTable, let table = widget.table, column < Int64(table.tableColumns.count) else {
            return answerInvalid
        }
        muted = true
        table.sortDescriptors = column < 0 ? [] : [NSSortDescriptor(key: "\(column)", ascending: ascending)]
        muted = false
        return answerOk
    }

    // What the person clicking a header of table `widget` does, through the
    // system's own path: the table's sort set as a click sets it, the
    // column's header showing its arrow, and the system's call told.
    func injectSort(_ widget: Native, _ column: Int64, _ ascending: Bool) -> Int64 {
        guard let table = widget.table, column >= 0, column < Int64(table.tableColumns.count) else {
            return answerInvalid
        }
        table.sortDescriptors = [NSSortDescriptor(key: "\(column)", ascending: ascending)]
        return answerOk
    }

    // The person clicked a header: the column its first descriptor names,
    // and its order.
    func tableView(_ tableView: NSTableView, sortDescriptorsDidChange oldDescriptors: [NSSortDescriptor]) {
        guard let id = tables[ObjectIdentifier(tableView)], let first = tableView.sortDescriptors.first, let key = first.key, let column = Int64(key) else {
            return
        }
        queue(eventSort, id, column, first.ascending ? 1 : 0)
    }

    // A table's size where nothing else decides it: its columns at their
    // width, and a list's height with its header.
    func tablePreferred(_ widget: Native) -> NSSize? {
        guard widget.kind == kindTable, let table = widget.table else {
            return nil
        }
        let header = table.headerView?.frame.height ?? 0
        let wide = table.tableColumns.reduce(CGFloat(0)) { $0 + $1.width + table.intercellSpacing.width }
        return NSSize(width: max(fieldWidth, wide), height: areaHeight + header)
    }
}

@c
public func hero_ui_appkit_set_columns(_ id: Int64, _ titles: UnsafePointer<UInt8>?, _ count: Int64) -> Int64 {
    let bytes: [UInt8] = titles.map { Array(UnsafeBufferPointer(start: $0, count: Int(count))) } ?? []
    var names: [String] = []
    var start = 0
    for at in 0..<bytes.count where bytes[at] == 0 {
        names.append(String(decoding: bytes[start..<at], as: UTF8.self))
        start = at + 1
    }
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setColumns(id, names) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_set_sort(_ id: Int64, _ column: Int64, _ ascending: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setSort(id, column, ascending == 1) ?? answerNotFound
    }
}

// MARK: a tree: items folded and unfolded

// `hero_ui.h`'s `HERO_UI_TREE` and `HERO_UI_EVENT_UNFOLD`.
private let kindTree: Int64 = 29
private let eventUnfold: Int64 = 16

// One item of a tree: the object the outline view holds for it, its number
// the program's.
private final class TreeItem: NSObject {
    let number: Int64
    let under: Int64
    var text: String
    var foldable: Bool
    var children: [Int64] = []

    init(number: Int64, under: Int64, text: String, foldable: Bool) {
        self.number = number
        self.under = under
        self.text = text
        self.foldable = foldable
    }
}

// The items of one tree, by number, and the numbers of its top items.
private final class Forest {
    var items: [Int64: TreeItem] = [:]
    var top: [Int64] = []
}

// Each tree's items, by the tree's widget number.
@MainActor
private var forests: [Int64: Forest] = [:]

// An outline view whose Return and keypad Enter open the row chosen, as a
// list's do.
private final class ReturningOutline: NSOutlineView {
    var returned: ((Int) -> Void)?

    override func keyDown(with event: NSEvent) {
        if (event.keyCode == 0x24 || event.keyCode == 0x4C), selectedRow >= 0, let returned = returned {
            returned(selectedRow)
            return
        }
        super.keyDown(with: event)
    }
}

extension Backend: NSOutlineViewDataSource, NSOutlineViewDelegate {
    // A tree's view: an outline view of one column with no header, in a
    // scroll view, one item chosen at most.
    func makeTree(_ widget: Native) -> NSView {
        let scroll = NSScrollView(frame: NSRect(x: 0, y: 0, width: fieldWidth, height: areaHeight))
        scroll.hasVerticalScroller = true
        scroll.borderType = .bezelBorder
        let outline = ReturningOutline(frame: scroll.bounds)
        let column = NSTableColumn(identifier: NSUserInterfaceItemIdentifier("items"))
        column.resizingMask = .autoresizingMask
        outline.addTableColumn(column)
        outline.outlineTableColumn = column
        outline.headerView = nil
        outline.allowsMultipleSelection = false
        outline.allowsEmptySelection = true
        outline.dataSource = self
        outline.delegate = self
        outline.target = self
        outline.doubleAction = #selector(treeDoubleClicked(_:))
        outline.returned = { [weak self, weak outline] row in
            if let outline = outline {
                self?.treeOpened(outline, row)
            }
        }
        scroll.documentView = outline
        forests[widget.id] = Forest()
        return scroll
    }

    // The outline view of tree `widget`.
    func outlineOf(_ widget: Native) -> NSOutlineView? {
        return (widget.view as? NSScrollView)?.documentView as? NSOutlineView
    }

    // The tree's widget number an outline view stands for.
    func treeId(_ outline: NSOutlineView) -> Int64? {
        return widgets.values.first(where: { $0.kind == kindTree && outlineOf($0) === outline })?.id
    }

    // Adds item `item` under `under` of tree `id`, at the end.
    func treeAdd(_ id: Int64, _ item: Int64, _ under: Int64, _ text: String, _ foldable: Bool) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard widget.kind == kindTree, let forest = forests[id], let outline = outlineOf(widget) else {
            return answerInvalid
        }
        if forest.items[item] != nil || (under != 0 && forest.items[under] == nil) {
            return answerNotFound
        }
        forest.items[item] = TreeItem(number: item, under: under, text: text, foldable: foldable)
        muted = true
        if under == 0 {
            forest.top.append(item)
            outline.reloadItem(nil, reloadChildren: true)
        } else if let parent = forest.items[under] {
            parent.children.append(item)
            outline.reloadItem(parent, reloadChildren: true)
        }
        muted = false
        return answerOk
    }

    // Takes item `item` and every item under it out of tree `id`.
    func treeRemove(_ id: Int64, _ item: Int64) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard widget.kind == kindTree, let forest = forests[id], let outline = outlineOf(widget) else {
            return answerInvalid
        }
        guard let gone = forest.items[item] else {
            return answerNotFound
        }
        var held: [Int64] = [item]
        var at = 0
        while at < held.count {
            held.append(contentsOf: forest.items[held[at]]?.children ?? [])
            at += 1
        }
        muted = true
        for each in held {
            forest.items[each] = nil
        }
        if gone.under == 0 {
            forest.top.removeAll { $0 == item }
            outline.reloadItem(nil, reloadChildren: true)
        } else if let parent = forest.items[gone.under] {
            parent.children.removeAll { $0 == item }
            outline.reloadItem(parent, reloadChildren: true)
        }
        muted = false
        return answerOk
    }

    // Unfolds or folds item `item` of tree `id`; nothing is queued where
    // `quiet`, the person's event where not.
    func treeUnfold(_ id: Int64, _ item: Int64, _ on: Bool, _ quiet: Bool) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard widget.kind == kindTree, let forest = forests[id], let outline = outlineOf(widget) else {
            return answerInvalid
        }
        guard let node = forest.items[item] else {
            return answerNotFound
        }
        if !node.foldable && node.children.isEmpty {
            return answerInvalid
        }
        muted = quiet
        if on {
            outline.expandItem(node)
        } else {
            outline.collapseItem(node)
        }
        muted = false
        return answerOk
    }

    // The item chosen in tree `widget`, as a selection.
    func treeChosen(_ widget: Native) -> [Int64] {
        guard let outline = outlineOf(widget), outline.selectedRow >= 0, let node = outline.item(atRow: outline.selectedRow) as? TreeItem else {
            return []
        }
        return [node.number]
    }

    // Chooses item `numbers`' one in tree `widget`, or none; the items over
    // it unfolded so it shows. Nothing is queued where `quiet`.
    func treeChoose(_ widget: Native, _ numbers: [Int64], _ quiet: Bool) -> Int64 {
        guard let outline = outlineOf(widget), let forest = forests[widget.id], numbers.count <= 1 else {
            return answerInvalid
        }
        muted = quiet
        defer {
            muted = false
        }
        guard let number = numbers.first else {
            outline.deselectAll(nil)
            return answerOk
        }
        guard let node = forest.items[number] else {
            return answerInvalid
        }
        // The person chooses only an item they see; the program's choice
        // unfolds the items over it.
        if !quiet && outline.row(forItem: node) < 0 {
            return answerInvalid
        }
        var over: [TreeItem] = []
        var at = node.under
        while at != 0, let parent = forest.items[at] {
            over.insert(parent, at: 0)
            at = parent.under
        }
        for parent in over {
            outline.expandItem(parent)
        }
        let row = outline.row(forItem: node)
        if row < 0 {
            return answerInvalid
        }
        outline.selectRowIndexes(IndexSet(integer: row), byExtendingSelection: false)
        return answerOk
    }

    // What the person does to tree `widget` through the system's own path:
    // an item chosen, opened, unfolded or folded. Nil for every other
    // widget.
    func injectTree(_ kind: Int64, _ widget: Native, _ value: Int64, _ second: Int64) -> Int64? {
        guard widget.kind == kindTree else {
            return nil
        }
        guard let outline = outlineOf(widget) else {
            return answerInvalid
        }
        switch kind {
        case eventSelect:
            return treeChoose(widget, value < 0 ? [] : [value], false)
        case eventActivate:
            guard let node = forests[widget.id]?.items[value] else {
                return answerInvalid
            }
            if outline.row(forItem: node) < 0 {
                return answerInvalid
            }
            if treeChosen(widget) != [value] {
                let chose = treeChoose(widget, [value], false)
                if chose != answerOk {
                    return chose
                }
            }
            treeOpened(outline, outline.row(forItem: node))
            return answerOk
        case eventUnfold:
            return treeUnfold(widget.id, value, second == 1, false)
        default:
            return answerInvalid
        }
    }

    // Row `row` of `outline` opened by the person.
    func treeOpened(_ outline: NSOutlineView, _ row: Int) {
        guard outline.isEnabled, let node = outline.item(atRow: row) as? TreeItem, let id = treeId(outline) else {
            return
        }
        queue(eventActivate, id, node.number, 0)
    }

    @objc func treeDoubleClicked(_ sender: NSOutlineView) {
        if sender.clickedRow >= 0 {
            treeOpened(sender, sender.clickedRow)
        }
    }

    func outlineView(_ outlineView: NSOutlineView, numberOfChildrenOfItem item: Any?) -> Int {
        guard let id = treeId(outlineView), let forest = forests[id] else {
            return 0
        }
        if let node = item as? TreeItem {
            return node.children.count
        }
        return forest.top.count
    }

    func outlineView(_ outlineView: NSOutlineView, child index: Int, ofItem item: Any?) -> Any {
        guard let id = treeId(outlineView), let forest = forests[id] else {
            return NSNull()
        }
        let numbers = (item as? TreeItem)?.children ?? forest.top
        guard index >= 0, index < numbers.count, let node = forest.items[numbers[index]] else {
            return NSNull()
        }
        return node
    }

    func outlineView(_ outlineView: NSOutlineView, isItemExpandable item: Any) -> Bool {
        guard let node = item as? TreeItem else {
            return false
        }
        return node.foldable || !node.children.isEmpty
    }

    func outlineView(_ outlineView: NSOutlineView, viewFor tableColumn: NSTableColumn?, item: Any) -> NSView? {
        guard let node = item as? TreeItem else {
            return nil
        }
        let cell = NSTextField(labelWithString: node.text)
        cell.lineBreakMode = .byTruncatingTail
        return cell
    }

    func outlineViewSelectionDidChange(_ notification: Notification) {
        if let outline = notification.object as? NSOutlineView, let id = treeId(outline) {
            queue(eventSelect, id, 0, 0)
        }
    }

    func outlineViewItemDidExpand(_ notification: Notification) {
        treeFolded(notification, 1)
    }

    func outlineViewItemDidCollapse(_ notification: Notification) {
        treeFolded(notification, 0)
    }

    // An item unfolded, 1, or folded, 0, queued with its number.
    func treeFolded(_ notification: Notification, _ state: Int64) {
        guard let outline = notification.object as? NSOutlineView, let node = notification.userInfo?["NSObject"] as? TreeItem, let id = treeId(outline) else {
            return
        }
        queue(eventUnfold, id, node.number, state)
    }
}

@c
public func hero_ui_appkit_tree_add(_ id: Int64, _ item: Int64, _ under: Int64, _ text: UnsafePointer<UInt8>?, _ count: Int64, _ foldable: Int64) -> Int64 {
    let words: String = text.map { String(decoding: UnsafeBufferPointer(start: $0, count: Int(count)), as: UTF8.self) } ?? ""
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.treeAdd(id, item, under, words, foldable == 1) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_tree_remove(_ id: Int64, _ item: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.treeRemove(id, item) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_tree_unfold(_ id: Int64, _ item: Int64, _ on: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.treeUnfold(id, item, on == 1, true) ?? answerNotFound
    }
}

// MARK: a context menu

// `hero_ui.h`'s `HERO_UI_CONTEXT_MENU`.
private let kindContextMenu: Int64 = 30
private let kindTray: Int64 = 31

// The height of a tray icon's image in the menu bar, in points: the bar's
// own glyphs' (Apple's Human Interface Guidelines give 16 to 18 points of a
// 22-point bar, unread here; 18 keeps an image of the program's whole).
private let trayHeight: CGFloat = 18

extension Backend {
    // The object a screen reader reads for widget `widget`: the view the
    // person types into or chooses in, a window's own window, a menu
    // item's item; none for a menu, a timer's or a tray icon's, whose
    // words are their own.
    func accessibleOf(_ widget: Native) -> NSAccessibilityProtocol? {
        if widget.kind == kindWindow {
            return widget.window
        }
        if widget.kind == kindMenuItem {
            return widget.item
        }
        if let status = widget.status {
            return status.button
        }
        return widget.table ?? widget.textView ?? widget.view
    }

    // Gives widget `id` the accessible name `name`, or takes it back where
    // `name` is empty.
    func setAccessibleName(_ id: Int64, _ name: String) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard let element = accessibleOf(widget) else {
            return answerInvalid
        }
        element.setAccessibilityLabel(name.isEmpty ? nil : name)
        return answerOk
    }

    // The accessible name widget `id` holds, "" for none.
    func accessibleName(_ id: Int64) -> String? {
        guard let widget = widgets[id], let element = accessibleOf(widget) else {
            return nil
        }
        return element.accessibilityLabel() ?? ""
    }

    // Ties label `label` to widget `id`, which is then read by the label's
    // words (`NSAccessibility`'s title element), or unties it where `label`
    // is 0.
    func setLabelFor(_ id: Int64, _ label: Int64) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard let element = accessibleOf(widget) as? NSView else {
            return answerInvalid
        }
        if label == 0 {
            element.setAccessibilityTitleUIElement(nil)
            return answerOk
        }
        guard let tied = widgets[label] else {
            return answerNotFound
        }
        guard tied.kind == kindLabel, let words = tied.view else {
            return answerInvalid
        }
        element.setAccessibilityTitleUIElement(words)
        return answerOk
    }
}

extension Backend {
    // Makes tray icon `made`, a status item of the menu bar's, its parent 0,
    // showing nothing until the program gives it words or an image.
    func makeTray(_ made: Native) -> Int64 {
        if made.parent != 0 {
            return answerInvalid
        }
        let status = NSStatusBar.system.statusItem(withLength: NSStatusItem.variableLength)
        status.isVisible = true
        made.status = status
        widgets[made.id] = made
        return answerOk
    }

    // Gives tray icon `tray` the words `text`: its tip, and its face where
    // it shows no image.
    func setTrayText(_ tray: Native, _ text: String) {
        guard let button = tray.status?.button else {
            return
        }
        button.toolTip = text.isEmpty ? nil : text
        button.title = button.image == nil ? text : ""
        tray.items = [text]
    }

    // Gives tray icon `tray` the image of `width` by `height` pixels at
    // `rgba`, drawn `trayHeight` points high in the menu bar, its words then
    // its tip alone; an image of no pixels shows its words again.
    func setTrayImage(_ tray: Native, _ rgba: UnsafePointer<UInt8>?, _ width: Int64, _ height: Int64) -> Int64 {
        guard let button = tray.status?.button else {
            return answerInvalid
        }
        let words = tray.items.first ?? ""
        if width == 0 || height == 0 {
            button.image = nil
            button.title = words
            return answerOk
        }
        guard let rgba = rgba, let rep = NSBitmapImageRep(bitmapDataPlanes: nil, pixelsWide: Int(width), pixelsHigh: Int(height), bitsPerSample: 8, samplesPerPixel: 4, hasAlpha: true, isPlanar: false, colorSpaceName: .deviceRGB, bitmapFormat: [.alphaNonpremultiplied], bytesPerRow: Int(width) * 4, bitsPerPixel: 32), let into = rep.bitmapData else {
            return answerLimit
        }
        into.update(from: rgba, count: Int(width) * Int(height) * 4)
        let image = NSImage(size: NSSize(width: trayHeight * CGFloat(width) / CGFloat(height), height: trayHeight))
        image.addRepresentation(rep)
        button.image = image
        button.title = ""
        return answerOk
    }

    // Shows the app in the Dock and with a menu bar of its own, `on`, or
    // keeps it out of both, its tray icons and windows staying, as a menu-bar
    // utility does (`NSApplication.ActivationPolicy.accessory`).
    func setDocked(_ on: Bool) -> Int64 {
        guard NSApp.setActivationPolicy(on ? .regular : .accessory) else {
            words = "the system refused to change whether the app stands in the Dock"
            return answerOther
        }
        return answerOk
    }

    // The view whose menu a context menu of `owner` is: the one the
    // person's right click lands on, a list's or a table's rows, a tree's
    // outline, a text area's text, a window's content.
    func contextTarget(_ owner: Native) -> NSView? {
        if owner.kind == kindWindow {
            return owner.window?.contentView
        }
        if owner.kind == kindTree {
            return outlineOf(owner)
        }
        if owner.kind == kindMenu || owner.kind == kindMenuItem || owner.kind == kindMenuSeparator || owner.kind == kindContextMenu || owner.kind == kindPage || owner.kind == kindPane {
            return nil
        }
        return owner.table ?? owner.textView ?? owner.view
    }
}

// MARK: files dropped on a window

// A drag the layer makes for `hero_ui_inject`: files dropped at a point of
// a window, read by the same destination methods as the person's drag.
@MainActor
private final class InjectedDrag: NSObject, NSDraggingInfo {
    let draggingDestinationWindow: NSWindow?
    let draggingLocation: NSPoint
    let draggingPasteboard: NSPasteboard
    var draggingFormation: NSDraggingFormation = .default
    var animatesToDestination = false
    var numberOfValidItemsForDrop = 0

    init(window: NSWindow, location: NSPoint, pasteboard: NSPasteboard, count: Int) {
        draggingDestinationWindow = window
        draggingLocation = location
        draggingPasteboard = pasteboard
        numberOfValidItemsForDrop = count
    }

    var draggingSourceOperationMask: NSDragOperation {
        return .copy
    }
    var draggedImageLocation: NSPoint {
        return draggingLocation
    }
    var draggedImage: NSImage? {
        return nil
    }
    var draggingSource: Any? {
        return nil
    }
    var draggingSequenceNumber: Int {
        return 0
    }
    var springLoadingHighlight: NSSpringLoadingHighlight {
        return .none
    }

    func slideDraggedImage(to screenPoint: NSPoint) {
    }

    func enumerateDraggingItems(options enumOpts: NSDraggingItemEnumerationOptions = [], for view: NSView?, classes classArray: [AnyClass], searchOptions: [NSPasteboard.ReadingOptionKey: Any] = [:], using block: (NSDraggingItem, Int, UnsafeMutablePointer<ObjCBool>) -> Void) {
    }

    func resetSpringLoading() {
    }
}

extension Backend {
    // The window widget whose window `window` is, where it takes dropped
    // files.
    func dropTarget(_ window: NSWindow?) -> Native? {
        guard let window = window else {
            return nil
        }
        return widgets.values.first { $0.kind == kindWindow && $0.window === window && $0.dropping }
    }

    // The paths of the files a drag holds, in its order.
    func droppedPaths(_ pasteboard: NSPasteboard) -> [String] {
        let read = pasteboard.readObjects(forClasses: [NSURL.self], options: [.urlReadingFileURLsOnly: true]) ?? []
        return read.compactMap { ($0 as? URL)?.path }.filter { !$0.isEmpty }
    }

    // Whether the drag `sender` holds files and stands over a window that
    // takes them.
    func takesDrop(_ sender: NSDraggingInfo) -> Bool {
        return dropTarget(sender.draggingDestinationWindow) != nil && !droppedPaths(sender.draggingPasteboard).isEmpty
    }

    // Queues the drop `sender` for its window: the point in its content,
    // the paths each followed by one zero byte.
    func dropped(_ sender: NSDraggingInfo) -> Bool {
        guard let widget = dropTarget(sender.draggingDestinationWindow), let content = widget.window?.contentView, !muted else {
            return false
        }
        let paths = droppedPaths(sender.draggingPasteboard)
        if paths.isEmpty {
            return false
        }
        let at = content.convert(sender.draggingLocation, from: nil)
        var joined: [UInt8] = []
        for path in paths {
            joined.append(contentsOf: Array(path.utf8))
            joined.append(0)
        }
        pushed += 1
        joined.withUnsafeBufferPointer { bytes in
            push(eventDrop, widget.id, Int64(max(0, at.x.rounded())), Int64(max(0, at.y.rounded())), bytes.baseAddress, Int64(bytes.count))
        }
        return true
    }

    @objc func draggingEntered(_ sender: NSDraggingInfo) -> NSDragOperation {
        return takesDrop(sender) ? .copy : []
    }

    @objc func draggingUpdated(_ sender: NSDraggingInfo) -> NSDragOperation {
        return takesDrop(sender) ? .copy : []
    }

    @objc func prepareForDragOperation(_ sender: NSDraggingInfo) -> Bool {
        return takesDrop(sender)
    }

    @objc func performDragOperation(_ sender: NSDraggingInfo) -> Bool {
        return dropped(sender)
    }

    func setDrop(_ id: Int64, _ on: Bool) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard widget.kind == kindWindow, let window = widget.window else {
            return answerInvalid
        }
        widget.dropping = on
        if on {
            window.registerForDraggedTypes([.fileURL])
        } else {
            window.unregisterDraggedTypes()
        }
        return answerOk
    }

    // Drops `paths` on window `id` at `x`, `y` of its content as the
    // person's drag would end: through the text area under the point where
    // one stands there, else through the window, each reading the drag as
    // it reads the person's.
    func drop(_ id: Int64, _ x: Int64, _ y: Int64, _ paths: [String]) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        guard widget.kind == kindWindow, let window = widget.window, let content = window.contentView, widget.dropping else {
            return answerInvalid
        }
        let point = NSPoint(x: CGFloat(x), y: CGFloat(y))
        if !content.bounds.contains(point) {
            return answerInvalid
        }
        let pasteboard = NSPasteboard(name: NSPasteboard.Name("org.heroes-lang.ui.drop"))
        defer {
            pasteboard.releaseGlobally()
        }
        pasteboard.clearContents()
        if !pasteboard.writeObjects(paths.map { URL(fileURLWithPath: $0) as NSURL }) {
            return answerOther
        }
        let drag = InjectedDrag(window: window, location: content.convert(point, to: nil), pasteboard: pasteboard, count: paths.count)
        if let area = content.hitTest(point) as? AreaText ?? (content.hitTest(point)?.enclosingScrollView?.documentView as? AreaText) {
            return area.prepareForDragOperation(drag) && area.performDragOperation(drag) ? answerOk : answerOther
        }
        return windowTakes(window, drag) ? answerOk : answerOther
    }

    // Hands the drag `drag` to `window` as AppKit hands the person's to a
    // window registered for it: the window's own destination methods, which
    // send each on to its delegate (measured 2026-11-22: `NSWindow` answers
    // `draggingEntered:`, `prepareForDragOperation:` and
    // `performDragOperation:` and calls its delegate's).
    func windowTakes(_ window: NSWindow, _ drag: NSDraggingInfo) -> Bool {
        typealias Operation = @convention(c) (AnyObject, Selector, AnyObject) -> UInt
        typealias Answer = @convention(c) (AnyObject, Selector, AnyObject) -> Bool
        let entered = NSSelectorFromString("draggingEntered:")
        let prepare = NSSelectorFromString("prepareForDragOperation:")
        let perform = NSSelectorFromString("performDragOperation:")
        guard window.responds(to: entered), window.responds(to: prepare), window.responds(to: perform) else {
            return false
        }
        if unsafeBitCast(window.method(for: entered), to: Operation.self)(window, entered, drag) == 0 {
            return false
        }
        return unsafeBitCast(window.method(for: prepare), to: Answer.self)(window, prepare, drag) && unsafeBitCast(window.method(for: perform), to: Answer.self)(window, perform, drag)
    }
}

@c
public func hero_ui_appkit_set_drop(_ id: Int64, _ on: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setDrop(id, on == 1) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_drop(_ id: Int64, _ x: Int64, _ y: Int64, _ paths: UnsafePointer<UInt8>?, _ count: Int64) -> Int64 {
    let joined = text(paths, count)
    let list = joined.split(separator: "\u{0}", omittingEmptySubsequences: true).map(String.init)
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.drop(id, x, y, list) ?? answerNotFound
    }
}

// MARK: undo and the selection in text widgets

private let undoUndo: Int64 = 1

extension Backend {
    // Text area `widget`'s own undo.
    func areaUndo(_ widget: Native) -> UndoManager {
        if let made = widget.undo {
            return made
        }
        let made = UndoManager()
        widget.undo = made
        return made
    }

    // The undo of text field or text area `widget`: a text area's own, a
    // text field's its field editor's while it has the keys, none else.
    func undoOf(_ widget: Native) -> UndoManager? {
        if widget.kind == kindTextArea {
            return areaUndo(widget)
        }
        if let field = widget.view as? NSTextField, let editor = field.currentEditor() {
            return editor.undoManager
        }
        return nil
    }

    // Undoes or redoes the last edit of `id`, telling the program nothing:
    // the answer, and whether one was there.
    func undo(_ id: Int64, _ which: Int64) -> (Int64, Bool) {
        guard let widget = widgets[id] else {
            return (answerNotFound, false)
        }
        guard widget.kind == kindTextField || widget.kind == kindTextArea else {
            return (answerInvalid, false)
        }
        guard let manager = undoOf(widget), which == undoUndo ? manager.canUndo : manager.canRedo else {
            return (answerOk, false)
        }
        muted = true
        if which == undoUndo {
            manager.undo()
        } else {
            manager.redo()
        }
        muted = false
        return (answerOk, true)
    }

    func canUndo(_ id: Int64, _ which: Int64) -> (Int64, Bool) {
        guard let widget = widgets[id] else {
            return (answerNotFound, false)
        }
        guard widget.kind == kindTextField || widget.kind == kindTextArea else {
            return (answerInvalid, false)
        }
        guard let manager = undoOf(widget) else {
            return (answerOk, false)
        }
        return (answerOk, which == undoUndo ? manager.canUndo : manager.canRedo)
    }

    // The selection `range` of `text`, in UTF-16 units, as bytes of its
    // UTF-8, each end at a character's first byte.
    func bytesOf(_ text: String, _ range: NSRange) -> (Int64, Int64) {
        return (byteAt(text, range.location), byteAt(text, range.location + range.length))
    }

    // The UTF-8 offset of UTF-16 offset `at` of `text`, back to the start
    // of the character it falls in.
    func byteAt(_ text: String, _ at: Int) -> Int64 {
        let units = text.utf16
        var index = units.index(units.startIndex, offsetBy: min(max(0, at), units.count))
        while index > units.startIndex, index.samePosition(in: text.unicodeScalars) == nil {
            index = units.index(before: index)
        }
        return Int64(text.utf8.distance(from: text.utf8.startIndex, to: index))
    }

    // The UTF-16 offset of byte `at` of `text`'s UTF-8; nil outside the
    // text or inside a character.
    func unitAt(_ text: String, _ at: Int64) -> Int? {
        let bytes = text.utf8
        guard at >= 0, at <= Int64(bytes.count) else {
            return nil
        }
        let index = bytes.index(bytes.startIndex, offsetBy: Int(at))
        guard index.samePosition(in: text.unicodeScalars) != nil else {
            return nil
        }
        return text.utf16.distance(from: text.utf16.startIndex, to: index)
    }

    func setTextSelection(_ id: Int64, _ start: Int64, _ end: Int64) -> Int64 {
        guard let widget = widgets[id] else {
            return answerNotFound
        }
        if let area = widget.textView, widget.kind == kindTextArea {
            let text = area.string
            guard let from = unitAt(text, start), let to = unitAt(text, end), from <= to else {
                return answerInvalid
            }
            let range = NSRange(location: from, length: to - from)
            area.setSelectedRange(range)
            area.scrollRangeToVisible(range)
            return answerOk
        }
        guard widget.kind == kindTextField, let field = widget.view as? NSTextField, field.isEnabled, !field.isHiddenOrHasHiddenAncestor, let window = field.window else {
            return answerInvalid
        }
        let text = field.stringValue
        guard let from = unitAt(text, start), let to = unitAt(text, end), from <= to else {
            return answerInvalid
        }
        if field.currentEditor() == nil && !window.makeFirstResponder(field) {
            return answerInvalid
        }
        guard let editor = field.currentEditor() else {
            return answerInvalid
        }
        editor.selectedRange = NSRange(location: from, length: to - from)
        widget.fieldRange = (start, end)
        return answerOk
    }

    func textSelection(_ id: Int64) -> (Int64, Int64, Int64) {
        guard let widget = widgets[id] else {
            return (answerNotFound, 0, 0)
        }
        if let area = widget.textView, widget.kind == kindTextArea {
            let (from, to) = bytesOf(area.string, area.selectedRange())
            return (answerOk, from, to)
        }
        guard widget.kind == kindTextField, let field = widget.view as? NSTextField else {
            return (answerInvalid, 0, 0)
        }
        if let editor = field.currentEditor() {
            let (from, to) = bytesOf(editor.string, editor.selectedRange)
            return (answerOk, from, to)
        }
        let end = Int64(field.stringValue.utf8.count)
        if let (from, to) = widget.fieldRange, to <= end {
            return (answerOk, from, to)
        }
        return (answerOk, end, end)
    }
}

@c
public func hero_ui_appkit_undo(_ id: Int64, _ which: Int64, _ done: UnsafeMutablePointer<Int64>?) -> Int64 {
    guard let done = done else {
        return answerInvalid
    }
    let (answer, did) = MainActor.assumeIsolated { () -> (Int64, Bool) in
        return backend()?.undo(id, which) ?? (answerNotFound, false)
    }
    done.pointee = did ? 1 : 0
    return answer
}

@c
public func hero_ui_appkit_can_undo(_ id: Int64, _ which: Int64, _ can: UnsafeMutablePointer<Int64>?) -> Int64 {
    guard let can = can else {
        return answerInvalid
    }
    let (answer, able) = MainActor.assumeIsolated { () -> (Int64, Bool) in
        return backend()?.canUndo(id, which) ?? (answerNotFound, false)
    }
    can.pointee = able ? 1 : 0
    return answer
}

@c
public func hero_ui_appkit_set_text_selection(_ id: Int64, _ start: Int64, _ end: Int64) -> Int64 {
    return MainActor.assumeIsolated { () -> Int64 in
        return backend()?.setTextSelection(id, start, end) ?? answerNotFound
    }
}

@c
public func hero_ui_appkit_text_selection(_ id: Int64, _ start: UnsafeMutablePointer<Int64>?, _ end: UnsafeMutablePointer<Int64>?) -> Int64 {
    guard let start = start, let end = end else {
        return answerInvalid
    }
    let (answer, from, to) = MainActor.assumeIsolated { () -> (Int64, Int64, Int64) in
        return backend()?.textSelection(id) ?? (answerNotFound, 0, 0)
    }
    start.pointee = from
    end.pointee = to
    return answer
}

// MARK: the app's identity

// Gives the app its name, which the application menu shows ("About",
// "Hide", "Quit"), where the identifier is the bundle's own or the app
// stands in none. A process outside a bundle keeps the executable's name
// in the menu bar's bold title, which the system reads from the bundle
// alone.
@c
public func hero_ui_appkit_set_identity(_ identifier: UnsafePointer<UInt8>?, _ identifierCount: Int64, _ name: UnsafePointer<UInt8>?, _ nameCount: Int64) -> Int64 {
    let named = text(identifier, identifierCount)
    let shown = text(name, nameCount)
    return MainActor.assumeIsolated { () -> Int64 in
        if let bundled = Bundle.main.bundleIdentifier, bundled != named {
            backend()?.words = "the app's bundle names itself \(bundled), and an app in a bundle is the bundle's identity"
            return answerInvalid
        }
        ProcessInfo.processInfo.processName = shown
        backend()?.refreshMenuBar()
        return answerOk
    }
}
