# Task

Below are five programs, each the whole of a file `main.hero` in a folder of its own. For each, its
author runs `heroes build main.hero` in that folder, then runs the program the build made, once, with
no arguments.

For each program, answer in order:

1. Does `heroes build main.hero` accept it? If it does not, say what it refuses and why, citing the
   specification, and go to the next program.
2. If it does, give, in order, every line the program writes when it runs, and say how it ends:
   normally, by an abort and what the abort says, or not at all. Cite the specification for each.

## Program 1

    function main()
        m: (mailbox i64) = mailbox()
        match m.receive(within_ms: 0)
            .ok v => print("got ", v)
            .err e => print("first: ", e.code)
        m.send(1).must()
        m.send(2).must()
        m.close()
        match m.send(3)
            .ok _ => print("sent 3")
            .err e => print("send: ", e.code)
        print("got ", m.receive(within_ms: 0).must())
        print("got ", m.receive(within_ms: -1).must())
        match m.receive(within_ms: -1)
            .ok v => print("got ", v)
            .err e => print("last: ", e.code)
        m.close()
        print("done")

## Program 2

    function total(m: (mailbox i64)) -> i64
        sum @= 0
        while true
            match m.receive(within_ms: -1)
                .ok v => sum @ sum + v
                .err _ => return sum

    function main()
        m: (mailbox i64) = mailbox()
        t = spawn(total, m).must()
        m.send(5).must()
        m.send(7).must()
        m.close()
        print("total ", t.wait())

## Program 3

    extern "hero_net.h"
        function hero_net_catch_stop() -> i64
        function hero_net_ask_stop()

    function main()
        _ = hero_net_catch_stop()
        m: (mailbox str) = mailbox()
        m.send("queued").must()
        hero_net_ask_stop()
        match m.receive(within_ms: -1)
            .ok s => print("got ", s)
            .err e => print("receive: ", e.code)
        m.close()

## Program 4

    function forever(m: (mailbox i64)) -> i64
        return m.receive(within_ms: -1).default(-1)

    function main()
        m: (mailbox i64) = mailbox()
        _ = spawn(forever, m).must()
        print("main ends")

## Program 5

    extern "stdlib.h"
        record Block tag void
        function malloc(size: u64) -> Block acquires free
        function free(p: Block consumes)

    function keep(m: (mailbox Block)) -> i64
        b = m.receive(within_ms: -1).must()
        free(b)
        return 1

    function main()
        m: (mailbox Block) = mailbox()
        t = spawn(keep, m).must()
        b @= malloc(16)
        if args().len() < 100
            m.send(@b).must()
        print("sent")
        print("freed ", t.wait())
        free(b)
        m.close()
