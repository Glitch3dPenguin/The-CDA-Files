# game_call.S — Windows-ABI bridge with shadow-area isolation.
#
# The game's functions use MSVC prologues that spill rbx/rbp/rsi/rdi into
# the CALLER's 32-byte shadow area ([rsp+8..rsp+0x28] at game entry).
# The MSVC ABI permits this (the caller must not keep live data there),
# but GCC (SysV) does keep live local slots there across calls, so an
# inline-asm `call *%reg` from C corrupts the C frame.
#
# This thunk takes its own frame (48B below the return address) so the
# game's shadow-clobber window lands inside the thunk's own frame.
# SysV in:  rdi=f, rsi=a0, rdx=a1, rcx=a2, r8=a3
# Windows:  rcx=a0, rdx=a1, r8=a2, r9=a3 ; rax = return
#
# Alignment: entry rsp ≡ 8 (mod 16); 3 pushes (24) + sub 0x30 (48)
# => rsp ≡ 0 (mod 16) at the call instruction (Windows ABI requirement).

	.text
	.globl game_call
	.type game_call, @function
game_call:
	push	%rbx
	push	%r12
	push	%r13
	sub	$0x30, %rsp
	mov	%r8, %r11			# a3 -> r11 (rcx is a2 in SysV; save before moving rcx)
	mov	%rcx, %r10			# a2 -> r10
	mov	%rsi, %rcx			# a0 -> rcx
	mov	%r10, %r8			# a2 -> r8
	mov	%r11, %r9			# a3 -> r9
	# a1 already in rdx (SysV arg3 == Windows a1)
	call	*%rdi
	add	$0x30, %rsp
	pop	%r13
	pop	%r12
	pop	%rbx
	ret
	.size game_call, .-game_call
