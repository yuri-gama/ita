# Mini-Shell

## Milestone 1

- fork(2) to create new process
- dup2(2) to change the new process' standart input and output
- execv(3) to load the binary image into the memory and execute it
- The **parent** proccess (mini-shell) must wait for the child process (command triggered by the user) to finish with the wait(2) function
- |: pipe redirection (**E.g.:** prog1 | prog2)
- \>: the process' stdout will be a file
- <: stdin read from a file (**E.g.:** prog < in.txt)


Some valid inputs to mini-shell:

```bash
./prog par1 par2 parn
/bin/ls
/bin/cat test.txt | /bin/grep 123
/bin/cat test.txt > out.txt
./prog < in.txt
./prog1 | ./prog2 | ./prog3 | progn > out.txt
```
