	.build_version macos, 27, 0	sdk_version 27, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_h_main_SQLITE_ROW              ; -- Begin function h_main_SQLITE_ROW
	.p2align	2
_h_main_SQLITE_ROW:                     ; @h_main_SQLITE_ROW
	.cfi_startproc
; %bb.0:
	mov	w0, #100                        ; =0x64
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_h_main_f                       ; -- Begin function h_main_f
	.p2align	2
_h_main_f:                              ; @h_main_f
	.cfi_startproc
; %bb.0:
	ubfx	w8, w0, #8, #8
	add	w0, w8, #100
	ret
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
