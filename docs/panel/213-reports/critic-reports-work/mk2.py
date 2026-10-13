import sys, importlib.util
spec = importlib.util.spec_from_file_location("fbc", sys.argv[1])
m = importlib.util.module_from_spec(spec); spec.loader.exec_module(m)
ok = '"as" @get_child_with_name call "ok" @get_child_with_name call @get_value call'
err = '"as" @get_child_with_name call "err" @get_child_with_name call "code" @get_child_with_name call "ptr" @get_child_with_name call @summary call'
asm = 'dup "tag" @get_child_with_name call @get_value_as_unsigned call 0u = { ' + ok + ' } { ' + err + ' } ifelse'
bc = bytes(m.compile(asm))
def uleb(n):
    out = bytearray()
    while True:
        b = n & 0x7f; n >>= 7
        if n: out.append(b | 0x80)
        else: out.append(b); return bytes(out)
name = sys.argv[2].encode() + b"\0"
entry = uleb(int(sys.argv[4])) + bytes([0]) + uleb(len(bc)) + bc
body = uleb(len(name)) + name + entry
rec = uleb(1) + uleb(len(body)) + body
sect = sys.argv[3]
print('__attribute__((used, section("%s"))) static const unsigned char hero_opt_formatter[%d] = {%s};' % (sect, len(rec), ",".join(str(b) for b in rec)))
print("/* bytecode %d bytes: %s */" % (len(bc), bc.hex()))
