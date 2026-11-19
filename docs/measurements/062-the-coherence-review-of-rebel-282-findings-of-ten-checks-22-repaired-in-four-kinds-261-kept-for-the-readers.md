# The coherence review of Rebel, its measured half: 282 findings of ten checks, 22 repaired in four kinds, 261 kept for the readers

Measured 2026-11-19 by the lane `lane-review` of M-rebel-library, the milestone page's item 31 (`docs/roadmap/milestones/M-rebel-library.md`): *measured by tools first, each an instrument of the net rather than a reading*, then read by one reader per layer who has not written it. This is the first half and what it repaired; the readers start from it. Every number below is the instruments' own, printed by `heroes run tests/harness/rebel_review.hero` from the repository's root, over Rebel 0.1.0 at the lane's base, `rebel-round` at `f4e3594d5` (1,495 modules, 144 ledgers, 144 rows of `examples-rebel/README.md` § A1), and again at the lane's head after the repairs (1,492 modules, 145 ledgers, 145 rows). A path below is under `packages/rebel/0.1.0/files/` unless it names its root; a line is the base's.

**Where this record stands.** `.claude/rules/records.md` § A record is never rewritten and § The top of `docs/` name `docs/measurements/` a tree of dated records, the project's account, and `tests/harness/suite_records.hero` holds it in `RECORD`; neither names a place for a milestone's record written before its close, since a milestone's record is its journal and a lane does not close a milestone (§ Working in lanes). So the review's record is a measurement, appended to by the readers under its sections with their dates, and the journal at the close cites it.

## The instruments

Each is a module of `tests/harness/` with its own tests and a planted witness that makes each check fire, and the `rebel` suite (`tests/harness/suite_rebel.hero`) judges them on every run of the net: a finding the review did not keep fails, and so does a kept finding the tree no longer shows. What the review keeps, each with its reason, is `tests/harness/rebel_kept.hero`.

| check | what it finds | its threshold, and the measurement it rests on | module |
|---|---|---|---|
| `ledgers` | the same capability done by its own package in two ledgers: one id, each ledger naming a function of its own package | none | `rebel_ledgers.hero` |
| `twins` | two public functions of two packages with one name and one signature, each face's own types written qualified | none | `rebel_faces.hero` |
| `prefixed` | a declared type naming many of its face's functions, first words or last (`deque_push`, `add_complex`) | 12: the smallest structure of `containers` before its split (`86dd93d9e^`), `cache_`'s 12 of 122 | `rebel_faces.hero` |
| `oversized` | a face past a size | 90 public functions: Tukey's fence over the 149 faces' counts, quartiles 13, 26 and 44 | `rebel_faces.hero` |
| `names` | a name § A3's grammar does not allow: case, `is_`/`has_` answering `bool`, `contains` and `index_of` one meaning, `format_x` text, `with_x` a copy, `to_x` a value, an abbreviation a reader decodes | a closed list of abbreviations, read from the faces' parameters | `rebel_names.hero` |
| `codes` | a code a `fail` writes outside § A4's closed list and the language's own, literal or through constants followed across `use` bindings | the list `package_codes.CLOSED` | `rebel_codes.hero`, `package_codes.hero`, `module_text.hero` |
| `layers` | a `use` of a package not below its own, private modules' and floors' included; a module no row holds | none | `rebel_layers.hero` |
| `unlisted` | a face with no row of § A1 or no ledger; a floor not named `hero_<x>` | none | `rebel_layers.hero` |
| `bodies` | one body in two packages, normalised: comments and blank rows gone, parameters by place | 5 rows: the median of the 16,736 functions' bodies (quartiles 1, 5, 12) | `rebel_bodies.hero` |
| `one-layer` | two packages of one layer sharing a body or a binding of C, which neither may take from the other | none | `rebel_bodies.hero` |

Counted at the base and at the head:

| check | base | head |
|---|---|---|
| `ledgers` | 35 | 35 |
| `twins` | 31 | 31 |
| `prefixed` | 8 | 8 |
| `oversized` | 11 | 11 |
| `names` | 47 | 47 |
| `codes` | 4 | 0 |
| `layers` | 12 | 4 |
| `unlisted` | 9 | 6 |
| `bodies` | 97 | 95 |
| `one-layer` | 28 | 24 |
| all | 282 | 261 |

## What each instrument found at the base

### `ledgers`: 35

