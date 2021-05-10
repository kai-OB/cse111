	.file	"code-vector-sum.cpp"
	.text
	.p2align 4,,15
	.globl	_Z10sum_vectorRKSt6vectorIiSaIiEE
	.type	_Z10sum_vectorRKSt6vectorIiSaIiEE, @function
_Z10sum_vectorRKSt6vectorIiSaIiEE:
.LFB884:
	.cfi_startproc
	movq	(%rdi), %rdx
	movq	8(%rdi), %rcx
	xorl	%eax, %eax
	cmpq	%rcx, %rdx
	je	.L4
	.p2align 4,,10
	.p2align 3
.L3:
	addl	(%rdx), %eax
	addq	$4, %rdx
	cmpq	%rdx, %rcx
	jne	.L3
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	ret
	.cfi_endproc
.LFE884:
	.size	_Z10sum_vectorRKSt6vectorIiSaIiEE, .-_Z10sum_vectorRKSt6vectorIiSaIiEE
	.ident	"GCC: (GNU) 8.3.1 20190311 (Red Hat 8.3.1-3)"
	.section	.note.GNU-stack,"",@progbits
