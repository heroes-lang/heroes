//! Pre-tokenisation: cutting text into the pieces BPE merges within.
//!
//! Before any merging happens, a BPE tokeniser splits the text on a fixed
//! pattern so that merges can never cross a boundary — which is why
//! `"hello"` and `" hello"` are different tokens, and why a run of spaces
//! is its own piece. Both vendored tables use the GPT-2 pattern, stated in
//! `claude-legacy.json`'s own `pat_str` field:
//!
//! ```text
//! 's|'t|'re|'ve|'m|'ll|'d| ?\p{L}+| ?\p{N}+| ?[^\s\p{L}\p{N}]+|\s+(?!\S)|\s+
//! ```
//!
//! Read as alternatives tried in order: a few English contractions; then an
//! optional single space followed by a run of letters, or of digits, or of
//! anything that is neither letter, digit nor space; then whitespace —
//! greedily, **except** that a run of whitespace followed by a non-space
//! gives back its last character, so the space that begins the next word
//! belongs to that word.
//!
//! It is written out by hand here rather than pulled from a regex crate:
//! the workspace has zero dependencies (`Cargo.toml`) and this is ~70 lines.

/// Split `text` into pre-token pieces, in order. Concatenating the result
/// reproduces the input exactly.
pub(super) fn pretokenize(text: &str) -> Vec<String> {
    let chars: Vec<char> = text.chars().collect();
    let mut pieces = Vec::new();
    let mut i = 0;
    while i < chars.len() {
        let taken = contraction(&chars, i)
            .or_else(|| run(&chars, i, |c| c.is_alphabetic()))
            .or_else(|| run(&chars, i, |c| c.is_numeric()))
            .or_else(|| run(&chars, i, is_other))
            .or_else(|| whitespace(&chars, i))
            .unwrap_or(1);
        pieces.push(chars[i..i + taken].iter().collect());
        i += taken;
    }
    pieces
}

/// Neither letter, digit, nor whitespace — the third `run` class.
fn is_other(c: char) -> bool {
    !c.is_whitespace() && !c.is_alphabetic() && !c.is_numeric()
}

const CONTRACTIONS: [&str; 7] = ["'s", "'t", "'re", "'ve", "'m", "'ll", "'d"];

fn contraction(chars: &[char], i: usize) -> Option<usize> {
    CONTRACTIONS.iter().find_map(|c| {
        let want: Vec<char> = c.chars().collect();
        (chars.len() >= i + want.len() && chars[i..i + want.len()] == want[..]).then_some(want.len())
    })
}

/// ` ?<class>+` — an optional leading space, then one or more of the class.
/// Returns the length in characters, or `None` if the class does not match.
fn run(chars: &[char], i: usize, class: fn(char) -> bool) -> Option<usize> {
    let start = if chars[i] == ' ' && chars.get(i + 1).is_some_and(|&c| class(c)) {
        i + 1
    } else {
        i
    };
    if !chars.get(start).is_some_and(|&c| class(c)) {
        return None;
    }
    let mut end = start;
    while chars.get(end).is_some_and(|&c| class(c)) {
        end += 1;
    }
    Some(end - i)
}

/// `\s+(?!\S)|\s+` — a whitespace run, giving back its last character when
/// something non-space follows, so that space joins the next word.
fn whitespace(chars: &[char], i: usize) -> Option<usize> {
    if !chars[i].is_whitespace() {
        return None;
    }
    let mut end = i;
    while chars.get(end).is_some_and(|c| c.is_whitespace()) {
        end += 1;
    }
    let stop = if end < chars.len() && end > i { end - 1 } else { end };
    Some(if stop > i { stop - i } else { end - i })
}