- capability `address/parse` is done by the package of each ledger: mail.ledger:94: one mailbox read: a name, a quoted local part, a domain literal, comments, encoded words in the name; heroes mail.parse_address, mail.Address; net.ledger:8: an address read from text, without a port; heroes net.parse_address (kept, `TWO_MEANINGS`)
- capability `bits/count` is done by the package of each ledger: containers/bits.ledger:30: how many bits are set, and whether none or all are; heroes bits.count_ones, bits.is_clear, bits.is_full; math.ledger:492: how many of an integer's 64 bits are ones; heroes math.count_ones (kept, `TWO_MEANINGS`)
- capability `client/base-url` is done by the package of each ledger: llm/anthropic.ledger:38: another address: a proxy, a gateway, a test's server; heroes llm/anthropic.Client; llm/openai.ledger:39: another server that speaks the format: Ollama, vLLM, llama.cpp, LM Studio, a proxy; heroes llm/openai.local_client, llm/openai.ollama_client, llm/openai.OLLAMA_URL, llm/openai.BASE_URL (kept, `TWO_LLM_CLIENTS`)
- capability `client/env` is done by the package of each ledger: llm/anthropic.ledger:25: the key and the address read from the environment; heroes llm/anthropic.client_from_env; llm/openai.ledger:27: the key, the address, the organization and the project read from the environment; heroes llm/openai.client_from_env; s3.ledger:118: a client made from the environment and the shared files; heroes s3.client_from_env (kept, `EACH_ITS_SERVICE`)
- capability `client/headers` is done by the package of each ledger: llm/anthropic.ledger:44: fields added to every request, and betas named; heroes llm/anthropic.Client; llm/openai.ledger:46: fields added to every request; heroes llm/openai.Client (kept, `TWO_LLM_CLIENTS`)
- capability `client/key` is done by the package of each ledger: llm/anthropic.ledger:18: a client made with an API key, sent as `x-api-key`; heroes llm/anthropic.default_client, llm/anthropic.Client; llm/openai.ledger:20: a client made with an API key, sent as `Authorization: Bearer`; heroes llm/openai.default_client, llm/openai.Client (kept, `TWO_LLM_CLIENTS`)
- capability `client/retries` is done by the package of each ledger: http.ledger:341: a request sent again, with a doubling wait, on chosen statuses and on failures safe to retry; heroes http.Retries, http.Client.retries; llm/anthropic.ledger:51: a request sent again after a transient failure, waiting what the server asks; heroes llm/anthropic.Client; llm/openai.ledger:52: a request sent again after a transient failure, waiting what the server asks; heroes llm/openai.Client (kept, `RETRIES_OF_THEIR_OWN`)
- capability `client/timeout` is done by the package of each ledger: llm/anthropic.ledger:58: a time limit on an attempt, and on the wait for a stream's next bytes; heroes llm/anthropic.Client; llm/openai.ledger:59: a time limit on an attempt, and on the wait for a stream's next bytes; heroes llm/openai.Client (kept, `TWO_LLM_CLIENTS`)
- capability `client/transport` is done by the package of each ledger: llm/anthropic.ledger:65: the authorities trusted, the connection's own settings; heroes llm/anthropic.Client; llm/openai.ledger:65: the authorities trusted; heroes llm/openai.Client (kept, `TWO_LLM_CLIENTS`)
- capability `date/parse` is done by the package of each ledger: http.ledger:564: an HTTP date read in its three forms; heroes http.parse_date; i18n/dates.ledger:53: a date or time written in a language read back; heroes dates.parse_pattern (kept, `TWO_MEANINGS`)
- capability `document/operations` is done by the package of each ledger: graphql.ledger:43: the operations a document holds, their kinds, names and variables; heroes graphql.operations_of, graphql.Operation, graphql.Variable; openapi.ledger:57: an operation: tags, summary, description, operationId, parameters, body, responses, callbacks, deprecated, its own security and servers; heroes openapi.Operation, openapi.new_operation, openapi.has_request_body (kept, `TWO_MEANINGS`)
- capability `errors/advice` is done by the package of each ledger: llm/anthropic.ledger:257: whether a failure may pass if sent again, and after how long; heroes llm/anthropic.ApiError; llm/openai.ledger:202: whether a failure may pass if sent again, and after how long; heroes llm/openai.ApiError (kept, `TWO_LLM_CLIENTS`)
- capability `errors/codes` is done by the package of each ledger: http2.ledger:46: the error codes of RFC 9113 § 7 and their names; heroes http2.NO_ERROR, http2.PROTOCOL_ERROR, http2.INTERNAL_ERROR, http2.FLOW_CONTROL_ERROR, http2.SETTINGS_TIMEOUT, http2.STREAM_CLOSED, http2.FRAME_SIZE_ERROR, http2.REFUSED_STREAM, http2.CANCEL, http2.COMPRESSION_ERROR, http2.CONNECT_ERROR, http2.ENHANCE_YOUR_CALM, http2.INADEQUATE_SECURITY, http2.HTTP_1_1_REQUIRED, http2.error_name; jsonrpc.ledger:45: the specification's error codes and the range left to servers; heroes jsonrpc.PARSE_ERROR, jsonrpc.INVALID_REQUEST, jsonrpc.METHOD_NOT_FOUND, jsonrpc.INVALID_PARAMS, jsonrpc.INTERNAL_ERROR, jsonrpc.SERVER_ERROR_FIRST, jsonrpc.SERVER_ERROR_LAST (kept, `TWO_MEANINGS`)
- capability `errors/connection` is done by the package of each ledger: llm/anthropic.ledger:270: a connection refused, cut or timed out; heroes llm/anthropic.CLOSED, llm/anthropic.TIMEOUT; llm/openai.ledger:209: a connection refused, cut or timed out; heroes llm/openai.CLOSED, llm/openai.TIMEOUT (kept, `TWO_LLM_CLIENTS`)
- capability `errors/status` is done by the package of each ledger: llm/anthropic.ledger:250: a failure the API answered, by its status and its type; heroes llm/anthropic.INVALID, llm/anthropic.PERMISSION, llm/anthropic.NOT_FOUND, llm/anthropic.LIMIT, llm/anthropic.TIMEOUT, llm/anthropic.FAILED, llm/anthropic.ApiError, llm/anthropic.failure_of; llm/openai.ledger:195: a failure the server answered, by its status, type and code; heroes llm/openai.INVALID, llm/openai.PERMISSION, llm/openai.NOT_FOUND, llm/openai.LIMIT, llm/openai.TIMEOUT, llm/openai.FAILED, llm/openai.ApiError, llm/openai.failure_of (kept, `TWO_LLM_CLIENTS`)
- capability `html/escape` is done by the package of each ledger: html.ledger:285: text escaped to stand in a page; heroes html.escape, html.escape_text, html.escape_attribute; template.ledger:194: a text escaped as HTML; heroes template.escape_html (kept, `TEMPLATE_MAY_USE_HTML`)
- capability `html/unescape` is done by the package of each ledger: html.ledger:292: character references read, in text or in an attribute's value; heroes html.unescape, html.unescape_attribute; template.ledger:201: HTML's character references read; heroes template.unescape_html (kept, `TEMPLATE_MAY_USE_HTML`)
- capability `http/client` is done by the package of each ledger: graphql.ledger:232: a request sent to a server and its response read; heroes graphql.call_http, graphql.format_request, graphql.parse_response; jsonrpc.ledger:152: calls sent over HTTP and their answers read; heroes jsonrpc.exchange_http, jsonrpc.call_http (kept, `TWO_MEANINGS`)
- capability `http/server` is done by the package of each ledger: graphql.ledger:225: a request read from HTTP by GET and POST and answered in the media type the client accepts; heroes graphql.read_http, graphql.respond_http, graphql.refusal_http, graphql.serve_http; jsonrpc.ledger:145: a JSON-RPC body read from an HTTP request and its answers written as the response, a method other than POST and a type other than JSON refused; heroes jsonrpc.read_http, jsonrpc.respond_http, jsonrpc.refusal_http (kept, `TWO_MEANINGS`)
- capability `message/values` is done by the package of each ledger: jsonrpc.ledger:18: a request, a notification and an answer as values: method, parameters, result, error; heroes jsonrpc.Message, jsonrpc.Reply, jsonrpc.Error; protobuf.ledger:197: a field's value read as the language's own types; heroes protobuf.Value, protobuf.kind_of, protobuf.bool_of, protobuf.int_of, protobuf.uint_of, protobuf.float_of, protobuf.text_of, protobuf.bytes_of, protobuf.message_of (kept, `TWO_MEANINGS`)
- capability `name/reverse` is done by the package of each ledger: dns.ledger:116: the name of a reverse lookup and the address it names; heroes dns.parse_reverse_name, net.to_reverse_name; net.ledger:392: the name of an address; heroes net.name_of (kept, `TWO_MEANINGS`)
- capability `number/percent` is done by the package of each ledger: i18n/numbers.ledger:22: a fraction written as a percentage; heroes numbers.format_percent; numbers.ledger:189: a ratio written as a percentage; heroes numbers.format_percent (kept, `PLAIN_AND_LOCALE`)
- capability `options` is done by the package of each ledger: grpc.ledger:64: what a channel or a server allows: the largest message, the user agent, HTTP/2's own options; heroes grpc.Options, grpc.default_options; http2.ledger:107: what an end advertises and the bounds it holds the peer to; heroes http2.Options, http2.Limits, http2.default_options (kept, `TWO_MEANINGS`)
- capability `random/bytes` is done by the package of each ledger: os.ledger:352: bytes from the system's generator for keys; heroes os.random_bytes; random.ledger:43: bytes from a seeded generator; heroes random.next_bytes, random.next_bytes_chacha8 (kept, `TWO_MEANINGS`)
- capability `raw/request` is done by the package of each ledger: llm/anthropic.ledger:276: any endpoint reached with the client's key, version and retries, its answer whole with its fields; heroes llm/anthropic.send_raw; llm/openai.ledger:215: any endpoint reached with the client's key and retries, its answer whole with its fields; heroes llm/openai.send_raw (kept, `TWO_LLM_CLIENTS`)
- capability `response/json` is done by the package of each ledger: graphql.ledger:257: a request and a response written as JSON and read back; heroes graphql.parse_request, graphql.format_request, graphql.parse_response, graphql.format_response, graphql.request_of, graphql.request_value, graphql.response_of, graphql.response_value, graphql.error_of, graphql.error_value; http.ledger:187: a response's body parsed as JSON; heroes http.json_of (kept, `TWO_MEANINGS`)
- capability `response/redirect` is done by the package of each ledger: http.ledger:159: a response sending the client elsewhere; heroes http.respond_redirect; web.ledger:244: a redirect; heroes http.respond_redirect, web.path_for (kept, `NAMES_HTTPS`)
- capability `server/dispatch` is done by the package of each ledger: grpc.ledger:117: the next call found by its path, an unknown one answered UNIMPLEMENTED; heroes grpc.next_call, grpc.ServerCall; jsonrpc.ledger:78: a whole text answered by a server with no state: each request found and handled, each notification handled and unanswered, a batch's answers in one array, nothing written for notifications alone; heroes jsonrpc.dispatch; mcp.ledger:72: each request found and checked, the package answering what it can from the description, the rest handed to the program; heroes mcp.handle, mcp.handle_method, mcp.Request, mcp.tool_call_of, mcp.resource_read_of, mcp.prompt_get_of, mcp.completion_of, mcp.tool_named (kept, `TWO_MEANINGS`)
- capability `status/text` is done by the package of each ledger: http.ledger:8: the text of a status; heroes http.status_text; process.ledger:259: how a child ended, as words; heroes process.format_status (kept, `TWO_MEANINGS`)
- capability `structured-output` is done by the package of each ledger: llm/anthropic.ledger:283: an answer held to a JSON schema and read as JSON; heroes llm/anthropic.with_field, json.parse; llm/openai.ledger:221: an answer held to a JSON schema and read as JSON; heroes llm/openai.with_chat_field, llm/openai.with_response_field, json.parse (kept, `TWO_LLM_CLIENTS`)
- capability `text/words` is done by the package of each ledger: strings.ledger:226: the words of a text, between runs of white space; heroes strings.words; unicode/segment.ledger:29: a text cut into words by UAX #29; heroes unicode/segment.words, unicode/segment.word_segments, unicode/segment.word_boundaries (kept, `TWO_MEANINGS`)
- capability `vector/arithmetic` is done by the package of each ledger: math/linear.ledger:27: vectors added, taken away, negated, scaled, multiplied part by part; heroes linear.add_vec2, linear.sub_vec2, linear.neg_vec2, linear.scale_vec2, linear.mul_vec2, and the same for `vec3` and `vec4`; vectors.ledger:90: two vectors added, subtracted, multiplied element by element, a vector scaled; heroes vectors.add, vectors.subtract, vectors.multiply, vectors.scale (kept, `TWO_SHAPES_OF_VECTOR`)
- capability `vector/dot` is done by the package of each ledger: math/linear.ledger:34: the dot product; heroes linear.dot_vec2, linear.dot_vec3, linear.dot_vec4; vectors.ledger:35: the dot product of two vectors; heroes vectors.dot (kept, `TWO_SHAPES_OF_VECTOR`)
- capability `vector/normalize` is done by the package of each ledger: math/linear.ledger:57: a vector's direction, of length 1; heroes linear.normalize_vec2, linear.normalize_vec3, linear.normalize_vec4; vectors.ledger:82: a vector scaled to length one; heroes vectors.normalize (kept, `TWO_SHAPES_OF_VECTOR`)
- capability `write/json` is done by the package of each ledger: openapi.ledger:155: a document written as JSON: each object's fields in the specification's order, indented or not; heroes openapi.to_json, openapi.format_json, openapi.info_json, openapi.server_json, openapi.tag_json, openapi.external_docs_json, openapi.path_item_json, openapi.operation_json, openapi.parameter_json, openapi.request_body_json, openapi.media_type_json, openapi.encoding_json, openapi.response_json, openapi.header_json, openapi.example_json, openapi.link_json, openapi.callback_json, openapi.components_json, openapi.security_scheme_json, openapi.security_requirement_json; websocket.ledger:144: a JSON value written as a text; heroes websocket.write_json (kept, `TWO_MEANINGS`)

### `twins`: 31

