// The Heroes extension's one script, which VS Code loads from `main` in
// package.json. Everything else the extension does is declared there: the
// grammar, the language configuration, the icons, the breakpoints, and the two
// problem matchers that read the compiler's diagnostics.
//
// Two things need code:
//
// - **the `heroes` debug type.** F5 on a `.hero` file builds it with
//   `heroes build -O0`, the build whose debug information describes the
//   variables (panel 197), and hands the binary to `lldb-dap`, the LLVM
//   project's debug adapter, through its own extension. The emitted C carries
//   `#line` back to the `.hero` file, so the breakpoints, the stack and the
//   stepping are Heroes source. This project writes no debug adapter
//   (docs/roadmap/milestones/M-vscode-extension.md, the Debugging bullet);
//   it hands lldb one fixed formatter, `lldb/heroes_lldb.py` beside this
//   script, which shows the values in Heroes' own syntax (panel 213).
// - **the `heroes` task type**, which runs the compiler on a file with the
//   problem matchers attached, so its diagnostics land in the Problems view.
//
// No dependency and no build step: plain CommonJS against the `vscode` API,
// which VS Code hands the extension at run time.
"use strict";

const vscode = require("vscode");
const childProcess = require("child_process");
const fs = require("fs");
const os = require("os");
const path = require("path");
const manifest = require("./package.json");

// The LLVM project's extension for `lldb-dap`, and the debug type it adds.
const LLDB_DAP_EXTENSION = "llvm-vs-code-extensions.lldb-dap";
const LLDB_DAP_TYPE = "lldb-dap";

// The commands that extension registers so another debugger's extension can
// run its configuration through the same resolution its own launches get
// (llvm-project commit 0e0ea714f3, 2025-08-06), in the order VS Code calls them.
const LLDB_DAP_RESOLVERS = [
    "lldb-dap.resolveDebugConfiguration",
    "lldb-dap.resolveDebugConfigurationWithSubstitutedVariables",
];

// The attributes a `heroes` configuration has that an `lldb-dap` one does not:
// they say what to build, and every other attribute is handed on unchanged.
const OWN_ATTRIBUTES = ["program", "buildArgs"];

// The words the launch chooses itself, so `buildArgs` may not hold them: the
// level is the one that describes the variables, and the binary goes where
// the launch looks for it.
const CHOSEN_WORDS = ["-O0", "-O2", "-o", "--emit-c", "--dump-ir"];

// The verbs a `heroes` task runs, each on one file.
const TASK_COMMANDS = ["check", "build", "run", "test"];

// The problem matchers declared in package.json, by the name a task uses.
const MATCHERS = manifest.contributes.problemMatchers.map((matcher) => "$" + matcher.name);

let output;
let diagnostics;

function activate(context) {
    output = vscode.window.createOutputChannel("Heroes");
    diagnostics = vscode.languages.createDiagnosticCollection("heroes-launch");
    context.subscriptions.push(output, diagnostics);
    context.subscriptions.push(vscode.debug.registerDebugConfigurationProvider("heroes", new HeroesLaunch()));
    context.subscriptions.push(vscode.tasks.registerTaskProvider("heroes", new HeroesTasks()));
}

function deactivate() {}

// The `heroes` debug type. VS Code calls `resolveDebugConfiguration` before
// it substitutes `${file}` and the other variables, and
// `resolveDebugConfigurationWithSubstitutedVariables` after; the build needs
// the real path, so it happens in the second, which returns a configuration
// of the `lldb-dap` type. VS Code then starts that type's adapter, and a
// restart re-resolves the original `heroes` configuration only where
// launch.json changed it.
class HeroesLaunch {
    resolveDebugConfiguration(folder, config) {
        if (!config.type && !config.request && !config.name) {
            const editor = vscode.window.activeTextEditor;

            if (!editor || editor.document.languageId !== "hero") {
                vscode.window.showErrorMessage(
                    "Heroes: open the .hero file that holds `function main()` to debug it, or add a Heroes configuration to launch.json."
                );
                return undefined;
            }
            config = { type: "heroes", request: "launch", name: "Debug the active Heroes file", program: "${file}" };
        }

        if (config.request !== "launch") {
            vscode.window.showErrorMessage(
                "Heroes: a `heroes` configuration launches a program; to attach to a running one, write an `lldb-dap` configuration."
            );
            return undefined;
        }

        if (config.program === undefined) {
            config.program = "${file}";
        }
        return config;
    }

