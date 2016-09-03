	.text
	.align 4,0x90
.globl __Z18fast9_corner_scorePKhPKii
__Z18fast9_corner_scorePKhPKii:
LFB788:
	pushq	%rbx
LCFI0:
	leal	255(%rdx), %r8d
	sarl	%r8d
	movzbl	(%rdi), %r11d
	movslq	(%rsi),%rax
	movzbl	(%rdi,%rax), %ebx
	movl	%edx, %r9d
	movl	$255, %r10d
	leal	(%r8,%r11), %edx
	movl	%r11d, %ecx
	subl	%r8d, %ecx
	cmpl	%ebx, %edx
	jge	L3
	.align 4,0x90
L993:
	movslq	4(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L5
	movslq	8(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L7
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L9
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L11
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L13
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L15
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L992
	cmpl	%eax, %ecx
	jle	L22
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L24
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	.align 4,0x90
L21:
	movl	%r8d, %r10d
L986:
	leal	-1(%r10), %eax
	cmpl	%r9d, %eax
	je	L989
	cmpl	%r10d, %r9d
	je	L989
	leal	(%r10,%r9), %edx
	movl	%edx, %eax
	shrl	$31, %eax
	leal	(%rax,%rdx), %r8d
	sarl	%r8d
	leal	(%r8,%r11), %edx
	movl	%r11d, %ecx
	subl	%r8d, %ecx
	cmpl	%ebx, %edx
	jl	L993
L3:
	cmpl	%ebx, %ecx
	jle	L466
	movslq	4(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L468
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L470
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L474
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L476
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L478
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L480
	movslq	8(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L466:
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L927
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L931
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L933
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L935
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L937
	movslq	8(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L939
	movslq	4(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L5:
	cmpl	%eax, %ecx
	jle	L358
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L360
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L364
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L366
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L368
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L370
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L468:
	cmpl	%eax, %ecx
	jle	L525
	movslq	8(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L527
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L529
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L534
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L536
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L538
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L358:
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L415
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L419
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L421
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L423
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L425
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L7:
	cmpl	%eax, %ecx
	jle	L267
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L269
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L273
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L275
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L277
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L927:
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L960
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L962
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L964
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L966
	movslq	8(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L968
	movslq	4(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L989:
	movl	%r9d, %eax
	popq	%rbx
	ret
L360:
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L391
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L393
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L395
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L397
	movslq	8(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L525:
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L876
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L881
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L883
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L885
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L887
	movslq	8(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L470:
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L501
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L503
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L505
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L507
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L9:
	cmpl	%eax, %ecx
	jle	L193
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L195
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L199
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L201
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L415:
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L447
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L449
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L451
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L453
	movslq	8(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L267:
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L315
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L319
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L321
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L323
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L315:
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L345
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L347
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L349
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L193:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L232
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L236
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L238
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L876:
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L903
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L905
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L907
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L909
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L269:
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L298
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L300
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L302
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L527:
	cmpl	%eax, %ecx
	jle	L575
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L577
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L994
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L599
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L601
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L11:
	cmpl	%eax, %ecx
	jle	L121
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L123
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L125
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L128
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L13:
	cmpl	%eax, %ecx
	jle	L65
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L995
	cmpl	%eax, %ecx
	jle	L85
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L121:
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L166
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L170
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L931:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L575:
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L833
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L839
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L841
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L843
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L529:
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L554
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L556
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L558
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L232:
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L259
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L261
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L195:
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L221
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L223
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L419:
	movslq	8(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L166:
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L189
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L960:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	jmp	L19
	.align 4,0x90
L65:
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L996
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L15:
	cmpl	%eax, %ecx
	jle	L34
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L36
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L38
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L123:
	cmpl	%eax, %ecx
	jle	L147
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L155
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L833:
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L855
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L857
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L859
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L577:
	cmpl	%eax, %ecx
	jle	L616
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L618
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L620
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L628
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L933:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L364:
	movslq	8(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L474:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
L19:
	movl	%r8d, %r9d
	jmp	L986
L616:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L798
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L805
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L807
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L147:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L34:
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L54
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L996:
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L273:
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L421:
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L319:
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L476:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L366:
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L962:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L501:
	movslq	8(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L125:
	cmpl	%eax, %ecx
	jle	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L994:
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L585
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L587
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L935:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L992:
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L391:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L995:
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L69
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L85:
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L69:
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L964:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L503:
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L478:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L36:
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L275:
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L903:
	movslq	8(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L798:
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L816
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L818
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L618:
	cmpl	%eax, %ecx
	jle	L663
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L665
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L667
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L199:
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L881:
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L321:
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L236:
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L393:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L937:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L423:
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L447:
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L368:
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L22:
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L54:
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L24:
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L201:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L663:
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L771
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L779
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L966:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L505:
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L554:
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L277:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L905:
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L480:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L38:
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L298:
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L449:
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L370:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L128:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L939:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L425:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L883:
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L323:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L238:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L395:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L534:
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L170:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L620:
	cmpl	%eax, %ecx
	jle	L632
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L634
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L536:
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L855:
	movslq	12(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L397:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L632:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L665:
	cmpl	%eax, %ecx
	jle	L702
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L704
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L706
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L708
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L556:
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L968:
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L907:
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L885:
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L300:
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L507:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L451:
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L345:
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L771:
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L786
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L302:
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L857:
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L538:
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L909:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L887:
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L634:
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L644
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L558:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L839:
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L453:
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L347:
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L667:
	cmpl	%eax, %ecx
	jle	L677
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L679
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L221:
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L702:
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L752
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L599:
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L841:
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L349:
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L587:
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L585:
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L818:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L816:
	movslq	16(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L843:
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L859:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L261:
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L259:
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L679:
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L677:
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L223:
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L704:
	cmpl	%eax, %ecx
	jle	L724
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L726
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L728
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L706:
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
	.align 4,0x90
L601:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L752:
	cmpl	%eax, %ecx
	jle	L21
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
	.align 4,0x90
L724:
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L741
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	44(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	48(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L726:
	cmpl	%eax, %ecx
	jle	L737
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L741:
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L737:
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L708:
	cmpl	%eax, %ecx
	jle	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L728:
	cmpl	%eax, %ecx
	jle	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L779:
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L628:
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L805:
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L644:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L807:
	movslq	52(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jge	L21
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %edx
	jl	L19
	jmp	L21
L155:
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L189:
	movslq	56(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	60(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
L786:
	movslq	20(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	24(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	28(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	32(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	36(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jle	L21
	movslq	40(%rsi),%rax
	movzbl	(%rax,%rdi), %eax
	cmpl	%eax, %ecx
	jg	L19
	jmp	L21
LFE788:
	.cstring
LC0:
	.ascii "vector::_M_fill_insert\0"
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
	.align 1
	.align 4
.globl __ZNSt6vectorI2xySaIS0_EE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPS0_S2_EEmRKS0_
	.weak_definition __ZNSt6vectorI2xySaIS0_EE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPS0_S2_EEmRKS0_
__ZNSt6vectorI2xySaIS0_EE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPS0_S2_EEmRKS0_:
LFB917:
	pushq	%r15
LCFI1:
	pushq	%r14
LCFI2:
	pushq	%r13
LCFI3:
	pushq	%r12
LCFI4:
	pushq	%rbp
LCFI5:
	pushq	%rbx
LCFI6:
	subq	$40, %rsp
LCFI7:
	movq	%rdi, %r12
	movq	%rsi, %rbp
	movq	%rdx, 8(%rsp)
	movq	%rcx, %r14
	movq	%rdx, %rbx
	testq	%rdx, %rdx
	je	L1058
	movq	8(%rdi), %r8
	movq	16(%rdi), %rax
	subq	%r8, %rax
	sarq	$3, %rax
	cmpq	%rax, %rdx
	jbe	L1067
	movq	%r8, %rcx
	subq	(%rdi), %rcx
	sarq	$3, %rcx
	movabsq	$2305843009213693951, %rsi
	movq	%rsi, %rax
	subq	%rcx, %rax
	cmpq	%rax, %rdx
	ja	L1068
	leaq	8(%rsp), %rax
	leaq	24(%rsp), %rdx
	cmpq	%rcx, %rbx
	cmovbe	%rdx, %rax
	movq	%rcx, 24(%rsp)
	movq	(%rax), %rdx
	leaq	(%rcx,%rdx), %rax
	movq	$-8, %r15
	cmpq	%rax, %rcx
	jbe	L1069
L1038:
	movq	%r15, %rdi
	call	__Znwm
	movq	%rax, %r13
	movq	(%r12), %rdi
	movq	%rax, %rdx
	movq	%rax, %rsi
	cmpq	%rdi, %rbp
	je	L1041
	movq	%rdi, %rcx
	.align 4
L1042:
	testq	%rdx, %rdx
	je	L1043
	movq	(%rcx), %rax
	movq	%rax, (%rdx)
L1043:
	addq	$8, %rcx
	addq	$8, %rdx
	cmpq	%rcx, %rbp
	jne	L1042
	movq	%rdx, %rsi
L1041:
	movq	%rbx, %rcx
	jmp	L1046
	.align 4
L1070:
	addq	$8, %rdx
L1046:
	testq	%rdx, %rdx
	je	L1047
	movq	(%r14), %rax
	movq	%rax, (%rdx)
L1047:
	decq	%rcx
	jne	L1070
	leaq	(%rsi,%rbx,8), %rbx
	movq	8(%r12), %rdx
	cmpq	%rdx, %rbp
	je	L1051
	movq	%rbp, %rsi
	.align 4
L1053:
	testq	%rbx, %rbx
	je	L1054
	movq	(%rsi), %rax
	movq	%rax, (%rbx)
L1054:
	addq	$8, %rbx
	addq	$8, %rsi
	cmpq	%rsi, %rdx
	jne	L1053
L1051:
	testq	%rdi, %rdi
	je	L1056
	call	__ZdlPv
L1056:
	movq	%r13, (%r12)
	movq	%rbx, 8(%r12)
	leaq	(%r13,%r15), %rax
	movq	%rax, 16(%r12)
L1058:
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
	.align 4
L1067:
	movq	(%rcx), %rcx
	movq	%r8, %r9
	subq	%rsi, %r9
	sarq	$3, %r9
	cmpq	%r9, %rdx
	jae	L1002
	leaq	0(,%rdx,8), %r10
	movq	%r8, %r9
	subq	%r10, %r9
	cmpq	%r9, %r8
	je	L1004
	movq	%r9, %rsi
	movq	%r8, %rdx
	jmp	L1006
	.align 4
L1071:
	addq	$8, %rdx
L1006:
	testq	%rdx, %rdx
	je	L1007
	movq	(%rsi), %rax
	movq	%rax, (%rdx)
L1007:
	addq	$8, %rsi
	cmpq	%rsi, %r8
	jne	L1071
L1004:
	leaq	(%r8,%r10), %rax
	movq	%rax, 8(%r12)
	movq	%r9, %rdi
	subq	%rbp, %rdi
	sarq	$3, %rdi
	testq	%rdi, %rdi
	jle	L1010
	movq	%r8, %rsi
	leaq	-8(%r9), %rdx
	.align 4
L1012:
	movq	(%rdx), %rax
	movq	%rax, -8(%rsi)
	subq	$8, %rsi
	subq	$8, %rdx
	decq	%rdi
	jne	L1012
L1010:
	leaq	(%rbp,%r10), %rax
	cmpq	%rax, %rbp
	je	L1058
	movq	%rbp, %rsi
	.align 4
L1014:
	movq	%rcx, (%rsi)
	addq	$8, %rsi
	cmpq	%rsi, %rax
	jne	L1014
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
L1002:
	movq	%rdx, %rsi
	subq	%r9, %rsi
	je	L1015
	movq	%rsi, %rdx
	movq	%r8, %rax
	jmp	L1017
	.align 4
L1072:
	addq	$8, %rax
L1017:
	testq	%rax, %rax
	je	L1018
	movq	%rcx, (%rax)
L1018:
	decq	%rdx
	jne	L1072
L1015:
	leaq	(%r8,%rsi,8), %r10
	cmpq	%r8, %rbp
	je	L1021
	movq	%rbp, %rsi
	movq	%r10, %rdi
	movq	%rbp, %rdx
	jmp	L1023
	.align 4
L1073:
	addq	$8, %rdi
L1023:
	testq	%rdi, %rdi
	je	L1024
	movq	(%rdx), %rax
	movq	%rax, (%rdi)
L1024:
	addq	$8, %rdx
	cmpq	%rdx, %r8
	jne	L1073
	leaq	(%r10,%r9,8), %rax
	movq	%rax, 8(%r12)
	.align 4
L1028:
	movq	%rcx, (%rsi)
	addq	$8, %rsi
	cmpq	%r8, %rsi
	jne	L1028
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
L1069:
	cmpq	%rsi, %rax
	ja	L1036
	leaq	0(,%rax,8), %r15
	jmp	L1038
L1021:
	leaq	(%r10,%r9,8), %rax
	movq	%rax, 8(%r12)
	jmp	L1058
L1036:
	call	__ZSt17__throw_bad_allocv
L1068:
	leaq	LC0(%rip), %rdi
	call	__ZSt20__throw_length_errorPKc
LFE917:
	.align 1
	.align 4
.globl __ZNSt6vectorIiSaIiEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPiS1_EEmRKi
	.weak_definition __ZNSt6vectorIiSaIiEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPiS1_EEmRKi
__ZNSt6vectorIiSaIiEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPiS1_EEmRKi:
LFB958:
	pushq	%r15
LCFI8:
	pushq	%r14
LCFI9:
	pushq	%r13
LCFI10:
	pushq	%r12
LCFI11:
	pushq	%rbp
LCFI12:
	pushq	%rbx
LCFI13:
	subq	$40, %rsp
LCFI14:
	movq	%rdi, %r14
	movq	%rsi, %r15
	movq	%rdx, 8(%rsp)
	movq	%rcx, (%rsp)
	movq	%rdx, %rbp
	testq	%rdx, %rdx
	je	L1136
	movq	8(%rdi), %r13
	movq	16(%rdi), %rax
	subq	%r13, %rax
	sarq	$2, %rax
	cmpq	%rax, %rdx
	ja	L1077
	movl	(%rcx), %r12d
	movq	%r13, %rbx
	subq	%rsi, %rbx
	sarq	$2, %rbx
	cmpq	%rbx, %rdx
	jb	L1159
	movq	%rdx, %r8
	subq	%rbx, %r8
	je	L1092
	movq	%r13, %rdx
	andl	$15, %edx
	shrq	$2, %rdx
	negq	%rdx
	andl	$3, %edx
	cmpq	%r8, %rdx
	cmova	%r8, %rdx
	movq	%r8, %rsi
	movq	%r13, %rcx
	testq	%rdx, %rdx
	je	L1096
	.align 4
L1097:
	movl	%r12d, (%rcx)
	decq	%rsi
	addq	$4, %rcx
	movq	%r8, %rax
	subq	%rsi, %rax
	cmpq	%rdx, %rax
	jb	L1097
	cmpq	%rdx, %r8
	je	L1092
L1096:
	movq	%r8, %r11
	subq	%rdx, %r11
	movq	%r11, %r9
	shrq	$2, %r9
	leaq	0(,%r9,4), %r10
	testq	%r10, %r10
	je	L1138
	movd	%r12d, %xmm1
	pshufd	$0, %xmm1, %xmm0
	leaq	(%r13,%rdx,4), %rdi
	xorl	%edx, %edx
	.align 4
L1101:
	movq	%rdx, %rax
	salq	$4, %rax
	movdqa	%xmm0, (%rax,%rdi)
	incq	%rdx
	cmpq	%rdx, %r9
	ja	L1101
	leaq	(%rcx,%r10,4), %rcx
	subq	%r10, %rsi
	cmpq	%r11, %r10
	je	L1092
	.align 4
L1138:
	movl	%r12d, (%rcx)
	addq	$4, %rcx
	decq	%rsi
	jne	L1138
L1092:
	leaq	(%r13,%r8,4), %rdi
	movq	%rdi, 8(%r14)
	movq	%r13, %rbp
	subq	%r15, %rbp
	movq	%rbp, %rdx
	movq	%r15, %rsi
	call	_memmove
	leaq	0(,%rbx,4), %rax
	addq	%rax, 8(%r14)
	cmpq	%r13, %r15
	je	L1136
	leaq	-4(%rbp), %rax
	shrq	$2, %rax
	leaq	1(%rax), %rdx
	movq	%r15, %rax
	andl	$15, %eax
	shrq	$2, %rax
	negq	%rax
	andl	$3, %eax
	cmpq	%rax, %rdx
	movq	%rax, %rcx
	cmovbe	%rdx, %rcx
	movq	%r15, %rdi
	testq	%rcx, %rcx
	je	L1106
	xorl	%eax, %eax
	.align 4
L1107:
	movl	%r12d, (%rdi)
	addq	$4, %rdi
	incq	%rax
	cmpq	%rax, %rcx
	ja	L1107
	cmpq	%rdx, %rcx
	je	L1136
L1106:
	subq	%rcx, %rdx
	movq	%rdx, %r8
	shrq	$2, %r8
	leaq	0(,%r8,4), %r9
	testq	%r9, %r9
	je	L1109
	movd	%r12d, %xmm1
	pshufd	$0, %xmm1, %xmm0
	leaq	(%r15,%rcx,4), %rsi
	xorl	%ecx, %ecx
	.align 4
L1111:
	movq	%rcx, %rax
	salq	$4, %rax
	movdqa	%xmm0, (%rax,%rsi)
	incq	%rcx
	cmpq	%rcx, %r8
	ja	L1111
	leaq	(%rdi,%r9,4), %rdi
	cmpq	%r9, %rdx
	je	L1136
L1109:
	movq	%rdi, %rax
	.align 4
L1113:
	movl	%r12d, (%rax)
	addq	$4, %rax
	cmpq	%rax, %r13
	jne	L1113
	.align 4
L1136:
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
	.align 4
L1077:
	movq	%r13, %rcx
	subq	(%rdi), %rcx
	sarq	$2, %rcx
	movabsq	$4611686018427387903, %rsi
	movq	%rsi, %rax
	subq	%rcx, %rax
	cmpq	%rax, %rdx
	ja	L1160
	leaq	8(%rsp), %rax
	leaq	24(%rsp), %rdx
	cmpq	%rcx, %rbp
	cmovbe	%rdx, %rax
	movq	%rcx, 24(%rsp)
	movq	(%rax), %rdx
	leaq	(%rcx,%rdx), %rax
	movq	$-4, %r13
	cmpq	%rax, %rcx
	jbe	L1161
L1123:
	movq	%r13, %rdi
	call	__Znwm
	movq	%rax, %r12
	movq	(%r14), %rsi
	movq	%r15, %rbx
	subq	%rsi, %rbx
	movq	%rbx, %rdx
	movq	%rax, %rdi
	call	_memmove
	leaq	(%r12,%rbx), %rdi
	movq	(%rsp), %rax
	movl	(%rax), %ecx
	movq	%rdi, %rdx
	andl	$15, %edx
	shrq	$2, %rdx
	negq	%rdx
	andl	$3, %edx
	cmpq	%rbp, %rdx
	cmova	%rbp, %rdx
	movq	%rbp, %r8
	movq	%rdi, %rsi
	testq	%rdx, %rdx
	je	L1126
	.align 4
L1127:
	movl	%ecx, (%rsi)
	decq	%r8
	addq	$4, %rsi
	movq	%rbp, %rax
	subq	%r8, %rax
	cmpq	%rdx, %rax
	jb	L1127
	cmpq	%rdx, %rbp
	je	L1129
L1126:
	movq	%rbp, %rbx
	subq	%rdx, %rbx
	movq	%rbx, %r10
	shrq	$2, %r10
	leaq	0(,%r10,4), %r11
	testq	%r11, %r11
	je	L1137
	movd	%ecx, %xmm1
	pshufd	$0, %xmm1, %xmm0
	leaq	(%rdi,%rdx,4), %r9
	xorl	%edx, %edx
	.align 4
L1132:
	movq	%rdx, %rax
	salq	$4, %rax
	movdqa	%xmm0, (%rax,%r9)
	incq	%rdx
	cmpq	%r10, %rdx
	jb	L1132
	leaq	(%rsi,%r11,4), %rsi
	subq	%r11, %r8
	cmpq	%rbx, %r11
	je	L1129
	.align 4
L1137:
	movl	%ecx, (%rsi)
	addq	$4, %rsi
	decq	%r8
	jne	L1137
L1129:
	leaq	(%rdi,%rbp,4), %rbp
	movq	8(%r14), %rbx
	subq	%r15, %rbx
	movq	%rbx, %rdx
	movq	%r15, %rsi
	movq	%rbp, %rdi
	call	_memmove
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	je	L1134
	call	__ZdlPv
L1134:
	movq	%r12, (%r14)
	leaq	(%rbp,%rbx), %rax
	movq	%rax, 8(%r14)
	leaq	(%r12,%r13), %rax
	movq	%rax, 16(%r14)
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
L1161:
	cmpq	%rsi, %rax
	ja	L1121
	leaq	0(,%rax,4), %r13
	jmp	L1123
L1159:
	leaq	0(,%rdx,4), %rbx
	movq	%r13, %rbp
	subq	%rbx, %rbp
	movq	%r13, %rdx
	subq	%rbp, %rdx
	movq	%rbp, %rsi
	movq	%r13, %rdi
	call	_memmove
	addq	%rbx, 8(%r14)
	movq	%rbp, %rdx
	subq	%r15, %rdx
	andq	$-4, %rdx
	subq	%rdx, %r13
	movq	%r15, %rsi
	movq	%r13, %rdi
	call	_memmove
	leaq	(%r15,%rbx), %r9
	cmpq	%r9, %r15
	je	L1136
	movq	%r9, %rax
	subq	%r15, %rax
	subq	$4, %rax
	shrq	$2, %rax
	leaq	1(%rax), %rsi
	movq	%r15, %rax
	andl	$15, %eax
	shrq	$2, %rax
	negq	%rax
	andl	$3, %eax
	cmpq	%rax, %rsi
	movq	%rax, %rdx
	cmovbe	%rsi, %rdx
	movq	%r15, %rcx
	testq	%rdx, %rdx
	je	L1084
	xorl	%eax, %eax
	.align 4
L1085:
	movl	%r12d, (%rcx)
	addq	$4, %rcx
	incq	%rax
	cmpq	%rdx, %rax
	jb	L1085
	cmpq	%rsi, %rdx
	je	L1136
L1084:
	movq	%rsi, %r10
	subq	%rdx, %r10
	movq	%r10, %rdi
	shrq	$2, %rdi
	leaq	0(,%rdi,4), %r8
	testq	%r8, %r8
	je	L1087
	movd	%r12d, %xmm1
	pshufd	$0, %xmm1, %xmm0
	leaq	(%r15,%rdx,4), %rsi
	xorl	%edx, %edx
	.align 4
L1089:
	movq	%rdx, %rax
	salq	$4, %rax
	movdqa	%xmm0, (%rax,%rsi)
	incq	%rdx
	cmpq	%rdi, %rdx
	jb	L1089
	leaq	(%rcx,%r8,4), %rcx
	cmpq	%r10, %r8
	je	L1136
L1087:
	movq	%rcx, %rax
	.align 4
L1091:
	movl	%r12d, (%rax)
	addq	$4, %rax
	cmpq	%rax, %r9
	jne	L1091
	jmp	L1136
L1121:
	call	__ZSt17__throw_bad_allocv
L1160:
	leaq	LC0(%rip), %rdi
	call	__ZSt20__throw_length_errorPKc
LFE958:
	.align 1
	.align 4
.globl __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE9_M_insertEPSt18_Rb_tree_node_baseSD_RKS5_
	.weak_definition __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE9_M_insertEPSt18_Rb_tree_node_baseSD_RKS5_
__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE9_M_insertEPSt18_Rb_tree_node_baseSD_RKS5_:
LFB1016:
	movq	%rbx, -48(%rsp)
LCFI15:
	movq	%rbp, -40(%rsp)
LCFI16:
	movq	%r12, -32(%rsp)
LCFI17:
	movq	%r13, -24(%rsp)
LCFI18:
	movq	%r14, -16(%rsp)
LCFI19:
	movq	%r15, -8(%rsp)
LCFI20:
	subq	$72, %rsp
LCFI21:
	movq	%rdi, %r14
	movq	%rdx, %r15
	movq	%rcx, %r13
	testq	%rsi, %rsi
	je	L1185
L1163:
	movb	$1, 15(%rsp)
L1166:
	movl	$64, %edi
LEHB0:
	call	__Znwm
LEHE0:
	movq	%rax, %rbp
	cmpq	$-32, %rax
	je	L1167
	movl	(%r13), %eax
	movl	%eax, 32(%rbp)
	movq	16(%r13), %rdx
	subq	8(%r13), %rdx
	sarq	$2, %rdx
	movq	$0, 40(%rbp)
	movq	$0, 48(%rbp)
	movq	$0, 56(%rbp)
	testq	%rdx, %rdx
	je	L1169
	movabsq	$4611686018427387903, %rax
	cmpq	%rax, %rdx
	ja	L1186
	leaq	0(,%rdx,4), %rbx
	movq	%rbx, %rdi
LEHB1:
	call	__Znwm
LEHE1:
	movq	%rax, 40(%rbp)
	movq	%rax, 48(%rbp)
	addq	%rax, %rbx
	movq	%rbx, 56(%rbp)
L1169:
	movq	40(%rbp), %rbx
	movq	16(%r13), %r12
	movq	8(%r13), %rsi
	subq	%rsi, %r12
	movq	%r12, %rdx
	movq	%rbx, %rdi
	call	_memmove
	addq	%r12, %rbx
	movq	%rbx, 48(%rbp)
L1167:
	leaq	8(%r14), %rcx
	movzbl	15(%rsp), %edi
	movq	%r15, %rdx
	movq	%rbp, %rsi
LEHB2:
	call	__ZSt29_Rb_tree_insert_and_rebalancebPSt18_Rb_tree_node_baseS0_RS_
LEHE2:
	incq	40(%r14)
	movq	%rbp, %rax
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	movq	40(%rsp), %r12
	movq	48(%rsp), %r13
	movq	56(%rsp), %r14
	movq	64(%rsp), %r15
	addq	$72, %rsp
	ret
	.align 4
L1185:
	leaq	8(%rdi), %rax
	cmpq	%rdx, %rax
	je	L1163
	movl	(%rcx), %eax
	cmpl	32(%rdx), %eax
	setl	15(%rsp)
	jmp	L1166
L1186:
LEHB3:
	call	__ZSt17__throw_bad_allocv
LEHE3:
L1180:
L1173:
L1174:
L1183:
L1175:
L1176:
	movq	%rax, %rdi
	call	___cxa_begin_catch
	movq	%rbp, %rdi
	call	__ZdlPv
LEHB4:
	call	___cxa_rethrow
LEHE4:
L1181:
L1184:
L1182:
	movq	%rax, %rbx
L1177:
	call	___cxa_end_catch
	movq	%rbx, %rdi
LEHB5:
	call	__Unwind_Resume
LEHE5:
LFE1016:
	.section __TEXT,__gcc_except_tab
	.align 2
GCC_except_table0:
LLSDA1016:
	.byte	0xff
	.byte	0x9b
	.byte	0x59
	.byte	0x3
	.byte	0x4e
	.set L$set$0,LEHB0-LFB1016
	.long L$set$0
	.set L$set$1,LEHE0-LEHB0
	.long L$set$1
	.long	0x0
	.byte	0x0
	.set L$set$2,LEHB1-LFB1016
	.long L$set$2
	.set L$set$3,LEHE1-LEHB1
	.long L$set$3
	.set L$set$4,L1180-LFB1016
	.long L$set$4
	.byte	0x3
	.set L$set$5,LEHB2-LFB1016
	.long L$set$5
	.set L$set$6,LEHE2-LEHB2
	.long L$set$6
	.long	0x0
	.byte	0x0
	.set L$set$7,LEHB3-LFB1016
	.long L$set$7
	.set L$set$8,LEHE3-LEHB3
	.long L$set$8
	.set L$set$9,L1180-LFB1016
	.long L$set$9
	.byte	0x3
	.set L$set$10,LEHB4-LFB1016
	.long L$set$10
	.set L$set$11,LEHE4-LEHB4
	.long L$set$11
	.set L$set$12,L1182-LFB1016
	.long L$set$12
	.byte	0x0
	.set L$set$13,LEHB5-LFB1016
	.long L$set$13
	.set L$set$14,LEHE5-LEHB5
	.long L$set$14
	.long	0x0
	.byte	0x0
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.byte	0x7d
	.align 2
	.long	0

	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
	.align 1
	.align 4
.globl __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE16_M_insert_uniqueERKS5_
	.weak_definition __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE16_M_insert_uniqueERKS5_
__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE16_M_insert_uniqueERKS5_:
LFB1017:
	pushq	%r12
LCFI22:
	pushq	%rbp
LCFI23:
	pushq	%rbx
LCFI24:
	subq	$80, %rsp
LCFI25:
	movq	%rdi, %rbp
	movq	%rsi, %r12
	movq	16(%rdi), %rax
	leaq	8(%rdi), %rbx
	testq	%rax, %rax
	je	L1188
	movq	%rax, %rdx
	movl	(%rsi), %ecx
	jmp	L1190
	.align 4
L1204:
	movq	16(%rdx), %rax
	testq	%rax, %rax
	je	L1194
L1205:
	movq	%rax, %rdx
L1190:
	cmpl	%ecx, 32(%rdx)
	setg	%sil
	jg	L1204
	movq	24(%rdx), %rax
	testq	%rax, %rax
	jne	L1205
L1194:
	movq	%rdx, %rbx
	movq	%rdx, %rax
	testb	%sil, %sil
	jne	L1188
	cmpl	32(%rax), %ecx
	jg	L1206
L1201:
	movq	%rax, 32(%rsp)
	xorl	%edx, %edx
L1200:
	movb	%dl, 24(%rsp)
	movq	24(%rsp), %rdx
	addq	$80, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
L1188:
	cmpq	24(%rbp), %rbx
	jne	L1198
	movq	%r12, %rcx
	movq	%rbx, %rdx
	xorl	%esi, %esi
	movq	%rbp, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE9_M_insertEPSt18_Rb_tree_node_baseSD_RKS5_
	movq	%rax, 64(%rsp)
	movl	$1, %edx
	jmp	L1200
L1198:
	movq	%rbx, %rdi
	call	__ZSt18_Rb_tree_decrementPSt18_Rb_tree_node_base
	movl	(%r12), %ecx
	cmpl	32(%rax), %ecx
	jle	L1201
L1206:
	movq	%r12, %rcx
	movq	%rbx, %rdx
	xorl	%esi, %esi
	movq	%rbp, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE9_M_insertEPSt18_Rb_tree_node_baseSD_RKS5_
	movq	%rax, 48(%rsp)
	movl	$1, %edx
	jmp	L1200
LFE1017:
	.align 1
	.align 4
.globl __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE16_M_insert_uniqueESt17_Rb_tree_iteratorIS5_ERKS5_
	.weak_definition __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE16_M_insert_uniqueESt17_Rb_tree_iteratorIS5_ERKS5_
__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE16_M_insert_uniqueESt17_Rb_tree_iteratorIS5_ERKS5_:
LFB948:
	movq	%rbx, -24(%rsp)
LCFI26:
	movq	%rbp, -16(%rsp)
LCFI27:
	movq	%r12, -8(%rsp)
LCFI28:
	subq	$88, %rsp
LCFI29:
	movq	%rdi, %r12
	movq	%rdx, %rbp
	movq	%rsi, %rbx
	leaq	8(%rdi), %rax
	cmpq	%rsi, %rax
	je	L1232
	movl	32(%rsi), %eax
	cmpl	%eax, (%rdx)
	jge	L1214
	cmpq	%rsi, 24(%rdi)
	je	L1220
	movq	%rsi, %rdi
	call	__ZSt18_Rb_tree_decrementPSt18_Rb_tree_node_base
	movq	%rax, %rdx
	movl	32(%rax), %eax
	cmpl	(%rbp), %eax
	jge	L1218
	cmpq	$0, 24(%rdx)
	je	L1230
L1220:
	movq	%rbp, %rcx
	movq	%rbx, %rdx
	movq	%rbx, %rsi
	movq	%r12, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE9_M_insertEPSt18_Rb_tree_node_baseSD_RKS5_
	movq	%rax, %rbx
	.align 4
L1213:
	movq	%rbx, %rax
	movq	64(%rsp), %rbx
	movq	72(%rsp), %rbp
	movq	80(%rsp), %r12
	addq	$88, %rsp
	ret
	.align 4
L1214:
	jle	L1213
	cmpq	32(%rdi), %rsi
	je	L1231
	movq	%rsi, %rdi
	call	__ZSt18_Rb_tree_incrementPSt18_Rb_tree_node_base
	movq	%rax, %rsi
	movl	(%rbp), %eax
	cmpl	32(%rsi), %eax
	jge	L1225
	cmpq	$0, 24(%rbx)
	je	L1231
	movq	%rbp, %rcx
	movq	%rsi, %rdx
	movq	%r12, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE9_M_insertEPSt18_Rb_tree_node_baseSD_RKS5_
	movq	%rax, %rbx
	jmp	L1213
L1230:
	movq	%rbp, %rcx
	xorl	%esi, %esi
	movq	%r12, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE9_M_insertEPSt18_Rb_tree_node_baseSD_RKS5_
	movq	%rax, %rbx
	jmp	L1213
L1231:
	movq	%rbp, %rcx
	movq	%rbx, %rdx
	xorl	%esi, %esi
	movq	%r12, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE9_M_insertEPSt18_Rb_tree_node_baseSD_RKS5_
	movq	%rax, %rbx
	jmp	L1213
	.align 4
L1232:
	cmpq	$0, 40(%rdi)
	je	L1210
	movq	32(%rdi), %rdx
	movl	32(%rdx), %eax
	cmpl	(%rbp), %eax
	jl	L1230
L1210:
	movq	%rbp, %rsi
	movq	%r12, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE16_M_insert_uniqueERKS5_
	movq	%rax, (%rsp)
	movq	%rdx, 8(%rsp)
	movq	%rax, 48(%rsp)
	movzbl	8(%rsp), %edx
	movb	%dl, 56(%rsp)
	movq	%rax, %rbx
	jmp	L1213
L1218:
	movq	%rbp, %rsi
	movq	%r12, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE16_M_insert_uniqueERKS5_
	movq	%rax, (%rsp)
	movq	%rdx, 8(%rsp)
	movq	%rax, 32(%rsp)
	movzbl	8(%rsp), %edx
	movb	%dl, 40(%rsp)
	movq	%rax, %rbx
	jmp	L1213
L1225:
	movq	%rbp, %rsi
	movq	%r12, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE16_M_insert_uniqueERKS5_
	movq	%rax, (%rsp)
	movq	%rdx, 8(%rsp)
	movq	%rax, 16(%rsp)
	movzbl	8(%rsp), %edx
	movb	%dl, 24(%rsp)
	movq	%rax, %rbx
	jmp	L1213
LFE948:
	.align 1
	.align 4
.globl __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE9_M_insertEPSt18_Rb_tree_node_baseSE_RKS6_
	.weak_definition __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE9_M_insertEPSt18_Rb_tree_node_baseSE_RKS6_
__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE9_M_insertEPSt18_Rb_tree_node_baseSE_RKS6_:
LFB981:
	pushq	%r15
LCFI30:
	pushq	%r14
LCFI31:
	pushq	%r13
LCFI32:
	pushq	%r12
LCFI33:
	pushq	%rbp
LCFI34:
	pushq	%rbx
LCFI35:
	subq	$8, %rsp
LCFI36:
	movq	%rdi, %r13
	movq	%rdx, %r14
	movq	%rcx, %r12
	testq	%rsi, %rsi
	je	L1262
L1234:
	movl	$1, %r15d
L1237:
	movl	$64, %edi
LEHB6:
	call	__Znwm
	movq	%rax, %rbx
	cmpq	$-32, %rax
	je	L1238
	movl	(%r12), %eax
	movl	%eax, 32(%rbx)
	movq	16(%r12), %rdx
	subq	8(%r12), %rdx
	sarq	$3, %rdx
	movq	$0, 40(%rbx)
	movq	$0, 48(%rbx)
	movq	$0, 56(%rbx)
	testq	%rdx, %rdx
	jne	L1263
L1240:
	movq	40(%rbx), %rsi
	movq	16(%r12), %rdi
	movq	8(%r12), %rax
	cmpq	%rax, %rdi
	je	L1245
	movq	%rax, %rdx
	.align 4
L1247:
	testq	%rsi, %rsi
	je	L1248
	movq	(%rdx), %rax
	movq	%rax, (%rsi)
L1248:
	addq	$8, %rsi
	addq	$8, %rdx
	cmpq	%rdx, %rdi
	jne	L1247
L1245:
	movq	%rsi, 48(%rbx)
L1238:
	leaq	8(%r13), %rcx
	movzbl	%r15b, %edi
	movq	%r14, %rdx
	movq	%rbx, %rsi
	call	__ZSt29_Rb_tree_insert_and_rebalancebPSt18_Rb_tree_node_baseS0_RS_
LEHE6:
	incq	40(%r13)
	movq	%rbx, %rax
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
L1263:
	movabsq	$2305843009213693951, %rax
	cmpq	%rax, %rdx
	ja	L1264
	leaq	0(,%rdx,8), %rbp
	movq	%rbp, %rdi
LEHB7:
	call	__Znwm
	movq	%rax, 40(%rbx)
	movq	%rax, 48(%rbx)
	addq	%rax, %rbp
	movq	%rbp, 56(%rbx)
	jmp	L1240
L1262:
	leaq	8(%rdi), %rax
	cmpq	%rdx, %rax
	je	L1234
	movl	(%rcx), %eax
	cmpl	32(%rdx), %eax
	setl	%r15b
	jmp	L1237
L1264:
	call	__ZSt17__throw_bad_allocv
LEHE7:
L1256:
L1244:
L1250:
L1259:
L1251:
L1252:
	movq	%rax, %rdi
	call	___cxa_begin_catch
	movq	%rbx, %rdi
	call	__ZdlPv
LEHB8:
	call	___cxa_rethrow
LEHE8:
L1257:
L1260:
L1258:
	movq	%rax, %rbp
L1253:
	call	___cxa_end_catch
	movq	%rbp, %rdi
LEHB9:
	call	__Unwind_Resume
LEHE9:
LFE981:
	.section __TEXT,__gcc_except_tab
	.align 2
GCC_except_table1:
LLSDA981:
	.byte	0xff
	.byte	0x9b
	.byte	0x41
	.byte	0x3
	.byte	0x34
	.set L$set$15,LEHB6-LFB981
	.long L$set$15
	.set L$set$16,LEHE6-LEHB6
	.long L$set$16
	.long	0x0
	.byte	0x0
	.set L$set$17,LEHB7-LFB981
	.long L$set$17
	.set L$set$18,LEHE7-LEHB7
	.long L$set$18
	.set L$set$19,L1256-LFB981
	.long L$set$19
	.byte	0x3
	.set L$set$20,LEHB8-LFB981
	.long L$set$20
	.set L$set$21,LEHE8-LEHB8
	.long L$set$21
	.set L$set$22,L1258-LFB981
	.long L$set$22
	.byte	0x0
	.set L$set$23,LEHB9-LFB981
	.long L$set$23
	.set L$set$24,LEHE9-LEHB9
	.long L$set$24
	.long	0x0
	.byte	0x0
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.byte	0x7d
	.align 2
	.long	0

	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
	.align 1
	.align 4
.globl __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueERKS6_
	.weak_definition __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueERKS6_
__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueERKS6_:
LFB982:
	pushq	%r12
LCFI37:
	pushq	%rbp
LCFI38:
	pushq	%rbx
LCFI39:
	subq	$80, %rsp
LCFI40:
	movq	%rdi, %rbp
	movq	%rsi, %r12
	movq	16(%rdi), %rax
	leaq	8(%rdi), %rbx
	testq	%rax, %rax
	je	L1266
	movq	%rax, %rdx
	movl	(%rsi), %ecx
	jmp	L1268
	.align 4
L1282:
	movq	16(%rdx), %rax
	testq	%rax, %rax
	je	L1272
L1283:
	movq	%rax, %rdx
L1268:
	cmpl	%ecx, 32(%rdx)
	setg	%sil
	jg	L1282
	movq	24(%rdx), %rax
	testq	%rax, %rax
	jne	L1283
L1272:
	movq	%rdx, %rbx
	testb	%sil, %sil
	jne	L1266
	cmpl	32(%rbx), %ecx
	jg	L1284
L1279:
	movq	%rbx, 32(%rsp)
	movq	%rbx, %rax
	xorl	%edx, %edx
L1278:
	movb	%dl, 24(%rsp)
	movq	24(%rsp), %rdx
	addq	$80, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
L1266:
	cmpq	24(%rbp), %rbx
	jne	L1276
	movq	%r12, %rcx
	movq	%rbx, %rdx
	xorl	%esi, %esi
	movq	%rbp, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE9_M_insertEPSt18_Rb_tree_node_baseSE_RKS6_
	movq	%rax, 64(%rsp)
	movl	$1, %edx
	jmp	L1278
L1276:
	movq	%rbx, %rdi
	call	__ZSt18_Rb_tree_decrementPSt18_Rb_tree_node_base
	movl	(%r12), %ecx
	movq	%rbx, %rdx
	movq	%rax, %rbx
	cmpl	32(%rbx), %ecx
	jle	L1279
L1284:
	movq	%r12, %rcx
	xorl	%esi, %esi
	movq	%rbp, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE9_M_insertEPSt18_Rb_tree_node_baseSE_RKS6_
	movq	%rax, 48(%rsp)
	movl	$1, %edx
	jmp	L1278
LFE982:
	.align 1
	.align 4
.globl __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueESt17_Rb_tree_iteratorIS6_ERKS6_
	.weak_definition __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueESt17_Rb_tree_iteratorIS6_ERKS6_
__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueESt17_Rb_tree_iteratorIS6_ERKS6_:
LFB907:
	movq	%rbx, -24(%rsp)
LCFI41:
	movq	%rbp, -16(%rsp)
LCFI42:
	movq	%r12, -8(%rsp)
LCFI43:
	subq	$88, %rsp
LCFI44:
	movq	%rdi, %r12
	movq	%rdx, %rbp
	movq	%rsi, %rbx
	leaq	8(%rdi), %rax
	cmpq	%rsi, %rax
	je	L1310
	movl	32(%rsi), %eax
	cmpl	%eax, (%rdx)
	jge	L1292
	cmpq	%rsi, 24(%rdi)
	je	L1298
	movq	%rsi, %rdi
	call	__ZSt18_Rb_tree_decrementPSt18_Rb_tree_node_base
	movq	%rax, %rdx
	movl	32(%rax), %eax
	cmpl	(%rbp), %eax
	jge	L1296
	cmpq	$0, 24(%rdx)
	je	L1308
L1298:
	movq	%rbp, %rcx
	movq	%rbx, %rdx
	movq	%rbx, %rsi
	movq	%r12, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE9_M_insertEPSt18_Rb_tree_node_baseSE_RKS6_
	movq	%rax, %rbx
	.align 4
L1291:
	movq	%rbx, %rax
	movq	64(%rsp), %rbx
	movq	72(%rsp), %rbp
	movq	80(%rsp), %r12
	addq	$88, %rsp
	ret
	.align 4
L1292:
	jle	L1291
	cmpq	32(%rdi), %rsi
	je	L1309
	movq	%rsi, %rdi
	call	__ZSt18_Rb_tree_incrementPSt18_Rb_tree_node_base
	movq	%rax, %rsi
	movl	(%rbp), %eax
	cmpl	32(%rsi), %eax
	jge	L1303
	cmpq	$0, 24(%rbx)
	je	L1309
	movq	%rbp, %rcx
	movq	%rsi, %rdx
	movq	%r12, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE9_M_insertEPSt18_Rb_tree_node_baseSE_RKS6_
	movq	%rax, %rbx
	jmp	L1291
L1308:
	movq	%rbp, %rcx
	xorl	%esi, %esi
	movq	%r12, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE9_M_insertEPSt18_Rb_tree_node_baseSE_RKS6_
	movq	%rax, %rbx
	jmp	L1291
L1309:
	movq	%rbp, %rcx
	movq	%rbx, %rdx
	xorl	%esi, %esi
	movq	%r12, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE9_M_insertEPSt18_Rb_tree_node_baseSE_RKS6_
	movq	%rax, %rbx
	jmp	L1291
	.align 4
L1310:
	cmpq	$0, 40(%rdi)
	je	L1288
	movq	32(%rdi), %rdx
	movl	32(%rdx), %eax
	cmpl	(%rbp), %eax
	jl	L1308
L1288:
	movq	%rbp, %rsi
	movq	%r12, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueERKS6_
	movq	%rax, (%rsp)
	movq	%rdx, 8(%rsp)
	movq	%rax, 48(%rsp)
	movzbl	8(%rsp), %edx
	movb	%dl, 56(%rsp)
	movq	%rax, %rbx
	jmp	L1291
L1296:
	movq	%rbp, %rsi
	movq	%r12, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueERKS6_
	movq	%rax, (%rsp)
	movq	%rdx, 8(%rsp)
	movq	%rax, 32(%rsp)
	movzbl	8(%rsp), %edx
	movb	%dl, 40(%rsp)
	movq	%rax, %rbx
	jmp	L1291
L1303:
	movq	%rbp, %rsi
	movq	%r12, %rdi
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueERKS6_
	movq	%rax, (%rsp)
	movq	%rdx, 8(%rsp)
	movq	%rax, 16(%rsp)
	movzbl	8(%rsp), %edx
	movb	%dl, 24(%rsp)
	movq	%rax, %rbx
	jmp	L1291
LFE907:
	.text
	.align 4,0x90
.globl __Z11fast9_scorePKhiP2xyii
__Z11fast9_scorePKhiP2xyii:
LFB790:
	pushq	%r15
LCFI45:
	pushq	%r14
LCFI46:
	pushq	%r13
LCFI47:
	pushq	%r12
LCFI48:
	pushq	%rbp
LCFI49:
	pushq	%rbx
LCFI50:
	subq	$168, %rsp
LCFI51:
	movq	%rdi, 24(%rsp)
	movl	%esi, %r13d
	movq	%rdx, 16(%rsp)
	movl	%ecx, %r15d
	movl	%r8d, 12(%rsp)
	movq	_gFASTMem@GOTPCREL(%rip), %r12
	movq	64(%r12), %rax
	leaq	56(%r12), %rbp
	.align 4,0x90
L1346:
	testq	%rax, %rax
	je	L1312
L1347:
	movl	32(%rax), %edx
	testl	%edx, %edx
	jle	L1315
	movq	%rax, %rbp
	movq	16(%rax), %rax
	testq	%rax, %rax
	jne	L1347
L1312:
	leaq	56(%r12), %rax
	cmpq	%rax, %rbp
	je	L1318
	movq	%rbp, %rbx
	cmpl	$1, 32(%rbp)
	jle	L1320
L1318:
	movq	$0, 128(%rsp)
	movq	$0, 136(%rsp)
	movq	$0, 144(%rsp)
	movl	$1, 96(%rsp)
	movq	$0, 104(%rsp)
	movq	$0, 112(%rsp)
	movq	$0, 120(%rsp)
	xorl	%edx, %edx
	xorl	%esi, %esi
	xorl	%edi, %edi
	call	_memmove
	movq	$0, 112(%rsp)
	leaq	96(%rsp), %rdx
	movq	%rbp, %rsi
	leaq	48(%r12), %rdi
LEHB10:
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE16_M_insert_uniqueESt17_Rb_tree_iteratorIS5_ERKS5_
LEHE10:
	movq	%rax, %rbp
	movq	104(%rsp), %rdi
	testq	%rdi, %rdi
	je	L1321
	call	__ZdlPv
L1321:
	movq	128(%rsp), %rdi
	movq	%rbp, %rbx
	testq	%rdi, %rdi
	je	L1320
	call	__ZdlPv
	movq	%rbp, %rbx
L1342:
L1320:
	movslq	%r15d,%rdx
	movq	48(%rbx), %rsi
	movq	40(%rbx), %r12
	movq	%rsi, %rax
	subq	%r12, %rax
	sarq	$2, %rax
	cmpq	%rax, %rdx
	ja	L1348
L1332:
	testl	%r15d, %r15d
	jle	L1336
	leal	(%r13,%r13), %edi
	leal	(%rdi,%r13), %esi
	movl	%esi, %edx
	negl	%edx
	movl	%esi, 32(%rsp)
	leal	1(%rsi), %eax
	movl	%eax, 36(%rsp)
	leal	2(%r13,%r13), %ecx
	movl	%ecx, 40(%rsp)
	leal	3(%r13), %eax
	movl	%eax, 44(%rsp)
	movl	$3, 48(%rsp)
	movl	$3, %eax
	subl	%r13d, %eax
	movl	%eax, 52(%rsp)
	movl	%edi, %eax
	negl	%eax
	addl	$2, %eax
	movl	%eax, 56(%rsp)
	leal	1(%rdx), %eax
	movl	%eax, 60(%rsp)
	movl	%edx, 64(%rsp)
	decl	%edx
	movl	%edx, 68(%rsp)
	negl	%ecx
	movl	%ecx, 72(%rsp)
	movl	$-3, %edx
	movl	%edx, %eax
	subl	%r13d, %eax
	movl	%eax, 76(%rsp)
	movl	%edx, 80(%rsp)
	leal	(%r13,%rdx), %eax
	movl	%eax, 84(%rsp)
	subl	$2, %edi
	movl	%edi, 88(%rsp)
	decl	%esi
	movl	%esi, 92(%rsp)
	xorl	%ebp, %ebp
	xorl	%ebx, %ebx
	leaq	32(%rsp), %r14
	.align 4,0x90
L1338:
	movl	%r13d, %edi
	movq	16(%rsp), %rax
	imull	4(%rax,%rbx,8), %edi
	movslq	%edi,%rdi
	addq	24(%rsp), %rdi
	movslq	(%rax,%rbx,8),%rax
	addq	%rax, %rdi
	movl	12(%rsp), %edx
	movq	%r14, %rsi
	call	__Z18fast9_corner_scorePKhPKii
	movl	%eax, (%r12,%rbx,4)
	incl	%ebp
	incq	%rbx
	cmpl	%r15d, %ebp
	jne	L1338
L1336:
	movq	%r12, %rax
	addq	$168, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
	.align 4,0x90
L1315:
	movq	24(%rax), %rax
	jmp	L1346
L1348:
	movl	$0, 156(%rsp)
	leaq	156(%rsp), %rcx
	subq	%rax, %rdx
	leaq	40(%rbx), %rdi
LEHB11:
	call	__ZNSt6vectorIiSaIiEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPiS1_EEmRKi
	movq	40(%rbx), %r12
	jmp	L1332
L1340:
	movq	%rax, %rbx
L1325:
	movq	104(%rsp), %rdi
	testq	%rdi, %rdi
	je	L1326
	call	__ZdlPv
L1326:
L1341:
L1328:
	movq	128(%rsp), %rdi
	testq	%rdi, %rdi
	je	L1329
	call	__ZdlPv
L1329:
L1331:
	movq	%rbx, %rdi
	call	__Unwind_Resume
LEHE11:
LFE790:
	.section __TEXT,__gcc_except_tab
GCC_except_table2:
LLSDA790:
	.byte	0xff
	.byte	0xff
	.byte	0x3
	.byte	0x1a
	.set L$set$25,LEHB10-LFB790
	.long L$set$25
	.set L$set$26,LEHE10-LEHB10
	.long L$set$26
	.set L$set$27,L1340-LFB790
	.long L$set$27
	.byte	0x0
	.set L$set$28,LEHB11-LFB790
	.long L$set$28
	.set L$set$29,LEHE11-LEHB11
	.long L$set$29
	.long	0x0
	.byte	0x0
	.text
	.align 4,0x90
.globl __Z12fast9_detectPKhiiiiPi
__Z12fast9_detectPKhiiiiPi:
LFB791:
	pushq	%r15
LCFI52:
	pushq	%r14
LCFI53:
	pushq	%r13
LCFI54:
	pushq	%r12
LCFI55:
	pushq	%rbp
LCFI56:
	pushq	%rbx
LCFI57:
	subq	$360, %rsp
LCFI58:
	movq	%rdi, 136(%rsp)
	movl	%esi, %r12d
	movl	%edx, %r13d
	movl	%ecx, 132(%rsp)
	movl	%r8d, 128(%rsp)
	movq	%r9, 120(%rsp)
	movq	_gFASTMem@GOTPCREL(%rip), %rdx
	movq	16(%rdx), %rax
	leaq	8(%rdx), %rcx
	movq	%rcx, 168(%rsp)
	movq	%rcx, %rsi
L2429:
	testq	%rax, %rax
	je	L1352
L2432:
	movl	32(%rax), %r9d
	testl	%r9d, %r9d
	js	L1354
	movq	%rax, %rsi
	movq	16(%rax), %rax
	testq	%rax, %rax
	jne	L2432
L1352:
	movq	_gFASTMem@GOTPCREL(%rip), %rbx
	leaq	8(%rbx), %rax
	cmpq	%rax, %rsi
	je	L1357
	movq	%rsi, %rbx
	movl	32(%rsi), %r8d
	testl	%r8d, %r8d
	jle	L1359
L1357:
	movq	$0, 304(%rsp)
	movq	$0, 312(%rsp)
	movq	$0, 320(%rsp)
	movl	$0, 240(%rsp)
	movq	$0, 248(%rsp)
	movq	$0, 264(%rsp)
L2417:
L1364:
	movq	$0, 256(%rsp)
	leaq	240(%rsp), %rdx
	movq	_gFASTMem@GOTPCREL(%rip), %rdi
LEHB12:
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueESt17_Rb_tree_iteratorIS6_ERKS6_
LEHE12:
	movq	%rax, %rbx
	movq	248(%rsp), %rdi
	testq	%rdi, %rdi
	je	L1369
	call	__ZdlPv
L1369:
	movq	304(%rsp), %rdi
	testq	%rdi, %rdi
	je	L1359
L1379:
	call	__ZdlPv
L2420:
L1359:
	movq	40(%rbx), %rdx
	movq	48(%rbx), %rsi
	movq	%rsi, %rax
	subq	%rdx, %rax
	sarq	$3, %rax
	cmpq	$511, %rax
	ja	L1380
	movl	$0, 340(%rsp)
	movl	$0, 336(%rsp)
	cmpq	$512, %rax
	jbe	L1382
	leaq	4096(%rdx), %rax
	movq	%rax, 48(%rbx)
L1380:
	movq	%rdx, 152(%rsp)
	movl	132(%rsp), %edx
	addl	%edx, %edx
	movl	132(%rsp), %r14d
	leal	(%rdx,%r14), %ecx
	leal	1(%rcx), %r9d
	leal	2(%r14,%r14), %r10d
	movl	%r14d, %r11d
	addl	$3, %r11d
	movl	$3, %edi
	subl	%r14d, %edi
	movl	%edx, %eax
	negl	%eax
	addl	$2, %eax
	movl	%ecx, %esi
	negl	%esi
	leal	1(%rsi), %ebx
	leal	-1(%rsi), %ebp
	movl	%r10d, %r15d
	negl	%r15d
	movl	$-3, %r8d
	subl	%r14d, %r8d
	subl	$3, %r14d
	movl	%r14d, 188(%rsp)
	subl	$2, %edx
	movl	%edx, 184(%rsp)
	leal	-1(%rcx), %edx
	movl	%edx, 180(%rsp)
	leal	-3(%r13), %edx
	cmpl	$3, %edx
	jle	L1384
	subl	$3, %r12d
	movl	%r12d, 192(%rsp)
	movl	%ecx, 196(%rsp)
	movl	$0, 148(%rsp)
	movl	$512, 160(%rsp)
	movl	$3, 164(%rsp)
	movslq	%ecx,%rcx
	movq	%rcx, 112(%rsp)
	movq	_gFASTMem@GOTPCREL(%rip), %r14
	addq	$8, %r14
	movq	%r14, 104(%rsp)
	leaq	336(%rsp), %rcx
	movq	%rcx, 96(%rsp)
	leaq	208(%rsp), %r14
	movq	%r14, 88(%rsp)
	movslq	%ebx,%r13
	movslq	%esi,%r12
	movslq	%ebp,%r14
	cltq
	movq	%rax, 80(%rsp)
	movslq	%r15d,%r15
	movslq	%r8d,%r8
	movq	%r8, 72(%rsp)
	movslq	188(%rsp),%rax
	movq	%rax, 64(%rsp)
	movslq	184(%rsp),%rcx
	movq	%rcx, 56(%rsp)
	movslq	180(%rsp),%rbx
	movq	%rbx, 48(%rsp)
	movslq	%edi,%rdi
	movq	%rdi, 40(%rsp)
	movslq	%r11d,%r11
	movq	%r11, 32(%rsp)
	movslq	%r10d,%r10
	movq	%r10, 24(%rsp)
	movslq	%r9d,%r9
	movq	%r9, 16(%rsp)
	movl	%edx, 12(%rsp)
L1386:
	cmpl	$3, 192(%rsp)
	jle	L2409
	movslq	196(%rsp),%rax
	addq	136(%rsp), %rax
	movq	%rax, 200(%rsp)
	movl	$3, %ebp
	jmp	L1387
	.align 4,0x90
L2434:
	movq	16(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1390
	movq	24(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1392
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1394
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1396
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1398
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1400
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1402
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
L1404:
	movl	160(%rsp), %eax
	cmpl	%eax, 148(%rsp)
	je	L2433
L2371:
	movslq	148(%rsp),%rax
	movq	152(%rsp), %rcx
	leaq	(%rcx,%rax,8), %rax
	movl	%ebp, (%rax)
	movl	164(%rsp), %ebx
	movl	%ebx, 4(%rax)
	incl	148(%rsp)
	.align 4,0x90
L1406:
	incl	%ebp
	cmpl	%ebp, 192(%rsp)
	jle	L2409
L1387:
	movslq	%ebp,%rax
	movq	200(%rsp), %rcx
	addq	%rax, %rcx
	movzbl	(%rcx), %eax
	movl	128(%rsp), %ebx
	leal	(%rax,%rbx), %edx
	movl	%eax, %esi
	subl	%ebx, %esi
	movq	112(%rsp), %rbx
	movzbl	(%rcx,%rbx), %eax
	cmpl	%eax, %edx
	jl	L2434
	cmpl	%eax, %esi
	jle	L1851
	movq	16(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1853
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1855
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1859
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1861
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1863
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1865
	movq	24(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	incl	%ebp
	cmpl	%ebp, 192(%rsp)
	jg	L1387
L2409:
	incl	164(%rsp)
	movl	132(%rsp), %eax
	addl	%eax, 196(%rsp)
	movl	12(%rsp), %edx
	cmpl	%edx, 164(%rsp)
	jne	L1386
L2410:
	movl	148(%rsp), %ebx
	movq	120(%rsp), %rcx
	movl	%ebx, (%rcx)
	movq	152(%rsp), %rax
	addq	$360, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
L1354:
	movq	24(%rax), %rax
	jmp	L2429
	.align 4,0x90
L1390:
	cmpl	%eax, %esi
	jle	L1743
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1745
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1749
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1751
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1753
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1755
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movl	160(%rsp), %eax
	cmpl	%eax, 148(%rsp)
	jne	L2371
L2433:
	movq	_gFASTMem@GOTPCREL(%rip), %rdx
	movq	16(%rdx), %rax
	movq	168(%rsp), %rsi
L2430:
	testq	%rax, %rax
	je	L2375
L2435:
	movl	32(%rax), %edi
	testl	%edi, %edi
	js	L2377
	movq	%rax, %rsi
	movq	16(%rax), %rax
	testq	%rax, %rax
	jne	L2435
L2375:
	cmpq	104(%rsp), %rsi
	je	L2380
	movq	%rsi, %rbx
	movl	32(%rsi), %ecx
	testl	%ecx, %ecx
	jle	L2382
L2380:
	movq	$0, 272(%rsp)
	movq	$0, 280(%rsp)
	movq	$0, 288(%rsp)
	movl	$0, 208(%rsp)
	movq	$0, 216(%rsp)
	movq	$0, 232(%rsp)
L2413:
L2387:
	movq	$0, 224(%rsp)
	movq	88(%rsp), %rdx
	movq	_gFASTMem@GOTPCREL(%rip), %rdi
LEHB13:
	call	__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueESt17_Rb_tree_iteratorIS6_ERKS6_
LEHE13:
	movq	%rax, %rbx
	movq	216(%rsp), %rdi
	testq	%rdi, %rdi
	je	L2394
	call	__ZdlPv
L2394:
	movq	272(%rsp), %rdi
	testq	%rdi, %rdi
	je	L2382
	call	__ZdlPv
L2416:
L2382:
	movl	160(%rsp), %edx
	addl	%edx, %edx
	movl	%edx, 160(%rsp)
	movq	40(%rbx), %rax
	movq	48(%rbx), %rsi
	movq	%rsi, %rcx
	subq	%rax, %rcx
	sarq	$3, %rcx
	movslq	%edx,%rdx
	cmpq	%rcx, %rdx
	jbe	L2405
	movl	$0, 340(%rsp)
	movl	$0, 336(%rsp)
	subq	%rcx, %rdx
	leaq	40(%rbx), %rdi
	movq	96(%rsp), %rcx
LEHB14:
	call	__ZNSt6vectorI2xySaIS0_EE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPS0_S2_EEmRKS0_
	movq	40(%rbx), %rax
L2405:
	movq	%rax, 152(%rsp)
	jmp	L2371
	.align 4,0x90
L1851:
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2312
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2316
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2318
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L2320
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2322
	movq	24(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2324
	movq	16(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1853:
	cmpl	%eax, %esi
	jle	L1910
	movq	24(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1912
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1914
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1919
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1921
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1923
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L2312:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L2345
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L2347
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L2349
	movq	32(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L2351
	movq	24(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L2353
	movq	16(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1743:
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1800
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1804
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1806
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1808
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1810
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1392:
	cmpl	%eax, %esi
	jle	L1652
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1654
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1658
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1660
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1662
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1910:
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2261
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2266
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2268
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L2270
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2272
	movq	24(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1745:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1776
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1778
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1780
	movq	32(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1782
	movq	24(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1855:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1886
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1888
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1890
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1892
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1394:
	cmpl	%eax, %esi
	jle	L1578
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1580
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1584
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1586
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1652:
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1700
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1704
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1706
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1708
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1800:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1832
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1834
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1836
	movq	32(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1838
	movq	24(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1396:
	cmpl	%eax, %esi
	jle	L1506
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1508
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1510
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1513
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1654:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1683
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1685
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1687
	movq	32(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1912:
	cmpl	%eax, %esi
	jle	L1960
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1962
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1964
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1970
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1972
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L2261:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L2288
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L2290
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L2292
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L2294
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1578:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1617
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1621
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1623
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1700:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1730
	movq	40(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1732
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1734
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1580:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1606
	movq	40(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1608
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1914:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1939
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1941
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1943
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1960:
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2218
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2224
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2226
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L2228
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1398:
	cmpl	%eax, %esi
	jle	L1450
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1452
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1454
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1617:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1644
	movq	40(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1646
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L2316:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1506:
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1551
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1555
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1804:
	movq	24(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1551:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1574
	movq	40(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1508:
	cmpl	%eax, %esi
	jle	L1532
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1540
	movq	40(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1859:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1749:
	movq	24(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1962:
	cmpl	%eax, %esi
	jle	L2001
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L2003
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2005
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2013
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1450:
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1487
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L2345:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L2218:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L2240
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L2242
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L2244
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L2318:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1400:
	cmpl	%eax, %esi
	jle	L1419
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1421
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1423
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L2377:
	movq	24(%rax), %rax
	jmp	L2430
L1964:
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1984
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1986
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1861:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L2320:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1402:
	cmpl	%eax, %esi
	jle	L1407
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1409
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L1704:
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1510:
	cmpl	%eax, %esi
	jle	L1406
	movq	40(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1751:
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1886:
	movq	24(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	40(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1452:
	cmpl	%eax, %esi
	jle	L1470
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	48(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L2001:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2183
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2190
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2192
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1776:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	48(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1806:
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1658:
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1532:
	movq	40(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1419:
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1439
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L2347:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1487:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	48(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1439:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	48(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1808:
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1660:
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L2349:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1753:
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1407:
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L1621:
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1454:
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1421:
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1832:
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	48(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1888:
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	40(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L2003:
	cmpl	%eax, %esi
	jle	L2048
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2050
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2052
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1470:
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1584:
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1706:
	movzbl	3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L2266:
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L2288:
	movq	24(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	40(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L2322:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L1863:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1778:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L2183:
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L2201
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L2203
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1780:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2290:
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	40(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2324:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L1409:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	48(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1810:
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L1555:
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1423:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1662:
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L1683:
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	48(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L1834:
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2005:
	cmpl	%eax, %esi
	jle	L2017
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2019
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1890:
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L1708:
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L1623:
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L1939:
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	40(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1513:
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1865:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L2048:
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2156
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2164
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1586:
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L2268:
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L1919:
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L2351:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L1755:
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L1730:
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L1892:
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L1941:
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L2353:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L1782:
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2292:
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2156:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L2171
	movq	48(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L2050:
	cmpl	%eax, %esi
	jle	L2087
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2089
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L2091
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2093
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L2270:
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L1921:
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L2240:
	movq	32(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	40(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1685:
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L1836:
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2017:
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L2089:
	cmpl	%eax, %esi
	jle	L2109
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2111
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2113
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L2201:
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1986:
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L2244:
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1970:
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L2226:
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1608:
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1734:
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L1644:
	movq	64(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	48(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1382:
	leaq	336(%rsp), %rcx
	movl	$512, %edx
	subq	%rax, %rdx
	leaq	40(%rbx), %rdi
	call	__ZNSt6vectorI2xySaIS0_EE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPS0_S2_EEmRKS0_
	movq	40(%rbx), %rdx
	jmp	L1380
L1984:
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L2272:
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L1732:
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L1606:
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	48(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1687:
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2242:
	movzbl	3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1923:
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L2087:
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L2137
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jl	L1404
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L2052:
	cmpl	%eax, %esi
	jle	L2062
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L2064
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L2224:
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1943:
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L2294:
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2019:
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L2029
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L1838:
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2064:
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L2062:
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1972:
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L2137:
	cmpl	%eax, %esi
	jle	L1406
	movq	64(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	48(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	72(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L2091:
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L2228:
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
	.align 4,0x90
L1646:
	movq	64(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L2203:
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
	.align 4,0x90
L2109:
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L2126
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	72(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movzbl	-3(%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L1384:
	movl	$0, 148(%rsp)
	jmp	L2410
L1574:
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2093:
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2418:
	movq	%rax, %rbx
L1373:
	movq	248(%rsp), %rdi
	testq	%rdi, %rdi
	je	L1374
	call	__ZdlPv
L1374:
L2419:
L1376:
	movq	304(%rsp), %rdi
	testq	%rdi, %rdi
	je	L2402
L2428:
	call	__ZdlPv
L2402:
L2404:
	movq	%rbx, %rdi
	call	__Unwind_Resume
LEHE14:
L2414:
	movq	%rax, %rbx
L2398:
	movq	216(%rsp), %rdi
	testq	%rdi, %rdi
	je	L2399
	call	__ZdlPv
L2399:
L2415:
L2401:
	movq	272(%rsp), %rdi
	testq	%rdi, %rdi
	jne	L2428
	jmp	L2402
L2190:
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L1540:
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2029:
	movq	40(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2126:
	cmpl	%eax, %esi
	jle	L1406
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2111:
	cmpl	%eax, %esi
	jle	L2122
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jg	L1404
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2171:
	movq	40(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	80(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r13,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r12,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r14,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movzbl	(%r15,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2122:
	movq	56(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
L2013:
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L2192:
	movq	64(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L2164:
	movq	56(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	movq	48(%rsp), %rbx
	movzbl	(%rbx,%rcx), %eax
	cmpl	%eax, %edx
	jge	L1406
	jmp	L1404
L2113:
	cmpl	%eax, %esi
	jle	L1406
	movq	48(%rsp), %rdx
	movzbl	(%rdx,%rcx), %eax
	cmpl	%eax, %esi
	jle	L1406
	jmp	L1404
LFE791:
	.section __TEXT,__gcc_except_tab
GCC_except_table3:
LLSDA791:
	.byte	0xff
	.byte	0xff
	.byte	0x3
	.byte	0x27
	.set L$set$30,LEHB12-LFB791
	.long L$set$30
	.set L$set$31,LEHE12-LEHB12
	.long L$set$31
	.set L$set$32,L2418-LFB791
	.long L$set$32
	.byte	0x0
	.set L$set$33,LEHB13-LFB791
	.long L$set$33
	.set L$set$34,LEHE13-LEHB13
	.long L$set$34
	.set L$set$35,L2414-LFB791
	.long L$set$35
	.byte	0x0
	.set L$set$36,LEHB14-LFB791
	.long L$set$36
	.set L$set$37,LEHE14-LEHB14
	.long L$set$37
	.long	0x0
	.byte	0x0
	.text
	.section __TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
EH_frame1:
	.set L$set$38,LECIE1-LSCIE1
	.long L$set$38
LSCIE1:
	.long	0x0
	.byte	0x1
	.ascii "zPLR\0"
	.byte	0x1
	.byte	0x78
	.byte	0x10
	.byte	0x7
	.byte	0x9b
	.long	___gxx_personality_v0+4@GOTPCREL
	.byte	0x10
	.byte	0x10
	.byte	0xc
	.byte	0x7
	.byte	0x8
	.byte	0x90
	.byte	0x1
	.align 3
LECIE1:
.globl __Z18fast9_corner_scorePKhPKii.eh
__Z18fast9_corner_scorePKhPKii.eh:
LSFDE1:
	.set L$set$39,LEFDE1-LASFDE1
	.long L$set$39
LASFDE1:
	.long	LASFDE1-EH_frame1
	.quad	LFB788-.
	.set L$set$40,LFE788-LFB788
	.quad L$set$40
	.byte	0x8
	.quad	0x0
	.byte	0x4
	.set L$set$41,LCFI0-LFB788
	.long L$set$41
	.byte	0xe
	.byte	0x10
	.byte	0x83
	.byte	0x2
	.align 3
LEFDE1:
.globl __ZNSt6vectorI2xySaIS0_EE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPS0_S2_EEmRKS0_.eh
	.weak_definition __ZNSt6vectorI2xySaIS0_EE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPS0_S2_EEmRKS0_.eh
__ZNSt6vectorI2xySaIS0_EE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPS0_S2_EEmRKS0_.eh:
LSFDE3:
	.set L$set$42,LEFDE3-LASFDE3
	.long L$set$42
LASFDE3:
	.long	LASFDE3-EH_frame1
	.quad	LFB917-.
	.set L$set$43,LFE917-LFB917
	.quad L$set$43
	.byte	0x8
	.quad	0x0
	.byte	0x4
	.set L$set$44,LCFI1-LFB917
	.long L$set$44
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$45,LCFI2-LCFI1
	.long L$set$45
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$46,LCFI3-LCFI2
	.long L$set$46
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$47,LCFI4-LCFI3
	.long L$set$47
	.byte	0xe
	.byte	0x28
	.byte	0x4
	.set L$set$48,LCFI5-LCFI4
	.long L$set$48
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.set L$set$49,LCFI6-LCFI5
	.long L$set$49
	.byte	0xe
	.byte	0x38
	.byte	0x4
	.set L$set$50,LCFI7-LCFI6
	.long L$set$50
	.byte	0xe
	.byte	0x60
	.byte	0x83
	.byte	0x7
	.byte	0x86
	.byte	0x6
	.byte	0x8c
	.byte	0x5
	.byte	0x8d
	.byte	0x4
	.byte	0x8e
	.byte	0x3
	.byte	0x8f
	.byte	0x2
	.align 3
LEFDE3:
.globl __ZNSt6vectorIiSaIiEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPiS1_EEmRKi.eh
	.weak_definition __ZNSt6vectorIiSaIiEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPiS1_EEmRKi.eh
__ZNSt6vectorIiSaIiEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPiS1_EEmRKi.eh:
LSFDE5:
	.set L$set$51,LEFDE5-LASFDE5
	.long L$set$51
LASFDE5:
	.long	LASFDE5-EH_frame1
	.quad	LFB958-.
	.set L$set$52,LFE958-LFB958
	.quad L$set$52
	.byte	0x8
	.quad	0x0
	.byte	0x4
	.set L$set$53,LCFI8-LFB958
	.long L$set$53
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$54,LCFI9-LCFI8
	.long L$set$54
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$55,LCFI10-LCFI9
	.long L$set$55
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$56,LCFI11-LCFI10
	.long L$set$56
	.byte	0xe
	.byte	0x28
	.byte	0x4
	.set L$set$57,LCFI12-LCFI11
	.long L$set$57
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.set L$set$58,LCFI13-LCFI12
	.long L$set$58
	.byte	0xe
	.byte	0x38
	.byte	0x4
	.set L$set$59,LCFI14-LCFI13
	.long L$set$59
	.byte	0xe
	.byte	0x60
	.byte	0x83
	.byte	0x7
	.byte	0x86
	.byte	0x6
	.byte	0x8c
	.byte	0x5
	.byte	0x8d
	.byte	0x4
	.byte	0x8e
	.byte	0x3
	.byte	0x8f
	.byte	0x2
	.align 3
LEFDE5:
.globl __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE9_M_insertEPSt18_Rb_tree_node_baseSD_RKS5_.eh
	.weak_definition __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE9_M_insertEPSt18_Rb_tree_node_baseSD_RKS5_.eh
__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE9_M_insertEPSt18_Rb_tree_node_baseSD_RKS5_.eh:
LSFDE7:
	.set L$set$60,LEFDE7-LASFDE7
	.long L$set$60
LASFDE7:
	.long	LASFDE7-EH_frame1
	.quad	LFB1016-.
	.set L$set$61,LFE1016-LFB1016
	.quad L$set$61
	.byte	0x8
	.quad	LLSDA1016-.
	.byte	0x4
	.set L$set$62,LCFI21-LFB1016
	.long L$set$62
	.byte	0xe
	.byte	0x50
	.byte	0x8f
	.byte	0x2
	.byte	0x8e
	.byte	0x3
	.byte	0x8d
	.byte	0x4
	.byte	0x8c
	.byte	0x5
	.byte	0x86
	.byte	0x6
	.byte	0x83
	.byte	0x7
	.align 3
LEFDE7:
.globl __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE16_M_insert_uniqueERKS5_.eh
	.weak_definition __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE16_M_insert_uniqueERKS5_.eh
__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE16_M_insert_uniqueERKS5_.eh:
LSFDE9:
	.set L$set$63,LEFDE9-LASFDE9
	.long L$set$63
LASFDE9:
	.long	LASFDE9-EH_frame1
	.quad	LFB1017-.
	.set L$set$64,LFE1017-LFB1017
	.quad L$set$64
	.byte	0x8
	.quad	0x0
	.byte	0x4
	.set L$set$65,LCFI22-LFB1017
	.long L$set$65
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$66,LCFI23-LCFI22
	.long L$set$66
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$67,LCFI24-LCFI23
	.long L$set$67
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$68,LCFI25-LCFI24
	.long L$set$68
	.byte	0xe
	.byte	0x70
	.byte	0x83
	.byte	0x4
	.byte	0x86
	.byte	0x3
	.byte	0x8c
	.byte	0x2
	.align 3
LEFDE9:
.globl __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE16_M_insert_uniqueESt17_Rb_tree_iteratorIS5_ERKS5_.eh
	.weak_definition __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE16_M_insert_uniqueESt17_Rb_tree_iteratorIS5_ERKS5_.eh
__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorIiSaIiEEESt10_Select1stIS5_ESt4lessIiESaIS5_EE16_M_insert_uniqueESt17_Rb_tree_iteratorIS5_ERKS5_.eh:
LSFDE11:
	.set L$set$69,LEFDE11-LASFDE11
	.long L$set$69
LASFDE11:
	.long	LASFDE11-EH_frame1
	.quad	LFB948-.
	.set L$set$70,LFE948-LFB948
	.quad L$set$70
	.byte	0x8
	.quad	0x0
	.byte	0x4
	.set L$set$71,LCFI29-LFB948
	.long L$set$71
	.byte	0xe
	.byte	0x60
	.byte	0x8c
	.byte	0x2
	.byte	0x86
	.byte	0x3
	.byte	0x83
	.byte	0x4
	.align 3
LEFDE11:
.globl __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE9_M_insertEPSt18_Rb_tree_node_baseSE_RKS6_.eh
	.weak_definition __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE9_M_insertEPSt18_Rb_tree_node_baseSE_RKS6_.eh
__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE9_M_insertEPSt18_Rb_tree_node_baseSE_RKS6_.eh:
LSFDE13:
	.set L$set$72,LEFDE13-LASFDE13
	.long L$set$72
LASFDE13:
	.long	LASFDE13-EH_frame1
	.quad	LFB981-.
	.set L$set$73,LFE981-LFB981
	.quad L$set$73
	.byte	0x8
	.quad	LLSDA981-.
	.byte	0x4
	.set L$set$74,LCFI30-LFB981
	.long L$set$74
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$75,LCFI31-LCFI30
	.long L$set$75
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$76,LCFI32-LCFI31
	.long L$set$76
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$77,LCFI33-LCFI32
	.long L$set$77
	.byte	0xe
	.byte	0x28
	.byte	0x4
	.set L$set$78,LCFI34-LCFI33
	.long L$set$78
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.set L$set$79,LCFI35-LCFI34
	.long L$set$79
	.byte	0xe
	.byte	0x38
	.byte	0x4
	.set L$set$80,LCFI36-LCFI35
	.long L$set$80
	.byte	0xe
	.byte	0x40
	.byte	0x83
	.byte	0x7
	.byte	0x86
	.byte	0x6
	.byte	0x8c
	.byte	0x5
	.byte	0x8d
	.byte	0x4
	.byte	0x8e
	.byte	0x3
	.byte	0x8f
	.byte	0x2
	.align 3
LEFDE13:
.globl __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueERKS6_.eh
	.weak_definition __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueERKS6_.eh
__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueERKS6_.eh:
LSFDE15:
	.set L$set$81,LEFDE15-LASFDE15
	.long L$set$81
LASFDE15:
	.long	LASFDE15-EH_frame1
	.quad	LFB982-.
	.set L$set$82,LFE982-LFB982
	.quad L$set$82
	.byte	0x8
	.quad	0x0
	.byte	0x4
	.set L$set$83,LCFI37-LFB982
	.long L$set$83
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$84,LCFI38-LCFI37
	.long L$set$84
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$85,LCFI39-LCFI38
	.long L$set$85
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$86,LCFI40-LCFI39
	.long L$set$86
	.byte	0xe
	.byte	0x70
	.byte	0x83
	.byte	0x4
	.byte	0x86
	.byte	0x3
	.byte	0x8c
	.byte	0x2
	.align 3
LEFDE15:
.globl __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueESt17_Rb_tree_iteratorIS6_ERKS6_.eh
	.weak_definition __ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueESt17_Rb_tree_iteratorIS6_ERKS6_.eh
__ZNSt8_Rb_treeIiSt4pairIKiSt6vectorI2xySaIS3_EEESt10_Select1stIS6_ESt4lessIiESaIS6_EE16_M_insert_uniqueESt17_Rb_tree_iteratorIS6_ERKS6_.eh:
LSFDE17:
	.set L$set$87,LEFDE17-LASFDE17
	.long L$set$87
LASFDE17:
	.long	LASFDE17-EH_frame1
	.quad	LFB907-.
	.set L$set$88,LFE907-LFB907
	.quad L$set$88
	.byte	0x8
	.quad	0x0
	.byte	0x4
	.set L$set$89,LCFI44-LFB907
	.long L$set$89
	.byte	0xe
	.byte	0x60
	.byte	0x8c
	.byte	0x2
	.byte	0x86
	.byte	0x3
	.byte	0x83
	.byte	0x4
	.align 3
LEFDE17:
.globl __Z11fast9_scorePKhiP2xyii.eh
__Z11fast9_scorePKhiP2xyii.eh:
LSFDE19:
	.set L$set$90,LEFDE19-LASFDE19
	.long L$set$90
LASFDE19:
	.long	LASFDE19-EH_frame1
	.quad	LFB790-.
	.set L$set$91,LFE790-LFB790
	.quad L$set$91
	.byte	0x8
	.quad	LLSDA790-.
	.byte	0x4
	.set L$set$92,LCFI45-LFB790
	.long L$set$92
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$93,LCFI46-LCFI45
	.long L$set$93
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$94,LCFI47-LCFI46
	.long L$set$94
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$95,LCFI48-LCFI47
	.long L$set$95
	.byte	0xe
	.byte	0x28
	.byte	0x4
	.set L$set$96,LCFI49-LCFI48
	.long L$set$96
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.set L$set$97,LCFI50-LCFI49
	.long L$set$97
	.byte	0xe
	.byte	0x38
	.byte	0x4
	.set L$set$98,LCFI51-LCFI50
	.long L$set$98
	.byte	0xe
	.byte	0xe0,0x1
	.byte	0x83
	.byte	0x7
	.byte	0x86
	.byte	0x6
	.byte	0x8c
	.byte	0x5
	.byte	0x8d
	.byte	0x4
	.byte	0x8e
	.byte	0x3
	.byte	0x8f
	.byte	0x2
	.align 3
LEFDE19:
.globl __Z12fast9_detectPKhiiiiPi.eh
__Z12fast9_detectPKhiiiiPi.eh:
LSFDE21:
	.set L$set$99,LEFDE21-LASFDE21
	.long L$set$99
LASFDE21:
	.long	LASFDE21-EH_frame1
	.quad	LFB791-.
	.set L$set$100,LFE791-LFB791
	.quad L$set$100
	.byte	0x8
	.quad	LLSDA791-.
	.byte	0x4
	.set L$set$101,LCFI52-LFB791
	.long L$set$101
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$102,LCFI53-LCFI52
	.long L$set$102
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$103,LCFI54-LCFI53
	.long L$set$103
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$104,LCFI55-LCFI54
	.long L$set$104
	.byte	0xe
	.byte	0x28
	.byte	0x4
	.set L$set$105,LCFI56-LCFI55
	.long L$set$105
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.set L$set$106,LCFI57-LCFI56
	.long L$set$106
	.byte	0xe
	.byte	0x38
	.byte	0x4
	.set L$set$107,LCFI58-LCFI57
	.long L$set$107
	.byte	0xe
	.byte	0xa0,0x3
	.byte	0x83
	.byte	0x7
	.byte	0x86
	.byte	0x6
	.byte	0x8c
	.byte	0x5
	.byte	0x8d
	.byte	0x4
	.byte	0x8e
	.byte	0x3
	.byte	0x8f
	.byte	0x2
	.align 3
LEFDE21:
	.constructor
	.destructor
	.align 1
	.subsections_via_symbols
