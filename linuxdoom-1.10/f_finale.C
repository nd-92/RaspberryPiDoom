// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//	Game completion, final screen animation.
//
//-----------------------------------------------------------------------------

static const char rcsid[] = "$Id: f_finale.c,v 1.5 1997/02/03 21:26:34 b1 Exp $";

#include <ctype.h>
#include <algorithm>

// Functions.
#include "i_system.H"
#include "m_swap.H"
#include "z_zone.H"
#include "v_video.H"
#include "w_wad.H"
#include "s_sound.H"

// Data.
#include "dstrings.H"
#include "sounds.H"

#include "doomstat.H"
#include "r_state.H"

// ?
// #include "doomstat.H"
// #include "r_local.H"
// #include "f_finale.H"

// Stage of animation:
//  0 = text, 1 = art screen, 2 = character cast
int finalestage;

int finalecount;

#define TEXTSPEED 3
#define TEXTWAIT 250

const char *e1text = E1TEXT;
const char *e2text = E2TEXT;
const char *e3text = E3TEXT;
const char *e4text = E4TEXT;

const char *c1text = C1TEXT;
const char *c2text = C2TEXT;
const char *c3text = C3TEXT;
const char *c4text = C4TEXT;
const char *c5text = C5TEXT;
const char *c6text = C6TEXT;

const char *p1text = P1TEXT;
const char *p2text = P2TEXT;
const char *p3text = P3TEXT;
const char *p4text = P4TEXT;
const char *p5text = P5TEXT;
const char *p6text = P6TEXT;

const char *t1text = T1TEXT;
const char *t2text = T2TEXT;
const char *t3text = T3TEXT;
const char *t4text = T4TEXT;
const char *t5text = T5TEXT;
const char *t6text = T6TEXT;

const char *finaletext;
const char *finaleflat;

void F_StartCast(void);
void F_CastTicker(void);

int castnum;
int casttics;
state_t *caststate;
boolean castdeath;
int castframes;
int castonmelee;
boolean castattacking;

//
// Final DOOM 2 animation
// Casting by id Software.
//   in order of appearance
//
typedef struct
{
	const char *name;
	mobjtype_t type;
} castinfo_t;

castinfo_t castorder[] = {
	{CC_ZOMBIE, MT_POSSESSED},
	{CC_SHOTGUN, MT_SHOTGUY},
	{CC_HEAVY, MT_CHAINGUY},
	{CC_IMP, MT_TROOP},
	{CC_DEMON, MT_SERGEANT},
	{CC_LOST, MT_SKULL},
	{CC_CACO, MT_HEAD},
	{CC_HELL, MT_KNIGHT},
	{CC_BARON, MT_BRUISER},
	{CC_ARACH, MT_BABY},
	{CC_PAIN, MT_PAIN},
	{CC_REVEN, MT_UNDEAD},
	{CC_MANCU, MT_FATSO},
	{CC_ARCH, MT_VILE},
	{CC_SPIDER, MT_SPIDER},
	{CC_CYBER, MT_CYBORG},
	{CC_HERO, MT_PLAYER},
	{NULL, MT_PLAYER}};

#include "hu_stuff.H"
extern const patch_t *hu_font[HU_FONTSIZE];
extern gamestate_t wipegamestate;

//
// F_CastResponder
//
boolean F_CastResponder(const event_t *ev)
{
	if (ev->type != ev_keydown)
	{
		return false;
	}

	if (castdeath)
	{
		return true;
	} // already in dying frames

	// go into death frame
	castdeath = true;
	caststate = &states[mobjinfo[castorder[castnum].type].deathstate];
	casttics = caststate->tics;
	castframes = 0;
	castattacking = false;
	if (mobjinfo[castorder[castnum].type].deathsound)
	{
		S_StartSound(NULL, mobjinfo[castorder[castnum].type].deathsound);
	}

	return true;
}
void F_CastDrawer(void);

