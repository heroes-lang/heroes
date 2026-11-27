# Quotes of reader B: every O and every S

No unit was labelled S: no unit's own answer block offers § 9's "running out of stack aborts" as a reason for that
program's verdict. Where § 9 comes up, the reader sets it aside ("does not apply", "not reached") or raises it once for
several programs in a choice point.

## u847741 (h4), O

The answer block under `h4`:

> **Why:** §13 says a callback is "never … a signal handler, nor called by one", and it supplies the abort wording "that
> C called it from inside one". But the text never says this case must abort. It only says "may abort", and only about
> the cases it lists.

Choice point 2, which names `h4` and decides its verdict:

> **h4: a callback called from inside a signal handler.**
> - My choice: "never … nor called by one" states a rule but gives no run-time result. The only abort the text names is
>   a "may abort", so h4 is `NOT PROMISED`.

Why O: the reader keeps "handler" as a signal handler (so not H) and says the "may" covers only the cases it lists, which
leaves out h4's direct call. Nothing here is switched, so W ("covers this switch") does not fit either. What decided the
verdict was reading a prohibition with no stated run-time result as `NOT PROMISED`. None of W N L H T I P R S names that.

## sha256

`shasum -a 256 labels-B.tsv`:

4711967027340cd345d73ba557ba6fc82d7f6c363dfe44abf338f6d6fa9ef5b7