    async resolveDebugConfigurationWithSubstitutedVariables(folder, config, token) {
        const base = folder ? folder.uri.fsPath : undefined;
        const program = programOf(config, base);

        if (program.error) {
            vscode.window.showErrorMessage("Heroes: " + program.error);
            return undefined;
        }
        const extra = buildArgsOf(config);

        if (extra.error) {
            vscode.window.showErrorMessage("Heroes: " + extra.error);
            return undefined;
        }
        folder = folder || vscode.workspace.getWorkspaceFolder(vscode.Uri.file(program.path));
        const root = folder ? folder.uri.fsPath : path.dirname(program.path);

        // The adapter is looked for before the build, so a missing one costs
        // no compile.
        if (!(await adapterReady())) {
            return undefined;
        }
        const compiler = compilerFor(folder);

        if (compiler.error) {
            vscode.window.showErrorMessage("Heroes: " + compiler.error);
            return undefined;
        }
        const binary = binaryFor(program.path, root);
        const built = await buildForDebugging({ compiler: compiler.path, program: program.path, binary, extra: extra.words, cwd: root, token });

        if (!built) {
            return undefined;
        }
        let launch = {};

        for (const key of Object.keys(config)) {
            if (!OWN_ATTRIBUTES.includes(key)) {
                launch[key] = config[key];
            }
        }
        launch.type = LLDB_DAP_TYPE;
        launch.program = binary;

        if (launch.cwd === undefined) {
            launch.cwd = root;
        }

        if (launch.args === undefined) {
            launch.args = [];
        }

        // On a record's or a str's collapsed row, its fields: `{f_x:3, f_y:4}`
        // and `{ptr:"probe", len:5}` where lldb-dap's own default shows
        // `h_probe_Point @ 0x16fdfe160` (measured over the Debug Adapter
        // Protocol, 2026-10-12). Left alone where the configuration or the
        // user's lldb-dap settings say otherwise.
        if (launch.enableAutoVariableSummaries === undefined && !userChose("enableAutoVariableSummaries", folder)) {
            launch.enableAutoVariableSummaries = true;
        }
        launch.initCommands = withFormatter(launch.initCommands);
        const commands = await vscode.commands.getCommands(true);

        for (const command of LLDB_DAP_RESOLVERS) {
            if (!commands.includes(command)) {
                continue;
            }
            launch = await vscode.commands.executeCommand(command, folder, launch, token);

            // undefined or null: lldb-dap stopped the launch and said why.
            if (!launch) {
                return launch;
            }
        }
        return launch;
    }
}

// The `.hero` file a configuration names, as an absolute path that exists.
function programOf(config, base) {
    const named = config.program;

    if (typeof named !== "string" || named.length === 0) {
        return { error: "the configuration's `program` must name the .hero file that holds `function main()`." };
    }
    const absolute = path.isAbsolute(named) ? named : path.resolve(base || process.cwd(), named);

    if (path.extname(absolute) !== ".hero") {
        return { error: "`program` is " + absolute + ", which is not a .hero file; the launch builds the .hero file and debugs what it builds." };
    }

    if (!isFile(absolute)) {
        return { error: "`program` is " + absolute + ", and there is no such file." };
    }
    return { path: absolute };
}

// The configuration's further words for `heroes build`, refusing the ones the
// launch chooses itself.
function buildArgsOf(config) {
    const words = config.buildArgs === undefined ? [] : config.buildArgs;

    if (!Array.isArray(words) || words.some((word) => typeof word !== "string")) {
        return { error: "`buildArgs` must be a list of words for `heroes build`, such as [\"--include\", \"vendor\"]." };
    }
    const chosen = words.find((word) => CHOSEN_WORDS.includes(word));

    if (chosen !== undefined) {
        return { error: "`buildArgs` holds `" + chosen + "`, which the launch chooses itself: it builds at -O0, the level whose debug information describes the variables, into build/debug." };
    }
    return { words };
}

