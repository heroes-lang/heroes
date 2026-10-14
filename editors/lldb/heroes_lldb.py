"""lldb shows a stopped Heroes program's values in Heroes' own literal syntax.

WHAT IT IS. One fixed formatter, loaded into lldb with
`command script import <checkout>/editors/lldb/heroes_lldb.py`: it registers
summaries and synthetic children for the C types the Heroes runtime and
emitter make, so a frame that showed `h2_p = (f_x = 3, f_y = 4)` and
`h4_names = 0x00000001005f98a0` shows `Point(x: 3, y: 4)` and
`["ada", "grace"]`. It is a component the debugger loads, not a capability
of `heroes`: panel 213's resolution (routes G and J, ratified by the author on
2026-10-14) rules that CLAUDE.md § 10's *never a script* governs how
a capability of the one command is reached, and this file is reached through
lldb's prompt or the editor. `editors/vscode/extension.js` loads it with
nothing typed, through lldb-dap's `initCommands`; a terminal lldb user types
the import, or puts it in their own `~/.lldbinit` (`editors/README.md`
§ Debugging). It retires the half of design.md Part 2's *No typed variable
inspection in v1* that a formatter can reach; the names stay the emitted C's
(design.md Part 8 item 12), which no formatter changes.

WHAT IT SHOWS (spec § 2, § 3, § 6, § 9, § 10):

- a record as its construction, `Point(x: 3, y: 4)`, with children `x`, `y`;
- a `str` as a string literal read by its `len`, never to its NUL, with the
  spec's escapes (`\\n \\t \\r \\\\ \\"`) and `\\u{1b}` for a character a line
  cannot show; a NUL inside it as `\\u{0}`, the code escape's spelling of a
  character no literal may hold (spec § 2);
- `[T]` as `[e0, e1, ...]` with children `[0]`, `[1]`, ...;
- `{K: V}` as `{k: v, ...}` with children named by their keys, `["ada"]`,
  in bucket order, the order `keys(m)` gives (spec § 10: no order);
- `T?` as `ok(v)` or `fail(code: "...", msg: "...")`, the live arm alone;
- a variant as `.case(field: v)`, or `.case` for a case with no fields, and a
  `match` arm's payload binding the same way;
- a slot holding the runtime's one non-value, a NULL `str`, `[T]` or `{K: V}`
  (`runtime/heroes_runtime.h`: *an unassigned or moved-out slot*), as
  `<unassigned>`, the word of the runtime's own panic for reading one.

A container's summary writes at most `SHOWN` elements, then `... N more`;
its children stop at `CHILDREN`; a `str`'s summary reads at most
`TEXT_BYTES`, then `... N more bytes`.

WHAT IT RESTS ON, AND HOW IT STEPS ASIDE (design.md §1.12, robustness, applied
to the tool: the moment a debugger is used is the moment memory may be wrong,
and lldb-dap asks for every slot of a frame, written or not; panel 213's
ffi-pragmatist saw sixteen tracebacks on unwritten slots):

- **every offset and size is the binary's own debug information**:
  `GetChildMemberWithName`, `GetByteSize`, `GetOffsetInBytes`. The two layout
  facts no struct declares are the runtime's documented ones
  (design.md §4.20): an array's elements follow its header in-line, and a
  `str`'s block header sits immediately before its bytes;
- **an element's type is named by its descriptor**, the `elem`, `key` or
  `val` pointer's symbol (`hero_desc_str` is `HeroStr`, `h_m_Point_desc` is
  `h_m_Point`), the pointer exactly at the symbol's start; else by the
  DWARF static variable at that address (a stripped binary with a dSYM keeps
  that and loses the symbol); and the type's size must equal the
  descriptor's own `size`. **Where neither resolves, the container shows its
  address, never a guess**;
- **a header nobody wrote cannot make the debugger read gigabytes**:
  `0 <= len <= cap`, the elements' bytes at most `SPAN`, the last element's
  last byte readable, the count `-1` (a constant's static block) or at least
  1; a map's `len < cap`, `cap` a power of two, its regions in order, every
  state byte 0 or 1 and, where `cap <= SCAN`, as many live buckets as `len`;
  a `str` non-negative in length, its block's magic word one of the
  runtime's two, its byte at `len` the NUL;
- **`f_` is taken off only in a type the emitter made**, an `h_` name whose
  every member opens with `f_`, never in a C struct (`sys/mount.h` declares
  `f_bsize`); a `tag` must be one of its enum's enumerators and a `T?`'s 0 or
  1, or the value is not read;
- **any failure falls back to lldb's own display, and nothing reaches the
  console**: every entry point lldb calls catches every exception, a summary
  answering `""` (lldb then prints the C members) or, for a pointer, the
  address, and a provider answering the C members unchanged;
- **it is keyed on the runtime's C type names**, so a renamed runtime type is
  matched by nothing and lldb shows it as C; a renamed member makes the
  formatter step aside the same way.

Python as Xcode's lldb runs it (3.9) and Ubuntu 24.04's lldb 18: no `match`,
no `X | Y` in annotations, nothing newer than 3.9's standard library.
Its instrument is the `lldb` suite's cases that import it from the tree,
`tests/golden/lldb/` (`tests/harness/suite_lldb.hero`).
"""

import re
import struct

import lldb

