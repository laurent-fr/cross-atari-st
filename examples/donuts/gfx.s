
	XDEF _gemdos_necin
	
	XDEF _xbios_physbase
	XDEF _xbios_logbase
	XDEF _xbios_vsync
	XDEF _xbios_setpalette
	
	XDEF _show_donuts

_gemdos_necin:
	move.w #8,-(sp)
	trap #1
	addq.l #2,sp
	rts

_xbios_physbase:
	move.w #2,-(sp)
	trap #14
	addq.l #2,sp
	rts

_xbios_logbase:
	move.w #3,-(sp)
	trap #14
	addq.l #2,sp
	rts

_xbios_vsync:
	move.w #37,-(sp)
	trap #14
	addq.l #2,sp
	rts

_xbios_setpalette:
	move.l 4(sp),d0
	move.l d0,-(sp)
	move.w #6,-(sp)
	trap #14
	addq.l #6,sp
	rts

_show_donuts:
	move.l 4(sp),a1 ; dst addr 
	move.l 8(sp),a0 ; sprite array addr 
	move.l 12(sp),d0 ; X scroll (0<=X<8)
	move.l 16(sp),d1 ; Y scroll (0<=Y<64)

	movem.l d2-d7/a2-a4,-(sp) ; save other registers

	lsl.w #8,d0 ; offset in sprite table for X scroll
	lsl.w #1,d0 ; (* 512)
	adda.l d0,a0 ; update sprite start addr for X scroll

	lsl.w #3,d1; select 1st sprite line (* 8)

	move.w #179,d2 ; 180 lines to draw

show_donuts_loop:
	move.l 0(a0,d1.l),d3 ; get 16 pixels of donuts
	move.l 4(a0,d1.l),d4
	move.l d3,d5		 ; copy 4 times into available registers
	move.l d4,d6
	move.l d3,d7
	move.l d4,a2
	move.l d3,a3
	move.l d4,a4

	; copy one line to screen
	rept 5
	movem.l d3-d7/a2-a4,(a1) ; put 32 bytes of donuts on screen
	lea 32(a1),a1	; move 32 bytes ahead
	endr

	addq.w #8,d1 ; next sprite line 
	andi.w #$01ff,d1 ; limit sprite offset 

	dbra d2,show_donuts_loop

	movem.l (sp)+,d2-d7/a2-a4 ; restore registers

	rts 