// Where the debug build of `program` goes: under the root's `build/debug`,
// at the file's own path from the root, so two `main.hero` in two folders
// are two binaries. `heroes build` gives an author's name with no suffix the
// platform's, so the name the launch hands lldb-dap carries it too.
function binaryFor(program, root) {
    const relative = path.relative(root, program);
    const inside = relative.length > 0 && !relative.startsWith("..") && !path.isAbsolute(relative);
    const stem = (inside ? relative : path.basename(program)).replace(/\.hero$/, "");
    return path.join(root, "build", "debug", stem) + (process.platform === "win32" ? ".exe" : "");
}

// The compiler: the `heroes.path` setting where it is set, else the folder's
// own `heroes` (a checkout of the language builds one at its root), else the
// first `heroes` on PATH.
function compilerFor(folder) {
    const scope = folder ? folder.uri : undefined;
    const base = folder ? folder.uri.fsPath : undefined;
    const said = String(vscode.workspace.getConfiguration("heroes", scope).get("path", "") || "").trim();

    if (said.length > 0) {
        let named = said === "~" || said.startsWith("~/") || said.startsWith("~\\") ? path.join(os.homedir(), said.slice(1)) : said;

        if (!path.isAbsolute(named) && !/[\\/]/.test(named)) {
            const found = onPath(named);
            return found ? { path: found } : { error: "the setting `heroes.path` says `" + said + "`, and no such program is on PATH." };
        }

        if (!path.isAbsolute(named)) {
            if (!base) {
                return { error: "the setting `heroes.path` says `" + said + "`, a relative path, and no workspace folder is open to read it from." };
            }
            named = path.join(base, named);
        }
        return isExecutable(named) ? { path: named } : { error: "the setting `heroes.path` says `" + said + "`, and " + named + " is not a program that can be run." };
    }

    if (base) {
        const own = path.join(base, programName("heroes"));

        if (isExecutable(own)) {
            return { path: own };
        }
    }
    const found = onPath("heroes");
    return found ? { path: found } : { error: "no `heroes` compiler was found: set `heroes.path`, put `heroes` on PATH, or build it at the workspace folder's root." };
}

function programName(name) {
    return process.platform === "win32" && path.extname(name) === "" ? name + ".exe" : name;
}

function onPath(name) {
    const value = process.env.PATH || process.env.Path || "";

    for (const dir of value.split(path.delimiter)) {
        if (dir.length === 0) {
            continue;
        }
        const candidate = path.join(dir, programName(name));

        if (isExecutable(candidate)) {
            return candidate;
        }
    }
    return undefined;
}

function isFile(candidate) {
    try {
        return fs.statSync(candidate).isFile();
    } catch (_) {
        return false;
    }
}

function isExecutable(candidate) {
    if (!isFile(candidate)) {
        return false;
    }

    if (process.platform === "win32") {
        return true;
    }

    try {
        fs.accessSync(candidate, fs.constants.X_OK);
        return true;
    } catch (_) {
        return false;
    }
}

