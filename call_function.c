//        Copyright 2013 Ben Burns
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
    
#include <stdio.h>
#include <stdlib.h>

// This program demonstrates a buffer over flow. One function is called,
// but the return address is modified such that control flows
// to another function.

// function to call
void f()
{
  printf("now in f.\n");
	exit(0);
}

// buffer overflow function
void c()
{
	// on the local stack, before the frame pointer
	// http://insecure.org/stf/smashstack.html
	char b[5];
	int *ret;
	
	// where does 24 come from?
	// (gdb) b c
	// Breakpoint 1 at 0x400568: file main.c, line 14.
	// (gdb) run
	// Starting program: .../a.out 

	// Breakpoint 1, c () at main.c:14
	// (gdb) p /x $ebp
	// p /x $ebp
	// $1 = 0xffffe510
	// (gdb) p /x &b
	// p /x &b
	// $2 = 0x7fffffffe500

	// http://eli.thegreenplace.net/2011/09/06/stack-frame-layout-on-x86-64/
	// need to adjust for one more word, which is the return address.
	// word is 8
	// (0xe510 + word) - 0xe500 = 24 
	ret = (int*)(b + 24);
	
	// where does 70 come from?
	// return address (0x400593) - address to call (f() = 0x40054c) = 70
	(*ret) -= 70;
}

int main()
{
	// call function
	c();

	printf("done.\n");
	return 0;
}

// program output:
//	$ ./a.out 
//	now in f.


// gdb asm:
// 0x40054c <f>                    push   %rbp
// 0x40054d <f+1>                  mov    %rsp,%rbp
// 0x400550 <f+4>                  mov    $0x40065c,%edi
// 0x400555 <f+9>                  callq  0x400410 <puts@plt>
// 0x40055a <f+14>                 mov    $0x0,%edi
// 0x40055f <f+19>                 callq  0x400430 <exit@plt>
// 0x400564 <c>                    push   %rbp
// 0x400565 <c+1>                  mov    %rsp,%rbp
// 0x400568 <c+4>                  lea    -0x10(%rbp),%rax
// 0x40056c <c+8>                  add    $0x18,%rax
// 0x400570 <c+12>                 mov    %rax,-0x8(%rbp)
// 0x400574 <c+16>                 mov    -0x8(%rbp),%rax
// 0x400578 <c+20>                 mov    (%rax),%eax
// 0x40057a <c+22>                 lea    -0x46(%rax),%edx
// 0x40057d <c+25>                 mov    -0x8(%rbp),%rax
// 0x400581 <c+29>                 mov    %edx,(%rax)
// 0x400583 <c+31>                 pop    %rbp
// 0x400584 <c+32>                 retq
// 0x400585 <main>                 push   %rbp
// 0x400586 <main+1>               mov    %rsp,%rbp
// 0x400589 <main+4>               mov    $0x0,%eax
// 0x40058e <main+9>               callq  0x400564 <c>
// 0x400593 <main+14>              mov    $0x400666,%edi
// 0x400598 <main+19>              callq  0x400410 <puts@plt>
// 0x40059d <main+24>              mov    $0x0,%eax
// 0x4005a2 <main+29>              pop    %rbp
// 0x4005a3 <main+30>              retq
