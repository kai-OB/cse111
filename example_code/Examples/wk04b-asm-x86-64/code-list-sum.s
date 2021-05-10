	.file	"code-list-sum.cpp"
	.text
	.p2align 4,,15
	.globl	_Z8sum_listRKSt4listIiSaIiEE
	.type	_Z8sum_listRKSt4listIiSaIiEE, @function
_Z8sum_listRKSt4listIiSaIiEE:
.LFB651:
	.cfi_startproc
	movq	(%rdi), %rdx
	xorl	%eax, %eax
	cmpq	%rdx, %rdi
	je	.L4
	.p2align 4,,10
	.p2align 3
.L3:
	addl	16(%rdx), %eax
	movq	(%rdx), %rdx
	cmpq	%rdx, %rdi
	jne	.L3
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	ret
	.cfi_endproc
.LFE651:
	.size	_Z8sum_listRKSt4listIiSaIiEE, .-_Z8sum_listRKSt4listIiSaIiEE
	.ident	"GCC: (GNU) 8.3.1 20190311 (Red Hat 8.3.1-3)"
	.section	.note.GNU-stack,"",@progbits