//
// F_StartFinale
//
void F_StartFinale(void)
{
	gameaction = ga_nothing;
	gamestate = GS_FINALE;
	viewactive = false;
	automapactive = false;

	// Okay - IWAD dependend stuff.
	// This has been changed severly, and
	//  some stuff might have changed in the process.
	switch (gamemode)
	{

	// DOOM 1 - E1, E3 or E4, but each nine missions
	case shareware:
	case registered:
	case retail:
	{
		S_ChangeMusic(mus_victor, true);

		switch (gameepisode)
		{
		case 1:
			finaleflat = "FLOOR4_8";
			finaletext = e1text;
			break;
		case 2:
			finaleflat = "SFLR6_1";
			finaletext = e2text;
			break;
		case 3:
			finaleflat = "MFLR8_4";
			finaletext = e3text;
			break;
		case 4:
			finaleflat = "MFLR8_3";
			finaletext = e4text;
			break;
		default:
			// Ouch.
			break;
		}
		break;
	}

	// DOOM II and missions packs with E1, M34
	case commercial:
	{
		S_ChangeMusic(mus_read_m, true);

		switch (gamemap)
		{
		case 6:
			finaleflat = "SLIME16";
			finaletext = c1text;
			break;
		case 11:
			finaleflat = "RROCK14";
			finaletext = c2text;
			break;
		case 20:
			finaleflat = "RROCK07";
			finaletext = c3text;
			break;
		case 30:
			finaleflat = "RROCK17";
			finaletext = c4text;
			break;
		case 15:
			finaleflat = "RROCK13";
			finaletext = c5text;
			break;
		case 31:
			finaleflat = "RROCK19";
			finaletext = c6text;
			break;
		default:
			// Ouch.
			break;
		}
		break;
	}

	// Indeterminate.
	default:
		S_ChangeMusic(mus_read_m, true);
		finaleflat = "F_SKY1"; // Not used anywhere else.
		finaletext = c1text;   // FIXME - other text, music?
		break;
	}

	finalestage = 0;
	finalecount = 0;
}

boolean F_Responder(const event_t *event)
{
	if (finalestage == 2)
	{
		return F_CastResponder(event);
	}

	return false;
}

//
// F_Ticker
//
void F_Ticker(void)
{
	// check for skipping
	if ((gamemode == commercial) && (finalecount > 50))
	{
		int i;
		// go on to the next level
		for (i = 0; i < MAXPLAYERS; i++)
		{
			if (players[i].cmd.buttons)
			{
				break;
			}
		}

		if (i < MAXPLAYERS)
		{
			if (gamemap == 30)
			{
				F_StartCast();
			}
			else
			{
				gameaction = ga_worlddone;
			}
		}
	}

	// advance animation
	finalecount++;

	if (finalestage == 2)
	{
		F_CastTicker();
		return;
	}

	if (gamemode == commercial)
	{
		return;
	}

	// Cast length of finaletext to int from size_t
	// Don't alter signedness of finalecount or finalestage
	if (!finalestage && finalecount > static_cast<int>(strlen(finaletext)) * TEXTSPEED + TEXTWAIT)
	{
		finalecount = 0;
		finalestage = 1;
		wipegamestate = GS_WIPE; // force a wipe
		if (gameepisode == 3)
		{
			S_StartMusic(mus_bunny);
		}
	}
}

//
// F_TextWrite
//