CATEGORY = "heroes"

## The bounds, each a fixed number, so a header nobody wrote costs a bounded read

# Elements or entries one summary writes before `... N more`.
SHOWN = 16

# Children a container offers; lldb-dap pages them, `frame variable` stops at
# its own `target.max-children-count`.
CHILDREN = 1 << 16

# Bytes of a `str` one summary reads and writes.
TEXT_BYTES = 1024

# Values one summary writes in all, nested ones included, and how deep it goes:
# sixteen elements of sixteen elements of sixteen would otherwise be 4096 reads.
BUDGET = 256
DEPTH = 6

# Bytes a header may claim for its elements: past it the header is garbage, no
# block of a stopped process being a terabyte.
SPAN = 1 << 40

# Buckets of a map read to count its live entries against its `len`.
SCAN = 1 << 16

# How state bytes are read when a map's children are asked for.
CHUNK = 1 << 16

## The runtime's names (runtime/heroes_runtime.h), the formatter's whole key

STR = "HeroStr"
STR_HEADER = "HeroStrHeader"
ARRAY = "HeroArrayHeader"
MAP = "HeroMapHeader"
FAILURE = "HeroFailure"
DESC = "HeroDesc"

# A str block's magic word, plain and with its low bit set where the bytes
# hold a NUL (`HERO_STR_MAGIC`, `HERO_STR_MAGIC_NUL`). A macro, so no debug
# information carries it; a runtime that changed it makes every str fall back
# to lldb's display, and the `lldb` suite's cases go red.
STR_MAGICS = (0x4845524F53535452, 0x4845524F53535453)

# A static block's count (`HERO_STR_STATIC`, `HERO_ARRAY_STATIC`): -1.
STATIC_COUNT = -1

UNASSIGNED = "<unassigned>"

# The descriptors the runtime ships, and the C type each describes.
SCALAR_DESCS = {
    "hero_desc_int": "int64_t",
    "hero_desc_i8": "int8_t",
    "hero_desc_i16": "int16_t",
    "hero_desc_i32": "int32_t",
    "hero_desc_u8": "uint8_t",
    "hero_desc_u16": "uint16_t",
    "hero_desc_u32": "uint32_t",
    "hero_desc_u64": "uint64_t",
    "hero_desc_f32": "float",
    "hero_desc_f64": "double",
    "hero_desc_bool": "_Bool",
}
RUNTIME_DESCS = {
    "hero_desc_str": STR,
    "hero_desc_array": ARRAY + " *",
    "hero_desc_map": MAP + " *",
    "hero_desc_failure": FAILURE,
}
FUNC_DESC = "hero_desc_func"

# A function value's C type, as the runtime names it (`runtime/parts/desc.c`,
# `sizeof(HeroFn)`): a bare function pointer, no closures in v1.
FUNC = "HeroFn"

# The basic type each scalar falls back to where the binary's DWARF names none.
BASIC = {
    "int64_t": lldb.eBasicTypeLongLong,
    "int8_t": lldb.eBasicTypeSignedChar,
    "int16_t": lldb.eBasicTypeShort,
    "int32_t": lldb.eBasicTypeInt,
    "uint8_t": lldb.eBasicTypeUnsignedChar,
    "uint16_t": lldb.eBasicTypeUnsignedShort,
    "uint32_t": lldb.eBasicTypeUnsignedInt,
    "uint64_t": lldb.eBasicTypeUnsignedLongLong,
    "float": lldb.eBasicTypeFloat,
    "double": lldb.eBasicTypeDouble,
    "_Bool": lldb.eBasicTypeBool,
}

# A type the emitter made: `h_<module>_<name>` with the module alphanumeric
# (selfhost/emit/mangle.hero), or a `T?`, `h_0opt_<hash>`. The other `h_0`
# names, a function type `h_0fn_` and a thunk `h_0cb_`, are left to lldb.
EMITTED = r"^h_(0opt_[0-9a-f]+|[A-Za-z][A-Za-z0-9]*_[A-Za-z0-9_]+)$"
EMITTED_RE = re.compile(EMITTED)
OPTION_RE = re.compile(r"^h_0opt_[0-9a-f]+$")
FUNCTION_TYPE_RE = re.compile(r"^h_0fn_[0-9a-f]+$")
MODULE_RE = re.compile(r"^h_([A-Za-z][A-Za-z0-9]*)_(.+)$")
FUNCTION_RE = re.compile(r"^h_([A-Za-z][A-Za-z0-9]*)_([A-Za-z_][A-Za-z0-9_]*)$")

# The characters a string writes by their code (selfhost/raw_chars.hero): the
# control characters, the twelve bidirectional controls, U+2028 and U+2029.
ESCAPES = {"\n": "\\n", "\t": "\\t", "\r": "\\r", "\\": "\\\\", '"': '\\"'}
BIDI = frozenset([1564, 8206, 8207, 8234, 8235, 8236, 8237, 8238, 8294, 8295, 8296, 8297])
SEPARATORS = frozenset([8232, 8233])


class Unread(Exception):
    """A value that cannot be read soundly: the formatter steps aside."""


## What one stopped process holds, asked once per launch

