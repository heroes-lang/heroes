```
    function sqlite3_open(path: cstr lent, @out: Db acquires sqlite3_close) -> i64
    function sqlite3_close(db: Db consumes) -> i64
```