// The formatter that shows Heroes values in the Variables view, `Point(x: 3,
// y: 4)`, `["ada", "grace"]`, `ok(4)`, where lldb alone shows the emitted C's
// structs and an array's address (editors/lldb/heroes_lldb.py, panel 213). The
// packaging copies it beside this script, so the import comes first and the
// user's own `initCommands` run after it, unchanged. Where the file is not
// there, a package made before it, the launch goes on as it did and the output
// channel says why the values are C; where `initCommands` is not a list, it is
// left for lldb-dap to refuse in its own words.
function withFormatter(initCommands) {
    const formatter = path.join(__dirname, "lldb", "heroes_lldb.py");

    if (initCommands !== undefined && !Array.isArray(initCommands)) {
        return initCommands;
    }

    if (!isFile(formatter)) {
        output.appendLine("(no " + formatter + " in this package, so lldb shows the values as C: package the extension again, editors/README.md § Install)");
        return initCommands;
    }
    const quoted = '"' + formatter.replace(/\\/g, "\\\\").replace(/"/g, '\\"') + '"';
    return ["command script import " + quoted].concat(initCommands || []);
}

// Whether the user set one of lldb-dap's own settings anywhere.
function userChose(key, folder) {
    const seen = vscode.workspace.getConfiguration("lldb-dap", folder ? folder.uri : undefined).inspect(key);
    return Boolean(seen) && [seen.globalValue, seen.workspaceValue, seen.workspaceFolderValue].some((value) => value !== undefined);
}

// The LLDB DAP extension, activated; where it is not installed the user is
// told what to install and shown it.
async function adapterReady() {
    const extension = vscode.extensions.getExtension(LLDB_DAP_EXTENSION);

    if (!extension) {
        const show = "Show LLDB DAP";
        vscode.window
            .showErrorMessage(
                "Heroes: debugging a Heroes program needs the LLDB DAP extension (" + LLDB_DAP_EXTENSION + "), which runs lldb-dap; this extension builds the program and hands it over.",
                show
            )
            .then((picked) => {
                if (picked === show) {
                    vscode.commands.executeCommand("extension.open", LLDB_DAP_EXTENSION);
                }
            });
        return false;
    }

    if (!extension.isActive) {
        await extension.activate();
    }
    return true;
}

// `heroes build -O0`, with what it says in the Heroes output channel and its
// diagnostics in the Problems view. True where it built the binary.
async function buildForDebugging(job) {
    fs.mkdirSync(path.dirname(job.binary), { recursive: true });
    const words = ["build", job.program, "-O0", "-o", job.binary].concat(job.extra);
    output.appendLine("$ " + [job.compiler].concat(words).join(" "));

    const result = await vscode.window.withProgress(
        { location: vscode.ProgressLocation.Notification, title: "Heroes: building " + path.basename(job.program) + " at -O0", cancellable: true },
        (_, cancel) => run(job.compiler, words, job.cwd, [job.token, cancel])
    );
    const said = result.stderr + result.stdout;
    output.append(said);
    diagnostics.clear();

    if (result.cancelled) {
        output.appendLine("(the build was cancelled)");
        return false;
    }

    if (result.spawnError) {
        output.appendLine(String(result.spawnError));
        output.show(true);
        vscode.window.showErrorMessage("Heroes: " + job.compiler + " could not be run: " + result.spawnError.message);
        return false;
    }
    const found = matchDiagnostics(said, job.cwd);

    for (const [file, list] of found) {
        diagnostics.set(vscode.Uri.file(file), list);
    }

    if (result.code === 0 && isFile(job.binary)) {
        return true;
    }
    output.show(true);
    const ended = result.signal ? "ended by " + result.signal : "exit " + result.code;
    const first = said.split(/\r?\n/).find((line) => line.trim().length > 0) || ended;
    vscode.window.showErrorMessage("Heroes: " + path.basename(job.program) + " did not build, so nothing was launched. " + first);
    return false;
}

// One process to its end: its exit code (or the signal that ended it) and
// what it wrote, or why it could not start, or that it was cancelled.
function run(command, words, cwd, tokens) {
    return new Promise((resolve) => {
        let cancelled = false;
        const child = childProcess.execFile(command, words, { cwd, maxBuffer: 64 * 1024 * 1024, windowsHide: true }, (error, stdout, stderr) => {
            const text = { stdout: String(stdout || ""), stderr: String(stderr || "") };

            if (cancelled) {
                resolve(Object.assign(text, { cancelled: true }));
            } else if (!error) {
                resolve(Object.assign(text, { code: 0 }));
            } else if (typeof error.code === "number") {
                resolve(Object.assign(text, { code: error.code }));
            } else if (error.signal) {
                resolve(Object.assign(text, { code: null, signal: error.signal }));
            } else {
                resolve(Object.assign(text, { spawnError: error }));
            }
        });

        for (const token of tokens) {
            if (token && token.onCancellationRequested) {
                token.onCancellationRequested(() => {
                    cancelled = true;
                    child.kill();
                });
            }
        }
    });
}

// The diagnostics in a compiler's text, read by the problem matchers declared
// in package.json, so the launch and a task read one text the same way: the
// full form `heroes build` prints, `check --brief`'s one line, and a hole;
// each matcher's lines in a row, the location resolved from the build's directory,
// the severity as VS Code reads a matcher's (`error`, `warning`, `info`, and
// the matcher's own for any other word, `unsupported` among them).
function matchDiagnostics(text, cwd) {
    const lines = text.split(/\r?\n/);
    const byFile = new Map();

    for (const matcher of manifest.contributes.problemMatchers) {
        const patterns = [].concat(matcher.pattern).map((one) => Object.assign({}, one, { expression: new RegExp(one.regexp) }));
        let i = 0;

        while (i + patterns.length <= lines.length) {
            const fields = {};
            let all = true;

            for (let k = 0; k < patterns.length && all; k++) {
                const got = patterns[k].expression.exec(lines[i + k]);

                if (!got) {
                    all = false;
                    continue;
                }

                for (const field of ["file", "line", "column", "severity", "code", "message"]) {
                    if (patterns[k][field] !== undefined) {
                        fields[field] = got[patterns[k][field]];
                    }
                }
            }

            if (!all) {
                i += 1;
                continue;
            }
            const line = Math.max(Number(fields.line) - 1, 0);
            const column = Math.max(Number(fields.column) - 1, 0);
            const one = new vscode.Diagnostic(new vscode.Range(line, column, line, column), fields.message || "", severityOf(fields.severity, matcher.severity));
            one.source = matcher.source;

            if (fields.code !== undefined) {
                one.code = fields.code;
            }
            const file = path.resolve(cwd, fields.file);
            byFile.set(file, (byFile.get(file) || []).concat([one]));
            i += patterns.length;
        }
    }
    return byFile;
}

function severityOf(word, otherwise) {
    const said = String(word || "").toLowerCase();
    const named = { error: "Error", warning: "Warning", info: "Information", information: "Information", hint: "Hint" };
    const fallback = named[String(otherwise || "error").toLowerCase()] || "Error";
    return vscode.DiagnosticSeverity[named[said] || fallback];
}

// The `heroes` task type: `{ "type": "heroes", "command": "check", "file":
// "${file}" }` in tasks.json runs `heroes check` on that file, VS Code
// substituting the variables, with both problem matchers attached. The four
// verbs are also offered on the active `.hero` file.
class HeroesTasks {
    provideTasks() {
        const editor = vscode.window.activeTextEditor;

        if (!editor || editor.document.languageId !== "hero") {
            return [];
        }
        const folder = vscode.workspace.getWorkspaceFolder(editor.document.uri);
        const scope = folder || vscode.TaskScope.Workspace;
        return TASK_COMMANDS.map((command) => taskFor({ type: "heroes", command, file: "${file}" }, scope, command + " the active file")).filter(Boolean);
    }

    resolveTask(task) {
        return taskFor(task.definition, task.scope, task.name);
    }
}

function taskFor(definition, scope, name) {
    if (!TASK_COMMANDS.includes(definition.command) || typeof definition.file !== "string") {
        vscode.window.showErrorMessage("Heroes: a `heroes` task names a `command` (" + TASK_COMMANDS.join(", ") + ") and a `file`.");
        return undefined;
    }
    const extra = definition.args === undefined ? [] : definition.args;

    if (!Array.isArray(extra) || extra.some((word) => typeof word !== "string")) {
        vscode.window.showErrorMessage("Heroes: a `heroes` task's `args` is a list of words.");
        return undefined;
    }
    const folder = scope && scope.uri ? scope : undefined;
    const compiler = compilerFor(folder);

    if (compiler.error) {
        vscode.window.showErrorMessage("Heroes: " + compiler.error);
        return undefined;
    }
    const options = folder ? { cwd: folder.uri.fsPath } : {};
    const words = [definition.command, definition.file].concat(extra);
    const task = new vscode.Task(definition, scope || vscode.TaskScope.Workspace, name, "heroes", new vscode.ProcessExecution(compiler.path, words, options), MATCHERS);

    if (definition.command === "build" || definition.command === "check") {
        task.group = vscode.TaskGroup.Build;
    } else if (definition.command === "test") {
        task.group = vscode.TaskGroup.Test;
    }
    return task;
}

module.exports = { activate, deactivate };