class Process(object):
    """The types and descriptors of one launch, cached until the next."""

    def __init__(self, target):
        self.target = target
        self.types = {}
        self.kinds = {}
        self.descs = {}
        self.statics = None

    def find(self, name):
        """The type named `name` (`T *` read as a pointer to `T`), or an invalid one."""
        if name in self.types:
            return self.types[name]
        if name.endswith(" *"):
            found = self.find(name[:-2])
            found = found.GetPointerType() if found.IsValid() else found
        else:
            found = self.target.FindFirstType(name)
            if not found.IsValid() and name in BASIC:
                found = self.target.GetBasicType(BASIC[name])
            if not found.IsValid() and name == "_Bool":
                found = self.target.FindFirstType("bool")
        self.types[name] = found
        return found

    def runtime(self, name, members):
        """A runtime struct whose members are all there, or Unread."""
        found = self.find(name)
        if not found.IsValid():
            raise Unread(name)
        have = field_names(found)
        for member in members:
            if member not in have:
                raise Unread(name + "." + member)
        return found

    def symbol_at(self, address):
        """The name of the data symbol that starts exactly at `address`, or None."""
        if address in self.descs:
            return self.descs[address]
        named = None
        if address not in (0, lldb.LLDB_INVALID_ADDRESS):
            resolved = self.target.ResolveLoadAddress(address)
            symbol = resolved.GetSymbol()
            if symbol.IsValid() and symbol.GetStartAddress().GetLoadAddress(self.target) == address:
                named = symbol.GetName()
            if not named:
                named = self.static_at(address)
        self.descs[address] = named
        return named

    def static_at(self, address):
        """The DWARF static variable at `address` whose name ends `_desc`, or None."""
        if self.statics is None:
            self.statics = {}
            found = self.target.FindGlobalVariables("_desc$", 100000, lldb.eMatchTypeRegex)
            for i in range(found.GetSize()):
                var = found.GetValueAtIndex(i)
                at = var.GetLoadAddress()
                if at not in (0, lldb.LLDB_INVALID_ADDRESS) and var.GetName():
                    self.statics[at] = var.GetName()
        return self.statics.get(address)


CACHE = {"key": None, "process": None}


def process_of(value):
    """The cache of the launch `value` belongs to."""
    target = value.GetTarget()
    process = value.GetProcess()
    key = (target.GetExecutable().fullpath, process.GetUniqueID(), process.GetProcessID())
    if CACHE["key"] != key:
        CACHE["key"] = key
        CACHE["process"] = Process(target)
    return CACHE["process"]


## Small readers

def field_names(sbtype):
    canonical = sbtype.GetCanonicalType()
    return [canonical.GetFieldAtIndex(i).GetName() for i in range(canonical.GetNumberOfFields())]


def member(value, name):
    """`value`'s member `name`, read without any formatter, or Unread."""
    got = value.GetNonSyntheticValue().GetChildMemberWithName(name)
    if not got.IsValid() or not got.GetError().Success():
        raise Unread(name)
    return got


def signed(value):
    err = lldb.SBError()
    got = value.GetValueAsSigned(err, 0)
    if not err.Success():
        raise Unread("signed")
    return got


def unsigned(value):
    err = lldb.SBError()
    got = value.GetValueAsUnsigned(err, 0)
    if not err.Success():
        raise Unread("unsigned")
    return got


def read(value, address, size):
    """`size` bytes at `address`, or Unread."""
    if size == 0:
        return b""
    err = lldb.SBError()
    data = value.GetProcess().ReadMemory(address, size, err)
    if not err.Success() or data is None or len(data) != size:
        raise Unread("memory")
    return data


def word(value, address, size):
    """The unsigned little-or-big-endian integer of `size` bytes at `address`."""
    data = read(value, address, size)
    order = "little" if value.GetTarget().GetByteOrder() == lldb.eByteOrderLittle else "big"
    return int.from_bytes(data, order)


def type_name(sbtype):
    """The type's own name, a leading `const` or `volatile` off. Not
    `GetUnqualifiedType`, which also unwraps a typedef of a function pointer
    (`HeroFn` comes back `void (*)()`)."""
    name = sbtype.GetName() or ""
    stripped = True
    while stripped:
        stripped = False
        for qualifier in ("const ", "volatile "):
            if name.startswith(qualifier):
                name = name[len(qualifier):]
                stripped = True
    return name


def unwrap(value, wanted):
    """`value` itself where its type is `wanted`, its pointee where it points at
    one (an `@` parameter is a pointer to the caller's place), else Unread."""
    plain = value.GetNonSyntheticValue()
    if type_name(plain.GetType()) == wanted:
        return plain
    if plain.TypeIsPointerType() and type_name(plain.GetType().GetPointeeType()) == wanted:
        if unsigned(plain) == 0:
            raise Unread("null pointer to " + wanted)
        pointee = plain.Dereference().GetNonSyntheticValue()
        if pointee.GetError().Success():
            return pointee
    raise Unread("not a " + wanted)


def made(value, name, address, sbtype):
    """A child named `name` of type `sbtype` at `address`, read lazily."""
    child = value.CreateValueFromAddress(name, address, sbtype)
    if not child.IsValid():
        raise Unread("child")
    small(child)
    return child


def renamed(value, name, part):
    """`part` under the name `name`: at its address where it has one, else its bytes."""
    at = part.GetLoadAddress()
    if at not in (0, lldb.LLDB_INVALID_ADDRESS):
        return made(value, name, at, part.GetType())
    child = value.CreateValueFromData(name, part.GetData(), part.GetType())
    if not child.IsValid():
        raise Unread("child")
    small(child)
    return child


