struct pt { int x; int y; };
struct inner { unsigned char b[3]; };
struct outer { struct inner inn; struct pt pts[2]; long tail; };
