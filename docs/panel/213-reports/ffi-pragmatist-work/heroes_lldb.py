# Panel 213 ffi-pragmatist probe: an lldb formatter reading TODAY's runtime
# (HERO_RUNTIME_ABI 30), no descriptor field added. Element types come from the
# descriptor's SYMBOL name, the one thing the binary already holds.
import lldb
import re

SCALARS = {
    "hero_desc_int": "int64_t", "hero_desc_i8": "int8_t", "hero_desc_i16": "int16_t",
    "hero_desc_i32": "int32_t", "hero_desc_u8": "uint8_t", "hero_desc_u16": "uint16_t",
    "hero_desc_u32": "uint32_t", "hero_desc_u64": "uint64_t", "hero_desc_f32": "float",
    "hero_desc_f64": "double", "hero_desc_bool": "bool", "hero_desc_str": "HeroStr",
    "hero_desc_failure": "HeroFailure",
}
BASIC = {"int64_t": lldb.eBasicTypeLongLong, "int8_t": lldb.eBasicTypeSignedChar,
         "int16_t": lldb.eBasicTypeShort, "int32_t": lldb.eBasicTypeInt,
         "uint8_t": lldb.eBasicTypeUnsignedChar, "uint16_t": lldb.eBasicTypeUnsignedShort,
         "uint32_t": lldb.eBasicTypeUnsignedInt, "uint64_t": lldb.eBasicTypeUnsignedLongLong,
         "float": lldb.eBasicTypeFloat, "double": lldb.eBasicTypeDouble, "bool": lldb.eBasicTypeBool}


def _type(target, name):
    if name.endswith(" *"):
        return _type(target, name[:-2]).GetPointerType()
    t = target.FindFirstType(name)
    if not t.IsValid() and name in BASIC:
        t = target.GetBasicType(BASIC[name])
    return t


def desc_type(valobj, desc_ptr):
    """The C type a descriptor describes, read from the descriptor's symbol."""
    target = valobj.GetTarget()
    sym = target.ResolveLoadAddress(desc_ptr).GetSymbol()
    name = sym.GetName() or ""
    if name in SCALARS:
        tname = SCALARS[name]
    elif name == "hero_desc_array":
        tname = "HeroArrayHeader *"
    elif name == "hero_desc_map":
        tname = "HeroMapHeader *"
    elif name.endswith("_desc"):
        tname = name[: -len("_desc")]
    else:
        return None, name
    return _type(target, tname), name


def str_summary(valobj, _dict):
    ptr = valobj.GetChildMemberWithName("ptr").GetValueAsUnsigned(0)
    n = valobj.GetChildMemberWithName("len").GetValueAsSigned(0)
    if ptr == 0:
        return "<unassigned>"
    if n == 0:
        return '""'
    err = lldb.SBError()
    data = valobj.GetProcess().ReadMemory(ptr, min(n, 1024), err)
    if not err.Success():
        return "<unreadable>"
    return '"' + data.decode("utf-8", "replace").replace('"', '\\"') + '"'


def _header(valobj):
    v = valobj.GetNonSyntheticValue()
    return (v.Dereference() if v.TypeIsPointerType() else v).GetNonSyntheticValue()


def _addr(valobj):
    v = valobj.GetNonSyntheticValue()
    return v.GetValueAsUnsigned(0) if v.TypeIsPointerType() else v.GetLoadAddress()


class ArrayProvider:
    def __init__(self, valobj, _dict):
        self.valobj = valobj

    def update(self):
        self.n, self.t = 0, None
        v = self.valobj.GetNonSyntheticValue()
        if _addr(v) in (0, lldb.LLDB_INVALID_ADDRESS):
            return False
        h = _header(v)
        self.n = max(0, min(h.GetChildMemberWithName("len").GetValueAsSigned(0), 10000))
        self.t, _ = desc_type(v, h.GetChildMemberWithName("elem").GetValueAsUnsigned(0))
        self.base = _addr(v) + h.GetType().GetByteSize()
        self.size = self.t.GetByteSize() if self.t and self.t.IsValid() else 0
        return False

    def num_children(self):
        return self.n if self.size else 0

    def get_child_index(self, name):
        try:
            return int(name.lstrip("[").rstrip("]"))
        except ValueError:
            return -1

    def get_child_at_index(self, i):
        return self.valobj.CreateValueFromAddress("[%d]" % i, self.base + i * self.size, self.t)