def small(child):
    """A `u8` or `i8` is a number in Heroes (spec § 2), never C's character."""
    if type_name(child.GetType()) in ("uint8_t", "int8_t"):
        child.SetFormat(lldb.eFormatDecimal)


## What a C type is to Heroes

def kind_of(proc, sbtype):
    """`str`, `array`, `map`, `failure`, `option`, `variant`, `case`, `record`,
    `function` (`HeroFn`, a `h_0fn_` type), a scalar's kind, or None for a type
    the formatter leaves alone."""
    name = type_name(sbtype)
    if name in proc.kinds:
        return proc.kinds[name]
    kind = classify(proc, sbtype, name)
    proc.kinds[name] = kind
    return kind


def classify(proc, sbtype, name):
    if name == STR:
        return "str"
    if name == ARRAY + " *":
        return "array"
    if name == MAP + " *":
        return "map"
    if name == FAILURE:
        return "failure"
    canonical = sbtype.GetCanonicalType()
    basic = canonical.GetBasicType()
    if basic in (lldb.eBasicTypeBool,):
        return "bool"
    if basic in (lldb.eBasicTypeFloat, lldb.eBasicTypeDouble):
        return "float"
    if basic in (
        lldb.eBasicTypeSignedChar, lldb.eBasicTypeUnsignedChar, lldb.eBasicTypeChar,
        lldb.eBasicTypeShort, lldb.eBasicTypeUnsignedShort, lldb.eBasicTypeInt,
        lldb.eBasicTypeUnsignedInt, lldb.eBasicTypeLong, lldb.eBasicTypeUnsignedLong,
        lldb.eBasicTypeLongLong, lldb.eBasicTypeUnsignedLongLong,
    ):
        return "integer"
    if (name == FUNC or FUNCTION_TYPE_RE.match(name)) and canonical.IsPointerType():
        return "function"
    if not EMITTED_RE.match(name):
        return None
    if canonical.GetTypeClass() != lldb.eTypeClassStruct:
        return None
    fields = field_names(sbtype)
    if OPTION_RE.match(name):
        return "option" if option_shaped(canonical) else None
    if fields and fields[0] == "tag" and fields[1:] in ([], ["as"]):
        return "variant" if variant_cases(proc, sbtype) is not None else None
    if not fields or any(not f.startswith("f_") for f in fields):
        return None
    if case_parent(proc, name) is not None:
        return "case"
    return "record"


def option_shaped(canonical):
    """A `T?`: `tag`, then a union `as` holding `err` (a HeroFailure) and, but for `()?`, `ok`."""
    if [canonical.GetFieldAtIndex(i).GetName() for i in range(canonical.GetNumberOfFields())] != ["tag", "as"]:
        return False
    union = canonical.GetFieldAtIndex(1).GetType().GetCanonicalType()
    arms = {}
    for i in range(union.GetNumberOfFields()):
        arms[union.GetFieldAtIndex(i).GetName()] = union.GetFieldAtIndex(i).GetType()
    if "err" not in arms or type_name(arms["err"]) != FAILURE:
        return False
    return set(arms) <= set(["ok", "err"])


def variant_cases(proc, sbtype):
    """A variant's cases by tag value, `{value: case}`, or None where the type
    is not one: every enumerator of its `tag` named `<type>_tag_<case>`."""
    name = type_name(sbtype)
    canonical = sbtype.GetCanonicalType()
    tag = canonical.GetFieldAtIndex(0).GetType().GetCanonicalType()
    if tag.GetTypeClass() != lldb.eTypeClassEnumeration:
        return None
    members = tag.GetEnumMembers()
    prefix = name + "_tag_"
    cases = {}
    for i in range(members.GetSize()):
        one = members.GetTypeEnumMemberAtIndex(i)
        if not one.GetName().startswith(prefix):
            return None
        cases[one.GetValueAsSigned()] = one.GetName()[len(prefix):]
    return cases or None


def case_parent(proc, name):
    """The variant `h_m_T` whose case `c` is the type `h_m_T_c_c`, or None. A
    record named `T_c_c` in the same module would be the same C name, which
    clang refuses, so a variant with that case settles it."""
    at = name.rfind("_c_")
    while at > 0:
        parent, case = name[:at], name[at + 3:]
        found = proc.find(parent)
        if found.IsValid() and found.GetCanonicalType().GetTypeClass() == lldb.eTypeClassStruct:
            fields = field_names(found)
            if fields and fields[0] == "tag":
                cases = variant_cases(proc, found)
                if cases is not None and case in cases.values():
                    return parent
        at = name.rfind("_c_", 0, at)
    return None


def bare(name):
    """`Point` for `h_m_Point`: the name the author wrote."""
    got = MODULE_RE.match(name)
    return got.group(2) if got else name


## The renderer: a value as a Heroes literal

class Budget(object):
    def __init__(self):
        self.left = BUDGET

    def take(self):
        self.left -= 1
        return self.left >= 0


def show(value, budget, depth):
    """`value` as a Heroes literal. A part that cannot be read soundly is
    written as its address, `<0x...>`, and never as a guess."""
    if not budget.take() or depth > DEPTH:
        return "..."
    try:
        return render(value, budget, depth)
    except Exception:
        return addressed(value)


