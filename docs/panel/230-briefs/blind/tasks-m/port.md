# A program to write: a port

The program is run as a command, with the arguments given, by somebody who reads what it prints and
the status it exits with.

The program receives one argument, the path of a configuration file whose every line holds a key,
`=`, and a value, such as `host=example.org`. It prints the value of the key `port` followed by a
line feed. A file that cannot be read, a line that holds no `=`, and a file in which no line has the
key `port` are each an error, and the program reports it.
