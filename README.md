What I have implemented so far:
- The core shell loop.
- Token parsing.
- Some core  commandline functions such as cd, pwd, echo, 

This is basically the classic shell cycle:

while (1) {
    read();
    parse();
    execute();
}