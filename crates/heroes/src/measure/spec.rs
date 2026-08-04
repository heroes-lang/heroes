//! The spec against the compiler: the two must name the same language.
//!
//! `spec/heroes-spec.md` is not documentation, it is **the prompt** (§1.6) — the
//! whole context a model writing Heroes ever has. So a rule the compiler enforces
//! and the spec omits is not terseness, it is a wrong briefing, and the project
//! shipped one for four milestones: panel 013 ratified `(function(A) -> B)` while
//! the spec still said `(fn(A) -> B)`, which is a *reserved-word error* in every
//! position. §4.20's inventory listed `sort` and the int-to-string conversion; the
//! spec listed neither, so `sort = function:` was spec-legal and
//! resolver-rejected.
//!
//! These tests are the mechanism that makes that class of defect loud. They do
//! not check prose — they check the two places where the spec makes a *claim the
//! compiler can be asked about*: the built-in names it promises, and the spellings
//! it uses for forms the lexer has an opinion about.

#[cfg(test)]
mod tests {
    use crate::measure::spec_path;
    use crate::resolve::BUILTINS;

    fn spec() -> String {
        std::fs::read_to_string(spec_path()).expect("the spec must exist")
    }

    /// Every identifier written inside a code span — a backticked fragment or a
    /// fenced block. That is where the spec makes claims a compiler can be asked
    /// about; its prose is not searched, because `all` and `has` are English words.
    fn identifiers_in_code() -> std::collections::BTreeSet<String> {
        let text = spec();
        let mut code = String::new();
        for (index, part) in text.split("```").enumerate() {
            if index % 2 == 1 {
                code.push_str(part);
                code.push('\n');
                continue;
            }
            for (inner, span) in part.split('`').enumerate() {
                if inner % 2 == 1 {
                    code.push_str(span);
                    code.push('\n');
                }
            }
        }
        code.split(|c: char| !c.is_alphanumeric() && c != '_')
            .filter(|word| !word.is_empty())
            .map(|word| word.to_string())
            .collect()
    }

    /// Every name the compiler reserves must appear in the spec, because a name a
    /// model cannot see is a name it will redeclare — and one it cannot find is a
    /// name it will write by hand.
    #[test]
    fn the_spec_names_every_builtin_the_compiler_reserves() {
        let mentioned = identifiers_in_code();
        let missing: Vec<&str> = BUILTINS
            .iter()
            .map(|b| b.name)
            .filter(|name| !mentioned.contains(*name))
            .collect();
        assert_eq!(
            missing,
            Vec::<&str>::new(),
            "the compiler reserves these and the spec never mentions them"
        );
    }

    /// …and the reverse: a name the spec offers must exist, or a model will call
    /// something that is not there.
    #[test]
    fn every_name_the_spec_offers_as_a_builtin_exists() {
        let text = spec();
        let start = text.find("Built-ins:").expect("the built-in list must exist");
        let end = text[start..].find("\n\n").map_or(text.len(), |at| start + at);
        let listed: Vec<String> = text[start..end]
            .split('`')
            .skip(1)
            .step_by(2)
            .map(|name| name.trim_end_matches("(...)").trim().to_string())
            .filter(|name| !name.is_empty() && !name.contains(' '))
            .collect();
        assert!(listed.len() > 10, "the list looks truncated: {listed:?}");
        for name in listed {
            let bare = name.split('(').next().unwrap_or(&name).to_string();
            assert!(
                BUILTINS.iter().any(|b| b.name == bare),
                "the spec offers `{bare}` and the compiler has no such built-in"
            );
        }
    }

    /// No word the lexer rejects may appear **in a code span**. `fn` is the case
    /// that happened: the foreign-word registry calls it an error *and* the spec
    /// used it in the function type, so every function-typed annotation a model
    /// wrote from the spec was a compile error with a certain fix attached — for
    /// four milestones.
    ///
    /// Code spans only. The prose says "Construct an error with…", and forbidding
    /// the English word "construct" because `struct` is a substring of it would be
    /// a test that fails for a reason nobody can act on.
    #[test]
    fn the_spec_never_uses_a_word_the_compiler_rejects() {
        let mentioned = identifiers_in_code();
        let forbidden: Vec<&String> = mentioned
            .iter()
            .filter(|word| crate::lexer::is_foreign_word(word))
            .collect();
        assert_eq!(
            forbidden,
            Vec::<&String>::new(),
            "these appear in the spec's code and the lexer rejects them with a prescribed fix"
        );
    }

    /// The budget, as a test rather than a habit: §1.6's ceiling is 3000 measured
    /// (panel 012), and the soft line is 2000. Breaching the hard one fails here.
    #[test]
    fn the_spec_is_under_the_measured_ceiling() {
        let text = spec();
        let measured = crate::measure::measure(&text, &crate::measure::vendor_dir())
            .expect("the vendored tokenisers must load");
        assert!(
            measured.max() <= 3000,
            "the spec measures {} against §1.6's ceiling of 3000",
            measured.max()
        );
    }
}
