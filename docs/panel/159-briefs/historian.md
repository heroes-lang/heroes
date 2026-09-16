# Panel 159 — historian brief

Read `docs/panel/159-briefs/00-shared.md` first. **Verify every claim by web
search, with a source URL and the date checked.** Unsourced precedent is
inadmissible — write it as unsourced rather than dropping it. No veto.

**At most six searches, one at a time, and report early.** Your seat delivered
well at 156 and 158 under that rule and was lost to the watchdog at 155 without
it.

## The question

A small language's specification describes `sort` as *walking the elements in
order* and never says **ascending**. It gives an assignment form for maps,
`m[k] @ v`, and none for arrays. And it says a program's entry point is
`function main()` without saying whether that function may return a failure.

## What to find, in priority order

1. **Does any language specification state its sort's DIRECTION, and do any
   fail to?** The interesting evidence is a language where the direction was
   left to the implementation and later pinned, or a bug report from somebody
   who assumed the wrong one. C's `qsort` takes a comparator so the question
   does not arise; Python's `list.sort()` documents ascending-by-default
   explicitly; Go's `sort.Ints`. **The sharper question is STABILITY**, which is
   the same class of silence and has a rich history — Java's
   `Arrays.sort` splits stable and unstable by overload and documents which,
   and the JDK changed its implementation and had to keep the promise. Find what
   a specification that stayed silent on either paid.
2. **An entry point that may fail.** Rust's `fn main() -> Result<(), E>` was
   added by RFC 1937 (`?` in `main`) — find that RFC and quote what it says the
   problem was. Go's `main` returns nothing and `os.Exit` is the route; C's
   `main` returns `int`. Haskell's `main :: IO ()`. **The valuable finding is a
   language that REFUSED a fallible entry point and said why**, which is the
   position this language currently holds without writing it down.
3. Only if you have room: an indexed assignment form that a specification
   omitted while the implementation accepted it — the general shape of "the
   grammar permits what the prose never teaches".

Do not recommend a design. If precedent is thin, say so.

**Return your report as your FINAL MESSAGE, in full** — you have no write tool
and the coordinator writes it to disk verbatim.
