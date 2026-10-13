int f(int a) {
    __attribute__((nodebug)) int hidden_tmp = a * 3;
    int visible_loc = a + 4;
#pragma clang attribute push (__attribute__((nodebug)), apply_to = variable(unless(is_parameter)))
    int pragma_hidden_one = a - 1;
    int pragma_hidden_two = a - 2;
#pragma clang attribute pop
    return hidden_tmp + visible_loc + pragma_hidden_one + pragma_hidden_two;
}
