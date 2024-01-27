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
//	Mission begin melt/wipe screen special effect.
//
//-----------------------------------------------------------------------------

static const char rcsid[] = "$Id: f_wipe.c,v 1.2 1997/02/03 22:45:09 b1 Exp $";

#include "z_zone.H"
#include "i_video.H"
#include "v_video.H"
#include "m_random.H"

#include "doomdef.H"

#include "f_wipe.H"

//
// SCREEN WIPE PACKAGE
//

// when zero, stop the wipe
static boolean go = 0;

// static byte *wipe_scr_start;
// static byte *wipe_scr_end;
// static byte *wipe_scr;
// static short *wipe_scr_start;
// static short *wipe_scr_end;
// static short *wipe_scr;

static int *wipe_scr_start;
static int *wipe_scr_end;
static int *wipe_scr;

// void wipe_shittyColMajorXform(short *array, const size_t width, const size_t height)
void wipe_shittyColMajorXform(
    short *array,
    const size_t width,
    const size_t height)
{
    // int x;
    // int y;
    short *dest;

    dest = static_cast<short *>(Z_Malloc(width * height * 2, PU_STATIC, 0));

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            dest[x * height + y] = array[y * width + x];
        }
    }

    memcpy(array, dest, width * height * 2);

    Z_Free(dest);
}

int wipe_initColorXForm(
    const size_t width,
    const size_t height,
    __attribute__((unused)) const byte ticks)
{
    memcpy(wipe_scr, wipe_scr_start, width * height);
    return 0;
}

int wipe_doColorXForm(
    const size_t width,
    const size_t height,
    // const short ticks)
    const byte ticks)
{
    boolean changed;
    int *w;
    const int *e;
    // int newval;
    // short newval;
    // byte newval;

    changed = false;
    w = wipe_scr;
    e = wipe_scr_end;

    while (w != wipe_scr + width * height)
    {
        if (*w != *e)
        {
            if (*w > *e)
            {
                const byte newval = *w - ticks;
                if (newval < *e)
                {
                    *w = *e;
                }
                else
                {
                    *w = newval;
                }
                changed = true;
            }
            else if (*w < *e)
            {
                const byte newval = *w + ticks;
                if (newval > *e)
                {
                    *w = *e;
                }
                else
                {
                    *w = newval;
                }
                changed = true;
            }
        }
        w++;
        e++;
    }

    return !changed;
}

int wipe_exitColorXForm(
    __attribute__((unused)) const size_t width,
    __attribute__((unused)) const size_t height,
    __attribute__((unused)) const byte ticks)
{
    return 0;
}

static int *y;
// static size_t *y;

int wipe_initMelt(
    const size_t width,
    const size_t height,
    __attribute__((unused)) const byte ticks)
{
    // copy start screen to main screen
    memcpy(wipe_scr, wipe_scr_start, width * height);

    // makes this wipe faster (in theory)
    // to have stuff in column-major format
    // wipe_shittyColMajorXform((short *)wipe_scr_start, width / 2, height);
    // wipe_shittyColMajorXform((short *)wipe_scr_end, width / 2, height);

    // This cast seems borderline psychotic
    wipe_shittyColMajorXform(static_cast<short *>(static_cast<void *>(wipe_scr_start)), width / 2, height);
    wipe_shittyColMajorXform(static_cast<short *>(static_cast<void *>(wipe_scr_end)), width / 2, height);

    // setup initial column positions
    // (y<0 => not ready to scroll yet)
    y = static_cast<int *>(Z_Malloc(width * sizeof(int), PU_STATIC, 0));
    y[0] = -(M_Random() % 16);
    for (size_t i = 1; i < width; i++)
    {
        const int r = (M_Random() % 3) - 1;
        y[i] = y[i - 1] + r;
        if (y[i] > 0)
        {
            y[i] = 0;
        }
        else if (y[i] == -16)
        {
            y[i] = -15;
        }
    }

    return 0;
}

int wipe_doMelt(int width, const int height, byte ticks)
{
    // int i;
    // int j;
    int dy;
    // int idx;
    // size_t idx;

    int *s;
    int *d;
    boolean done = true;

    width /= 2;

    while (ticks--)
    {
        for (size_t i = 0; i < width; i++)
        {
            if (y[i] < 0)
            {
                y[i]++;
                done = false;
            }
            else if (y[i] < static_cast<int>(height))
            {
                dy = (y[i] < 16) ? y[i] + 1 : 8;
                if (y[i] + dy >= static_cast<int>(height))
                {
                    dy = static_cast<int>(height) - y[i];
                }
                s = &(wipe_scr_end)[i * height + y[i]];
                d = &(wipe_scr)[y[i] * width + i];
                size_t idx = 0;
                for (int j = dy; j; j--)
                {
                    d[idx] = *(s++);
                    idx += width;
                }
                y[i] += dy;
                s = &(reinterpret_cast<short *>(wipe_scr_start))[i * height];
                d = &(reinterpret_cast<short *>(wipe_scr))[static_cast<size_t>(y[i]) * width + i];
                idx = 0;
                for (size_t j = height - static_cast<size_t>(y[i]); j; j--)
                {
                    d[idx] = *(s++);
                    idx += width;
                }
                done = false;
            }
        }
    }

    return done;
}

int wipe_exitMelt(
    __attribute__((unused)) const size_t width,
    __attribute__((unused)) const size_t height,
    __attribute__((unused)) const byte ticks)
{
    Z_Free(y);
    return 0;
}

int wipe_StartScreen(
    __attribute__((unused)) const int x,
    __attribute__((unused)) const int y_,
    __attribute__((unused)) const int width,
    __attribute__((unused)) const int height)
{
    wipe_scr_start = screens[2];
    I_ReadScreen(wipe_scr_start);
    return 0;
}

int wipe_EndScreen(int x, int y, int width, int height)
{
    wipe_scr_end = screens[3];
    I_ReadScreen(wipe_scr_end);
    V_DrawBlock(x, y, 0, width, height, wipe_scr_start); // restore start scr.
    return 0;
}

int wipe_ScreenWipe(int wipeno, int x, int y, int width, int height, int ticks)
{
    int rc;
    static int (*wipes[])(size_t, size_t, byte) = {
        wipe_initColorXForm, wipe_doColorXForm, wipe_exitColorXForm,
        wipe_initMelt, wipe_doMelt, wipe_exitMelt};

    void V_MarkRect(int, int, int, int);

    // initial stuff
    if (!go)
    {
        go = 1;
        // wipe_scr = (byte *) Z_Malloc(width*height, PU_STATIC, 0); // DEBUG
        wipe_scr = screens[0];
        (*wipes[wipeno * 3])(width, height, ticks);
    }

    // do a piece of wipe-in
    V_MarkRect(0, 0, width, height);
    rc = (*wipes[wipeno * 3 + 1])(width, height, ticks);
    //  V_DrawBlock(x, y, 0, width, height, wipe_scr); // DEBUG

    // final stuff
    if (rc)
    {
        go = 0;
        (*wipes[wipeno * 3 + 2])(width, height, ticks);
    }

    return !go;
}
