# Critic, panel 178 reader test: something listening at /tmp/app.sock, so task 2 can connect.
import os, socket
p = '/tmp/app.sock'
try: os.unlink(p)
except FileNotFoundError: pass
s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM); s.bind(p); s.listen(64)
while True:
    c, _ = s.accept(); c.close()
