# The Italian vocabulary `/where` speaks from

**This is the one file in the skill that is Italian, and it is Italian data
rather than Italian prose.** `/where` explains the project to the author out
loud, in Italian, because the author reads Italian and CLAUDE.md §1.1 makes
their comprehension the objective. The wording below was tuned over many
sittings so that somebody who has never written a compiler understands a lexer,
an AST, a refcount and a fixpoint on first reading — so it is stored rather
than re-invented each session, which would drift and would cost the author the
one thing that accumulates: always the same picture, always the same words.

It is the third item under CLAUDE.md §11's **second declared exception** — a
translation that is itself a deliverable — beside the Italian edition of the
site and the two bilingual books. Everything else in this skill, and everything
else in the repository's working text, is English.

**How to use it.** The section headings and the pipeline drawing are emitted
verbatim. The analogies are the fixed vocabulary: the same term always gets the
same explanation, so the author's mental model builds instead of resetting. A
term with no row here gets a new plain-Italian gloss the first time it appears,
and the row is added below afterwards.

## Section headings

| # | heading |
|---|---|
| 1 | `«La mappa»` |
| 2 | `«Dove siamo»` |
| 3 | `«Cosa stiamo costruendo adesso, spiegato semplice»` |
| 4 | `«Tocca a te»` |
| 5 | `«La pillola di oggi»` |

## The pipeline drawing

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
  (l'ordine è quello scritto in docs/ROADMAP.md § The chain; il nome
   non promette una posizione)
```

## Two sentences that recur

- What a compiler is, when the report needs to start there: *un traduttore da
  un linguaggio che capisci tu a uno che capisce la macchina, fatto a catena di
  montaggio: ogni stazione fa una sola trasformazione*.
- Why a pending diagnosis is worth the author's time: *la diagnosi ha più
  valore se provi a indovinare la causa prima di leggere il fix*.

## Canonical analogies

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
