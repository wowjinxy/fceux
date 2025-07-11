/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <string.h>
#include "share.h"
#include "fkb.h"
#define AK(x)	FKB_ ## x

static uint8 bufit[0x49];
static uint8 ksrow;
static uint8 kscol;
static uint8 ksenable;

static uint16 matrix[9][2][4] =
{
	{ { AK(F8), AK(RETURN), AK(BRACKETLEFT), AK(BRACKETRIGHT) },
	  { AK(KANA), AK(RIGHTSHIFT), AK(BACKSLASH), AK(STOP) } },
	{ { AK(F7), AK(AT), AK(COLON), AK(SEMICOLON) },
	  { AK(UNDERSCORE), AK(SLASH), AK(MINUS), AK(CARET) } },
	{ { AK(F6), AK(O), AK(L), AK(K) },
	  { AK(PERIOD), AK(COMMA), AK(P), AK(0) } },
	{ { AK(F5), AK(I), AK(U), AK(J) },
	  { AK(M), AK(N), AK(9), AK(8) } },
	{ { AK(F4), AK(Y), AK(G), AK(H) },
	  { AK(B), AK(V), AK(7), AK(6) } },
	{ { AK(F3), AK(T), AK(R), AK(D) },
	  { AK(F), AK(C), AK(5), AK(4) } },
	{ { AK(F2), AK(W), AK(S), AK(A) },
	  { AK(X), AK(Z), AK(E), AK(3) } },
	{ { AK(F1), AK(ESCAPE), AK(Q), AK(CONTROL) },
	  { AK(LEFTSHIFT), AK(GRAPH), AK(1), AK(2) } },
	{ { AK(CLEAR), AK(UP), AK(RIGHT), AK(LEFT) },
	  { AK(DOWN), AK(SPACE), AK(DELETE), AK(INSERT) } },
};

static void FKB_Write(uint8 v) {
	uint8 col = (v & 2) >> 1;
	ksenable = v & 4;
	if (ksenable) {
		if (v & 1) {
			ksrow = 0;
		} else if (kscol && !col) {
			ksrow = (ksrow + 1) % 10;
		}
		kscol = col;
	}
}

static uint8 FKB_Read(int w, uint8 ret) {
	if (w) {
		int state = 0;
		int x;

		ret &= ~0x1E;
		if (ksrow == 9) return(ret);
		for (x = 0; x < 4; x++)
			if (bufit[ matrix[ksrow][kscol][x] & 0xFF ] || bufit[ matrix[ksrow][kscol][x] >> 8])
				state |= 1 << (x + 1);
		return(ret | ((ksenable ? (state ^ 0x1E) : 0)));
	}
	return(ret);
}

static void FKB_Strobe(void) {
}

static void FKB_Update(void *data, int arg) {
	memcpy(bufit + 1, data, sizeof(bufit) - 1);
}

static INPUTCFC FKB = { FKB_Read, FKB_Write, FKB_Strobe, FKB_Update, 0, 0 };

INPUTCFC *FCEU_InitFKB(void) {
	memset(bufit, 0, sizeof(bufit));
	ksenable = ksrow = kscol = 0;
	return(&FKB);
}
