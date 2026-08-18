//! The BPE rank table and the merge loop.
//!
//! A BPE tokeniser is a ranked list of byte sequences. To tokenise a piece,
//! start with one part per byte and repeatedly merge the *lowest-ranked*
//! adjacent pair that appears in the table, until no adjacent pair does.
//! The token count is the number of parts left. That is the whole
//! algorithm — the vocabulary is where the work went, and we vendor it.
//!
//! Two on-disk shapes are read (`vendor/tokenizers/README.md`):
//! `<base64> <rank>` per line, and Anthropic's JSON whose `bpe_ranks` field
//! is one space-separated string of base64 in rank order. Neither loader
//! parses JSON properly — the workspace has zero dependencies and both
//! formats are regular enough to scan.

use std::collections::BTreeMap;

/// Token bytes → rank. `BTreeMap` per the Cyclone rule: iteration order is
/// deterministic, which matters everywhere in this project.
pub struct Ranks {
    map: BTreeMap<Vec<u8>, u32>,
}

impl Ranks {
    pub fn len(&self) -> usize {
        self.map.len()
    }

    pub fn is_empty(&self) -> bool {
        self.map.is_empty()
    }

    /// `<base64> <rank>` per line (the `.tiktoken` shape). Lines that do
    /// not split into two fields are skipped rather than failing the run:
    /// a trailing newline is not a corrupt table.
    pub fn from_tiktoken(text: &str) -> Ranks {
        let mut map = BTreeMap::new();
        for line in text.lines() {
            let mut fields = line.split_whitespace();
            let (Some(token), Some(rank)) = (fields.next(), fields.next()) else {
                continue;
            };
            if let (Some(bytes), Ok(rank)) = (base64_decode(token), rank.parse::<u32>()) {
                map.insert(bytes, rank);
            }
        }
        Ranks { map }
    }

    /// Anthropic's `claude.json`: the `bpe_ranks` field holds base64 tokens
    /// in rank order, separated by spaces, after a two-field header.
    pub fn from_claude_json(text: &str) -> Ranks {
        let mut map = BTreeMap::new();
        if let Some(field) = json_string_field(text, "bpe_ranks") {
            for (rank, token) in field.split(' ').skip(2).enumerate() {
                if let Some(bytes) = base64_decode(token) {
                    map.insert(bytes, rank as u32);
                }
            }
        }
        Ranks { map }
    }

    /// How many tokens one pre-token piece costs.
    ///
    /// The parts are tracked as boundary offsets into `piece`, so a merge
    /// is one `remove` and no copying. Quadratic in the number of parts,
    /// which is fine: pieces are words, not documents.
    pub fn count_piece(&self, piece: &[u8]) -> usize {
        if piece.len() <= 1 {
            return piece.len();
        }
        let mut bounds: Vec<usize> = (0..=piece.len()).collect();
        while bounds.len() > 2 {
            let mut best: Option<(u32, usize)> = None;
            for i in 0..bounds.len() - 2 {
                let pair = &piece[bounds[i]..bounds[i + 2]];
                if let Some(&rank) = self.map.get(pair) {
                    if best.is_none_or(|(best_rank, _)| rank < best_rank) {
                        best = Some((rank, i));
                    }
                }
            }
            match best {
                Some((_, i)) => {
                    bounds.remove(i + 1);
                }
                None => break,
            }
        }
        bounds.len() - 1
    }
}

/// Extract `"<name>": "<value>"` from JSON without a parser. Good enough
/// for one known field in one known file; not a general reader.
fn json_string_field<'a>(text: &'a str, name: &str) -> Option<&'a str> {
    let key = format!("\"{name}\"");
    let after_key = &text[text.find(&key)? + key.len()..];
    let open = after_key.find('"')? + 1;
    let rest = &after_key[open..];
    Some(&rest[..rest.find('"')?])
}

fn base64_decode(s: &str) -> Option<Vec<u8>> {
    let value = |c: u8| -> Option<u32> {
        Some(match c {
            b'A'..=b'Z' => (c - b'A') as u32,
            b'a'..=b'z' => (c - b'a') as u32 + 26,
            b'0'..=b'9' => (c - b'0') as u32 + 52,
            b'+' => 62,
            b'/' => 63,
            _ => return None,
        })
    };
    let mut out = Vec::new();
    let mut acc: u32 = 0;
    let mut bits = 0u32;
    for &c in s.as_bytes() {
        if c == b'=' {
            break;
        }
        acc = (acc << 6) | value(c)?;
        bits += 6;
        if bits >= 8 {
            bits -= 8;
            out.push((acc >> bits) as u8);
            acc &= (1 << bits) - 1;
        }
    }
    Some(out)
}