void F_TextWrite(void)
{
	const byte *src;
	// byte *dest;
	char *dest;

	// int w;
	// int count;
	// char *ch;
	// int c;
	// int cx;
	// int cy;

	// erase the entire screen to a tiled background
	src = static_cast<byte *>(W_CacheLumpName(finaleflat, PU_CACHE));
	dest = screens[0];

	for (int y = 0; y < SCREENHEIGHT; y++)
	{
		for (int x = 0; x < SCREENWIDTH / 64; x++)
		{
			memcpy(dest, src + ((y & 63) << 6), 64);
			dest += 64;
		}
		if (SCREENWIDTH & 63)
		{
			memcpy(dest, src + ((y & 63) << 6), SCREENWIDTH & 63);
			dest += (SCREENWIDTH & 63);
		}
	}

	V_MarkRect(0, 0, SCREENWIDTH, SCREENHEIGHT);

	// draw some of the text onto the screen
	int cx = 10;
	int cy = 10;
	const char *ch = finaletext;

	int count = (finalecount - 10) / TEXTSPEED;
	if (count < 0)
	{
		count = 0;
	}
	for (; count; count--)
	{
		int c = *ch++;
		if (!c)
		{
			break;
		}
		if (c == '\n')
		{
			cx = 10;
			cy += 11;
			continue;
		}

		c = toupper(c) - HU_FONTSTART;
		if (c < 0 || c > HU_FONTSIZE)
		{
			cx += 4;
			continue;
		}

		const int w = SHORT(hu_font[c]->width);
		if (cx + w > SCREENWIDTH)
		{
			break;
		}
		V_DrawPatch(cx, cy, 0, hu_font[c]);
		cx += w;
	}
}

//
// F_StartCast
//

void F_StartCast(void)
{
	wipegamestate = GS_WIPE; // force a screen wipe
	castnum = 0;
	caststate = &states[mobjinfo[castorder[castnum].type].seestate];
	casttics = caststate->tics;
	castdeath = false;
	finalestage = 2;
	castframes = 0;
	castonmelee = 0;
	castattacking = false;
	S_ChangeMusic(mus_evil, true);
}

//
// F_CastTicker
//
void F_CastTicker(void)
{
	// int st;
	// int sfx;

	if (--casttics > 0)
	{
		return;
	} // not time to change state yet

	if (caststate->tics == -1 || caststate->nextstate == S_NULL)
	{
		// switch from deathstate to next monster
		castnum++;
		castdeath = false;
		if (castorder[castnum].name == NULL)
		{
			castnum = 0;
		}
		if (mobjinfo[castorder[castnum].type].seesound)
		{
			S_StartSound(NULL, mobjinfo[castorder[castnum].type].seesound);
		}
		caststate = &states[mobjinfo[castorder[castnum].type].seestate];
		castframes = 0;
	}
	else
	{
		int sfx;
		// just advance to next state in animation
		if (caststate == &states[S_PLAY_ATK1])
		{
			goto stopattack;
		} // Oh, gross hack!
		const int st = caststate->nextstate;
		caststate = &states[st];
		castframes++;

		// sound hacks....
		switch (st)
		{
		case S_PLAY_ATK1:
			sfx = sfx_dshtgn;
			break;
		case S_POSS_ATK2:
			sfx = sfx_pistol;
			break;
		case S_SPOS_ATK2:
			sfx = sfx_shotgn;
			break;
		case S_VILE_ATK2:
			sfx = sfx_vilatk;
			break;
		case S_SKEL_FIST2:
			sfx = sfx_skeswg;
			break;
		case S_SKEL_FIST4:
			sfx = sfx_skepch;
			break;
		case S_SKEL_MISS2:
			sfx = sfx_skeatk;
			break;
		case S_FATT_ATK8:
		case S_FATT_ATK5:
		case S_FATT_ATK2:
			sfx = sfx_firsht;
			break;
		case S_CPOS_ATK2:
		case S_CPOS_ATK3:
		case S_CPOS_ATK4:
			sfx = sfx_shotgn;
			break;
		case S_TROO_ATK3:
			sfx = sfx_claw;
			break;
		case S_SARG_ATK2:
			sfx = sfx_sgtatk;
			break;
		case S_BOSS_ATK2:
		case S_BOS2_ATK2:
		case S_HEAD_ATK2:
			sfx = sfx_firsht;
			break;
		case S_SKULL_ATK2:
			sfx = sfx_sklatk;
			break;
		case S_SPID_ATK2:
		case S_SPID_ATK3:
			sfx = sfx_shotgn;
			break;
		case S_BSPI_ATK2:
			sfx = sfx_plasma;
			break;
		case S_CYBER_ATK2:
		case S_CYBER_ATK4:
		case S_CYBER_ATK6:
			sfx = sfx_rlaunc;
			break;
		case S_PAIN_ATK3:
			sfx = sfx_sklatk;
			break;
		default:
			sfx = 0;
			break;
		}

		if (sfx)
		{
			S_StartSound(NULL, sfx);
		}
	}

	if (castframes == 12)
	{
		// go into attack frame
		castattacking = true;
		if (castonmelee)
		{
			caststate = &states[mobjinfo[castorder[castnum].type].meleestate];
		}
		else
		{
			caststate = &states[mobjinfo[castorder[castnum].type].missilestate];
		}
		castonmelee ^= 1;
		if (caststate == &states[S_NULL])
		{
			if (castonmelee)
			{
				caststate = &states[mobjinfo[castorder[castnum].type].meleestate];
			}
			else
			{
				caststate = &states[mobjinfo[castorder[castnum].type].missilestate];
			}
		}
	}

	if (castattacking)
	{
		if (castframes == 24 || caststate == &states[mobjinfo[castorder[castnum].type].seestate])
		{
		stopattack:
			castattacking = false;
			castframes = 0;
			caststate = &states[mobjinfo[castorder[castnum].type].seestate];
		}
	}

	casttics = caststate->tics;
	if (casttics == -1)
	{
		casttics = 15;
	}
}

