//! What a running program is expected to do, and the one reader of it.
//!
//! Two harnesses execute programs and diff what comes out: `golden.rs` over
//! `tests/golden/run/`, and `corpus.rs` over `examples/`. They ran the same
//! corpus convention, so the convention lives here rather than in whichever of
//! them was written first — a second copy is a second place for the meaning of
//! `!exit: 0` to drift.
//!
//! The convention is QBE's: the expected output lives in the case, and so does
//! the way the program ends. An expectation is the program's stdout, optionally
//! followed by one last line saying it does **not** end at exit 0 with a silent
//! stderr:
//!
//! - `!panic: <message>` — an abort, and the text stderr must carry;
//! - `!exit: <code>` — `exit(code)`, and the status the shell must see.
//!
//! Both exist because the interesting half of §4.14 is the aborts: a program
//! that overflows must **stop**, and the wrong emitter does not crash, it prints
//! a wrapped number at exit 0. That is only a golden if the harness can spell it.
//! A case with no last line still has to exit 0, so a program that starts
//! exiting by accident is still a failure.

use std::path::Path;

/// How a case ends, when it does not end at exit 0 with nothing on stderr.
pub enum Expectation {
    /// `!panic: <message>` — an abort, and the message stderr must carry.
    Panic(String),
    /// `!exit: <code>` — `exit(code)`, and the status the shell must see.
    Exit(i32),
}

/// Splits an expectation file into the stdout it promises and how it ends.
pub fn split_expectation(text: &str) -> (String, Option<Expectation>) {
    let mut lines: Vec<&str> = text.lines().collect();
    let mut ending = None;
    if let Some(last) = lines.last() {
        if let Some(message) = last.strip_prefix("!panic: ") {
            ending = Some(Expectation::Panic(message.to_string()));
            lines.pop();
        } else if let Some(code) = last.strip_prefix("!exit: ") {
            ending = Some(Expectation::Exit(code.trim().parse().expect("an exit code")));
            lines.pop();
        }
    }
    let mut stdout = lines.join("\n");
    if !stdout.is_empty() {
        stdout.push('\n');
    }
    (stdout, ending)
}

/// One execution, against one expectation. `level` names the configuration, so a
/// failure says which of the three disagreed with the other two.
pub fn check(
    case: &Path,
    level: &str,
    output: &std::process::Output,
    expected: &str,
    ending: &Option<Expectation>,
) {
    // **The whole ending, in the failure message.** A mismatch on stdout used to
    // report stdout and nothing else, so a program that printed nothing looked
    // exactly like a program that failed to build, aborted early, or was killed —
    // four different causes behind one message. That cost a CI round trip on the
    // first Linux run this project ever had (M-program-corpus): the only machine
    // that could answer the question was the one that could not be asked twice.
    let ending = match output.status.code() {
        Some(code) => format!("exit {code}"),
        None => "killed by a signal".to_string(),
    };
    assert_eq!(
        String::from_utf8_lossy(&output.stdout),
        expected,
        "{} prints something else at {level} — the same corpus, one configuration apart\n  \
         it ended: {ending}\n  its stderr was:\n{}",
        case.display(),
        String::from_utf8_lossy(&output.stderr)
    );
    match ending {
        None => assert_eq!(
            output.status.code(),
            Some(0),
            "{} did not exit 0 at {level}",
            case.display()
        ),
        Some(Expectation::Exit(code)) => assert_eq!(
            output.status.code(),
            Some(*code),
            "{} did not exit {code} at {level}",
            case.display()
        ),
        Some(Expectation::Panic(message)) => {
            let said = String::from_utf8_lossy(&output.stderr);
            assert!(
                said.contains(message),
                "{} at {level} must abort with {message:?}, and said:\n{said}",
                case.display()
            );
            assert_ne!(
                output.status.code(),
                Some(0),
                "{} at {level} must not exit 0: an abort that returns success is the silent \
                 wrong answer this case exists to catch",
                case.display()
            );
        }
    }
}
