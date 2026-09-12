# M-web-framework — the framework that composes the packages


**Scheduled, no warrant** (author instruction 2026-09-03, the same session). It
composes M-core-packages' packages in Go's and Echo's shape, everything explicit:
routes as a table of function values, `(function(Request) -> Response)` keyed by
method and path — top-level functions are values (`spec:112-117`); records for
request and response; middleware as a chain of functions, because v1 has no
closures (`selfhost/emit/ctype.hero:375-380`) — and **that is now the ruling and
not a placeholder**: M-closures-verdict refused them on 2026-09-08, so the chain
of named top-level functions is the shape, and the ffi seat's prediction is scored
here (a middleware chain over C sockets needs zero closures at the boundary,
because every callback it binds carries a `void *` context); templates from `html/template`,
bodies from `encoding/json`, rows from `database/sql`. No metaprogramming and no
dynamic dispatch: the Rails and Django shape rests on Part 6's own rows
(`design.md:2382-2404`), and `design.md:2395` says why — *"this is why LLMs err
more on Rails/RSpec than on plain Ruby"*. What it inherits from
M-isolated-threads is concurrency alone: `serve` takes one connection at a time
until a thread can take an accepted descriptor, and a `Request` record is exactly
the message the separate-heap model wants, small and copied once
(`design.md:2512-2524`). **Acceptance**: a small application — the corpus's `todo`
over a database — served over HTTP, on the three platforms. Its full brief is
written at M-core-packages' close, when the packages exist and what a framework
must add is measured rather than guessed.

**The question the brief has to answer, in the falsifiable form the packages
session left it in** (2026-09-03, recorded here on 2026-09-04 because the note
that held it was retired and this is the only sentence of it that was not
already somewhere): **whether a framework with no closures reaches Echo's level
or collapses into `net/http` itself.** Middleware is planned *"as a chain of
functions, because v1 has no closures"*, and a chain of named top-level
functions that cannot capture may leave the framework with nothing to add over
the package it wraps — in which case the honest outcome is one package and no
framework. It is scored against the running application rather than argued, and
M-closures-verdict sits eight rows earlier precisely so its answer is known
first.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