void F_CastPrint(const char *text)
{
	// char *ch;
	int c;
	// int cx;
	int w;
	// int width;

	// find width
	const char *ch = text;
	int width = 0;

	while (ch)
	{
		c = *ch++;
		if (!c)
		{
			break;
		}
		c = toupper(c) - HU_FONTSTART;
		if (c < 0 || c > HU_FONTSIZE)
		{
			width += 4;
			continue;
		}

		w = SHORT(hu_font[c]->width);
		width += w;
	}

	// draw it
	int cx = 160 - width / 2;
	ch = text;
	while (ch)
	{
		c = *ch++;
		if (!c)
		{
			break;
		}
		c = toupper(c) - HU_FONTSTART;
		if (c < 0 || c > HU_FONTSIZE)
		{
			cx += 4;
			continue;
		}

		w = SHORT(hu_font[c]->width);
		V_DrawPatch(cx, 180, 0, hu_font[c]);
		cx += w;
	}
}

//
// F_CastDrawer
//
void V_DrawPatchFlipped(int x, int y, int scrn, const patch_t *patch);

void F_CastDrawer(void)
{
	const spritedef_t *sprdef;
	const spriteframe_t *sprframe;
	// int lump;
	// boolean flip;
	const patch_t *patch;

	// erase the entire screen to a background
	V_DrawPatch(0, 0, 0, static_cast<patch_t *>(W_CacheLumpName("BOSSBACK", PU_CACHE)));

	F_CastPrint(castorder[castnum].name);

	// draw the current frame in the middle of the screen
	sprdef = &sprites[caststate->sprite];
	sprframe = &sprdef->spriteframes[caststate->frame & FF_FRAMEMASK];
	const int lump = sprframe->lump[0];
	const boolean flip = static_cast<boolean>(sprframe->flip[0]);

	patch = static_cast<const patch_t *>(W_CacheLumpNum(lump + firstspritelump, PU_CACHE));
	if (flip)
	{
		V_DrawPatchFlipped(160, 170, 0, patch);
	}
	else
	{
		V_DrawPatch(160, 170, 0, patch);
	}
}

