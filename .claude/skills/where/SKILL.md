---
name: where
description: Orient the author — explain in very simple ITALIAN where the Heroes project stands, what is being built right now and why, assuming zero compiler knowledge. Use whenever the author asks "a che punto siamo", "cosa stiamo facendo", "spiegami", or returns after a break.
---

# /where — orient the author, gently

The author knows programming languages well but is **learning compiler
construction from zero through this project**. This skill produces a status
report that is also a small lesson. It must never make them feel behind.

## Output language: ITALIAN. Everything the author reads from this skill is Italian.
(The repo's "English artifacts" rule applies to files; this output is
conversation.) Keep technical identifiers as-is (`lexer/`, `heroes check`,
file names), but **every technical term gets a plain-Italian explanation the
first time it appears — no exceptions, take nothing for granted.**

## Step 1 — Gather the facts (do not guess)

```
git log --oneline -15
git tag --list --sort=creatordate
tail -8 DESIGN-LOG.md
head -40 docs/ROADMAP.md          # § Status + § The order: where we are, what is next
cat docs/debrief/QUEUE.md         # open items = everything owed to the author
cargo test --quiet 2>&1 | tail -2 # green or red, one line
```

Determine: current milestone (from tags + commit messages), what the last
3–5 commits actually did, what is waiting on the author.

## Step 2 — Emit the report, in this exact structure

### 1. «La mappa» — always the same drawing
Show the full pipeline with today's position marked. Use this fixed picture
(✅ done · 🔨 in progress · ⬜ not started), one line per stage:

```
testo .hero
  → [M-token-stream    lexer]
  → [M-syntax-tree     parser]
  → [M-typed-frontend  controllo dei tipi]
  → [M-ir-lowering     semplificazione + IR]
  → [M-native-backend  emissione C + runtime]
  → binario nativo

poi: [M-module-namespace moduli] → [M-ffi-ladder FFI]
  → [M-program-corpus corpus] → [M-selfhost-probe sonda]
  → [M-selfhost-port port] → [M-selfhost-fixpoint fixpoint] = v1
  (l'ordine è quello scritto in docs/ROADMAP.md § The order; il nome
   non promette una posizione)
```

### 2. «Dove siamo» — 3–6 sentences
What milestone we are in, what the last commits did, whether tests are green —
in everyday words. Numbers and file names welcome, jargon translated.

### 3. «Cosa stiamo costruendo adesso, spiegato semplice»
The heart of the skill: explain the CURRENT stage as if to a smart friend who
has never heard the word "compilatore". Rules:
- Start from what a compiler even is if relevant: *un traduttore da un
  linguaggio che capisci tu a uno che capisce la macchina, fatto a catena di
  montaggio: ogni stazione fa una sola trasformazione*.
- Use ONE tiny concrete example from the repo (3–6 lines of `.hero` or of a
  spike) and show what THIS stage does to it, before → after.
- Use the canonical analogies below — always the same ones, so the author's
  mental model accumulates instead of resetting.

### 4. «Tocca a te» — the author's pending items (never blocking)
Summarize the open `docs/debrief/QUEUE.md` items (ratifications, drills,
baseline run, exercises), each with the file path and why it is worth their
time — e.g. *la diagnosi ha più valore se provi a indovinare la causa prima
di leggere il fix*. Make clear nothing is waiting on them to proceed.

### 5. «La pillola di oggi» — one micro-lesson
ONE concept (3–5 sentences max), tied to the current stage, with its analogy.
End with a question the author can answer mentally to check they got it.
If the concept has a `docs/glossary/` entry, link it; if the pillola resolves
a fresh gap, distill it into a new glossary entry afterwards (a gap resolved
is an artifact earned — see `/debrief`).

## Canonical analogies (use these, always the same)

| Term | Italian explanation to use |
|---|---|
| compilatore | un traduttore a catena di montaggio: ogni stazione una trasformazione |
| lexer | taglia il testo in «parole» (token): `2+3` → [`2`, `+`, `3`] — come dividere una frase in parole prima di analizzarla |
| token | una parola del linguaggio, con etichetta: «numero 2», «segno più» |
| parser | fa l'analisi logica della frase: da una lista di parole costruisce l'albero «chi fa cosa a chi» |
| AST | l'albero della frase: `2+3*4` diventa un albero dove `*` sta sotto `+`, così la precedenza è nella FORMA, non più nel testo |
| type checker | il controllore che verifica che non stai sommando mele con banane, PRIMA di eseguire |
| diagnostica | il messaggio d'errore: qui è un prodotto di prima classe, scritto perché chi legge possa correggere senza aprire altri file |
| zucchero sintattico | scorciatoie di scrittura che una stazione riscrive nelle forme base: il resto della catena non le vede mai |
| IR / basic block | la ricetta riscritta in passi elementari («metti 2 in t0, somma…») raggruppati in blocchi senza bivi; i bivi sono salti fra blocchi |
| emissione C | l'ultima traduzione: dai passi elementari a codice C, che clang (un compilatore già esistente e fidato) trasforma in binario |
| runtime | la cassetta degli attrezzi in C che ogni programma compilato porta con sé (stampare, pannicare, contare i riferimenti) |
| refcount / COW | ogni valore ha un contatore di chi lo sta guardando; si copia davvero solo quando qualcuno modifica una cosa guardata anche da altri |
| golden test | una coppia «input + output atteso scritto su file»: se domani l'output cambia, il test urla |
| fixpoint | il compilatore scritto in Heroes ricompila se stesso e ottiene un risultato IDENTICO byte per byte: la prova che non dipende più da Rust |
| panel | il consiglio dei cinque giudici che esamina ogni modifica al linguaggio; adotta la via prudente e tu ratifichi con calma |

## Tone rules
- Never English jargon without immediate Italian gloss.
- Short sentences. No walls of text: the five sections, nothing more.
- Honest about what's broken or pending; never euphemistic, never alarmist.
- If the author seems lost across sessions, suggest re-reading ONE journal
  entry, not five documents.
