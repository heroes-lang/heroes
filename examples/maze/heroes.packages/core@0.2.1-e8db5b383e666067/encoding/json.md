# json: `use <core>/encoding/json`

JSON text read into a `Value` and a `Value` written as JSON text (RFC 8259), where a text that is not JSON is a failure the caller answers and never an abort.

## Types

`variant Value`, one of `.nothing`, `.boolean(value: bool)`, `.number(value: f64)`, `.string(value: str)`, `.array(items: [Value])` or `.object(fields: {str: Value})`: A JSON value, one of JSON's six kinds: `nothing` is JSON's `null`, a word Heroes refuses.

## Functions

- `parse(s: str) -> Value?`, called `json.parse(s)`: The value `s` holds as JSON text, white space allowed around it and nothing else after it.
- `render(v: Value) -> str?`, called `json.render(v)`: `v` as JSON text, compact, with an object's fields in the order of their names' bytes and each number laid out as Go's `encoding/json` lays it out: `300`, `0.5`, `1e+21`.
- `field(v: Value, key: str) -> Value?`, called `json.field(v, key)`: The value of the field `key` of the object `v`.

## Codes

- `does_not_fit`: where a number lies past the largest `f64` (`parse`).
- `missing_key`: where the object has no field `key` (`field`).
- `not_an_object`, `json.NOT_AN_OBJECT`: What `field` fails with where the value it is asked of is no object.
- `not_json`, `json.NOT_JSON`: What `parse` fails with where a text is not JSON, and `render` where a number is not finite, which no JSON text writes.
- `not_text`: where a `\u` escape names half of a surrogate pair alone (`parse`).
- `too_deep`, `json.TOO_DEEP`: What `parse` fails with where arrays and objects nest deeper than 512, and `render` where a value does.
