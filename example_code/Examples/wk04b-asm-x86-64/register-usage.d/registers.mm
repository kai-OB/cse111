.so Tmac.mm-etc
.if t .Newcentury-fonts
.INITR* \n[.F]
.SIZE 12 14
.TITLE "X86-64 ABI Register Usage"
.RCS $Id: registers.mm,v 1.69 2021-05-04 11:55:59-07 - - $
.SP
.DS
.TS
center tab(|);
|l  s     s     s     s     |l |l |,
|r  l     l     l     l     |l |l |,
|r1 l1fCB l1fCB l1fCB l1fCB |l |lw(236p) |.
_
Register|Callee|Usage
|quad|long|word|byte|Saved
_
0|%rax|%eax|%ax|%al||T{
.fi
.na
1st return register
.br
used in \f[CB]idiv\f[P] and \f[CB]imul\f[P] instructions
.br
\f[CB]<cstdarg>\f[P]
vector register argument count
T}
_
1|%rcx|%ecx|%cx|%cl||4th argument register
_
2|%rdx|%edx|%dx|%dl||T{
3rd argument register
2nd return register
used in \f[CB]idiv\f[P] and \f[CB]imul\f[P] instructions
T}
_
3|%rbx|%ebx|%bx|%dl|Yes|
_
4|%rsp|%esp|%sp|%spl|Yes|stack pointer
_
5|%rbp|%ebp|%bp|%bpl|Yes|T{
.na
.fi
frame pointer (optional)
T}
_
6|%rsi|%esi|%si|%sil||2nd argument register
_
7|%rdi|%edi|%di|%dil||1st argument register
_
8|%r8|%r8d|%r8w|%r8b||5th argument register
_
9|%r9|%r9d|%r9w|%r9b||6th argument register
_
10|%r10|%r10d|%r10w|%r10b||T{
.fi
.na
static chain pointer (optional)
T}
_
11|%r11|%r11d|%r11w|%r11b||
_
12|%r12|%r12d|%r12w|%r12b |Yes|
_
13|%r13|%r13d|%r13w|%r13b |Yes|
_
14|%r14|%r14d|%r14w|%r14b |Yes|
_
15|%r15|%r15d|%r15w|%r15b |Yes|T{
.fi
.na
global offset table (GOT) pointer (optional)
T}
_
.TE
.DE
.SP
Second byte of arithmetic registers (8086 compatibility)\(::
.V= %ah ,
.V= %bh ,
.V= %ch ,
.V= %dh .
.SP
.DS
.TS
tab(|); |l lfCB lfCB |.
_
Function prolog|pushq|%rpb
|movq|%rsp, %rbp
|subq|$N, %rsp
_
Function epilog|movq|%rpb, %rsp
|popq|%rbp
|ret
_
.TE
.DE