def _show(k):
    return k.GetSummary() or k.GetValue() or "?"


def array_summary(valobj, _dict):
    v = valobj.GetNonSyntheticValue()
    if v.GetValueAsUnsigned(0) == 0:
        return "<unassigned>"
    pr = ArrayProvider(v, None)
    pr.update()
    n = pr.num_children()
    items = [_show(pr.get_child_at_index(i)) for i in range(min(n, 16))]
    return "[" + ", ".join(items) + (", ..." if n > 16 else "") + "]"


class MapProvider:
    def __init__(self, valobj, _dict):
        self.valobj = valobj

    def update(self):
        self.live = []
        v = self.valobj.GetNonSyntheticValue()
        addr = _addr(v)
        if addr in (0, lldb.LLDB_INVALID_ADDRESS):
            return False
        h = _header(v)
        f = lambda n: h.GetChildMemberWithName(n).GetValueAsUnsigned(0)
        self.kt, _ = desc_type(v, f("key"))
        self.vt, _ = desc_type(v, f("val"))
        cap = min(f("cap"), 1 << 16)
        err = lldb.SBError()
        states = v.GetProcess().ReadMemory(addr + f("states"), cap, err) if cap else b""
        self.keys, self.vals = addr + f("keys"), addr + f("vals")
        self.live = [i for i in range(cap) if states and states[i] == 1]
        return False

    def num_children(self):
        return len(self.live)

    def get_child_index(self, name):
        return -1

    def get_child_at_index(self, j):
        i = self.live[j]
        k = self.valobj.CreateValueFromAddress("k", self.keys + i * self.kt.GetByteSize(), self.kt)
        ks = k.GetSummary() or k.GetValue()
        return self.valobj.CreateValueFromAddress("[%s]" % ks, self.vals + i * self.vt.GetByteSize(), self.vt)


def map_summary(valobj, _dict):
    v = valobj.GetNonSyntheticValue()
    if v.GetValueAsUnsigned(0) == 0:
        return "<unassigned>"
    pr = MapProvider(v, None)
    pr.update()
    items = []
    for j in range(min(pr.num_children(), 16)):
        c = pr.get_child_at_index(j)
        items.append("%s: %s" % (c.GetName()[1:-1], _show(c)))
    return "{" + ", ".join(items) + "}"


class RecordProvider:
    """A record h_<mod>_<Name>: its fields f_x shown as x."""

    def __init__(self, valobj, _dict):
        self.valobj = valobj

    def update(self):
        v = self.valobj.GetNonSyntheticValue()
        self.kids = [v.GetChildAtIndex(i) for i in range(v.GetNumChildren())]
        return False

    def num_children(self):
        return len(self.kids)

    def get_child_index(self, name):
        for i, k in enumerate(self.kids):
            if k.GetName() == "f_" + name:
                return i
        return -1

    def get_child_at_index(self, i):
        k = self.kids[i]
        return k.CreateValueFromData(k.GetName()[2:], k.GetData(), k.GetType())


class TaggedProvider:
    """A variant (tag enum + `as` union of c_<case>) or a T? (tag 0 ok / 1 err)."""

    def __init__(self, valobj, _dict):
        self.valobj = valobj

    def update(self):
        v = self.valobj.GetNonSyntheticValue()
        self.kids, self.case = [], None
        tag = v.GetChildMemberWithName("tag")
        u = v.GetChildMemberWithName("as")
        if v.GetTypeName().startswith("h_0opt_"):
            arm = "ok" if tag.GetValueAsSigned(0) == 0 else "err"
            self.case = arm
            a = u.GetChildMemberWithName(arm)
            if a.IsValid():
                self.kids = [a.CreateValueFromData(arm, a.GetData(), a.GetType())]
            return False
        enum = tag.GetValue() or ""  # h_shapes_Token_tag_num
        self.case = enum.split("_tag_", 1)[-1]
        a = u.GetChildMemberWithName("c_" + self.case)
        if a.IsValid():
            for i in range(a.GetNumChildren()):
                k = a.GetChildAtIndex(i)
                self.kids.append(k.CreateValueFromData(k.GetName()[2:], k.GetData(), k.GetType()))
        return False

    def num_children(self):
        return len(self.kids)

    def get_child_index(self, name):
        for i, k in enumerate(self.kids):
            if k.GetName() == name:
                return i
        return -1

    def get_child_at_index(self, i):
        return self.kids[i]


