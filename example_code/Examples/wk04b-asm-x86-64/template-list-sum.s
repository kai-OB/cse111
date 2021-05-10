	.file	"template-list-sum.cpp"
	.text
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB652:
	.cfi_startproc
	movq	data(%rip), %rax
	xorl	%edx, %edx
	cmpq	$data, %rax
	je	.L2
	.p2align 4,,10
	.p2align 3
.L3:
	addl	16(%rax), %edx
	movq	(%rax), %rax
	cmpq	$data, %rax
	jne	.L3
.L2:
	movl	%edx, extvar(%rip)
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE652:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.3.1 20190311 (Red Hat 8.3.1-3)"
	.section	.note.GNU-stack,"",@progbits
