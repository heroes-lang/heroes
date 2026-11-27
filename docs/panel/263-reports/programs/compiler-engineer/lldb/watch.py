import lldb
def arm(debugger, name):
    f = debugger.GetSelectedTarget().GetProcess().GetSelectedThread().GetSelectedFrame()
    v = f.EvaluateExpression("(unsigned long)((void *(*)(void *))(*(void **)&%s))((void *)&%s)" % (name, name))
    a = v.GetValueAsUnsigned()
    print("p263: %s's per-thread address 0x%x (%s)" % (name, a, v.GetError()))
    return a
def __lldb_init_module(debugger, d):
    pass
def go(debugger, command, result, d):
    a = arm(debugger, "hero_thread_is_home")
    b = arm(debugger, "hero_callback_mine")
    debugger.HandleCommand("watchpoint set expression -w write -s 1 -- 0x%x" % a)
    debugger.HandleCommand("watchpoint set expression -w write -s 8 -- 0x%x" % b)
