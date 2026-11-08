```
    function sqlite3_open(path: cstr lent, @out: Db acquires sqlite3_close | sqlite3_close_v2) -> i64
    function sqlite3_close(db: Db consumes) -> i64 when 0
    function sqlite3_close_v2(db: Db consumes) -> i64
```
