	.build_version macos, 27, 0	sdk_version 27, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_h_b_bump                       ; -- Begin function h_b_bump
	.p2align	2
_h_b_bump:                              ; @h_b_bump
	.cfi_startproc
; %bb.0:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
Lloh0:
	adrp	x1, l_.str@PAGE
Lloh1:
	add	x1, x1, l_.str@PAGEOFF
Lloh2:
	adrp	x2, l_.str.1@PAGE
Lloh3:
	add	x2, x2, l_.str.1@PAGEOFF
	bl	_hero_lend_local_name
	mov	x0, x19
	bl	_pthread_mutex_lock
	ldr	x8, [x19, #64]
	adds	x8, x8, #1
	b.vs	LBB0_2
; %bb.1:
	str	x8, [x19, #64]
Lloh4:
	adrp	x1, l_.str.2@PAGE
Lloh5:
	add	x1, x1, l_.str.2@PAGEOFF
Lloh6:
	adrp	x2, l_.str.1@PAGE
Lloh7:
	add	x2, x2, l_.str.1@PAGEOFF
	mov	x0, x19
	bl	_hero_lend_local_name
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	b	_pthread_mutex_unlock
LBB0_2:
	bl	_hero_panic_overflow
	.loh AdrpAdd	Lloh2, Lloh3
	.loh AdrpAdd	Lloh0, Lloh1
	.loh AdrpAdd	Lloh6, Lloh7
	.loh AdrpAdd	Lloh4, Lloh5
	.cfi_endproc
                                        ; -- End function
	.globl	_h_b_main                       ; -- Begin function h_b_main
	.p2align	2
_h_b_main:                              ; @h_b_main
	.cfi_startproc
; %bb.0:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
Lloh8:
	adrp	x1, l_.str.3@PAGE
Lloh9:
	add	x1, x1, l_.str.3@PAGEOFF
Lloh10:
	adrp	x20, l_.str.1@PAGE
Lloh11:
	add	x20, x20, l_.str.1@PAGEOFF
	mov	w0, #72                         ; =0x48
	mov	x2, x20
	bl	_hero_lend_local
	mov	x19, x0
	movi.2d	v0, #0000000000000000
	stp	q0, q0, [x0]
	stp	q0, q0, [x0, #32]
	str	xzr, [x0, #64]
Lloh12:
	adrp	x1, l_.str.4@PAGE
Lloh13:
	add	x1, x1, l_.str.4@PAGEOFF
	mov	x2, x20
	bl	_hero_lend_local_name
	str	xzr, [x19, #64]
	mov	x0, x19
	mov	x1, #0                          ; =0x0
	bl	_pthread_mutex_init
Lloh14:
	adrp	x1, l_.str@PAGE
Lloh15:
	add	x1, x1, l_.str@PAGEOFF
	mov	x0, x19
	mov	x2, x20
	bl	_hero_lend_local_name
	mov	x0, x19
	bl	_pthread_mutex_lock
	ldr	x8, [x19, #64]
	adds	x8, x8, #1
	b.vs	LBB1_4
; %bb.1:
	str	x8, [x19, #64]
Lloh16:
	adrp	x1, l_.str.2@PAGE
Lloh17:
	add	x1, x1, l_.str.2@PAGEOFF
	mov	x0, x19
	mov	x2, x20
	bl	_hero_lend_local_name
	mov	x0, x19
	bl	_pthread_mutex_unlock
Lloh18:
	adrp	x1, l_.str@PAGE
Lloh19:
	add	x1, x1, l_.str@PAGEOFF
	mov	x0, x19
	mov	x2, x20
	bl	_hero_lend_local_name
	mov	x0, x19
	bl	_pthread_mutex_lock
	ldr	x8, [x19, #64]
	adds	x8, x8, #1
	b.vs	LBB1_4
; %bb.2:
	str	x8, [x19, #64]
Lloh20:
	adrp	x1, l_.str.2@PAGE
Lloh21:
	add	x1, x1, l_.str.2@PAGEOFF
	mov	x0, x19
	mov	x2, x20
	bl	_hero_lend_local_name
	mov	x0, x19
	bl	_pthread_mutex_unlock
Lloh22:
	adrp	x1, l_.str@PAGE
Lloh23:
	add	x1, x1, l_.str@PAGEOFF
	mov	x0, x19
	mov	x2, x20
	bl	_hero_lend_local_name
	mov	x0, x19
	bl	_pthread_mutex_lock
	ldr	x8, [x19, #64]
	adds	x8, x8, #1
	b.vs	LBB1_4
; %bb.3:
	str	x8, [x19, #64]
Lloh24:
	adrp	x1, l_.str.2@PAGE
Lloh25:
	add	x1, x1, l_.str.2@PAGEOFF
	mov	x0, x19
	mov	x2, x20
	bl	_hero_lend_local_name
	mov	x0, x19
	bl	_pthread_mutex_unlock
Lloh26:
	adrp	x1, l_.str.5@PAGE
Lloh27:
	add	x1, x1, l_.str.5@PAGEOFF
	mov	x0, x19
	mov	x2, x20
	bl	_hero_lend_local_name
	ldr	x0, [x19, #64]
	bl	_hero_print_int
	bl	_hero_print_end
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	b	_hero_lend_local_give
LBB1_4:
	bl	_hero_panic_overflow
	.loh AdrpAdd	Lloh14, Lloh15
	.loh AdrpAdd	Lloh12, Lloh13
	.loh AdrpAdd	Lloh10, Lloh11
	.loh AdrpAdd	Lloh8, Lloh9
	.loh AdrpAdd	Lloh18, Lloh19
	.loh AdrpAdd	Lloh16, Lloh17
	.loh AdrpAdd	Lloh22, Lloh23
	.loh AdrpAdd	Lloh20, Lloh21
	.loh AdrpAdd	Lloh26, Lloh27
	.loh AdrpAdd	Lloh24, Lloh25
	.cfi_endproc
                                        ; -- End function
	.globl	_h_b_Counter_eq                 ; -- Begin function h_b_Counter_eq
	.p2align	2
_h_b_Counter_eq:                        ; @h_b_Counter_eq
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
Lloh28:
	adrp	x0, l_.str.6@PAGE
Lloh29:
	add	x0, x0, l_.str.6@PAGEOFF
	bl	_hero_panic
	.loh AdrpAdd	Lloh28, Lloh29
	.cfi_endproc
                                        ; -- End function
	.globl	_h_b_Counter_hash               ; -- Begin function h_b_Counter_hash
	.p2align	2
_h_b_Counter_hash:                      ; @h_b_Counter_hash
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
Lloh30:
	adrp	x0, l_.str.7@PAGE
Lloh31:
	add	x0, x0, l_.str.7@PAGEOFF
	bl	_hero_panic
	.loh AdrpAdd	Lloh30, Lloh31
	.cfi_endproc
                                        ; -- End function
	.globl	_h_0opt_f87774a_retain          ; -- Begin function h_0opt_f87774a_retain
	.p2align	2
_h_0opt_f87774a_retain:                 ; @h_0opt_f87774a_retain
	.cfi_startproc
; %bb.0:
	mov	x8, x0
	ldr	x9, [x8], #8
	cbz	x9, LBB4_2
; %bb.1:
	mov	x0, x8
	b	_hero_failure_retain
LBB4_2:
	ldp	x8, x1, [x0, #8]
	mov	x0, x8
	b	_hero_str_incref
	.cfi_endproc
                                        ; -- End function
	.globl	_h_0opt_f87774a_release         ; -- Begin function h_0opt_f87774a_release
	.p2align	2
_h_0opt_f87774a_release:                ; @h_0opt_f87774a_release
	.cfi_startproc
; %bb.0:
	mov	x8, x0
	ldr	x9, [x8], #8
	cbz	x9, LBB5_2
; %bb.1:
	mov	x0, x8
	b	_hero_failure_release
LBB5_2:
	ldp	x8, x1, [x0, #8]
	mov	x0, x8
	b	_hero_str_decref
	.cfi_endproc
                                        ; -- End function
	.globl	_h_0opt_f87774a_eq              ; -- Begin function h_0opt_f87774a_eq
	.p2align	2
_h_0opt_f87774a_eq:                     ; @h_0opt_f87774a_eq
	.cfi_startproc
; %bb.0:
	ldr	x8, [x0]
	ldr	x9, [x1]
	cmp	x8, x9
	b.ne	LBB6_3
; %bb.1:
	cbz	x8, LBB6_4
; %bb.2:
	add	x0, x0, #8
	add	x1, x1, #8
	b	_hero_failure_eq
LBB6_3:
	mov	w0, #0                          ; =0x0
	ret
LBB6_4:
Lloh32:
	adrp	x8, _hero_desc_str@GOTPAGE
Lloh33:
	ldr	x8, [x8, _hero_desc_str@GOTPAGEOFF]
Lloh34:
	ldr	x2, [x8, #24]
	add	x0, x0, #8
	add	x1, x1, #8
	br	x2
	.loh AdrpLdrGotLdr	Lloh32, Lloh33, Lloh34
	.cfi_endproc
                                        ; -- End function
	.globl	_h_0opt_f87774a_hash            ; -- Begin function h_0opt_f87774a_hash
	.p2align	2
_h_0opt_f87774a_hash:                   ; @h_0opt_f87774a_hash
	.cfi_startproc
; %bb.0:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, #435                       ; =0x1b3
	movk	x19, #256, lsl #32
	ldr	x8, [x0]
	cbz	x8, LBB7_2
; %bb.1:
	mov	x9, #8997                       ; =0x2325
	movk	x9, #33826, lsl #16
	movk	x9, #40164, lsl #32
	movk	x9, #52210, lsl #48
	eor	x8, x8, x9
	mul	x20, x8, x19
	add	x0, x0, #8
	bl	_hero_failure_hash
	eor	x8, x0, x20
	b	LBB7_3
LBB7_2:
Lloh35:
	adrp	x8, _hero_desc_str@GOTPAGE
Lloh36:
	ldr	x8, [x8, _hero_desc_str@GOTPAGEOFF]
Lloh37:
	ldr	x8, [x8, #32]
	add	x0, x0, #8
	blr	x8
	mov	x8, #47071                      ; =0xb7df
	movk	x8, #34305, lsl #16
	movk	x8, #48460, lsl #32
	movk	x8, #44899, lsl #48
	eor	x8, x0, x8
LBB7_3:
	mul	x0, x8, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	ret
	.loh AdrpLdrGotLdr	Lloh35, Lloh36, Lloh37
	.cfi_endproc
                                        ; -- End function
	.globl	_h_0opt_a8ea2_retain            ; -- Begin function h_0opt_a8ea2_retain
	.p2align	2
_h_0opt_a8ea2_retain:                   ; @h_0opt_a8ea2_retain
	.cfi_startproc
; %bb.0:
	ldr	x8, [x0], #8
	cbz	x8, LBB8_2
; %bb.1:
	b	_hero_failure_retain
LBB8_2:
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_h_0opt_a8ea2_release           ; -- Begin function h_0opt_a8ea2_release
	.p2align	2
_h_0opt_a8ea2_release:                  ; @h_0opt_a8ea2_release
	.cfi_startproc
; %bb.0:
	ldr	x8, [x0], #8
	cbz	x8, LBB9_2
; %bb.1:
	b	_hero_failure_release
LBB9_2:
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_h_0opt_a8ea2_eq                ; -- Begin function h_0opt_a8ea2_eq
	.p2align	2
_h_0opt_a8ea2_eq:                       ; @h_0opt_a8ea2_eq
	.cfi_startproc
; %bb.0:
	ldr	x8, [x0], #8
	ldr	x9, [x1], #8
	cmp	x8, x9
	b.ne	LBB10_3
; %bb.1:
	cbz	x8, LBB10_4
; %bb.2:
	b	_hero_failure_eq
LBB10_3:
	mov	w0, #0                          ; =0x0
	ret
LBB10_4:
	mov	w0, #1                          ; =0x1
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_h_0opt_a8ea2_hash              ; -- Begin function h_0opt_a8ea2_hash
	.p2align	2
_h_0opt_a8ea2_hash:                     ; @h_0opt_a8ea2_hash
	.cfi_startproc
; %bb.0:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x20, #435                       ; =0x1b3
	movk	x20, #256, lsl #32
	ldr	x8, [x0], #8
	mov	x9, #8997                       ; =0x2325
	movk	x9, #33826, lsl #16
	movk	x9, #40164, lsl #32
	movk	x9, #52210, lsl #48
	eor	x9, x8, x9
	mul	x19, x9, x20
	cbz	x8, LBB11_2
; %bb.1:
	bl	_hero_failure_hash
	eor	x8, x0, x19
	mul	x19, x8, x20
LBB11_2:
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	bl	_hero_args_set
	bl	_h_b_main
	bl	_hero_runtime_check_leaks
	mov	w0, #0                          ; =0x0
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
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
