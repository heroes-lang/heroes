# Task: a router that keeps state

Write one program, `main.hero`, in the language `spec.md` describes, from that document alone. It answers requests
from a fixed list, with no network.

A request is a record holding a method (`str`), a path (`str`) and a body (`str`). The program keeps one state
across the requests: the notes it holds, each a text under a name, and how many requests it has answered so far.

The routes:

- `GET /notes/<name>`: status 200 and the note's text; status 404 and `no note <name>` where there is none.
- `PUT /notes/<name>`: keeps the request's body as the note `<name>`, replacing one already there; status 201 and
  `stored <name>`.
- `DELETE /notes/<name>`: removes the note; status 204 and `deleted <name>`; status 404 and `no note <name>` where
  there is none.
- `GET /stats`: status 200 and `notes <n> requests <m>`, `<n>` the notes held and `<m>` the requests answered before
  this one.
- Any other request: status 404 and `no route`.

**How it must be built.** The routes are listed in one place, and **each route is bound there to the function that
answers it**, so that adding a route is one entry in that list and one function. The program must not choose the
answering function by comparing a route's name or a request's path in a chain of `if` or in a `match`; finding which
route's pattern a path fits (segment by segment, `<name>` taking any one segment) is the router's own work and may
compare segments. Each answering function receives what it needs from the state and from the request, and changes the
state only where its route does.

The requests, in this order, each answered by one line, `<status> <text>`:

1. `PUT /notes/a`, body `alpha`
2. `PUT /notes/b`, body `beta`
3. `GET /notes/a`
4. `DELETE /notes/b`
5. `GET /notes/b`
6. `GET /stats`
7. `POST /x`

Write the program whole, in the structure `brief.md` gives. Then say what it prints, line by line, and whether you
expect it to compile as written; and for each function you bind to a route, why each of its parameters is written
the way it is.