- `compare` has one signature, <0>(str, str) -> i64, in each of: path.hero:418: function compare(path: str, other: str) -> i64; strings.hero:145: function compare(a: str, b: str) -> i64 (kept, `TWO_MEANINGS`)
- `decode` has one signature, <0>([u8], image.Limits) -> image.Image?, in each of: image/bmp.hero:49: function decode(data: [u8], limits: image.Limits) -> image.Image?; image/netpbm.hero:52: function decode(data: [u8], limits: image.Limits) -> image.Image? (kept, `ONE_API_BY_DESIGN`)
- `decompress` has one signature, <0>([u8], i64) -> [u8]?, in each of: compress/brotli.hero:111: function decompress(data: [u8], limit: i64) -> [u8]?; compress/bzip2.hero:94: function decompress(data: [u8], limit: i64) -> [u8]?; compress/lz4.hero:186: function decompress(data: [u8], limit: i64) -> [u8]?; compress/xz.hero:136: function decompress(data: [u8], limit: i64) -> [u8]?; compress/zstd.hero:114: function decompress(data: [u8], limit: i64) -> [u8]? (kept, `ONE_API_BY_DESIGN`)
- `encoding` has one signature, <0>() -> tokenizer.Encoding, in each of: tokenizer/tables/cl100k_base.hero:22: function encoding() -> tokenizer.Encoding; tokenizer/tables/o200k_base.hero:20: function encoding() -> tokenizer.Encoding; tokenizer/tables/o200k_harmony.hero:22: function encoding() -> tokenizer.Encoding (kept, `ONE_API_BY_DESIGN`)
- `error_name` has one signature, <0>(i64) -> str?, in each of: http2.hero:163: function error_name(code: i64) -> str?; kafka.hero:1291: function error_name(code: i64) -> str? (kept, `TWO_MEANINGS`)
- `escape` has one signature, <0>(str) -> str, in each of: html.hero:1171: function escape(s: str) -> str; regex.hero:113: function escape(s: str) -> str (kept, `TWO_MEANINGS`)
- `format` has one signature, <0>(json.Value) -> str?, in each of: json.hero:404: function format(v: Value) -> str?; yaml.hero:194: function format(v: json.Value) -> str? (kept, `FORMATS_OF_ONE_VALUE`)
- `format_date` has one signature, <0>(time.Instant) -> str, in each of: http.hero:3257: function format_date(t: time.Instant) -> str; pdf.hero:442: function format_date(t: time.Instant) -> str (kept, `EACH_ITS_GRAMMAR`)
- `frame_size` has one signature, <0>([u8]) -> i64?, in each of: compress/lz4.hero:317: function frame_size(data: [u8]) -> i64?; compress/zstd.hero:204: function frame_size(data: [u8]) -> i64? (kept, `ONE_API_BY_DESIGN`)
- `is_ascii` has one signature, <0>(str) -> bool, in each of: strings.hero:675: function is_ascii(s: str) -> bool; unicode.hero:314: function is_ascii(c: str) -> bool (kept, `CHARACTER_AND_TEXT`)
- `is_lower` has one signature, <0>(str) -> bool, in each of: strings.hero:474: function is_lower(s: str) -> bool; unicode.hero:257: function is_lower(c: str) -> bool (kept, `CHARACTER_AND_TEXT`)
- `is_title` has one signature, <0>(str) -> bool, in each of: strings.hero:486: function is_title(s: str) -> bool; unicode.hero:263: function is_title(c: str) -> bool (kept, `CHARACTER_AND_TEXT`)
- `is_upper` has one signature, <0>(str) -> bool, in each of: strings.hero:480: function is_upper(s: str) -> bool; unicode.hero:251: function is_upper(c: str) -> bool (kept, `CHARACTER_AND_TEXT`)
- `is_valid` has one signature, <0>(str) -> bool, in each of: json.hero:252: function is_valid(text: str) -> bool; json/jq.hero:108: function is_valid(text: str) -> bool; json/path.hero:80: function is_valid(text: str) -> bool; semver.hero:101: function is_valid(text: str) -> bool; yaml.hero:168: function is_valid(text: str) -> bool (kept, `EACH_ITS_GRAMMAR`)
- `is_valid_filter` has one signature, <0>(str) -> bool, in each of: mqtt.hero:1136: function is_valid_filter(s: str) -> bool; nats.hero:909: function is_valid_filter(s: str) -> bool (kept, `EACH_ITS_GRAMMAR`)
- `labels_of` has one signature, <0>(str) -> [str]?, in each of: dns.hero:966: function labels_of(name: str) -> [str]?; unicode/encodings.hero:247: function labels_of(encoding: str) -> [str]? (kept, `TWO_MEANINGS`)
- `matches` has one signature, <0>(str, str) -> bool, in each of: mqtt.hero:1148: function matches(pattern: str, topic: str) -> bool; nats.hero:919: function matches(pattern: str, subject: str) -> bool (kept, `EACH_ITS_GRAMMAR`)
- `name` has one signature, <0>(str) -> str?, in each of: path.hero:155: function name(path: str) -> str?; unicode/names.hero:51: function name(c: str) -> str? (kept, `TWO_MEANINGS`)
- `names` has one signature, <0>() -> [str], in each of: tokenizer/tables.hero:40: function names() -> [str]; unicode/encodings.hero:254: function names() -> [str] (kept, `TWO_MEANINGS`)
- `parse` has one signature, <0>(str) -> json.Value?, in each of: json.hero:147: function parse(text: str) -> Value?; yaml.hero:110: function parse(text: str) -> json.Value? (kept, `FORMATS_OF_ONE_VALUE`)
- `parse_date` has one signature, <0>(str) -> time.Instant?, in each of: http.hero:3267: function parse_date(s: str) -> time.Instant?; pdf.hero:1797: function parse_date(s: str) -> time.Instant? (kept, `EACH_ITS_GRAMMAR`)
- `quote` has one signature, <0>(str) -> str, in each of: json.hero:527: function quote(s: str) -> str; process.hero:905: function quote(word: str) -> str (kept, `TWO_MEANINGS`)
- `remove_all` has one signature, <0>(str) -> ()?, in each of: fs.hero:447: function remove_all(path: str) -> ()?; keyring.hero:201: function remove_all(service: str) -> ()? (kept, `TWO_MEANINGS`)
- `set_deadline` has one signature, <0>(io.Stream, i64) -> ()?, in each of: ipc.hero:275: function set_deadline(s: io.Stream, ms: i64) -> ()?; tls.hero:474: function set_deadline(s: io.Stream, ms: i64) -> ()? (kept, `EACH_ITS_STREAM`)
- `skippable_frame` has one signature, <0>([u8], i64) -> [u8]?, in each of: compress/lz4.hero:330: function skippable_frame(data: [u8], magic: i64) -> [u8]?; compress/zstd.hero:217: function skippable_frame(data: [u8], magic: i64) -> [u8]? (kept, `ONE_API_BY_DESIGN`)
- `to_casefold` has one signature, <0>(str) -> str, in each of: strings.hero:458: function to_casefold(s: str) -> str; unicode.hero:379: function to_casefold(c: str) -> str (kept, `CHARACTER_AND_TEXT`)
- `to_lower` has one signature, <0>(str) -> str, in each of: strings.hero:429: function to_lower(s: str) -> str; unicode.hero:362: function to_lower(c: str) -> str (kept, `CHARACTER_AND_TEXT`)
- `to_title` has one signature, <0>(str) -> str, in each of: strings.hero:442: function to_title(s: str) -> str; unicode.hero:370: function to_title(c: str) -> str (kept, `CHARACTER_AND_TEXT`)
- `to_upper` has one signature, <0>(str) -> str, in each of: strings.hero:435: function to_upper(s: str) -> str; unicode.hero:354: function to_upper(c: str) -> str (kept, `CHARACTER_AND_TEXT`)
- `unescape` has one signature, <0>(str) -> str?, in each of: url.hero:330: function unescape(s: str) -> str?; xml.hero:542: function unescape(s: str) -> str? (kept, `TWO_MEANINGS`)
- `words` has one signature, <0>(str) -> [str], in each of: strings.hero:241: function words(s: str) -> [str]; unicode/segment.hero:124: function words(s: str) -> [str] (kept, `TWO_MEANINGS`)

### `prefixed`: 8

- big.hero: the type `Decimal` names 13 of its functions (`decimal` first or last): parse_decimal, format_decimal, add_decimal, subtract_decimal, multiply_decimal, divide_decimal, compare_decimal, sqrt_decimal, power_decimal, decimal_of_float, float_of_decimal, decimal_of_fraction, fraction_of_decimal (kept, `THREE_EXACT_NUMBERS`)
- big.hero: the type `Fraction` names 14 of its functions (`fraction` first or last): add_fraction, subtract_fraction, multiply_fraction, divide_fraction, power_fraction, compare_fraction, round_fraction, decimal_of_fraction, fraction_of_decimal, float_of_fraction, float32_of_fraction, fraction_of_float, parse_fraction, format_fraction (kept, `THREE_EXACT_NUMBERS`)
- math.hero: the type `Complex` names 30 of its functions (`complex` first or last): add_complex, sub_complex, mul_complex, div_complex, neg_complex, abs_complex, to_complex, project_complex, is_nan_complex, is_inf_complex, is_finite_complex, is_close_complex, exp_complex, ln_complex, log10_complex, log_complex, sqrt_complex, pow_complex, sin_complex, cos_complex, tan_complex, asin_complex, acos_complex, atan_complex, sinh_complex, cosh_complex, tanh_complex, asinh_complex, acosh_complex, atanh_complex (kept, `COMPLEX_IN_MATH`)
- math/linear.hero: the type `Vec2` names 15 of its functions (`vec2` first or last): add_vec2, sub_vec2, neg_vec2, scale_vec2, mul_vec2, dot_vec2, length_vec2, length_squared_vec2, distance_vec2, normalize_vec2, lerp_vec2, angle_vec2, cross_vec2, reflect_vec2, mul_mat2_vec2 (kept, `ONE_DOMAIN_OF_TYPES`)
- math/linear.hero: the type `Vec3` names 16 of its functions (`vec3` first or last): add_vec3, sub_vec3, neg_vec3, scale_vec3, mul_vec3, dot_vec3, length_vec3, length_squared_vec3, distance_vec3, normalize_vec3, lerp_vec3, angle_vec3, cross_vec3, reflect_vec3, mul_mat3_vec3, rotate_vec3 (kept, `ONE_DOMAIN_OF_TYPES`)
- math/linear.hero: the type `Vec4` names 13 of its functions (`vec4` first or last): add_vec4, sub_vec4, neg_vec4, scale_vec4, mul_vec4, dot_vec4, length_vec4, length_squared_vec4, distance_vec4, normalize_vec4, lerp_vec4, angle_vec4, mul_mat4_vec4 (kept, `ONE_DOMAIN_OF_TYPES`)
- math/linear.hero: the type `Mat3` names 17 of its functions (`mat3` first or last): identity_mat3, add_mat3, sub_mat3, scale_mat3, mul_mat3, transpose_mat3, determinant_mat3, inverse_mat3, translation_mat3, scaling_mat3, rotation_mat3, rotation_x_mat3, rotation_y_mat3, rotation_z_mat3, extend_mat3, upper_mat3, to_mat3 (kept, `ONE_DOMAIN_OF_TYPES`)
- math/linear.hero: the type `Mat4` names 15 of its functions (`mat4` first or last): identity_mat4, add_mat4, sub_mat4, scale_mat4, mul_mat4, transpose_mat4, determinant_mat4, inverse_mat4, translation_mat4, scaling_mat4, rotation_mat4, rotation_x_mat4, rotation_y_mat4, rotation_z_mat4, to_mat4 (kept, `ONE_DOMAIN_OF_TYPES`)

### `oversized`: 11

- bytes.hero: 124 public functions, past 90 (kept, `A_SPLIT_IS_NOT_LOCAL`)
- http.hero: 108 public functions, past 90 (kept, `A_SPLIT_IS_NOT_LOCAL`)
- math.hero: 155 public functions, past 90 (kept, `A_SPLIT_IS_NOT_LOCAL`)
- math/linear.hero: 114 public functions, past 90 (kept, `A_SPLIT_IS_NOT_LOCAL`)
- mcp.hero: 111 public functions, past 90 (kept, `A_SPLIT_IS_NOT_LOCAL`)
- net.hero: 105 public functions, past 90 (kept, `A_SPLIT_IS_NOT_LOCAL`)
- openapi.hero: 133 public functions, past 90 (kept, `A_SPLIT_IS_NOT_LOCAL`)
- pdf.hero: 92 public functions, past 90 (kept, `A_SPLIT_IS_NOT_LOCAL`)
- protobuf.hero: 94 public functions, past 90 (kept, `A_SPLIT_IS_NOT_LOCAL`)
- strings.hero: 101 public functions, past 90 (kept, `A_SPLIT_IS_NOT_LOCAL`)
- time.hero: 97 public functions, past 90 (kept, `A_SPLIT_IS_NOT_LOCAL`)