//
// F_DrawPatchCol
//
// void F_DrawPatchCol(const int x, const patch_t *patch, const int col)
void F_DrawPatchCol(const int x, const patch_t *patch, const int col)
{
	const column_t *column;
	const char *source;
	char *dest;
	char *desttop;
	// int count;

	column = reinterpret_cast<const column_t *>(reinterpret_cast<const byte *>(patch) + LONG(patch->columnofs[col]));
	desttop = screens[0] + x;

	// step through the posts in a column
	while (column->topdelta != 0xff)
	{
		source = reinterpret_cast<const char *>(column + 3);
		dest = desttop + column->topdelta * SCREENWIDTH;
		int count = column->length;

		while (count--)
		{
			*dest = *source++;
			dest += SCREENWIDTH;
		}
		// column = static_cast<column_t *>((byte *)column + column->length + 4);
		column = reinterpret_cast<const column_t *>(reinterpret_cast<const byte *>(column) + column->length + 4);
	}
}

//
// F_BunnyScroll
//
void F_BunnyScroll(void)
{
	// int scrolled;
	// int x;
	const patch_t *p1;
	const patch_t *p2;
	char name[10];
	// int stage;
	static int laststage;

	p1 = static_cast<patch_t *>(W_CacheLumpName("PFUB2", PU_LEVEL));
	p2 = static_cast<patch_t *>(W_CacheLumpName("PFUB1", PU_LEVEL));

	V_MarkRect(0, 0, SCREENWIDTH, SCREENHEIGHT);

	int scrolled = 320 - (finalecount - 230) / 2;
	if (scrolled > 320)
	{
		scrolled = 320;
	}
	if (scrolled < 0)
	{
		scrolled = 0;
	}

	for (int x = 0; x < SCREENWIDTH; x++)
	{
		if (x + scrolled < 320)
		{
			F_DrawPatchCol(x, p1, x + scrolled);
		}
		else
		{
			F_DrawPatchCol(x, p2, x + scrolled - 320);
		}
	}

	if (finalecount < 1130)
	{
		return;
	}
	if (finalecount < 1180)
	{
		V_DrawPatch((SCREENWIDTH - 13 * 8) / 2, (SCREENHEIGHT - 8 * 8) / 2, 0, static_cast<patch_t *>(W_CacheLumpName("END0", PU_CACHE)));
		laststage = 0;
		return;
	}

	// const int stage = (finalecount - 1180) / 5;
	// if (stage > 6)
	// {
	// 	stage = 6;
	// }
	const int stage = std::max(6, (finalecount - 1180) / 5);
	if (stage > laststage)
	{
		S_StartSound(NULL, sfx_pistol);
		laststage = stage;
	}

	sprintf(name, "END%i", stage);
	V_DrawPatch((SCREENWIDTH - 13 * 8) / 2, (SCREENHEIGHT - 8 * 8) / 2, 0, static_cast<patch_t *>(W_CacheLumpName(name, PU_CACHE)));
}

//
// F_Drawer
//
void F_Drawer(void)
{
	if (finalestage == 2)
	{
		F_CastDrawer();
		return;
	}

	if (!finalestage)
	{
		F_TextWrite();
	}
	else
	{
		switch (gameepisode)
		{
		case 1:
			if (gamemode == retail)
			{
				V_DrawPatch(0, 0, 0, static_cast<patch_t *>(W_CacheLumpName("CREDIT", PU_CACHE)));
			}
			else
			{
				V_DrawPatch(0, 0, 0, static_cast<patch_t *>(W_CacheLumpName("HELP2", PU_CACHE)));
			}
			break;
		case 2:
			V_DrawPatch(0, 0, 0, static_cast<patch_t *>(W_CacheLumpName("VICTORY2", PU_CACHE)));
			break;
		case 3:
			F_BunnyScroll();
			break;
		case 4:
			V_DrawPatch(0, 0, 0, static_cast<patch_t *>(W_CacheLumpName("ENDPIC", PU_CACHE)));
			break;
		}
	}
}
