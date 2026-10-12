# 042: a stopped frame shows the C names, and of its locals the author wrote fewer than one in ten

Measured 2026-10-12 on this Mac by lane typed-inspection, as M-typed-inspection
step 3 (issue 942, https://github.com/heroes-lang/heroes/issues/942). Two
questions, each answered by running lldb over programs built at `-O0`: what a
stopped program shows today for each shape of value, re-running the four
failures the issue recorded on 2026-09-06; and the census the milestone page
says decides its last step, how many of a frame's locals are temporaries, how
many are slots the lowering invented, and how many are bindings the author
wrote.

## 1. Provenance

| what | value |
|---|---|
| the machine | macOS 27.0.1 (26A434), arm64 |
| the debugger | `lldb-2103.0.34.103`, run as `lldb --no-lldbinit --no-use-colors --batch`, so no `.lldbinit` moves what it prints |
| the C compiler | Apple clang 21.0.0 (clang-2100.3.34.2) |
| the Heroes compiler | `./heroes`, `heroes 0.3.0`, built from the seed at `a5ca08f33` (the lane's step 2), SHA-256 `a6797fe27d812d6e5ec0c636a2dff460e6387ee0fccd428c25fae4eb2d847914` |
| the builds | `heroes build <file> -O0 -o <binary>`: since panel 197 the `-O0` build carries `-g`, the variables' debug info as well as the lines (`selfhost/cli/flags.hero`, `debug_words`) |
| scratch | the programs, binaries and transcripts under `build/census/`, which git ignores, written 2026-10-12 |

## 2. The program: one binding of each shape

`shapes.hero`, written for this measurement and kept here whole, since it lives
in no tracked file:

    record Point
        x: i64
        y: i64

    record Room
        name: str
        corner: Point

    variant Token
        num
            v: i64
        plus

    function half(n: i64) -> i64?
        if n % 2 != 0
            return fail(code: "odd", msg: "not even")
        return ok(n / 2)

    function main()
        count = 7
        name = "ada"
        p = Point(x: 3, y: 4)
        t: Token = .num(v: 42)
        names = ["ada", "grace"]
        points = [Point(x: 1, y: 2), Point(x: 5, y: 6)]
        ages = {"ada": 36, "grace": 85}
        good = half(8)
        bad = half(7)
        room = Room(name: "lab", corner: Point(x: 9, y: 10))
        print(count)
        print(name)
        print(p.x + names.len() + points.len() + ages.len())
        print(good.default(0) + bad.default(0))
        print(room.corner.y)

        match t
            .num n => print(n.v)
            .plus  => print(0)

It prints `7`, `ada`, `9`, `4`, `10`, `42`. Line 30 is `print(count)`, where every
binding of the table below has been made. The session, from the repository's
root (`p.txt` holds twenty `p` commands, each binding by the author's name and
then by its C name; `command source -e false` goes on past a command that fails,
where `--batch` alone stops at the first):

    heroes build build/census/shapes.hero -O0 -o build/census/shapes      # scratch, 2026-10-12
    lldb --no-lldbinit --no-use-colors --batch \
        -o "breakpoint set --file shapes.hero --line 30" -o run -o "frame variable" \
        -o "command source -e false build/census/p.txt" build/census/shapes   # scratch, 2026-10-12

The breakpoint resolved to one location, `shapes.hero:30:11`, and the process
stopped there, `stop reason = breakpoint 1.1`.

## 3. The before-table

What `frame variable` printed for the ten bindings, word for word (an address
is this run's):

    (int64_t) h0_count = 7
    (HeroStr) h1_name = (ptr = "ada", len = 3)
    (h_shapes_Point) h2_p = (f_x = 3, f_y = 4)
    (h_shapes_Token) h3_t = {
      tag = h_shapes_Token_tag_num
      as = {
        c_num = (f_v = 42)
      }
    }
    (HeroArrayHeader *) h4_names = 0x000000010064d890
    (HeroArrayHeader *) h5_points = 0x000000010064d8d0
    (HeroMapHeader *) h6_ages = 0x000000010064d910
    (h_0opt_e201354) h7_good = {
      tag = 0
      as = {
        ok = 4
        err = {
          code = (ptr = "", len = 0)
          msg = (ptr = 0x0000000000000000, len = 0)
        }
      }
    }
    (h_0opt_e201354) h8_bad = {
      tag = 1
      as = {
        ok = 4295075736
        err = {
          code = (ptr = "odd", len = 3)
          msg = (ptr = "not even", len = 8)
        }
      }
    }
    (h_shapes_Room) h9_room = {
      f_name = (ptr = "lab", len = 3)
      f_corner = (f_x = 9, f_y = 10)
    }

| shape | binding | what the frame shows of the value | `p <the author's name>` | `p <the C name>` |
|---|---|---|---|---|
| `i64` | `count` | `7`, as `h0_count` | `error: use of undeclared identifier 'count'` | `(int64_t) 7` |
| `str` | `name` | the text, `"ada"`, beside its length | `error: 'name' has unknown type; cast it to its declared type to use it` | `(HeroStr)  (ptr = "ada", len = 3)` |
| record | `p` | the fields, as `f_x` and `f_y` | `error: use of undeclared identifier 'p'` | `(h_shapes_Point)  (f_x = 3, f_y = 4)` |
| variant with payload | `t` | the case as an enumerator, `h_shapes_Token_tag_num`, and its payload under `as.c_num` | undeclared, as above | the same nine lines |
| `[str]` | `names` | an address, `HeroArrayHeader *`, nothing of `"ada"` or `"grace"` | undeclared | `(HeroArrayHeader *) 0x000000010064d890` |
| `[Point]` | `points` | an address, `HeroArrayHeader *` | undeclared | the address |
| `{str: i64}` | `ages` | an address, `HeroMapHeader *`, nothing of its entries | undeclared | `(HeroMapHeader *) 0x000000010064d910` |
| `i64?`, ok | `good` | both arms, `ok = 4` and an `err` half reading empty, under `h_0opt_e201354` | undeclared | the same eleven lines |
| `i64?`, err | `bad` | both arms, the error and an `ok` half reading `4295075736`, under `h_0opt_e201354` | undeclared | the same eleven lines |
| nested record | `room` | the fields, the inner record's in parentheses | undeclared | the same four lines |

Each refusal of an author's name is printed under a caret, with the note
`Expression evaluation in pure C not supported. Ran expression as 'ISO C++'.`

## 4. The four failures of 2026-09-06, re-run

1. **`p p` is `error: use of undeclared identifier 'p'`: holds, word for word.**
   Nine of the ten author's names answer the same sentence; `name` answers
   another, `'name' has unknown type; cast it to its declared type to use it`.
   `image lookup -s name` in the same stopped process found no symbol of that
   name, so what lldb took `name` to be is a question this run did not settle.
2. **A `[T]` and a `{K: V}` are an opaque `HeroArrayHeader *`: holds for `[T]`,
   and the map's type has another name.** Both arrays, of `str` and of a record,
   are `HeroArrayHeader *` and an address. The map is `HeroMapHeader *` today,
   an address too, so the opacity holds and the record's type name for a map
   does not.
3. **A `T?` prints both arms, including a garbage `err` half, under a hashed
   type name, `h_0opt_e201354`: holds, under the same name.** Both arms print
   for both values. Here the garbage sat in the `ok` half of the error value,
   `ok = 4295075736`; the `err` half of the ok value read empty, a zero length
   and a null `msg`.
4. **The frame is flooded: holds.** This `main` of 20 lines shows **111**
   locals, and `frame variable` printed **339** lines. The census below splits
   them, and two larger frames beside them.

## 5. The census

### The method, to re-run

A local's C name says which of two kinds it is: `mangle.value` names a
temporary `t<N>`, and `mangle.slot` names a slot `h<k>_<name>`, a mutable
parameter arriving as `ph<k>_<name>` (`mangle.out_param`), all three in
`selfhost/emit/mangle.hero`. Whether a slot was written by the author or
invented by the lowering, the C name does not say: `mangle.slot` drops the `$`
that marks an invented one (its own comment: *`$i` in slot 5 becomes `h5_i`*).
The IR still knows: `heroes build --dump-ir` prints a function's `slots` line,
each slot `name: Type` and an invented one's name opening with `$`
(`selfhost/ir/containers.hero`, `SlotKind`). The parameters stand before the
slots line's first slot, so slot `k` of the C is the line's entry `k` minus the
count of parameters.

So: stop in the function, keep the names `frame variable` prints at its top
level, count the `t<N>`, and join the rest to the IR's slots line, index for
index. In shell, from the repository's root, the scratch directory of
2026-10-12:

    D=build/census                                             # scratch, 2026-10-12
    L="lldb --no-lldbinit --no-use-colors --batch"
    names() { sed -n '/^(lldb) frame variable/,/^(lldb) [^f]/p' "$1" | grep -E '^\([^)]*\) [A-Za-z_][A-Za-z0-9_]* = ' | sed -E 's/^\([^)]*\) ([A-Za-z_][A-Za-z0-9_]*) = .*/\1/'; }
    slots() { grep -A1 "^function $2" "$1" | sed -n 2p | sed 's/^    slots  //; s/ · /\n/g' | sed -E 's/^([^:]+):.*/\1/'; }

    # (a) the case behind tests/emission/run-adversarial-aggregate-overwrite.c
    heroes build tests/golden/run/adversarial-aggregate-overwrite.hero -O0 -o $D/agg
    $L -o "breakpoint set --file adversarial-aggregate-overwrite.hero --line 71" -o run -o "frame variable" $D/agg > $D/agg.txt
    heroes build tests/golden/run/adversarial-aggregate-overwrite.hero --dump-ir > $D/agg.ir

    # (b) examples/interpreter/syn/expr.hero's compared
    heroes build examples/interpreter/main.hero -O0 -o $D/interp
    $L -o "breakpoint set --file syn/expr.hero --line 66" -o run -o "frame variable" $D/interp -- examples/interpreter/scripts/tour.script > $D/compared.txt
    # --dump-ir prints the root module's functions alone (selfhost/ir/print.hero,
    # `source.is_root`), so compared's module is made a root in a copy:
    mkdir -p $D/interp-copy && cp -R examples/interpreter/lex examples/interpreter/syn examples/interpreter/util examples/interpreter/run $D/interp-copy/
    { cat examples/interpreter/syn/expr.hero; printf '\nfunction main()\n    print(0)\n'; } > $D/interp-copy/exprroot.hero
    heroes build $D/interp-copy/exprroot.hero --dump-ir > $D/compared.ir

    names $D/agg.txt | grep -cE '^t[0-9]+$'                 # the temporaries
    slots $D/agg.ir 'main()' | grep -c '^\$'                 # the invented slots
    slots $D/agg.ir 'main()' | grep -vc '^\$'                # the author's slots

`syn/expr.hero` alone names a breakpoint in `compared`: `--file expr.hero` took
five locations, `examples/interpreter/run/expr.hero` sharing the base name. The
join was asked of every frame below and held, every slot lldb named being the
IR's slot with the `$` dropped, in order, from index 0.

### The numbers

| the frame | locals | `t<N>` | `$` slots | the author's slots | parameters | the author's, in all |
|---|---|---|---|---|---|---|
| (a) `adversarial-aggregate-overwrite.hero`'s `main`, 43 lines, stopped at :71 | **138** | 108 | 20 | 10 | 0 | **10** (7.2%) |
| (b) `syn/expr.hero`'s `compared`, 18 lines, stopped at :66 | **121** | 90 | 24 | 5 | 2 | **7** (5.8%) |
| (c) `shapes.hero`'s `main`, 20 lines, stopped at :30 | **111** | 89 | 11 | 11 | 0 | **11** (9.9%) |

`frame variable` printed 595 lines for (a), 853 for (b) and 339 for (c).

The `$` slots, by the IR's names: in (a) `$s0`, `$r0`, `$s1`, `$r1`, `$s2`,
`$r2` and `$own16` to `$own29`; in (b) `$f0` to `$f4`, `$ret0` and `$own13` to
`$own30`; in (c) `$f0`, `$r0`, `$f1`, `$r1`, `$s0` and `$own16` to `$own21`.
`$ret0` is a `return_slot`, the fourth `SlotKind` beside the three the milestone
page names (panel 190). The author's: in (a) `c`, `i`, `s`, `j`, `tag1`, `tag2`,
`n` and three match bindings all spelled `f`; in (b) the parameters `c` and `t`
and `left`, `op`, `right`, `made`, `second`; in (c) the ten of the table and the
match binding `n`.

### Against the numbers on the record

- **(a)**: the issue's 139, *111 named and 28 temporaries*, is reproduced by
  neither half. `main`'s prologue in the blessed emission declares 138 today, 30
  slots and 108 temporaries, and declared the same 138, 30 and 108 at
  `a69a373b6` (2026-09-05), the last change to that file before 2026-09-06; what
  was counted that day is a question rather than a premise.
- **(b)**: the 247 that issue 813 read off `--emit-c` is 119 in the prologue
  today, 29 slots and 90 temporaries, which is the count that issue's
  re-verification read on 2026-09-10; lldb lists the 2 parameters beside them,
  121.

## 6. What the numbers say, for the sitting on the mechanism

Facts only; the design is the sitting's (issue 939).

- **In each of the three frames the author wrote fewer than one local in ten**:
  7 to 11 of 111 to 138. The temporaries are 74 to 80% of a frame (89 of 111,
  108 of 138, 90 of 121), the invented slots 10 to 20% (11, 20 and 24).
- **A temporary is told apart by its C name alone**: `t<N>` is what
  `mangle.value` writes and no slot is spelled so, every slot opening `h<k>_` or
  `ph<k>_`. Leaving the temporaries out reads a fact about the mangler, never a
  premise about what an author writes.
- **An invented slot is not**: with the `$` dropped, `h16_own16` has the shape of
  `h0_count`, and `own16` is a name an author may write. In the emitted C and
  in its DWARF the distinction is gone; the IR alone holds it, and `--dump-ir`
  prints it for the root module alone.
- **Every local is in the frame at every line**, the prologue declaring them all
  (CLAUDE.md §7). At (b)'s :66, `made`, bound at :72, read `6171889304`, and
  `second`, bound at :73, read as an ok `add` (`tag = 0`): a binding the program
  has not reached shows as if it held a value. Which of a function's bindings
  exist at a line is a fact about lines, which a name carries in no form.
- **The flood is the lowering's and not the function's length**: (b) is 18 lines
  and shows 121 locals.

## 7. Unrun, and named

- **Linux.** The suite of step 2 runs there after the push; whether its lldb
  prints these shapes in the same words, the CI's leg is the instrument.
- **Another lldb.** One version here; the error text under a caret and its note
  are this version's rendering.
- **What `name` resolved to**, item 1 above.
- **The count of 2026-09-06**, section 5.

## 8. The scratch, kept beside this file

Added 2026-10-12, the same night, by the author's instruction given for the
panels' working folders, meant as: *put the evidence in the result, then throw
the folders away*. The 30 text files of `build/census/` that § 2 to § 5 name,
the programs, the IR dumps, the emitted C and the lldb transcripts, stand in
`042-a-stopped-frame-shows-the-c-names-and-of-its-locals-the-author-wrote-fewer-than-one-in-ten.tar.xz`, beside this file:
`tar -xJf docs/measurements/042-a-stopped-frame-shows-the-c-names-and-of-its-locals-the-author-wrote-fewer-than-one-in-ten.tar.xz -C build` puts them back at `build/census/`.
The three binaries are not kept; § 2's commands rebuild them.