def addressed(value):
    at = value.GetNonSyntheticValue().GetLoadAddress()
    if value.GetNonSyntheticValue().TypeIsPointerType():
        try:
            at = unsigned(value.GetNonSyntheticValue())
        except Unread:
            at = lldb.LLDB_INVALID_ADDRESS
    if at in (0, lldb.LLDB_INVALID_ADDRESS):
        return "<?>"
    return "<0x%x>" % at


def render(value, budget, depth):
    proc = process_of(value)
    plain = value.GetNonSyntheticValue()
    kind = kind_of(proc, plain.GetType())
    if kind is None:
        if plain.TypeIsPointerType() and kind_of(proc, plain.GetType().GetPointeeType()) is not None:
            return render(unwrap(plain, type_name(plain.GetType().GetPointeeType())), budget, depth)
        if ours(plain.GetType()):
            # A type this file registers and does not read: asking lldb for
            # its summary would ask this file again.
            raise Unread("left alone")
        text = plain.GetSummary() or plain.GetValue()
        if not text:
            raise Unread("no rendering")
        return text
    return RENDERERS[kind](proc, plain, budget, depth)


def ours(sbtype):
    """Whether `sbtype` is one this file registers a formatter for."""
    name = type_name(sbtype)
    if name.endswith(" *"):
        name = name[:-2]
    return name in (STR, FAILURE, ARRAY, MAP) or EMITTED_RE.match(name) is not None


def render_integer(proc, value, budget, depth):
    if is_signed(value.GetType().GetCanonicalType()):
        return str(signed(value))
    return str(unsigned(value))


def is_signed(canonical):
    return canonical.GetBasicType() in (
        lldb.eBasicTypeSignedChar, lldb.eBasicTypeShort, lldb.eBasicTypeInt,
        lldb.eBasicTypeLong, lldb.eBasicTypeLongLong, lldb.eBasicTypeChar,
    )


def render_bool(proc, value, budget, depth):
    got = unsigned(value)
    if got not in (0, 1):
        raise Unread("bool")
    return "true" if got else "false"


def render_float(proc, value, budget, depth):
    """As `print` writes it (runtime/parts/f64.c): `%.*g` at the fewest digits
    that read back as the same value, from 15 (`f64`) or 6 (`f32`), and `.0`
    where neither a point nor an exponent shows."""
    single = value.GetType().GetCanonicalType().GetBasicType() == lldb.eBasicTypeFloat
    err = lldb.SBError()
    data = value.GetData()
    number = data.GetFloat(err, 0) if single else data.GetDouble(err, 0)
    if not err.Success():
        raise Unread("float")
    return float_text(number, single)


def float_text(number, single):
    if number != number:
        return "nan"
    if number in (float("inf"), float("-inf")):
        return "inf" if number > 0 else "-inf"
    smallest = 1.1754943508222875e-38 if single else 2.2250738585072014e-308
    first = 1 if number != 0 and abs(number) < smallest else (6 if single else 15)
    text = ""
    for digits in range(first, (9 if single else 17) + 1):
        text = "%.*g" % (digits, number)
        back = float(text)
        if single:
            back = struct.unpack("<f", struct.pack("<f", back))[0]
        if back == number:
            break
    if not any(c in text for c in ".eE"):
        text += ".0"
    return text


def render_str(proc, value, budget, depth):
    text, more = str_text(proc, unwrap(value, STR))
    return text if more == 0 else "%s ... %d more bytes" % (text, more)


def str_text(proc, value):
    """A str's literal and how many of its bytes it leaves out, or
    (`<unassigned>`, 0) for the NULL non-value."""
    ptr = unsigned(member(value, "ptr"))
    length = signed(member(value, "len"))
    if ptr == 0:
        return UNASSIGNED, 0
    if length < 0 or length > SPAN:
        raise Unread("str length")
    header = proc.runtime(STR_HEADER, ("refcount", "magic"))
    size = header.GetByteSize()
    fields = dict((header.GetFieldAtIndex(i).GetName(), header.GetFieldAtIndex(i)) for i in range(header.GetNumberOfFields()))
    start = ptr - size
    magic = word(value, start + fields["magic"].GetOffsetInBytes(), fields["magic"].GetType().GetByteSize())
    if magic not in STR_MAGICS:
        raise Unread("str magic")
    count = word(value, start + fields["refcount"].GetOffsetInBytes(), fields["refcount"].GetType().GetByteSize())
    count = to_signed(count, fields["refcount"].GetType().GetByteSize())
    if count != STATIC_COUNT and count < 1:
        raise Unread("str count")
    if read(value, ptr + length, 1) != b"\x00":
        raise Unread("str terminator")
    shown = min(length, TEXT_BYTES)
    data = read(value, ptr, shown)
    text, used = quoted(data, shown < length)
    if text is None:
        raise Unread("str text")
    return text, length - used


def to_signed(number, size):
    bits = size * 8
    return number - (1 << bits) if number >= 1 << (bits - 1) else number