def tagged_summary(valobj, _dict):
    v = valobj.GetNonSyntheticValue()
    tag = v.GetChildMemberWithName("tag")
    if v.GetTypeName().startswith("h_0opt_"):
        if tag.GetValueAsSigned(0) == 0:
            ok = v.GetChildMemberWithName("as").GetChildMemberWithName("ok")
            return "ok(%s)" % ((ok.GetSummary() or ok.GetValue()) if ok.IsValid() else "")
        err = v.GetChildMemberWithName("as").GetChildMemberWithName("err")
        c = str_summary(err.GetChildMemberWithName("code"), None)
        m = str_summary(err.GetChildMemberWithName("msg"), None)
        return "fail(code: %s, msg: %s)" % (c, m)
    return "." + (tag.GetValue() or "?").split("_tag_", 1)[-1]


def __lldb_init_module(debugger, _dict):
    m = __name__
    run = lambda c: debugger.HandleCommand(c)
    run("type category define heroes")
    run("type summary add -w heroes -F %s.str_summary HeroStr" % m)
    run("type summary add -e -w heroes -F %s.array_summary 'HeroArrayHeader *'" % m)
    run("type synthetic add -w heroes -l %s.ArrayProvider 'HeroArrayHeader *'" % m)
    run("type summary add -e -w heroes -F %s.map_summary 'HeroMapHeader *'" % m)
    run("type synthetic add -w heroes -l %s.MapProvider 'HeroMapHeader *'" % m)
    # A T? and a variant: a struct with `tag` and `as`. Records: every other h_<mod>_<Upper>.
    run("type summary add -e -w heroes -F %s.tagged_summary -x '^h_0opt_'" % m)
    run("type synthetic add -w heroes -l %s.TaggedProvider -x '^h_0opt_'" % m)
    run("type category enable heroes")


class UserProvider:
    """h_<mod>_<Name>: a variant when it has `tag`, else a record."""

    def __init__(self, valobj, d):
        v = valobj.GetNonSyntheticValue()
        self.inner = (TaggedProvider if v.GetChildMemberWithName("tag").IsValid() else RecordProvider)(valobj, d)

    def update(self):
        return self.inner.update()

    def num_children(self):
        return self.inner.num_children()

    def get_child_index(self, n):
        return self.inner.get_child_index(n)

    def get_child_at_index(self, i):
        return self.inner.get_child_at_index(i)


def user_summary(valobj, d):
    v = valobj.GetNonSyntheticValue()
    if v.GetChildMemberWithName("tag").IsValid():
        return tagged_summary(valobj, d)
    name = re.sub(r"^h_[a-z0-9_]+?_(?=[A-Z])", "", v.GetTypeName())
    parts = []
    for i in range(v.GetNumChildren()):
        k = v.GetChildAtIndex(i)
        parts.append("%s: %s" % (k.GetName()[2:], k.GetSummary() or k.GetValue()))
    return "%s(%s)" % (name, ", ".join(parts))


USER = "'^h_[a-z0-9_]+_[A-Z][A-Za-z0-9]*$'"


def __lldb_init_module(debugger, _dict, _first=__lldb_init_module):
    _first(debugger, _dict)
    m = __name__
    debugger.HandleCommand("type summary add -e -w heroes -F %s.user_summary -x %s" % (m, USER))
    debugger.HandleCommand("type synthetic add -w heroes -l %s.UserProvider -x %s" % (m, USER))
