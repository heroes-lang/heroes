# writes the C an emitter would write: one __lldbsummaries record per type
import sys
def rec(name, summ):
    n = name.encode() + b"\0"; s = summ.encode() + b"\0"
    assert len(n) < 128 and len(s) < 128
    body = bytes([len(n)]) + n + bytes([len(s)]) + s
    assert len(body) < 128
    data = bytes([1, len(body)]) + body
    cname = "hero_lldb_" + name.replace(" ", "_")
    return "HERO_LLDB_SUMMARY static const unsigned char %s[%d] = {%s};" % (cname, len(data), ",".join(str(b) for b in data))
print('#ifdef __APPLE__\n#define HERO_LLDB_SUMMARY __attribute__((used, section("__TEXT,__lldbsummaries")))\n#else\n#define HERO_LLDB_SUMMARY __attribute__((used, section(".lldbsummaries")))\n#endif')
for name, summ in [("HeroStr", "${var.ptr%s}"),
                   ("h_shapes_Point", "Point(x: ${var.f_x}, y: ${var.f_y})"),
                   ("h_shapes_Room", "Room(name: ${var.f_name}, corner: ${var.f_corner})"),
                   ("h_shapes_Token", "${var.tag}"),
                   ("HeroArrayHeader", "len=${var.len}")]:
    print(rec(name, summ))
