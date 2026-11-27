# Counts the instructions one callback retires from the thunk's first instruction to its return, by function.
# Used as: lldb --batch -o "command script import count.py" -o "count <thunk> <hit>" <binary>
import lldb, collections
def count(debugger, command, result, internal_dict):
    thunk, hit = command.split()[0], int(command.split()[1])
    target = debugger.GetSelectedTarget()
    bp = target.BreakpointCreateByName(thunk)
    bp.SetIgnoreCount(hit - 1)
    err = lldb.SBError()
    process = target.Launch(debugger.GetListener(), None, None, None, None, None, None, 0, False, err)
    thread = process.GetSelectedThread()
    for _ in range(200):
        if thread.GetStopReason() == lldb.eStopReasonBreakpoint: break
        process.Continue(); thread = process.GetSelectedThread()
    frame = thread.GetFrameAtIndex(0)
    sp0 = frame.GetSP()
    ret = thread.GetFrameAtIndex(1).GetPC()
    by = collections.Counter(); n = 0
    while n < 20000:
        pc = thread.GetFrameAtIndex(0).GetPC()
        if pc == ret and thread.GetFrameAtIndex(0).GetSP() >= sp0: break
        name = thread.GetFrameAtIndex(0).GetFunctionName() or "?"
        by[name] += 1; n += 1
        thread.StepInstruction(False)
    print("total %d" % n)
    for k, v in by.most_common(): print("%6d %s" % (v, k))
    process.Kill()
def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand("command script add -f count.count count")