### `names`: 47

- amqp.hero:713: abbreviated: the parameter `opts` shortens `options`, in 1 functions, at lines 713 (kept, `PARAMETER_LABELS`)
- amqp.hero:889: abbreviated: the parameter `ch` shortens `channel`, in 25 functions, at lines 889, 898, 909, 929, 946, 980, 995, 1012, 1026, 1054, 1072, 1086, 1098, 1112, 1163, 1202, 1216, 1238, 1298, 1311, 1340, 1393, 1420, 1430, 1439 (kept, `PARAMETER_LABELS`)
- crypto/keypairs.hero:383: text: `function format_private_der(key: PrivateKey) -> [u8]?`: `format_x` writes a value as text, and this answers `[u8]?` (kept, `BYTES_NAMED_FORMAT`)
- crypto/keypairs.hero:391: text: `function format_public_der(key: PublicKey) -> [u8]?`: `format_x` writes a value as text, and this answers `[u8]?` (kept, `BYTES_NAMED_FORMAT`)
- crypto/keypairs.hero:467: text: `function format_private_raw(key: PrivateKey) -> [u8]?`: `format_x` writes a value as text, and this answers `[u8]?` (kept, `BYTES_NAMED_FORMAT`)
- crypto/keypairs.hero:479: text: `function format_public_raw(key: PublicKey) -> [u8]?`: `format_x` writes a value as text, and this answers `[u8]?` (kept, `BYTES_NAMED_FORMAT`)
- graphql.hero:1049: abbreviated: the parameter `msg` shortens `message`, in 1 functions, at lines 1049 (kept, `PARAMETER_LABELS`)
- grpc.hero:455: abbreviated: the parameter `conn` shortens `connection`, in 1 functions, at lines 455 (kept, `PARAMETER_LABELS`)
- grpc.hero:463: abbreviated: the parameter `ch` shortens `channel`, in 9 functions, at lines 463, 480, 511, 542, 573, 604, 616, 716, 790 (kept, `PARAMETER_LABELS`)
- image.hero:289: abbreviated: the parameter `img` shortens `image`, in 27 functions, at lines 289, 313, 336, 357, 384, 415, 436, 475, 499, 519, 541, 561, 582, 604, 625, 651, 701, 742, 790, 946, 971, 995, 1021, 1046, 1067, 1114, 1147 (kept, `PARAMETER_LABELS`)
- image.hero:825: abbreviated: the parameter `src` shortens `source`, in 2 functions, at lines 825, 861 (kept, `PARAMETER_LABELS`)
- image/bmp.hero:85: abbreviated: the parameter `img` shortens `image`, in 1 functions, at lines 85 (kept, `PARAMETER_LABELS`)
- image/gif.hero:216: abbreviated: the parameter `img` shortens `image`, in 1 functions, at lines 216 (kept, `PARAMETER_LABELS`)
- image/jpeg.hero:159: abbreviated: the parameter `img` shortens `image`, in 1 functions, at lines 159 (kept, `PARAMETER_LABELS`)
- image/netpbm.hero:103: abbreviated: the parameter `img` shortens `image`, in 2 functions, at lines 103, 161 (kept, `PARAMETER_LABELS`)
- image/png.hero:205: abbreviated: the parameter `img` shortens `image`, in 1 functions, at lines 205 (kept, `PARAMETER_LABELS`)
- jose.hero:481: text: `function format_public_der(key: Key) -> [u8]?`: `format_x` writes a value as text, and this answers `[u8]?` (kept, `BYTES_NAMED_FORMAT`)
- jose.hero:492: text: `function format_private_der(key: Key) -> [u8]?`: `format_x` writes a value as text, and this answers `[u8]?` (kept, `BYTES_NAMED_FORMAT`)
- jsonrpc.hero:255: abbreviated: the parameter `msg` shortens `message`, in 2 functions, at lines 255, 954 (kept, `PARAMETER_LABELS`)
- kafka.hero:510: abbreviated: the parameter `opts` shortens `options`, in 2 functions, at lines 510, 1020 (kept, `PARAMETER_LABELS`)
- ldap.hero:653: abbreviated: the parameter `opts` shortens `options`, in 1 functions, at lines 653 (kept, `PARAMETER_LABELS`)
- llm/anthropic.hero:780: abbreviated: the parameter `msg` shortens `message`, in 1 functions, at lines 780 (kept, `PARAMETER_LABELS`)
- llm/openai.hero:352: abbreviated: the parameter `msg` shortens `message`, in 1 functions, at lines 352 (kept, `PARAMETER_LABELS`)
- log.hero:348: abbreviated: the parameter `msg` shortens `message`, in 6 functions, at lines 348, 362, 372, 382, 392, 410 (kept, `PARAMETER_LABELS`)
- log/files.hero:133: abbreviated: the parameter `msg` shortens `message`, in 1 functions, at lines 133 (kept, `PARAMETER_LABELS`)
- mail.hero:220: text: `function format_message(m: Message, options: WriteOptions) -> [u8]?`: `format_x` writes a value as text, and this answers `[u8]?` (kept, `BYTES_NAMED_FORMAT`)
- mail.hero:297: text: `function format_header(fields: [Field], options: WriteOptions) -> [u8]?`: `format_x` writes a value as text, and this answers `[u8]?` (kept, `BYTES_NAMED_FORMAT`)
- mongodb.hero:367: abbreviated: the parameter `opts` shortens `options`, in 15 functions, at lines 367, 653, 810, 828, 849, 883, 898, 1111, 1138, 1271, 1343, 1373, 1405, 1570, 1663 (kept, `PARAMETER_LABELS`)
- mqtt.hero:653: abbreviated: the parameter `opts` shortens `options`, in 1 functions, at lines 653 (kept, `PARAMETER_LABELS`)
- nats.hero:387: abbreviated: the parameter `opts` shortens `options`, in 1 functions, at lines 387 (kept, `PARAMETER_LABELS`)
- pdf.hero:1376: abbreviated: the parameter `img` shortens `image`, in 1 functions, at lines 1376 (kept, `PARAMETER_LABELS`)
- redis.hero:292: abbreviated: the parameter `opts` shortens `options`, in 1 functions, at lines 292 (kept, `PARAMETER_LABELS`)
- redis.hero:642: abbreviated: the parameter `msg` shortens `message`, in 1 functions, at lines 642 (kept, `PARAMETER_LABELS`)
- regex.hero:117: abbreviated: the parameter `re` shortens `regex`, in 21 functions, at lines 117, 122, 127, 156, 161, 170, 179, 187, 197, 202, 297, 302, 307, 321, 327, 333, 347, 352, 357, 373, 379 (kept, `PARAMETER_LABELS`)
- sql.hero:487: abbreviated: the parameter `msg` shortens `message`, in 1 functions, at lines 487 (kept, `PARAMETER_LABELS`)
- sql/migrate.hero:154: abbreviated: the parameter `opts` shortens `options`, in 10 functions, at lines 154, 172, 237, 293, 325, 345, 371, 398, 483, 497 (kept, `PARAMETER_LABELS`)
- sql/mysql.hero:221: abbreviated: the parameter `opts` shortens `options`, in 1 functions, at lines 221 (kept, `PARAMETER_LABELS`)
- sql/odbc.hero:169: abbreviated: the parameter `opts` shortens `options`, in 1 functions, at lines 169 (kept, `PARAMETER_LABELS`)
- sql/postgres.hero:236: abbreviated: the parameter `opts` shortens `options`, in 1 functions, at lines 236 (kept, `PARAMETER_LABELS`)
- strings.hero:653: abbreviated: the parameter `arg` shortens `argument`, in 1 functions, at lines 653 (kept, `PARAMETER_LABELS`)
- threads.hero:56: abbreviated: the parameter `arg` shortens `argument`, in 3 functions, at lines 56, 72, 113 (kept, `PARAMETER_LABELS`)
- tokenizer.hero:124: abbreviated: the parameter `enc` shortens `encoding`, in 21 functions, at lines 124, 133, 139, 147, 162, 169, 178, 195, 205, 223, 232, 246, 257, 284, 299, 313, 323, 332, 341, 355, 372 (kept, `PARAMETER_LABELS`)
- tracing.hero:1147: abbreviated: the parameter `msg` shortens `message`, in 1 functions, at lines 1147 (kept, `PARAMETER_LABELS`)
- web.hero:849: copy: `function with_cors(r: http.Request, response: http.Response, cors: Cors) -> http.Response`: `with_x` answers a copy of what it is given with one thing changed, and this takes `http.Request` first and answers `http.Response` (kept, `REQUEST_FIRST`)
- web.hero:2022: copy: `function with_etag(r: http.Request, response: http.Response) -> http.Response`: `with_x` answers a copy of what it is given with one thing changed, and this takes `http.Request` first and answers `http.Response` (kept, `REQUEST_FIRST`)
- x509.hero:1581: abbreviated: the parameter `cert` shortens `certificate`, in 6 functions, at lines 1581, 1590, 1600, 1610, 2204, 2225 (kept, `PARAMETER_LABELS`)
- xlsx.hero:748: abbreviated: the parameter `wb` shortens `workbook`, in 8 functions, at lines 748, 775, 975, 1037, 1075, 1095, 1413, 1426 (kept, `PARAMETER_LABELS`)

### `codes`: 4

- numbers/internal/fewest.hero:102,109,113,116: a `fail` writes `undecided`, outside § A4's list (repaired at 56b)
- numbers/internal/quick.hero:69,82,86,106,150,158: a `fail` writes `undecided`, outside § A4's list (repaired at 56b)
- numbers/internal/side.hero:74,79,118,123: a `fail` writes `undecided`, outside § A4's list (repaired at 56b)
- sql/sqlite/hero_sqlite3.hero:181: a `fail` writes `none`, outside § A4's list (repaired at 56b)

### `layers`: 12

