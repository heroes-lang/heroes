# Task suite — metric 2

Target: 20 frozen tasks, author-confirmed. Format: one file per task,
`NN-slug.md`, containing only the task prose a model would receive (no
solutions here — graders live in the goldens once the compiler exists).

Guidelines (panel 000): tasks must be solvable from the spec alone, span the
surface (records, variants+match, `T?`+`?`, `@` parameters, loops, maps,
generics use, tests), and include at least five designed to *tempt* the
plausible mistakes the mutation operators encode (same-typed args, missing
variant case, empty-container inference).

Status: 5 assistant drafts below, `# UNVERIFIED — author must confirm`;
15 more needed from the author (they are the held-out set — the assistant
must NOT write them all, or the suite measures the assistant's priors).
