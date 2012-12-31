/* $Id: testrescue.S,v 1.1.1.1 2010/12/02 04:23:33 walf_wu Exp $
 *
 * Simple testcode to download by the rescue block.
 * Just lits some LEDs to show it was downloaded correctly.
 * 
 * Copyright (C) 1999 Axis Communications AB
 */

#define ASSEMBLER_MACROS_ONLY
#include <asm/sv_addr_ag.h>

	.text

	nop
	nop
	moveq	-1, $r2
	move.b	$r2, [R_PORT_PA_DIR]
	moveq	0, $r2
	move.b	$r2, [R_PORT_PA_DATA]	

endless:
	nop
	ba	endless
	nop

	
