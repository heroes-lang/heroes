# 044: the census structs M-core-packages constructs are `sockaddr`, and `SHA256_CTX` by one route, in one binding or two

Measured 2026-10-14 by the M-buildable-structs lane, for issue 1337
(https://github.com/heroes-lang/heroes/issues/1337): *count which census
structs M-core-packages constructs, and in how many bindings, because panel
178's payment rests on it*. Panel 178's spec-warden made R1 conditional
(`docs/panel/178-reports/spec-warden.md`, § `condition`): it would change to
object if *M-core-packages' plan puts every long-array construction inside 2
or fewer bindings, so the rent is never repaid*. The sentence landed on
2026-10-06 (`54c5f618`), paid instead by panel 194's registered predictions,
so this answers a question the payment still rests on, not one that gates the
landing.

## 1. What was asked, and of what

**The census criterion is panel 178's**: a record with an array longer than
8 (`docs/panel/178-briefs/census.sh`). **A binding constructs such a record**
when it hands C a record whose storage the program owns, by pointer or by
value, so the program has to build a value of it first; a record C hands back
behind a pointer it allocated, `FILE`, `sqlite3`, `CURL`, is a handle and is
never built.

**The functions are the ones `docs/roadmap/milestones/M-core-packages.md`
names**, read on 2026-10-14: `getenv` (`os`); `fdopen`, `fgets`, `fgetc`,
`fmemopen`, `write` (`io`, `bufio`, `bytes`); `time`, `timespec_get`,
`strftime`, `gmtime_r`, `nanosleep`, `clock_gettime` (`time`); prepare and
step, asked as `sqlite3_open`, `sqlite3_prepare_v2`, `sqlite3_step`,
`sqlite3_finalize`, `sqlite3_close` (`database/sql`); `curl_easy_setopt` and
its three neighbours (`net/http` client); `socket`, `bind`, `listen`, `accept`
(`net`, the server of 2026-09-03). **And four the page does not name, asked so
that the routes it leaves open are counted too**: `connect`, a client's;
`sqrt` and `pow` for `math`; `crypto/sha256` and `crypto/hmac` say only
`package "libcrypto"`, so both of OpenSSL's routes are asked, the low-level
`SHA256_Init`, `SHA256_Update`, `SHA256_Final`, and the one-shot `SHA256`,
`EVP_Digest`, `HMAC`, `HMAC_Init_ex`; and `poll` and `select`, the two ways a
`net` with more than one connection would wait. The page's `net/http` server
is Heroes alone over `net`, so it binds nothing of its own.

**The instrument reads clang's own AST**, so a typedef, an anonymous record
and a record nested by value are clang's and not a reading of header text:
`clang -x c -fsyntax-only -Xclang -ast-dump=json tu.c > ast.json`, then
`python3 -I constructs.py ast.json <the functions>`. Both files are below,
whole. On this Mac (Apple clang 21.0.0, the SDK's headers, OpenSSL from
`/opt/homebrew/opt/openssl@3/include`) and in `heroes-linux-arm64:latest`
(Debian clang 22.1.8, glibc), the same 37 functions asked on each.

## 2. What each answered

| function | Darwin arm64 | Linux arm64 |
|---|---|---|
| `bind`, `accept`, `connect` | `struct sockaddr`, longest array 14, **census** | the same |
| `SHA256_Init`, `SHA256_Update`, `SHA256_Final` | `SHA256_CTX`, longest array 16, **census** | the same |
| `select` | `fd_set`, longest array 32, **census** | `fd_set`, 16, **census** |
| `fgets`, `fgetc` | `FILE`, longest array 3 | `FILE`, 20, census, **a handle** C allocates (`fdopen`, `fmemopen`) |
| `timespec_get`, `nanosleep`, `clock_gettime`, `strftime`, `gmtime_r`, `poll` | `timespec`, `tm`, `pollfd`, `timeval`: no array | the same |
| the five `sqlite3` functions, `EVP_Digest`, `HMAC`, `HMAC_Init_ex` | opaque records only, handles | the same |
| `getenv`, `fdopen`, `fmemopen`, `write`, `time`, `socket`, `listen`, `sqrt`, `pow`, `SHA256`, the four `curl_easy_` | no record parameter | the same |

## 3. What it counts

**On the page's plan, M-core-packages constructs one census struct for
certain, `sockaddr`, in one binding, `net`**: `bind`, `connect` and `accept`
each take the program's own address, and the server of 2026-09-03 built it as
`record SockAddr tag sockaddr` *with the port's two bytes computed in Heroes*
(the page's words), which are two of `sa_data`'s 14.
**`SHA256_CTX` is constructed only if `crypto/sha256` binds OpenSSL's
low-level route**; its one-shot and its EVP routes hand C no record of the
program's. **`fd_set` only if `net` waits on `select`**, which the page does
not name and its one-threaded server does not need; `poll`'s `pollfd` holds no
array. `FILE` passes the criterion on glibc and is never built. So the
constructions live in **one binding, or two** (`net` and `crypto/sha256`),
never three on this plan, on both legs. The spec-warden's condition, *2 or
fewer bindings*, **holds**: what follows from it is a ruling no rule reaches,
the author's or a sitting's, and nothing here decides it.

**What it says of panel 178's registered payment**, *`rest: zero` in 3 or
more `.hero` files outside `tests/` and `docs/` at M-core-packages' close*:
a count of bindings is not a count of files, and whether the third file comes
is a premise this measurement cannot settle. A package over C is two modules,
and `rest: zero` stands wherever a record is built: inside `net`'s wrapper
alone if the package hides `sockaddr`, or in every program that builds an
address if it does not. With the constructions confined to the packages, the
count reads two at most; the spec-warden named this direction, *§1.5
(verbose where you declare) working against the rent*.

**What it changes for issue 1333** (route T, a string into a fixed field):
T returns if *M-core-packages binds 3 or more string fields of the third
kind*, bytes the program writes. Of the three census structs above, `sa_data`
holds a port and an address written as numbers, `SHA256_CTX` is filled by C,
and `fd_set` is written bit by bit through C's macros: **none is a string
field the program writes**, so the count is 0 and T keeps waiting.
`sockaddr_un`'s `sun_path`, the field panel 178 measured T on, would be one,
and the page names no Unix socket.

## 4. Its limits

The functions are the page's, and the page is a plan: a package that grows a
binding (`uname`, `statfs`, a Unix socket) is outside this count, and
re-running the instrument over the binding's functions is the whole of
re-measuring it. Windows was not run. The criterion counts the longest
array at any depth by value, which is panel 178's criterion and the critic's
typedef-aware recount of it (panel 194); a record whose arrays are all 8 or shorter
(`sockaddr_in`, 8) is not counted, though building it lists eight zeros.

## 5. The instrument

`tu.c`, the headers the page names:

    /* The headers M-core-packages' page names (docs/roadmap/milestones/M-core-packages.md). */
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <time.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <sys/un.h>
    #include <poll.h>
    #include <sys/select.h>
    #include <math.h>
    #include <sqlite3.h>
    #include <openssl/sha.h>
    #include <openssl/hmac.h>
    #include <openssl/evp.h>
    #if __has_include(<curl/curl.h>)
    #include <curl/curl.h>
    #endif

The functions, as asked:

    getenv
    fdopen fgets fgetc fmemopen write
    time timespec_get strftime gmtime_r nanosleep clock_gettime
    sqrt pow
    sqlite3_open sqlite3_prepare_v2 sqlite3_step sqlite3_finalize sqlite3_close
    curl_easy_init curl_easy_setopt curl_easy_perform curl_easy_cleanup
    socket bind listen accept connect
    SHA256_Init SHA256_Update SHA256_Final SHA256 EVP_Digest HMAC HMAC_Init_ex
    poll select

`constructs.py`:

    """Which records holding an array longer than 8 the C functions M-core-packages'
    page names take, by pointer or by value: the structs a binding of those
    functions constructs before it lends one (issue 1337, 2026-10-14).

    Usage: python3 -I constructs.py <ast.json> <functions...>
    where <ast.json> is `clang -x c -fsyntax-only -Xclang -ast-dump=json tu.c`.
    Reads clang's own AST, so typedefs, anonymous records and nested records are
    clang's and not a reading of header text. One line per function: each record a
    parameter names, the longest array it holds at any depth by value, `CENSUS`
    where that is longer than 8 (panel 178's census criterion).
    """
    import json, sys

    ast = json.load(open(sys.argv[1]))
    names = sys.argv[2:]
    by_id, typedefs, records, functions = {}, {}, {}, {}

    def index(d):
        by_id[d.get('id')] = d
        k = d.get('kind')
        if k == 'TypedefDecl':
            typedefs.setdefault(d.get('name'), d)
        elif k == 'RecordDecl' and d.get('completeDefinition') and d.get('name'):
            records[(d.get('tagUsed'), d['name'])] = d
        elif k == 'FunctionDecl':
            functions[d.get('name')] = d
        for c in d.get('inner', []):
            index(c)

    index(ast)

    def record_of_typedef(name, depth=0):
        """The record a typedef name stands for, through other typedefs, or None."""
        t = typedefs.get(name)
        if t is None or depth > 16:
            return None
        stack = list(t.get('inner', []))
        while stack:
            n = stack.pop(0)
            if n.get('kind') == 'RecordType':
                d = by_id.get(n['decl']['id'])
                if d is not None and not d.get('completeDefinition'):
                    d = records.get((d.get('tagUsed'), d.get('name')), d)
                return d
            if n.get('kind') == 'TypedefType':
                return record_of_typedef(n['decl']['name'], depth + 1)
            stack.extend(n.get('inner', []))
        return None

    def record_named(spelled):
        """`struct X`, `union X` or a typedef name, qualifiers and pointers off."""
        base = spelled.split('[')[0]
        for word in ('const', 'volatile', 'restrict', '*'):
            base = base.replace(word, ' ')
        base = ' '.join(base.split())
        for kind in ('struct', 'union'):
            if base.startswith(kind + ' '):
                tag = base[len(kind) + 1:]
                return records.get((kind, tag)), kind + ' ' + tag
        if base.replace('_', 'a').isalnum():
            return record_of_typedef(base), base
        return None, base

    def array_len(spelled):
        if '[' not in spelled:
            return 0
        n = 1
        for part in spelled[spelled.index('['):].replace(']', '').split('[')[1:]:
            n *= int(part) if part.strip().isdigit() else 0
        return n

    def longest(rec, seen):
        best = 0
        for f in rec.get('inner', []):
            k = f.get('kind')
            if k == 'RecordDecl' and f.get('completeDefinition') and f.get('id') not in seen:
                best = max(best, longest(f, seen | {f.get('id')}))
            if k != 'FieldDecl':
                continue
            spelled = f['type'].get('desugaredQualType', f['type']['qualType'])
            best = max(best, array_len(spelled))
            inner, _ = record_named(spelled)
            if inner is not None and inner.get('id') not in seen and inner.get('completeDefinition'):
                best = max(best, longest(inner, seen | {inner.get('id')}))
        return best

    for name in names:
        fn = functions.get(name)
        if fn is None:
            print(f'{name}\tNOT DECLARED')
            continue
        said = []
        for p in fn.get('inner', []):
            if p.get('kind') != 'ParmVarDecl':
                continue
            spelled = p['type'].get('desugaredQualType', p['type']['qualType'])
            rec, label = record_named(spelled)
            if rec is None:
                continue
            if not rec.get('completeDefinition'):
                said.append(f'{p["type"]["qualType"]} ({label}): opaque')
                continue
            n = longest(rec, {rec.get('id')})
            said.append(f'{p["type"]["qualType"]} ({label}): longest array {n}' + (' CENSUS' if n > 8 else ''))
        print(f'{name}\t' + ('; '.join(said) if said else 'no record parameter'))
