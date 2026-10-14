	.build_version macos, 27, 0	sdk_version 27, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_h_b_bump                       ; -- Begin function h_b_bump
	.p2align	2
_h_b_bump:                              ; @h_b_bump
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #128
	stp	x29, x30, [sp, #112]            ; 16-byte Folded Spill
	add	x29, sp, #112
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	adrp	x8, ___stack_chk_guard@GOTPAGE
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
	ldr	x8, [x8]
	stur	x8, [x29, #-8]
	str	x0, [sp, #24]
	; InlineAsm Start
	nop
	; InlineAsm End
	b	LBB0_1
LBB0_1:
	ldr	x0, [sp, #24]
	adrp	x1, l_.str@PAGE
	add	x1, x1, l_.str@PAGEOFF
	adrp	x2, l_.str.1@PAGE
	add	x2, x2, l_.str.1@PAGEOFF
	bl	_hero_lend_local_name
	ldr	x0, [sp, #24]
	bl	_counter_lock
	ldr	x1, [sp, #24]
	add	x0, sp, #32
	mov	x2, #72                         ; =0x48
	bl	_memcpy
	ldr	x8, [sp, #96]
	str	x8, [sp, #16]
	mov	x8, #1                          ; =0x1
	str	x8, [sp, #8]
	ldr	x8, [sp, #16]
	ldr	x9, [sp, #8]
	adds	x9, x8, x9
	cset	w8, vs
	str	x9, [sp]
	tbz	w8, #0, LBB0_3
	b	LBB0_2
LBB0_2:
	bl	_hero_panic_overflow
LBB0_3:
	ldr	x8, [sp]
	ldr	x9, [sp, #24]
	str	x8, [x9, #64]
	ldr	x0, [sp, #24]
	adrp	x1, l_.str.2@PAGE
	add	x1, x1, l_.str.2@PAGEOFF
	adrp	x2, l_.str.1@PAGE
	add	x2, x2, l_.str.1@PAGEOFF
	bl	_hero_lend_local_name
	ldr	x0, [sp, #24]
	bl	_counter_unlock
	ldur	x9, [x29, #-8]
	adrp	x8, ___stack_chk_guard@GOTPAGE
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
	ldr	x8, [x8]
	subs	x8, x8, x9
	b.eq	LBB0_5
	b	LBB0_4
LBB0_4:
	bl	___stack_chk_fail
LBB0_5:
	ldp	x29, x30, [sp, #112]            ; 16-byte Folded Reload
	add	sp, sp, #128
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function counter_lock
_counter_lock:                          ; @counter_lock
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	x0, [sp, #8]
	ldr	x0, [sp, #8]
	bl	_pthread_mutex_lock
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function counter_unlock
_counter_unlock:                        ; @counter_unlock
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	x0, [sp, #8]
	ldr	x0, [sp, #8]
	bl	_pthread_mutex_unlock
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_h_b_main                       ; -- Begin function h_b_main
	.p2align	2
_h_b_main:                              ; @h_b_main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #224
	stp	x29, x30, [sp, #208]            ; 16-byte Folded Spill
	add	x29, sp, #208
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	adrp	x8, ___stack_chk_guard@GOTPAGE
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
	ldr	x8, [x8]
	stur	x8, [x29, #-8]
	mov	x0, #72                         ; =0x48
	adrp	x1, l_.str.3@PAGE
	add	x1, x1, l_.str.3@PAGEOFF
	adrp	x2, l_.str.1@PAGE
	add	x2, x2, l_.str.1@PAGEOFF
	bl	_hero_lend_local
	str	x0, [sp, #48]
	; InlineAsm Start
	nop
	; InlineAsm End
	b	LBB3_1
LBB3_1:
	add	x0, sp, #56
	str	x0, [sp]                        ; 8-byte Folded Spill
	mov	x2, #72                         ; =0x48
	str	x2, [sp, #24]                   ; 8-byte Folded Spill
	mov	w1, #0                          ; =0x0
	str	w1, [sp, #12]                   ; 4-byte Folded Spill
	bl	_memset
	ldr	x1, [sp]                        ; 8-byte Folded Reload
	ldr	x2, [sp, #24]                   ; 8-byte Folded Reload
	sub	x0, x29, #80
	str	x0, [sp, #16]                   ; 8-byte Folded Spill
	bl	_memcpy
	ldr	x0, [sp, #16]                   ; 8-byte Folded Reload
	ldr	w1, [sp, #12]                   ; 4-byte Folded Reload
	ldr	x2, [sp, #24]                   ; 8-byte Folded Reload
	bl	_memset
	ldr	x1, [sp, #16]                   ; 8-byte Folded Reload
	ldr	x2, [sp, #24]                   ; 8-byte Folded Reload
	ldr	x0, [sp, #48]
	bl	_memcpy
	ldr	x0, [sp, #48]
	adrp	x1, l_.str.4@PAGE
	add	x1, x1, l_.str.4@PAGEOFF
	adrp	x2, l_.str.1@PAGE
	add	x2, x2, l_.str.1@PAGEOFF
	str	x2, [sp, #32]                   ; 8-byte Folded Spill
	bl	_hero_lend_local_name
	ldr	x0, [sp, #48]
	bl	_counter_init
	ldr	x0, [sp, #48]
	bl	_h_b_bump
	ldr	x0, [sp, #48]
	bl	_h_b_bump
	ldr	x0, [sp, #48]
	bl	_h_b_bump
	ldr	x2, [sp, #32]                   ; 8-byte Folded Reload
	ldr	x0, [sp, #48]
	adrp	x1, l_.str.5@PAGE
	add	x1, x1, l_.str.5@PAGEOFF
	bl	_hero_lend_local_name
	ldr	x0, [sp, #48]
	bl	_counter_value
	str	x0, [sp, #40]
	ldr	x0, [sp, #40]
	bl	_hero_print_int
	bl	_hero_print_end
	ldr	x0, [sp, #48]
	bl	_hero_lend_local_give
	ldur	x9, [x29, #-8]
	adrp	x8, ___stack_chk_guard@GOTPAGE
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
	ldr	x8, [x8]
	subs	x8, x8, x9
	b.eq	LBB3_3
	b	LBB3_2
LBB3_2:
	bl	___stack_chk_fail
LBB3_3:
	ldp	x29, x30, [sp, #208]            ; 16-byte Folded Reload
	add	sp, sp, #224
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function counter_init
_counter_init:                          ; @counter_init
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	x0, [sp, #8]
	ldr	x8, [sp, #8]
	str	xzr, [x8, #64]
	ldr	x0, [sp, #8]
	mov	x1, #0                          ; =0x0
	bl	_pthread_mutex_init
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function counter_value
_counter_value:                         ; @counter_value
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	x0, [sp, #8]
	ldr	x8, [sp, #8]
	ldr	x0, [x8, #64]
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_h_b_Counter_eq                 ; -- Begin function h_b_Counter_eq
	.p2align	2
_h_b_Counter_eq:                        ; @h_b_Counter_eq
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	x0, [sp, #8]
	str	x1, [sp]
	adrp	x0, l_.str.6@PAGE
	add	x0, x0, l_.str.6@PAGEOFF
	bl	_hero_panic
	.cfi_endproc
                                        ; -- End function
	.globl	_h_b_Counter_hash               ; -- Begin function h_b_Counter_hash
	.p2align	2
_h_b_Counter_hash:                      ; @h_b_Counter_hash
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	x0, [sp, #8]
	adrp	x0, l_.str.7@PAGE
	add	x0, x0, l_.str.7@PAGEOFF
	bl	_hero_panic
	.cfi_endproc
                                        ; -- End function
	.globl	_h_0opt_f87774a_retain          ; -- Begin function h_0opt_f87774a_retain
	.p2align	2
_h_0opt_f87774a_retain:                 ; @h_0opt_f87774a_retain
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	x0, [sp, #8]
	ldr	x8, [sp, #8]
	ldr	x8, [x8]
	cbnz	x8, LBB8_2
	b	LBB8_1
LBB8_1:
	ldr	x8, [sp, #8]
	ldr	x0, [x8, #8]
	ldr	x1, [x8, #16]
	bl	_hero_str_incref
	b	LBB8_3
LBB8_2:
	ldr	x8, [sp, #8]
	add	x0, x8, #8
	bl	_hero_failure_retain
	b	LBB8_3
LBB8_3:
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_h_0opt_f87774a_release         ; -- Begin function h_0opt_f87774a_release
	.p2align	2
_h_0opt_f87774a_release:                ; @h_0opt_f87774a_release
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	x0, [sp, #8]
	ldr	x8, [sp, #8]
	ldr	x8, [x8]
	cbnz	x8, LBB9_2
	b	LBB9_1
LBB9_1:
	ldr	x8, [sp, #8]
	ldr	x0, [x8, #8]
	ldr	x1, [x8, #16]
	bl	_hero_str_decref
	b	LBB9_3
LBB9_2:
	ldr	x8, [sp, #8]
	add	x0, x8, #8
	bl	_hero_failure_release
	b	LBB9_3
LBB9_3:
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_h_0opt_f87774a_eq              ; -- Begin function h_0opt_f87774a_eq
	.p2align	2
_h_0opt_f87774a_eq:                     ; @h_0opt_f87774a_eq
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #48
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	add	x29, sp, #32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	x0, [sp, #16]
	str	x1, [sp, #8]
	ldr	x8, [sp, #16]
	ldr	x8, [x8]
	ldr	x9, [sp, #8]
	ldr	x9, [x9]
	subs	x8, x8, x9
	b.eq	LBB10_2
	b	LBB10_1
LBB10_1:
	mov	w8, #0                          ; =0x0
	and	w8, w8, #0x1
	and	w8, w8, #0x1
	sturb	w8, [x29, #-1]
	b	LBB10_5
LBB10_2:
	ldr	x8, [sp, #16]
	ldr	x8, [x8]
	cbz	x8, LBB10_4
	b	LBB10_3
LBB10_3:
	ldr	x8, [sp, #16]
	add	x0, x8, #8
	ldr	x8, [sp, #8]
	add	x1, x8, #8
	bl	_hero_failure_eq
	and	w8, w0, #0x1
	sturb	w8, [x29, #-1]
	b	LBB10_5
LBB10_4:
	adrp	x8, _hero_desc_str@GOTPAGE
	ldr	x8, [x8, _hero_desc_str@GOTPAGEOFF]
	ldr	x8, [x8, #24]
	ldr	x9, [sp, #16]
	add	x0, x9, #8
	ldr	x9, [sp, #8]
	add	x1, x9, #8
	blr	x8
	and	w8, w0, #0x1
	sturb	w8, [x29, #-1]
	b	LBB10_5
LBB10_5:
	ldurb	w8, [x29, #-1]
	and	w0, w8, #0x1
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	add	sp, sp, #48
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_h_0opt_f87774a_hash            ; -- Begin function h_0opt_f87774a_hash
	.p2align	2
_h_0opt_f87774a_hash:                   ; @h_0opt_f87774a_hash
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #80
	stp	x29, x30, [sp, #64]             ; 16-byte Folded Spill
	add	x29, sp, #64
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	x9, #435                        ; =0x1b3
	movk	x9, #256, lsl #32
	str	x9, [sp, #24]                   ; 8-byte Folded Spill
	stur	x0, [x29, #-16]
	ldur	x8, [x29, #-16]
	stur	x8, [x29, #-24]
	ldur	x8, [x29, #-24]
	ldr	x10, [x8]
	mov	x8, #8997                       ; =0x2325
	movk	x8, #33826, lsl #16
	movk	x8, #40164, lsl #32
	movk	x8, #52210, lsl #48
	eor	x8, x8, x10
	mul	x8, x8, x9
	str	x8, [sp, #32]
	ldur	x8, [x29, #-24]
	ldr	x8, [x8]
	cbz	x8, LBB11_2
	b	LBB11_1
LBB11_1:
	ldr	x8, [sp, #32]
	str	x8, [sp, #16]                   ; 8-byte Folded Spill
	ldur	x8, [x29, #-24]
	add	x0, x8, #8
	bl	_hero_failure_hash
	ldr	x8, [sp, #16]                   ; 8-byte Folded Reload
	ldr	x9, [sp, #24]                   ; 8-byte Folded Reload
	eor	x8, x8, x0
	mul	x8, x8, x9
	stur	x8, [x29, #-8]
	b	LBB11_3
LBB11_2:
	ldr	x8, [sp, #32]
	str	x8, [sp, #8]                    ; 8-byte Folded Spill
	adrp	x8, _hero_desc_str@GOTPAGE
	ldr	x8, [x8, _hero_desc_str@GOTPAGEOFF]
	ldr	x8, [x8, #32]
	ldur	x9, [x29, #-24]
	add	x0, x9, #8
	blr	x8
	ldr	x8, [sp, #8]                    ; 8-byte Folded Reload
	ldr	x9, [sp, #24]                   ; 8-byte Folded Reload
	eor	x8, x8, x0
	mul	x8, x8, x9
	stur	x8, [x29, #-8]
	b	LBB11_3
LBB11_3:
	ldur	x0, [x29, #-8]
	ldp	x29, x30, [sp, #64]             ; 16-byte Folded Reload
	add	sp, sp, #80
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_h_0opt_a8ea2_retain            ; -- Begin function h_0opt_a8ea2_retain
	.p2align	2
_h_0opt_a8ea2_retain:                   ; @h_0opt_a8ea2_retain
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	x0, [sp, #8]
	ldr	x8, [sp, #8]
	ldr	x8, [x8]
	cbnz	x8, LBB12_2
	b	LBB12_1
LBB12_1:
	b	LBB12_3
LBB12_2:
	ldr	x8, [sp, #8]
	add	x0, x8, #8
	bl	_hero_failure_retain
	b	LBB12_3
LBB12_3:
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_h_0opt_a8ea2_release           ; -- Begin function h_0opt_a8ea2_release
	.p2align	2
_h_0opt_a8ea2_release:                  ; @h_0opt_a8ea2_release
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	x0, [sp, #8]
	ldr	x8, [sp, #8]
	ldr	x8, [x8]
	cbnz	x8, LBB13_2
	b	LBB13_1
LBB13_1:
	b	LBB13_3
LBB13_2:
	ldr	x8, [sp, #8]
	add	x0, x8, #8
	bl	_hero_failure_release
	b	LBB13_3
LBB13_3:
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_h_0opt_a8ea2_eq                ; -- Begin function h_0opt_a8ea2_eq
	.p2align	2
_h_0opt_a8ea2_eq:                       ; @h_0opt_a8ea2_eq
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #48
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	add	x29, sp, #32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	x0, [sp, #16]
	str	x1, [sp, #8]
	ldr	x8, [sp, #16]
	ldr	x8, [x8]
	ldr	x9, [sp, #8]
	ldr	x9, [x9]
	subs	x8, x8, x9
	b.eq	LBB14_2
	b	LBB14_1
LBB14_1:
	mov	w8, #0                          ; =0x0
	and	w8, w8, #0x1
	and	w8, w8, #0x1
	sturb	w8, [x29, #-1]
	b	LBB14_5
LBB14_2:
	ldr	x8, [sp, #16]
	ldr	x8, [x8]
	cbz	x8, LBB14_4
	b	LBB14_3
LBB14_3:
	ldr	x8, [sp, #16]
	add	x0, x8, #8
	ldr	x8, [sp, #8]
	add	x1, x8, #8
	bl	_hero_failure_eq
	and	w8, w0, #0x1
	sturb	w8, [x29, #-1]
	b	LBB14_5
LBB14_4:
	mov	w8, #1                          ; =0x1
	and	w8, w8, #0x1
	and	w8, w8, #0x1
	sturb	w8, [x29, #-1]
	b	LBB14_5
LBB14_5:
	ldurb	w8, [x29, #-1]
	and	w0, w8, #0x1
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	add	sp, sp, #48
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_h_0opt_a8ea2_hash              ; -- Begin function h_0opt_a8ea2_hash
	.p2align	2
_h_0opt_a8ea2_hash:                     ; @h_0opt_a8ea2_hash
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #64
	stp	x29, x30, [sp, #48]             ; 16-byte Folded Spill
	add	x29, sp, #48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	stur	x0, [x29, #-16]
	ldur	x8, [x29, #-16]
	str	x8, [sp, #24]
	ldr	x8, [sp, #24]
	ldr	x9, [x8]
	mov	x8, #8997                       ; =0x2325
	movk	x8, #33826, lsl #16
	movk	x8, #40164, lsl #32
	movk	x8, #52210, lsl #48
	eor	x8, x8, x9
	mov	x9, #435                        ; =0x1b3
	movk	x9, #256, lsl #32
	mul	x8, x8, x9
	str	x8, [sp, #16]
	ldr	x8, [sp, #24]
	ldr	x8, [x8]
	cbz	x8, LBB15_2
	b	LBB15_1
LBB15_1:
	ldr	x8, [sp, #16]
	str	x8, [sp, #8]                    ; 8-byte Folded Spill
	ldr	x8, [sp, #24]
	add	x0, x8, #8
	bl	_hero_failure_hash
	ldr	x8, [sp, #8]                    ; 8-byte Folded Reload
	eor	x8, x8, x0
	mov	x9, #435                        ; =0x1b3
	movk	x9, #256, lsl #32
	mul	x8, x8, x9
	stur	x8, [x29, #-8]
	b	LBB15_3
LBB15_2:
	ldr	x8, [sp, #16]
	stur	x8, [x29, #-8]
	b	LBB15_3
LBB15_3:
	ldur	x0, [x29, #-8]
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	add	sp, sp, #64
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #48
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	add	x29, sp, #32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w8, #0                          ; =0x0
	str	w8, [sp, #12]                   ; 4-byte Folded Spill
	stur	wzr, [x29, #-4]
	stur	w0, [x29, #-8]
	str	x1, [sp, #16]
	ldur	w0, [x29, #-8]
	ldr	x1, [sp, #16]
	bl	_hero_args_set
	bl	_h_b_main
	bl	_hero_runtime_check_leaks
	ldr	w0, [sp, #12]                   ; 4-byte Folded Reload
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	add	sp, sp, #48
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"counter_lock"

l_.str.1:                               ; @.str.1
	.asciz	"c"

l_.str.2:                               ; @.str.2
	.asciz	"counter_unlock"

l_.str.3:                               ; @.str.3
	.asciz	"b.main"

l_.str.4:                               ; @.str.4
	.asciz	"counter_init"

l_.str.5:                               ; @.str.5
	.asciz	"counter_value"

l_.str.6:                               ; @.str.6
	.asciz	"h_b_Counter_eq: a partial record has no structural equality"

l_.str.7:                               ; @.str.7
	.asciz	"h_b_Counter_hash: a partial record has no structural hash"

.subsections_via_symbols
