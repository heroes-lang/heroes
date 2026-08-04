//! `heroes doctor` — verify the toolchain the compiler depends on.
//!
//! Checks: clang presence and version, Xcode Command Line Tools, architecture,
//! and that the build cache directory is writable. Exit code is non-zero if a
//! required piece is missing, so scripts can gate on it.

use std::process::Command;

use crate::cli::Exit;

struct Check {
    name: &'static str,
    ok: bool,
    detail: String,
}

fn run_capture(cmd: &str, args: &[&str]) -> Option<String> {
    let out = Command::new(cmd).args(args).output().ok()?;
    if !out.status.success() {
        return None;
    }
    let text = String::from_utf8_lossy(&out.stdout);
    Some(text.lines().next().unwrap_or("").trim().to_string())
}

pub fn run() -> Exit {
    let mut checks: Vec<Check> = Vec::new();

    // clang — the backend. Generated C is compiled by it (plan: C11 emission).
    let clang = run_capture("clang", &["--version"]);
    checks.push(Check {
        name: "clang",
        ok: clang.is_some(),
        detail: clang.unwrap_or_else(|| "not found — install Xcode Command Line Tools".into()),
    });

    // Xcode CLT — provides the assembler and linker on macOS.
    let clt = run_capture("xcode-select", &["-p"]);
    checks.push(Check {
        name: "xcode CLT",
        ok: clt.is_some(),
        detail: clt.unwrap_or_else(|| "not found — run `xcode-select --install`".into()),
    });

    // Architecture — informational; the emitted C is portable, clang targets host.
    checks.push(Check {
        name: "arch",
        ok: true,
        detail: std::env::consts::ARCH.to_string(),
    });

    // The runtime — found by searching, so `doctor` is the place that says which of
    // the three candidates won. It is also the only place `$HEROES_RUNTIME` is
    // visible, being an input channel outside the argv table (CLAUDE.md §10).
    let runtime = crate::commands::toolchain::Toolchain::find();
    checks.push(match &runtime {
        Ok(found) => Check {
            name: "runtime",
            ok: true,
            detail: format!("{}", found.runtime.display()),
        },
        Err(message) => Check {
            name: "runtime",
            ok: false,
            detail: message.lines().next().unwrap_or("not found").to_string(),
        },
    });

    // build/ cache — must be creatable and writable.
    let cache_ok = std::fs::create_dir_all("build")
        .and_then(|_| {
            let probe = std::path::Path::new("build/.doctor-probe");
            std::fs::write(probe, b"ok")?;
            std::fs::remove_file(probe)
        })
        .is_ok();
    checks.push(Check {
        name: "build cache",
        ok: cache_ok,
        detail: if cache_ok {
            "build/ writable".into()
        } else {
            "cannot write to build/".into()
        },
    });

    let mut all_ok = true;
    for c in &checks {
        let mark = if c.ok { "ok " } else { "FAIL" };
        println!("{mark}  {:<12} {}", c.name, c.detail);
        all_ok &= c.ok;
    }

    if all_ok {
        Exit::Ok
    } else {
        Exit::Failed
    }
}
