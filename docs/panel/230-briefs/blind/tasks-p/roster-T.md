# A program that stopped

The program below was built and run as it stands. You are shown what it should print, what it printed on standard output, what it wrote on standard error, and the status it exited with.

## `roster.hero`

It should print:

```
Di 22
Bo 25
Ada 30
Cy 30
```

It printed nothing on standard output.

It wrote on standard error:

```
assert failed: sorted == want
```

It exited with status 134.

The program:

```hero
# Sorts the team by age, youngest first, keeping the input's order among people
# of the same age, and checks the result before printing it.

record Person
    name: str
    age: i64

function by_age(people: [Person]) -> [Person]
    out @= people
    i @= 1

    while i < out.len()
        j @= i

        while j > 0 && out[j - 1].age >= out[j].age
            held = out[j]
            out[j] @ out[j - 1]
            out[j - 1] @ held
            j @ j - 1

        i @ i + 1

    return out

function main()
    team = [
        Person(name: "Ada", age: 30)
        Person(name: "Bo", age: 25)
        Person(name: "Cy", age: 30)
        Person(name: "Di", age: 22)
    ]
    want = [
        Person(name: "Di", age: 22)
        Person(name: "Bo", age: 25)
        Person(name: "Ada", age: 30)
        Person(name: "Cy", age: 30)
    ]
    sorted = by_age(team)
    assert sorted == want

    for p in sorted
        print(p.name, " ", p.age)
```