def quoted(data, cut):
    """`data` as a Heroes string literal and how many of its bytes it used, or
    (None, 0) where the bytes are not UTF-8. Where the str was `cut` at
    TEXT_BYTES, a character split by the cut is left for `... N more bytes`."""
    used = len(data)
    text = None
    for back in range(0, 4 if cut else 1):
        try:
            text = data[:len(data) - back].decode("utf-8")
            used = len(data) - back
            break
        except UnicodeDecodeError:
            continue
    if text is None:
        return None, 0
    out = ['"']
    for ch in text:
        code = ord(ch)
        if ch in ESCAPES:
            out.append(ESCAPES[ch])
        elif code < 32 or 127 <= code < 160 or code in BIDI or code in SEPARATORS:
            out.append("\\u{%x}" % code)
        else:
            out.append(ch)
    out.append('"')
    return "".join(out), used


def render_failure(proc, value, budget, depth):
    value = unwrap(value, FAILURE)
    return "(code: %s, msg: %s)" % (show(member(value, "code"), budget, depth + 1), show(member(value, "msg"), budget, depth + 1))


def render_option(proc, value, budget, depth):
    arm, part = option_arm(value)
    if arm == "ok":
        return "ok()" if part is None else "ok(%s)" % show(part, budget, depth + 1)
    return "fail(code: %s, msg: %s)" % (show(member(part, "code"), budget, depth + 1), show(member(part, "msg"), budget, depth + 1))


def option_arm(value):
    """("ok", the value or None for `()?`) or ("err", the HeroFailure)."""
    tag = signed(member(value, "tag"))
    union = member(value, "as")
    if tag == 0:
        ok = union.GetChildMemberWithName("ok")
        return "ok", (ok if ok.IsValid() else None)
    if tag == 1:
        err = union.GetChildMemberWithName("err")
        if not err.IsValid():
            raise Unread("err")
        return "err", err
    raise Unread("option tag")


def render_variant(proc, value, budget, depth):
    case, payload = variant_case(proc, value)
    if payload is None:
        return "." + case
    return "." + case + fields_text(payload, budget, depth)


def variant_case(proc, value):
    """The live case's name and its payload (None for a case with no fields)."""
    cases = variant_cases(proc, value.GetType())
    tag = signed(member(value, "tag"))
    if cases is None or tag not in cases:
        raise Unread("variant tag")
    case = cases[tag]
    union = value.GetChildMemberWithName("as")
    payload = union.GetChildMemberWithName("c_" + case) if union.IsValid() else lldb.SBValue()
    if payload.IsValid():
        return case, payload
    if proc.find(type_name(value.GetType()) + "_c_" + case).IsValid():
        raise Unread("payload missing")
    return case, None


def render_case(proc, value, budget, depth):
    name = type_name(value.GetType())
    parent = case_parent(proc, name)
    if parent is None:
        raise Unread("case")
    return "." + name[len(parent) + 3:] + fields_text(value, budget, depth)


def render_record(proc, value, budget, depth):
    return bare(type_name(value.GetType())) + fields_text(value, budget, depth)


def fields_text(value, budget, depth):
    """`(x: 3, y: 4)`: each member's name with its `f_` off, and its value."""
    parts = []
    for i in range(value.GetNumChildren()):
        part = value.GetChildAtIndex(i)
        if not part.GetName().startswith("f_"):
            raise Unread("member")
        parts.append("%s: %s" % (part.GetName()[2:], show(part, budget, depth + 1)))
    return "(" + ", ".join(parts) + ")"


def render_array(proc, value, budget, depth):
    view = ArrayView(proc, value)
    if view.unassigned:
        return UNASSIGNED
    items = [show(view.child(i), budget, depth + 1) for i in range(min(view.length, SHOWN))]
    if view.length > SHOWN:
        items.append("... %d more" % (view.length - SHOWN))
    return "[" + ", ".join(items) + "]"


def render_map(proc, value, budget, depth):
    view = MapView(proc, value)
    if view.unassigned:
        return UNASSIGNED
    items = []
    for bucket in view.buckets(SHOWN):
        items.append("%s: %s" % (show(view.key(bucket), budget, depth + 1), show(view.entry(bucket, "v"), budget, depth + 1)))
    if view.length > len(items):
        items.append("... %d more" % (view.length - len(items)))
    return "{" + ", ".join(items) + "}"


def render_function(proc, value, budget, depth):
    """A function value, an address (spec § 7), as the name it is written
    with (`xs.fold(0, add)`) where a function's symbol starts there, its module
    left out as a record's is; else its address."""
    at = unsigned(value)
    if at == 0:
        raise Unread("null function")
    named = process_of(value).symbol_at(at)
    got = FUNCTION_RE.match(named or "")
    if not got:
        return "<0x%x>" % at
    return got.group(2)


RENDERERS = {
    "str": render_str,
    "array": render_array,
    "map": render_map,
    "failure": render_failure,
    "option": render_option,
    "variant": render_variant,
    "case": render_case,
    "record": render_record,
    "integer": render_integer,
    "bool": render_bool,
    "float": render_float,
    "function": render_function,
}


## The containers, read and checked

