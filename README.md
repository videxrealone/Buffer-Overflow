# Buffer Overflow

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

## Example 1

Basic demonstration of a stack buffer overflow. This example is compiled to a 32-bit program, and has stack protection disabled.

```
gcc -o stack stack.c -m32 -fno-stack-protector
```
Let's import the git repo using the command **git clone** :

```
git clone https://github.com/videxrealone/Buffer-Overflow/
```
## Goal

Using the file **stack**, you need to get through the condition **exploit == 0x9876** and have the message "**It worked!**" printed. No file modification is allowed.

## Solution

This exploit is about the function fgets. Our array is 10 bytes long and the function fgets is looking for up to 13 bytes. Thus, by giving more than 10 bytes to the standard input, we are going to overrun the buffer's boundary and overwrite an adjacent memory location. 
Let me guide you through a simple solution in which we'll be using python alongside **gdb**

Let's start by inspecting the source file **stack.c** :
```
cat stack.c
```
How to trigger the "***It worked!***" output ?

Let's debug the file using GCC :
```
gdb stack
```
Let's try running the program using the command run :

```
run
```
Now lets use a simple input that DOES NOT exceed 13 bytes :

```
AAAAAAAAAA
```
We can see that the program worked just fine, let's try re-running it while using an out of bound input. (any input that exceeds 13 bytes) :

```
run

AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
```
We can see that we got a segmentation fault and that the program crashed, thats because we have overflooded the stack and corrupted the return addresses and return addresses :

Let's quit gdb using **quit** :

```
quit
```
In linux we can execute simple python commands directly in the terminal using **-c**, can you run a simple **"print 'Hello world!' "** ?

```
python -c "------------"
```
Now that we know how to execute commands in python, I should tell you about a facinating secret in linux, you can actually run more than one command in a terminal! 
let's try it!
Let's run a simple echo with two different messages using the **" | "** (ALT GR + 6)  . 
```
echo I'm a message | echo I'm another message
```
Did you spot something unusual?
It's the execution order of the commands.

Let's go back to our program.
Let's try to force the condition **"exploit == 0x9876"**, what we're going to do is to simply concatinate the address with random letters.
There's something else you should know, **STACK WORKS IN REVERSE!** (you can google Little Endian & Big Endian)

![image](https://user-images.githubusercontent.com/91763346/194781982-6f6d27e8-62d9-462e-843c-0e2266ace887.png)

So now, knowing that it works as a little endian, we can easily inject it, to force the **"exploit == 0x9876"** but before that we got to run the executable using **./stack**

Using python :

```
python -c "print 'A'*10+'\x76\x98'" | ./stack
```

Remember that stack works in reverse! & that python is a very powerful tool that we need to learn.
