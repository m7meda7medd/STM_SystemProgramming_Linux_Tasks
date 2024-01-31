	.file	"femtoshell.c"
	.text
	.section	.rodata
	.align 8
.LC0:
	.string	"I can't hear you Say something > "
.LC1:
	.string	"exit"
.LC2:
	.string	"I got it, Bye Dear"
.LC3:
	.string	"you Said: %s? \n"
	.text
	.globl	GetShellMessage
	.type	GetShellMessage, @function
GetShellMessage:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$160, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -148(%rbp)
	movb	$0, -150(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L2
.L3:
	movzbl	-150(%rbp), %eax
	cltq
	movzbl	-149(%rbp), %edx
	movb	%dl, -144(%rbp,%rax)
	movzbl	-150(%rbp), %eax
	addl	$1, %eax
	movb	%al, -150(%rbp)
.L2:
	call	getchar@PLT
	movb	%al, -149(%rbp)
	cmpb	$10, -149(%rbp)
	jne	.L3
	movzbl	-150(%rbp), %eax
	cltq
	movb	$0, -144(%rbp,%rax)
	leaq	-144(%rbp), %rax
	leaq	.LC1(%rip), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L4
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$1, -148(%rbp)
	jmp	.L5
.L4:
	leaq	-144(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
.L5:
	movl	-148(%rbp), %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L7
	call	__stack_chk_fail@PLT
.L7:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	GetShellMessage, .-GetShellMessage
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