def element_type(proc, value, desc_address):
    """The C type a descriptor describes, checked against the descriptor's
    own `size`; Unread where the descriptor resolves to nothing."""
    named = proc.symbol_at(desc_address)
    if not named:
        raise Unread("descriptor")
    if named in SCALAR_DESCS:
        sbtype = proc.find(SCALAR_DESCS[named])
    elif named in RUNTIME_DESCS:
        sbtype = proc.find(RUNTIME_DESCS[named])
    elif named == FUNC_DESC:
        sbtype = proc.find(FUNC)
    elif named.endswith("_desc") and EMITTED_RE.match(named[:-len("_desc")]):
        sbtype = proc.find(named[:-len("_desc")])
    else:
        raise Unread("descriptor " + named)
    if not sbtype.IsValid():
        raise Unread("type of " + named)
    desc = proc.runtime(DESC, ("size",))
    size_field = [desc.GetFieldAtIndex(i) for i in range(desc.GetNumberOfFields()) if desc.GetFieldAtIndex(i).GetName() == "size"][0]
    size = word(value, desc_address + size_field.GetOffsetInBytes(), size_field.GetType().GetByteSize())
    if size != sbtype.GetByteSize() or size == 0:
        raise Unread("descriptor size")
    return sbtype


def counted(count):
    """A block's count: a live one at least 1, a constant's static block -1."""
    if count != STATIC_COUNT and count < 1:
        raise Unread("count")


class ArrayView(object):
    """A `[T]`: its header checked, its elements typed by its descriptor."""

    def __init__(self, proc, value):
        self.proc = proc
        self.pointer = unwrap(value, ARRAY + " *")
        header_type = proc.runtime(ARRAY, ("refcount", "len", "cap", "elem"))
        self.address = unsigned(self.pointer)
        self.unassigned = self.address == 0
        self.length = 0
        if self.unassigned:
            return
        header = self.pointer.Dereference().GetNonSyntheticValue()
        if not header.GetError().Success():
            raise Unread("header")
        counted(signed(member(header, "refcount")))
        self.length = signed(member(header, "len"))
        cap = signed(member(header, "cap"))
        self.type = element_type(proc, header, unsigned(member(header, "elem")))
        self.size = self.type.GetByteSize()
        if not 0 <= self.length <= cap or cap * self.size > SPAN:
            raise Unread("length")
        self.base = self.address + header_type.GetByteSize()
        if self.length > 0:
            read(header, self.base + self.length * self.size - 1, 1)

    def child(self, i, name=None):
        return made(self.pointer, name or "[%d]" % i, self.base + i * self.size, self.type)


class MapView(object):
    """A `{K: V}`: its header checked, its live buckets found by their state byte."""

    def __init__(self, proc, value):
        self.proc = proc
        self.pointer = unwrap(value, MAP + " *")
        header_type = proc.runtime(MAP, ("refcount", "len", "cap", "key", "val", "states", "keys", "vals"))
        self.address = unsigned(self.pointer)
        self.unassigned = self.address == 0
        self.length = 0
        self.live = []
        self.scanned = 0
        if self.unassigned:
            return
        header = self.pointer.Dereference().GetNonSyntheticValue()
        if not header.GetError().Success():
            raise Unread("header")
        self.header = header
        counted(signed(member(header, "refcount")))
        self.length = signed(member(header, "len"))
        self.cap = signed(member(header, "cap"))
        self.key_type = element_type(proc, header, unsigned(member(header, "key")))
        self.val_type = element_type(proc, header, unsigned(member(header, "val")))
        states = unsigned(member(header, "states"))
        keys = unsigned(member(header, "keys"))
        vals = unsigned(member(header, "vals"))
        ksize, vsize = self.key_type.GetByteSize(), self.val_type.GetByteSize()
        if self.cap < 1 or self.cap & (self.cap - 1) or not 0 <= self.length < self.cap:
            raise Unread("cap")
        if self.cap * (1 + ksize + vsize) > SPAN:
            raise Unread("span")
        if states < header_type.GetByteSize() or keys < states + self.cap or vals < keys + self.cap * ksize:
            raise Unread("regions")
        read(header, self.address + vals + self.cap * vsize - 1, 1)
        self.states, self.keys, self.vals = self.address + states, self.address + keys, self.address + vals
        if self.cap <= SCAN:
            self.scan(self.cap)
            if len(self.live) != self.length:
                raise Unread("live buckets")

    def scan(self, upto):
        """Read state bytes up to bucket `upto`, keeping the live ones."""
        while self.scanned < min(upto, self.cap):
            n = min(CHUNK, self.cap - self.scanned)
            for k, state in enumerate(bytearray(read(self.header, self.states + self.scanned, n))):
                if state == 1:
                    self.live.append(self.scanned + k)
                elif state != 0:
                    raise Unread("state byte")
            self.scanned += n

    def buckets(self, count):
        """The first `count` live buckets."""
        while len(self.live) < min(count, self.length) and self.scanned < self.cap:
            self.scan(self.scanned + CHUNK)
        return self.live[:count]

    def key(self, bucket):
        return made(self.pointer, "k", self.keys + bucket * self.key_type.GetByteSize(), self.key_type)

    def entry(self, bucket, name):
        return made(self.pointer, name, self.vals + bucket * self.val_type.GetByteSize(), self.val_type)


## What lldb calls: one summary, one provider, and neither raises

def summary(value, internal_dict):
    """The one-line summary of any registered type: a Heroes literal, or the
    fallback (`""`, so lldb prints the C members; a pointer's address)."""
    try:
        return show_top(value)
    except Exception:
        return fallback(value)