- containers/internal/codes: no row of § A1 holds this module, so its layer cannot be asked (kept, `CONTAINERS_SHARED_MODULES`)
- containers/internal/samples: no row of § A1 holds this module, so its layer cannot be asked (kept, `CONTAINERS_SHARED_MODULES`)
- containers/internal/storage: no row of § A1 holds this module, so its layer cannot be asked (kept, `CONTAINERS_SHARED_MODULES`)
- compress/brotli: `compress/brotli`, layer 2, uses `compress`, layer 3, in 12 `use` lines: compress/brotli.hero:29 `compress/internal/codes`, compress/brotli/internal/decoder.hero:13 `compress/internal/codes`, compress/brotli/internal/decoder.hero:14 `compress/internal/cursor`, compress/brotli/internal/encoder.hero:11 `compress/internal/bitout`, compress/brotli/internal/encoder.hero:12 `compress/internal/codes`, compress/brotli/internal/encoder.hero:13 `compress/internal/finder`, compress/brotli/internal/encoder.hero:14 `compress/internal/huffman`, compress/brotli/internal/header.hero:9 `compress/internal/codes`, compress/brotli/internal/header.hero:10 `compress/internal/cursor`, compress/brotli/internal/prefix.hero:7 `compress/internal/codes`, compress/brotli/internal/prefix.hero:8 `compress/internal/cursor`, compress/brotli/internal/prefix.hero:9 `compress/internal/huffman` (repaired at 56c)
- compress/bzip2: `compress/bzip2`, layer 2, uses `compress`, layer 3, in 5 `use` lines: compress/bzip2.hero:23 `compress/internal/codes`, compress/bzip2/internal/bitio.hero:7 `compress/internal/codes`, compress/bzip2/internal/decoder.hero:9 `compress/internal/codes`, compress/bzip2/internal/encoder.hero:11 `compress/internal/codes`, compress/bzip2/internal/encoder.hero:12 `compress/internal/huffman` (repaired at 56c)
- compress/lz4: `compress/lz4`, layer 3, uses `compress`, layer 3, in 9 `use` lines: compress/lz4.hero:30 `compress/internal/codes`, compress/lz4/internal/blocks.hero:5 `compress/internal/codes`, compress/lz4/internal/decoder.hero:7 `compress/internal/codes`, compress/lz4/internal/framer.hero:5 `compress/internal/codes`, compress/lz4/internal/frames.hero:8 `compress/internal/codes`, compress/lz4/internal/header.hero:6 `compress/internal/codes`, compress/lz4/internal/high.hero:6 `compress/internal/finder`, compress/lz4/internal/unframer.hero:5 `compress/internal/codes`, compress/lz4/internal/walk.hero:4 `compress/internal/codes` (repaired at 56c)
- compress/xz: `compress/xz`, layer 3, uses `compress`, layer 3, in 7 `use` lines: compress/xz.hero:24 `compress/internal/codes`, compress/xz/internal/container.hero:8 `compress/internal/codes`, compress/xz/internal/filters.hero:10 `compress/internal/codes`, compress/xz/internal/lzma2_dec.hero:5 `compress/internal/codes`, compress/xz/internal/lzma_dec.hero:8 `compress/internal/codes`, compress/xz/internal/writer.hero:9 `compress/internal/codes`, compress/xz/internal/writer.hero:10 `compress/internal/finder` (repaired at 56c)
- compress/zstd: `compress/zstd`, layer 2, uses `compress`, layer 3, in 11 `use` lines: compress/zstd.hero:26 `compress/internal/codes`, compress/zstd/internal/bits.hero:8 `compress/internal/codes`, compress/zstd/internal/dict.hero:6 `compress/internal/codes`, compress/zstd/internal/encoder.hero:9 `compress/internal/codes`, compress/zstd/internal/encoder.hero:10 `compress/internal/finder`, compress/zstd/internal/frame.hero:6 `compress/internal/codes`, compress/zstd/internal/fse.hero:6 `compress/internal/codes`, compress/zstd/internal/huff.hero:7 `compress/internal/codes`, compress/zstd/internal/literals.hero:5 `compress/internal/codes`, compress/zstd/internal/sections.hero:6 `compress/internal/huffman`, compress/zstd/internal/sequences.hero:7 `compress/internal/codes` (repaired at 56c)
- crypto/keypairs: `crypto/keypairs`, layer 2, uses `crypto`, layer 2, in 4 `use` lines: crypto/keypairs.hero:46 `crypto/hero_crypto`, crypto/keypairs/internal/failure.hero:8 `crypto/hero_crypto`, crypto/keypairs/internal/forms.hero:13 `crypto/hero_crypto`, crypto/keypairs/internal/operations.hero:9 `crypto/hero_crypto` (repaired at 56c)
- crypto/seal: `crypto/seal`, layer 2, uses `crypto`, layer 2, in 4 `use` lines: crypto/seal.hero:43 `crypto/hero_crypto`, crypto/seal/internal/failure.hero:8 `crypto/hero_crypto`, crypto/seal/internal/known.hero:10 `crypto/hero_crypto`, crypto/seal/internal/sealing.hero:7 `crypto/hero_crypto` (repaired at 56c)
- os: `os`, layer 0, uses `io`, layer 2, in 1 `use` lines: os/hero_disk.hero:21 `io/hero_stream` (kept, `OS_DISK_NAMES_STREAMS`)
- process: `process`, layer 3, uses `time`, layer 3, in 1 `use` lines: process/internal/clock.hero:5 `time/hero_time` (repaired at 56c)

### `unlisted`: 9

- math/libm.hero:16: a floor, its `extern` group binding C, and not named `hero_<x>` (§ A2) (repaired at 56e)
- tokenizer/tables: a face a program names, and no row of § A1 (repaired at 56e)
- tokenizer/tables: a face a program names, and no `tokenizer/tables.ledger` (§ A8) (repaired at 56e)
- tokenizer/tables/cl100k_base: a face a program names, and no row of § A1 (kept, `TABLES_OF_A_PACKAGE`)
- tokenizer/tables/cl100k_base: a face a program names, and no `tokenizer/tables/cl100k_base.ledger` (§ A8) (kept, `TABLES_OF_A_PACKAGE`)
- tokenizer/tables/o200k_base: a face a program names, and no row of § A1 (kept, `TABLES_OF_A_PACKAGE`)
- tokenizer/tables/o200k_base: a face a program names, and no `tokenizer/tables/o200k_base.ledger` (§ A8) (kept, `TABLES_OF_A_PACKAGE`)
- tokenizer/tables/o200k_harmony: a face a program names, and no row of § A1 (kept, `TABLES_OF_A_PACKAGE`)
- tokenizer/tables/o200k_harmony: a face a program names, and no `tokenizer/tables/o200k_harmony.ledger` (§ A8) (kept, `TABLES_OF_A_PACKAGE`)

### `bodies`: 97

