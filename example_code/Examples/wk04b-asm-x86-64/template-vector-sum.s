	.file	"template-vector-sum.cpp"
	.text
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB885:
	.cfi_startproc
	movq	data(%rip), %rax
	movq	data+8(%rip), %rcx
	xorl	%edx, %edx
	cmpq	%rcx, %rax
	je	.L2
	.p2align 4,,10
	.p2align 3
.L3:
	addl	(%rax), %edx
	addq	$4, %rax
	cmpq	%rax, %rcx
	jne	.L3
.L2:
	movl	%edx, extvar(%rip)
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE885:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.3.1 20190311 (Red Hat 8.3.1-3)"
	.section	.note.GNU-stack,"",@progbits