def show_top(value):
    budget = Budget()
    proc = process_of(value)
    plain = value.GetNonSyntheticValue()
    if plain.TypeIsPointerType() and kind_of(proc, plain.GetType()) not in ("array", "map"):
        plain = unwrap(plain, type_name(plain.GetType().GetPointeeType()))
    if kind_of(proc, plain.GetType()) is None:
        raise Unread("left alone")
    return render(plain, budget, 0)


def fallback(value):
    try:
        plain = value.GetNonSyntheticValue()
        if type_name(plain.GetType()) in (ARRAY + " *", MAP + " *"):
            return "0x%016x" % plain.GetValueAsUnsigned(0)
    except Exception:
        pass
    return ""


class Children(object):
    """The children of any registered type, in Heroes' terms: a record's and a
    case's fields, a variant's payload, a `T?`'s live arm, a container's
    elements. Where the value cannot be read soundly, its C members."""

    def __init__(self, value, internal_dict):
        self.value = value
        self.kids = []
        self.view = None
        self.count = 0

    def update(self):
        try:
            self.build()
        except Exception:
            self.raw()
        return False

    def raw(self):
        plain = self.value.GetNonSyntheticValue()
        self.view = None
        self.kids = [plain.GetChildAtIndex(i) for i in range(plain.GetNumChildren())]
        self.count = len(self.kids)

    def build(self):
        self.view, self.kids, self.count = None, [], 0
        proc = process_of(self.value)
        plain = self.value.GetNonSyntheticValue()
        kind = kind_of(proc, plain.GetType())
        if plain.TypeIsPointerType() and kind not in ("array", "map"):
            plain = unwrap(plain, type_name(plain.GetType().GetPointeeType()))
            kind = kind_of(proc, plain.GetType())
        if kind == "array":
            self.view = ArrayView(proc, plain)
            self.count = min(self.view.length, CHILDREN)
            return
        if kind == "map":
            self.view = MapView(proc, plain)
            self.count = len(self.view.buckets(CHILDREN))
            return
        if kind == "failure":
            self.raw()
            return
        if kind == "str":
            str_text(proc, unwrap(plain, STR))
        elif kind in ("record", "case"):
            self.kids = named_fields(plain)
        elif kind == "variant":
            case, payload = variant_case(proc, plain)
            self.kids = [] if payload is None else named_fields(payload)
        elif kind == "option":
            arm, part = option_arm(plain)
            if arm == "ok":
                self.kids = [] if part is None else [renamed(plain, "ok", part)]
            else:
                self.kids = [renamed(plain, "code", member(part, "code")), renamed(plain, "msg", member(part, "msg"))]
        else:
            raise Unread("kind")
        self.count = len(self.kids)

    def num_children(self):
        return self.count

    def get_child_index(self, name):
        try:
            if isinstance(self.view, ArrayView):
                index = int(name.strip("[]"))
                return index if 0 <= index < self.count else -1
            if isinstance(self.view, MapView):
                for j in range(self.count):
                    if self.child_name(j) == name:
                        return j
                return -1
            for j, kid in enumerate(self.kids):
                if kid.GetName() == name:
                    return j
        except Exception:
            pass
        return -1

    def child_name(self, j):
        view = self.view
        return "[%s]" % show(view.key(view.live[j]), Budget(), DEPTH - 1)

    def get_child_at_index(self, j):
        try:
            if j < 0 or j >= self.count:
                return None
            if isinstance(self.view, ArrayView):
                return self.view.child(j)
            if isinstance(self.view, MapView):
                return self.view.entry(self.view.live[j], self.child_name(j))
            return self.kids[j]
        except Exception:
            return None

    def has_children(self):
        return self.count > 0


def named_fields(value):
    """A record's or a payload's members under the names the author wrote."""
    out = []
    for i in range(value.GetNumChildren()):
        part = value.GetChildAtIndex(i)
        if not part.GetName().startswith("f_"):
            raise Unread("member")
        out.append(renamed(value, part.GetName()[2:], part))
    return out


## Registration

def __lldb_init_module(debugger, internal_dict):
    """`command script import` calls this: every formatter in one category."""
    here = __name__
    run = debugger.HandleCommand
    run("type category define %s" % CATEGORY)
    # A str, a failure and an emitted type: the summary is the whole value,
    # so its C members are not printed under it; an `@` parameter, a pointer
    # to one, shows its address and then the value it points at.
    for name in (STR, FAILURE):
        run("type summary add -w %s -F %s.summary %s" % (CATEGORY, here, name))
        run("type synthetic add -w %s -l %s.Children %s" % (CATEGORY, here, name))
    run("type summary add -w %s -F %s.summary -x '%s'" % (CATEGORY, here, EMITTED))
    run("type synthetic add -w %s -l %s.Children -x '%s'" % (CATEGORY, here, EMITTED))
    # An array and a map: the pointer itself, its address left out of the row
    # (`-v`, the summary writes it where the value cannot be read), and the
    # elements printed under it when asked (`-e`, `frame variable -P 1`).
    for name in (ARRAY, MAP):
        run("type summary add -w %s -e -v -p -F %s.summary '%s *'" % (CATEGORY, here, name))
        run("type synthetic add -w %s -p -l %s.Children '%s *'" % (CATEGORY, here, name))
    run("type category enable %s" % CATEGORY)