- one body of 7 rows in 2 packages: amqp/internal/link.hero:75: read; kafka/internal/link.hero:75: read (kept, `NO_PUBLIC_HOME`)
- one body of 9 rows in 2 packages: amqp/internal/link.hero:98: close; kafka/internal/link.hero:98: close (kept, `NO_PUBLIC_HOME`)
- one body of 5 rows in 2 packages: archive/internal/tar_format.hero:141: clamp; tui/internal/solve.hero:144: kept (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: big/internal/limbs.hero:33: trimmed; numbers/internal/big.hero:24: trimmed (kept, `NO_PUBLIC_HOME`)
- one body of 12 rows in 3 packages: big/internal/limbs.hero:68: compare; numbers/internal/big.hero:135: compare; stats/internal/big.hero:76: compare (kept, `NO_PUBLIC_HOME`)
- one body of 10 rows in 2 packages: bytes.hero:958: compile_patterns; strings.hero:770: compile_patterns (kept, `NO_PUBLIC_HOME`)
- one body of 10 rows in 2 packages: bytes.hero:979: compile_patterns_ignoring_case_ascii; strings.hero:792: compile_patterns_ignoring_case_ascii (kept, `NO_PUBLIC_HOME`)
- one body of 8 rows in 2 packages: bytes/internal/automaton.hero:68: child; strings/internal/automaton.hero:67: child (kept, `NO_PUBLIC_HOME`)
- one body of 13 rows in 2 packages: bytes/internal/automaton.hero:82: step; strings/internal/automaton.hero:81: step (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: bytes/internal/automaton.hero:102: edge_of; strings/internal/automaton.hero:101: edge_of (kept, `NO_PUBLIC_HOME`)
- one body of 116 rows in 2 packages: bytes/internal/automaton.hero:115: build; strings/internal/automaton.hero:114: build (kept, `NO_PUBLIC_HOME`)
- one body of 5 rows in 2 packages: bytes/internal/multi.hero:54: wins; strings/internal/multi.hero:66: wins (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: bytes/internal/trimming.hero:92: trim_prefix_all; strings/internal/trimming.hero:117: trim_prefix_all (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: bytes/internal/trimming.hero:103: trim_suffix_all; strings/internal/trimming.hero:128: trim_suffix_all (kept, `NO_PUBLIC_HOME`)
- one body of 17 rows in 2 packages: cbor/internal/convert.hero:62: advance; msgpack/internal/convert.hero:62: advance (kept, `NO_PUBLIC_HOME`)
- one body of 5 rows in 2 packages: cbor/internal/convert.hero:89: pointer_at; msgpack/internal/convert.hero:89: pointer_at (kept, `NO_PUBLIC_HOME`)
- one body of 15 rows in 2 packages: cbor/internal/floats.hero:28: pow2; msgpack/internal/floats.hero:30: pow2 (kept, `NO_PUBLIC_HOME`)
- one body of 32 rows in 2 packages: cbor/internal/floats.hero:51: parts_of; msgpack/internal/floats.hero:53: parts_of (kept, `NO_PUBLIC_HOME`)
- one body of 11 rows in 2 packages: cbor/internal/floats.hero:96: value_of; msgpack/internal/floats.hero:98: value_of (kept, `NO_PUBLIC_HOME`)
- one body of 13 rows in 2 packages: cbor/internal/floats.hero:129: single_value; msgpack/internal/floats.hero:113: single_value (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: cbor/internal/floats.hero:148: double_value; msgpack/internal/floats.hero:132: double_value (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: compress.hero:549: write; compress/lz4.hero:697: write (kept, `NO_PUBLIC_HOME`)
- one body of 8 rows in 2 packages: compress.hero:652: read_rest; compress/lz4.hero:801: read_rest (kept, `NO_PUBLIC_HOME`)
- one body of 5 rows in 2 packages: compress/lz4/internal/fast.hero:37: lane; hash/internal/xxhash.hero:80: lane32 (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: compress/xz/internal/lzma2_dec.hero:17: little; compress/zstd/internal/literals.hero:20: little (repaired at 56d)
- one body of 5 rows in 2 packages: containers/cache/internal/lru.hero:221: rebuild; containers/indexed/internal/table.hero:46: rebuild (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: containers/internal/samples.hero:14: by_bytes; i18n/collate/internal/root.hero:180: strings_less; i18n/collate/internal/weighing.hero:390: bytes_less (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: containers/internal/samples.hero:34: roll; graphs/internal/samples.hero:14: roll (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 3 packages: crypto/internal/word.hero:87: bytes_of; hash/internal/word.hero:51: bytes_of; strings/internal/bounds.hero:24: to_bytes (kept, `NO_PUBLIC_HOME`)
- one body of 5 rows in 2 packages: crypto/keypairs/internal/failure.hero:58: words; crypto/seal/internal/failure.hero:58: words (kept, `NO_PUBLIC_HOME`)
- one body of 5 rows in 2 packages: crypto/keypairs/internal/failure.hero:68: last; crypto/seal/internal/failure.hero:68: last (kept, `NO_PUBLIC_HOME`)
- one body of 7 rows in 2 packages: crypto/keypairs/internal/operations.hero:110: close; crypto/seal/internal/sealing.hero:119: close (kept, `NO_PUBLIC_HOME`)
- one body of 5 rows in 2 packages: csv/internal/scan.hero:83: line_end_len; toml/internal/scalar.hero:197: line_end_len (kept, `NO_PUBLIC_HOME`)
- one body of 7 rows in 7 packages: graphql/internal/lexer.hero:395: hex_digit; grpc/internal/texts.hero:96: value_of_digit; i18n/internal/po_file.hero:100: hex_value; ldap/internal/dn.hero:37: hex_value; pdf/internal/parse.hero:39: hex_value; protobuf/internal/lexer.hero:51: hex_value; xlsx/internal/text.hero:11: hex_value (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: grpc/internal/clock.hero:18: set_wait; http2/internal/wire.hero:380: set_wait (kept, `NO_PUBLIC_HOME`)
- one body of 7 rows in 3 packages: grpc/internal/fixture.hero:25: until_listening; http/internal/h2bridge.hero:98: until_listening; http2/internal/fixture.hero:26: until_listening (kept, `TEST_FIXTURES`)
- one body of 6 rows in 2 packages: html/sanitize/internal/values.hero:23: is_integer; regex/internal/template.hero:142: all_digits (kept, `NO_PUBLIC_HOME`)
- one body of 8 rows in 2 packages: http/internal/charset.hero:67: latin1; mail/internal/charsets.hero:119: latin1 (kept, `NO_PUBLIC_HOME`)
- one body of 7 rows in 2 packages: http/internal/syntax.hero:102: trim_ows; time/internal/retry_after.hero:17: trimmed (kept, `NO_PUBLIC_HOME`)
- one body of 9 rows in 2 packages: i18n/collate/internal/root.hero:27: digit_of; i18n/dates/internal/date_data.hero:79: digit_of (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: image/gif/internal/blocks.hero:237: name_is; openapi/internal/shape.hero:559: strings_begin (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: json.hero:419: format_indented; xml.hero:398: format_indented (kept, `NO_PUBLIC_HOME`)
- one body of 15 rows in 2 packages: json/internal/scan.hero:60: position; yaml/internal/source.hero:144: position (kept, `NO_PUBLIC_HOME`)
- one body of 14 rows in 2 packages: json/jq/internal/dates.hero:12: month_names; time/internal/forms.hero:50: month_names (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 3 packages: json/jq/internal/lexer.hero:174: holds_at; openapi/internal/routes.hero:148: has_at; web/internal/sniffing.hero:112: holds_at (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 3 packages: json/jq/internal/text.hero:170: begins; term/internal/words.hero:107: begins_with; web/internal/sniffing.hero:101: begins (kept, `NO_PUBLIC_HOME`)
- one body of 54 rows in 3 packages: json/patch/internal/same.hero:11: equal; json/path/internal/same.hero:11: equal; json/schema/internal/same.hero:11: equal (repaired at 56d)
- one body of 5 rows in 2 packages: json/path/internal/edit.hero:56: depth_of; json/schema/internal/ranking.hero:6: depth_of (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 6 packages: keyring/internal/names.hero:22: has_zero; os/internal/codes.hero:101: has_zero; serial/internal/calls.hero:29: holds_zero; sql/mysql/internal/wire.hero:112: has_zero; sql/postgres/internal/wire.hero:89: has_zero; tls/internal/text.hero:11: has_zero (kept, `NO_PUBLIC_HOME`)
- one body of 12 rows in 2 packages: ldap/internal/fake.hero:224: until_listening; mail/smtp/internal/fake.hero:147: until_listening (kept, `TEST_FIXTURES`)
- one body of 9 rows in 2 packages: ldap/internal/fake.hero:242: listening; mail/smtp/internal/fake.hero:165: listening (kept, `TEST_FIXTURES`)
- one body of 5 rows in 2 packages: llm/anthropic/internal/canned.hero:28: json_answer; llm/openai/internal/canned.hero:28: json_answer (kept, `TEST_FIXTURES`)
- one body of 6 rows in 2 packages: llm/anthropic/internal/canned.hero:43: events; llm/openai/internal/canned.hero:43: events (kept, `TEST_FIXTURES`)
- one body of 8 rows in 2 packages: llm/anthropic/internal/canned.hero:55: start; llm/openai/internal/canned.hero:55: start (kept, `TEST_FIXTURES`)
- one body of 30 rows in 2 packages: llm/anthropic/internal/canned.hero:78: serve; llm/openai/internal/canned.hero:78: serve (kept, `TEST_FIXTURES`)
- one body of 7 rows in 2 packages: llm/anthropic/internal/failures.hero:83: word; llm/openai/internal/failures.hero:82: word (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: llm/anthropic/internal/failures.hero:125: cut; llm/openai/internal/failures.hero:124: cut (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: llm/anthropic/internal/fields.hero:11: text_at; llm/openai/internal/fields.hero:11: text_at (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: llm/anthropic/internal/fields.hero:53: object; llm/openai/internal/fields.hero:53: object (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: llm/anthropic/internal/fields.hero:65: with_member; llm/openai/internal/fields.hero:65: with_member (kept, `NO_PUBLIC_HOME`)
- one body of 9 rows in 2 packages: llm/anthropic/internal/fields.hero:94: only; llm/openai/internal/fields.hero:84: only (kept, `NO_PUBLIC_HOME`)
- one body of 7 rows in 2 packages: llm/anthropic/internal/transport.hero:92: request_of; llm/openai/internal/transport.hero:92: request_of (kept, `NO_PUBLIC_HOME`)
- one body of 10 rows in 2 packages: llm/anthropic/internal/transport.hero:202: read_body; llm/openai/internal/transport.hero:204: read_body (kept, `NO_PUBLIC_HOME`)
- one body of 11 rows in 2 packages: llm/anthropic/internal/transport.hero:220: attempt_open; llm/openai/internal/transport.hero:222: attempt_open (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: llm/anthropic/internal/transport.hero:237: headed; llm/openai/internal/transport.hero:239: headed (kept, `NO_PUBLIC_HOME`)
- one body of 7 rows in 2 packages: mail/internal/params.hero:291: hex_value; mail/internal/transfer.hero:116: hex_value; mail/internal/words.hero:166: hex_value; sql/postgres/internal/target.hero:237: hex_of (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: mail/internal/write.hero:274: is_ascii; mail/smtp/internal/protocol.hero:146: is_ascii (kept, `NO_PUBLIC_HOME`)
- one body of 5 rows in 2 packages: markdown/internal/html_reading.hero:172: put_work; web/internal/tree.hero:98: put (kept, `NO_PUBLIC_HOME`)
- one body of 7 rows in 2 packages: metrics.hero:1700: otlp_to; tracing.hero:872: otlp_to (kept, `NO_PUBLIC_HOME`)
- one body of 7 rows in 2 packages: metrics/internal/otlp.hero:71: any_boolean; tracing/internal/otlp.hero:102: any_boolean (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: metrics/internal/otlp.hero:103: any_array; tracing/internal/otlp.hero:134: any_array (kept, `NO_PUBLIC_HOME`)
- one body of 21 rows in 2 packages: metrics/internal/otlp.hero:156: partial_of; tracing/internal/otlp.hero:187: partial_of (kept, `NO_PUBLIC_HOME`)
- one body of 12 rows in 2 packages: metrics/internal/otlp.hero:187: status_message; tracing/internal/otlp.hero:218: status_message (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: metrics/internal/posting.hero:87: env_text; tracing/internal/posting.hero:29: env_text (kept, `NO_PUBLIC_HOME`)
- one body of 12 rows in 2 packages: metrics/internal/posting.hero:104: env_headers; tracing/internal/posting.hero:46: env_headers (kept, `NO_PUBLIC_HOME`)
- one body of 11 rows in 2 packages: metrics/internal/posting.hero:134: retry_delay; tracing/internal/posting.hero:76: retry_delay (kept, `NO_PUBLIC_HOME`)
- one body of 27 rows in 2 packages: metrics/internal/posting.hero:152: post; tracing/internal/posting.hero:94: post (kept, `NO_PUBLIC_HOME`)
- one body of 5 rows in 2 packages: mqtt/internal/link.hero:85: is_secured; nats/internal/link.hero:118: is_secured (kept, `NO_PUBLIC_HOME`)
- one body of 5 rows in 2 packages: mqtt/internal/link.hero:97: arm; nats/internal/link.hero:130: arm (kept, `NO_PUBLIC_HOME`)
- one body of 12 rows in 2 packages: mqtt/internal/link.hero:111: read; nats/internal/link.hero:144: read (kept, `NO_PUBLIC_HOME`)
- one body of 9 rows in 2 packages: mqtt/internal/link.hero:130: write; nats/internal/link.hero:163: write (kept, `NO_PUBLIC_HOME`)
- one body of 12 rows in 2 packages: mqtt/internal/link.hero:144: close; nats/internal/link.hero:177: close (kept, `NO_PUBLIC_HOME`)
- one body of 5 rows in 2 packages: mqtt/internal/link.hero:170: stream_of; nats/internal/link.hero:203: stream_of (kept, `NO_PUBLIC_HOME`)
- one body of 7 rows in 2 packages: openapi/internal/shape.hero:375: found_of; template/internal/values.hero:19: kind_named (kept, `NO_PUBLIC_HOME`)
- one body of 5 rows in 2 packages: openapi/internal/shape.hero:385: push_item; url/internal/paths.hero:15: put (kept, `NO_PUBLIC_HOME`)
- one body of 5 rows in 2 packages: path/internal/relation.hero:214: length_order; semver/internal/order.hero:20: compare_numbers (kept, `NO_PUBLIC_HOME`)
- one body of 7 rows in 2 packages: qr/internal/bits.hero:296: char_width; url/internal/classes.hero:119: char_width (kept, `NO_PUBLIC_HOME`)
- one body of 7 rows in 2 packages: redis/internal/session.hero:110: send; sql/postgres/internal/session.hero:81: send (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: search/internal/store.hero:98: need; vectors/internal/store.hero:99: need (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: search/internal/store.hero:108: take_low; vectors/internal/store.hero:109: take_low (kept, `NO_PUBLIC_HOME`)
- one body of 10 rows in 2 packages: sql/mysql.hero:157: options; sql/postgres.hero:173: options (kept, `NO_PUBLIC_HOME`)
- one body of 5 rows in 2 packages: sql/mysql/internal/script.hero:46: finish; sql/postgres/internal/script.hero:41: finish (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: sql/mysql/internal/wire.hero:90: push_cstr; sql/postgres/internal/wire.hero:67: push_cstr (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: sql/odbc/internal/values.hero:17: zero_at; sql/sqlite/internal/values.hero:42: zero_at (kept, `NO_PUBLIC_HOME`)
- one body of 6 rows in 2 packages: template/internal/lex.hero:72: holds_at; xml/internal/markup.hero:33: holds_at (kept, `NO_PUBLIC_HOME`)
- one body of 9 rows in 5 packages: time/internal/cron_next.hero:96: changes_up_to; unicode/internal/search.hero:11: count_at_or_below; unicode/names/internal/naming.hero:28: count_at_or_below; unicode/normalize/internal/lookup.hero:70: count_at_or_below; unicode/segment/internal/text.hero:19: count_at_or_below (kept, `NO_PUBLIC_HOME`)
- one body of 7 rows in 2 packages: unicode/internal/utf8.hero:48: len_of; unicode/segment/internal/text.hero:44: utf8_len (kept, `NO_PUBLIC_HOME`)

### `one-layer`: 28

- `amqp` and `kafka`, both of layer 5, so neither may use the other, hold 2 bodies and 0 bindings in common: the body of amqp/internal/link.read = kafka/internal/link.read; the body of amqp/internal/link.close = kafka/internal/link.close (kept, `ONE_LAYER`)
- `bytes` and `strings`, both of layer 1, so neither may use the other, hold 9 bodies and 0 bindings in common: the body of bytes.compile_patterns = strings.compile_patterns; the body of bytes.compile_patterns_ignoring_case_ascii = strings.compile_patterns_ignoring_case_ascii; the body of bytes/internal/automaton.child = strings/internal/automaton.child; the body of bytes/internal/automaton.step = strings/internal/automaton.step; the body of bytes/internal/automaton.edge_of = strings/internal/automaton.edge_of; the body of bytes/internal/automaton.build = strings/internal/automaton.build; the body of bytes/internal/multi.wins = strings/internal/multi.wins; the body of bytes/internal/trimming.trim_prefix_all = strings/internal/trimming.trim_prefix_all; the body of bytes/internal/trimming.trim_suffix_all = strings/internal/trimming.trim_suffix_all (kept, `ONE_LAYER`)
- `compress` and `compress/lz4`, both of layer 3, so neither may use the other, hold 2 bodies and 0 bindings in common: the body of compress.write = compress/lz4.write; the body of compress.read_rest = compress/lz4.read_rest (parted at 56c)
- `containers/cache` and `containers/indexed`, both of layer 0, so neither may use the other, hold 1 bodies and 0 bindings in common: the body of containers/cache/internal/lru.rebuild = containers/indexed/internal/table.rebuild (kept, `ONE_LAYER`)
- `crypto/keypairs` and `crypto/seal`, both of layer 2, so neither may use the other, hold 3 bodies and 0 bindings in common: the body of crypto/keypairs/internal/failure.words = crypto/seal/internal/failure.words; the body of crypto/keypairs/internal/failure.last = crypto/seal/internal/failure.last; the body of crypto/keypairs/internal/operations.close = crypto/seal/internal/sealing.close (kept, `ONE_LAYER`)
- `fs` and `ipc`, both of layer 4, so neither may use the other, hold 0 bodies and 2 bindings in common: the binding of hero_os_pid of hero_os.h in fs/internal/fixture and ipc/hero_ipc; the binding of hero_run_exe_suffix of hero_os.h in fs/internal/fixture and ipc/hero_ipc (parted at 56c)
- `grpc` and `http`, both of layer 6, so neither may use the other, hold 1 bodies and 0 bindings in common: the body of grpc/internal/fixture.until_listening = http/internal/h2bridge.until_listening (kept, `ONE_LAYER`)
- `grpc` and `xlsx`, both of layer 6, so neither may use the other, hold 1 bodies and 0 bindings in common: the body of grpc/internal/texts.value_of_digit = xlsx/internal/text.hex_value (kept, `ONE_LAYER`)
- `json` and `xml`, both of layer 2, so neither may use the other, hold 1 bodies and 0 bindings in common: the body of json.format_indented = xml.format_indented (kept, `ONE_LAYER`)
- `json/jq` and `term`, both of layer 3, so neither may use the other, hold 1 bodies and 0 bindings in common: the body of json/jq/internal/text.begins = term/internal/words.begins_with (kept, `ONE_LAYER`)
- `json/jq` and `time`, both of layer 3, so neither may use the other, hold 1 bodies and 0 bindings in common: the body of json/jq/internal/dates.month_names = time/internal/forms.month_names (kept, `ONE_LAYER`)
- `json/patch` and `json/path`, both of layer 3, so neither may use the other, hold 1 bodies and 0 bindings in common: the body of json/patch/internal/same.equal = json/path/internal/same.equal (repaired at 56d)
- `json/patch` and `json/schema`, both of layer 3, so neither may use the other, hold 1 bodies and 0 bindings in common: the body of json/patch/internal/same.equal = json/schema/internal/same.equal (repaired at 56d)
- `json/path` and `json/schema`, both of layer 3, so neither may use the other, hold 2 bodies and 0 bindings in common: the body of json/path/internal/same.equal = json/schema/internal/same.equal; the body of json/path/internal/edit.depth_of = json/schema/internal/ranking.depth_of (kept, `ONE_LAYER`)
- `ldap` and `mail/smtp`, both of layer 5, so neither may use the other, hold 2 bodies and 0 bindings in common: the body of ldap/internal/fake.until_listening = mail/smtp/internal/fake.until_listening; the body of ldap/internal/fake.listening = mail/smtp/internal/fake.listening (kept, `ONE_LAYER`)
- `ldap` and `pdf`, both of layer 5, so neither may use the other, hold 1 bodies and 0 bindings in common: the body of ldap/internal/dn.hex_value = pdf/internal/parse.hex_value (kept, `ONE_LAYER`)
- `llm/anthropic` and `llm/openai`, both of layer 7, so neither may use the other, hold 14 bodies and 0 bindings in common: the body of llm/anthropic/internal/canned.json_answer = llm/openai/internal/canned.json_answer; the body of llm/anthropic/internal/canned.events = llm/openai/internal/canned.events; the body of llm/anthropic/internal/canned.start = llm/openai/internal/canned.start; the body of llm/anthropic/internal/canned.serve = llm/openai/internal/canned.serve; the body of llm/anthropic/internal/failures.word = llm/openai/internal/failures.word; the body of llm/anthropic/internal/failures.cut = llm/openai/internal/failures.cut; the body of llm/anthropic/internal/fields.text_at = llm/openai/internal/fields.text_at; the body of llm/anthropic/internal/fields.object = llm/openai/internal/fields.object; the body of llm/anthropic/internal/fields.with_member = llm/openai/internal/fields.with_member; the body of llm/anthropic/internal/fields.only = llm/openai/internal/fields.only; the body of llm/anthropic/internal/transport.request_of = llm/openai/internal/transport.request_of; the body of llm/anthropic/internal/transport.read_body = llm/openai/internal/transport.read_body; the body of llm/anthropic/internal/transport.attempt_open = llm/openai/internal/transport.attempt_open; the body of llm/anthropic/internal/transport.headed = llm/openai/internal/transport.headed (kept, `ONE_LAYER`)
- `metrics` and `tracing`, both of layer 7, so neither may use the other, hold 9 bodies and 0 bindings in common: the body of metrics.otlp_to = tracing.otlp_to; the body of metrics/internal/otlp.any_boolean = tracing/internal/otlp.any_boolean; the body of metrics/internal/otlp.any_array = tracing/internal/otlp.any_array; the body of metrics/internal/otlp.partial_of = tracing/internal/otlp.partial_of; the body of metrics/internal/otlp.status_message = tracing/internal/otlp.status_message; the body of metrics/internal/posting.env_text = tracing/internal/posting.env_text; the body of metrics/internal/posting.env_headers = tracing/internal/posting.env_headers; the body of metrics/internal/posting.retry_delay = tracing/internal/posting.retry_delay; the body of metrics/internal/posting.post = tracing/internal/posting.post (kept, `ONE_LAYER`)
- `mqtt` and `nats`, both of layer 8, so neither may use the other, hold 6 bodies and 0 bindings in common: the body of mqtt/internal/link.is_secured = nats/internal/link.is_secured; the body of mqtt/internal/link.arm = nats/internal/link.arm; the body of mqtt/internal/link.read = nats/internal/link.read; the body of mqtt/internal/link.write = nats/internal/link.write; the body of mqtt/internal/link.close = nats/internal/link.close; the body of mqtt/internal/link.stream_of = nats/internal/link.stream_of (kept, `ONE_LAYER`)
- `net` and `process`, both of layer 3, so neither may use the other, hold 0 bodies and 4 bindings in common: the binding of hero_net_close of hero_net.h in net/hero_net and process/hero_proc; the binding of hero_net_udp of hero_net.h in net/hero_net and process/hero_proc; the binding of hero_proc_id of hero_proc.h in net/hero_stream_net and process/hero_proc; the binding of hero_proc_last_error of hero_proc.h in net/hero_stream_net and process/hero_proc (parted at 56c)
- `os` and `path`, both of layer 0, so neither may use the other, hold 0 bodies and 2 bindings in common: the binding of hero_run_exe_suffix of hero_os.h in os/hero_disk and path/hero_os; the binding of hero_run_exe_suffix of hero_os.h in os/hero_os and path/hero_os (kept, `ONE_LAYER`)
- `redis` and `sql/postgres`, both of layer 5, so neither may use the other, hold 1 bodies and 0 bindings in common: the body of redis/internal/session.send = sql/postgres/internal/session.send (kept, `ONE_LAYER`)
- `search` and `vectors`, both of layer 2, so neither may use the other, hold 2 bodies and 0 bindings in common: the body of search/internal/store.need = vectors/internal/store.need; the body of search/internal/store.take_low = vectors/internal/store.take_low (kept, `ONE_LAYER`)
- `sql/mysql` and `sql/postgres`, both of layer 5, so neither may use the other, hold 4 bodies and 0 bindings in common: the body of sql/mysql/internal/wire.has_zero = sql/postgres/internal/wire.has_zero; the body of sql/mysql.options = sql/postgres.options; the body of sql/mysql/internal/script.finish = sql/postgres/internal/script.finish; the body of sql/mysql/internal/wire.push_cstr = sql/postgres/internal/wire.push_cstr (kept, `ONE_LAYER`)
- `sql/odbc` and `sql/sqlite`, both of layer 5, so neither may use the other, hold 1 bodies and 0 bindings in common: the body of sql/odbc/internal/values.zero_at = sql/sqlite/internal/values.zero_at (kept, `ONE_LAYER`)
- `unicode/names` and `unicode/normalize`, both of layer 1, so neither may use the other, hold 1 bodies and 0 bindings in common: the body of unicode/names/internal/naming.count_at_or_below = unicode/normalize/internal/lookup.count_at_or_below (kept, `ONE_LAYER`)
- `unicode/names` and `unicode/segment`, both of layer 1, so neither may use the other, hold 1 bodies and 0 bindings in common: the body of unicode/names/internal/naming.count_at_or_below = unicode/segment/internal/text.count_at_or_below (kept, `ONE_LAYER`)
- `unicode/normalize` and `unicode/segment`, both of layer 1, so neither may use the other, hold 1 bodies and 0 bindings in common: the body of unicode/normalize/internal/lookup.count_at_or_below = unicode/segment/internal/text.count_at_or_below (kept, `ONE_LAYER`)

## What moved

- **Step 56a**, `ac2c77fb2`: the instruments, the `rebel` suite, the readout, and `rebel_kept.hero` keeping all 282 findings of the base with their reasons.
- **Step 56b**, `08ae50aca`: `codes`, 4: `numbers/internal/quick`, `side` and `fewest` failed `undecided` where a fast route cannot decide, now `unsupported`; the failure never leaves `numbers`. `sql/sqlite/hero_sqlite3`'s `exec` held `fail(code: "none")` in the cell SQLite writes its words into, now `failed`.
- **Step 56c**, `776e5b529`: `layers`, 8, by § A1's table alone: `compress/zstd`, `brotli`, `bzip2` (2) and `lz4`, `xz` (3) use `compress`'s private modules, now layer 4; `crypto/seal` and `crypto/keypairs` (2) use the floor `crypto/hero_crypto`, now 3; `process` (3) uses `time/hero_time`, now 4, and `ipc`, its one user, 5. The moves parted three pairs of one layer (`compress` and `compress/lz4`, `net` and `process`, `fs` and `ipc`) and made two (`compress/xz` and `compress/zstd`, repaired at 56d; `fs` and `process`, kept).
- **Step 56d**, `1ba902167`: `bodies`, 2, and `one-layer`, 3: JSON equality, 54 rows written three times in `json/patch`, `json/path` and `json/schema`, is `json.equal(a:, b:)`, a function of `json`'s face with its doc, example and ledger block `json/equal`, and the three modules are gone; `little`, written twice in `compress/xz` and `compress/zstd`, is `compress/internal/cursor`'s.
- **Step 56e**, `48ee3ffca`: `unlisted`, 3: `math/libm.hero`, a floor, is `math/hero_math.hero`; `tokenizer/tables` has its row of § A1 (layer 3) and its own ledger, the three blocks `tokenizer.ledger` held for it moved whole.

What a caller sees changed in one place, which `json.ledger` says: `json` offers `equal`. Every other repair moves no name a program writes.

## What is kept, and why

The 261 findings at the head, each marked above with its reason, a constant of `tests/harness/rebel_kept.hero`; and the two the repairs made, both of `one-layer`: `compress/xz & compress/zstd`, repaired at 56d, and `fs & process`, kept under `ONE_LAYER` (the binding of `hero_dir_remove_tree`, `hero_fs_mkdir_all`, `hero_os_pid` and `hero_run_exe_suffix` of `hero_os.h` in `fs/internal/fixture` and `process/hero_proc`).

- `NO_PUBLIC_HOME`, 88: one helper in packages that may not share a private module (§ A2), and no package below them offers it publicly: made one only by giving it a public home, which is the readers' choice
- `PARAMETER_LABELS`, 37: a parameter's name is a label a caller writes where two parameters share a type (spec § 9), so the rename reaches callers in other packages and the examples: no local repair; kept for the readers, who choose the word (`ch` and `re` are the areas' habit)
- `TWO_MEANINGS`, 26: one word, two meanings: what each says it does differs (the finding quotes both), so neither answers the other's question; the readers decide whether one of the names should change
- `ONE_LAYER`, 24: two packages of one layer may not use each other (§ A1), so what they share is written twice until a package below both holds it publicly: the readers'
- `A_SPLIT_IS_NOT_LOCAL`, 11: a split moves names every program that uses the package writes, so it is no local repair; kept for the readers, who weigh it against the reference libraries' own sizes
- `TWO_LLM_CLIENTS`, 10: `llm/anthropic` and `llm/openai` are two clients of two APIs built to answer the same questions, each through its own types, by design (§ A1's two rows)
- `BYTES_NAMED_FORMAT`, 8: the area's own word for writing a value out, DER and raw keys and a message being bytes; § A3's `format_x` says text, and a rename (`encode_x`) moves names callers write: the readers'
- `CHARACTER_AND_TEXT`, 8: `unicode` asks of one character and `strings` of a text: one meaning at two sizes, which the parameter's name says (`c`, `s`), as § A3's one name, one meaning asks
- `TEST_FIXTURES`, 7: a fixture of each package's own tests (a canned server, a loopback listener) that no program reaches; one home for test fixtures is the readers' question
- `TABLES_OF_A_PACKAGE`, 6: a table of `tokenizer/tables`, data a program names alone so that it carries the one it uses; as a package of its own it would stand below `tokenizer/tables` and use that package's private modules (`made`, its data), which § A1's layers refuse, so its row and ledger are the readers' question: one package with four faces, or the private modules moved under each table
- `EACH_ITS_GRAMMAR`, 5: each answers for its own grammar or format (JSON, jq, JSONPath, SemVer, YAML; MQTT's and NATS's subjects; HTTP's and PDF's dates): one meaning, by design
- `ONE_API_BY_DESIGN`, 5: the one shape every member of a family keeps, so a program moves from one to another by its `use` line: each compression format, each image format, each tokenizer table a subpackage of its own so a program carries the one it uses (§ A2)
- `ONE_DOMAIN_OF_TYPES`, 5: `math/linear`'s types are combined by its functions (`mul_mat4_vec4`), one domain a split would make use itself across packages; without overloading the type's name stands in each function's name
- `CONTAINERS_SHARED_MODULES`, 3: the private modules the eight `containers/` subpackages share, left in the folder no row names by the split of step 9 (`86dd93d9e`); a row for `containers` would name no face, so where they belong is the readers' question
- `TWO_SHAPES_OF_VECTOR`, 3: `math/linear` holds vectors of two to four `f64` for geometry and `vectors` lists of `f32` of any length for embeddings: one operation on two shapes, which a language without overloading writes twice; the readers decide whether the ledgers should name each other
- `FORMATS_OF_ONE_VALUE`, 2: each format read into or written from the one `json.Value`, by design (`yaml`'s row of § A1)
- `REQUEST_FIRST`, 2: `web`'s middleware takes the request first and the response it changes second, as every handler of the package; a reorder moves every call, and is the readers' question
- `TEMPLATE_MAY_USE_HTML`, 2: `template` (layer 3) may use `html` (layer 2), so its own escaping could be `html`'s: removing two names from `template`'s face changes what its callers write, and is the readers' question
- `THREE_EXACT_NUMBERS`, 2: `big` holds three exact numbers, `Integer`, `Decimal` and `Fraction`, whose conversions name two at once (`decimal_of_fraction`): a split into subpackages is the readers' question, and moves names callers write
- `COMPLEX_IN_MATH`, 1: § A1's row puts complex numbers in `math` (C11 Annex G); a `math/complex` subpackage, the containers smell's strongest case today, is the readers' question, and moves 30 names callers write
- `EACH_ITS_SERVICE`, 1: each client reads its own service's variables from the environment, by design
- `EACH_ITS_STREAM`, 1: each sets the deadline of the streams its own package makes, which `io` leaves to the package that made the stream (`io.hero`'s doc); the readers ask whether `io` should own the word
- `NAMES_HTTPS`, 1: `web.ledger` names `http.respond_redirect` and adds `web.path_for`, a route's path: one capability, `http`'s, counted for `web` by the helper beside it
- `OS_DISK_NAMES_STREAMS`, 1: `os/hero_disk`, the floor of `hero_disk.h`, names `hero_stream.Stream` for its two lock calls; moving those two to a floor of `io` frees `os`, and moves what `fs` calls: no local repair, kept for the readers
- `PLAIN_AND_LOCALE`, 1: `numbers.format_percent` writes one fixed form and `i18n/numbers.format_percent` the form a language writes: whether the plain one is the root locale's is the readers' question, and either repair moves a name callers write
- `RETRIES_OF_THEIR_OWN`, 1: `http.Retries` is the general policy, and the two LLM clients wait what their server asks; whether they should hold an `http.Retries` is the readers' question

## For the readers

What the instruments cannot decide and the review did not, the strongest first by what each would move: `math`'s 30 functions of `Complex` (`prefixed`, and `math` the largest face at 155), a `math/complex` like the split of `containers`; `template.escape_html` and `unescape_html` beside `html`'s, which `template` may use; the 88 bodies with no public home below their packages, the largest the 116-row Aho-Corasick builder of `bytes` and `strings` (one layer, so neither may use the other) and `cbor`'s and `msgpack`'s floats, which `numbers`' public `f64_to_bits`, `bits_to_f32` and `f16_bits_to_f64` might carry, a question rather than a premise, unrun; `os`'s floor of `hero_disk.h` naming `io`'s stream; the private modules `containers/` keeps under no row; the three tables of `tokenizer/tables`; and the 37 abbreviated parameters, a rename each caller labels.

## How to run it again

`heroes run tests/harness/main.hero -- <compiler> rebel` judges, and `heroes run tests/harness/rebel_review.hero` prints every finding, kept or not, with the key a new one is kept under. A reader who repairs a finding takes its line out of `rebel_kept.hero` in the same commit, as the steps above did, and appends here, under this record, what moved and the day.

