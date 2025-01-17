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
//
// DESCRIPTION:
//	System interface, sound.
//
//-----------------------------------------------------------------------------

#ifndef __I_SOUND__
#define __I_SOUND__

#include "doomdef.H"

// UNIX hack, to be removed.
#ifdef SNDSERV
#include <stdio.h>
extern FILE *sndserver;
extern const char *sndserver_filename;
#endif

#include "doomstat.H"
#include "sounds.H"

// Init at program start...
void I_InitSound();

// ... update sound buffer and audio device at runtime...
void I_UpdateSound(void);
void I_SubmitSound(void);

// ... shut down and relase at program termination.
void I_ShutdownSound(void);

//
//  SFX I/O
//

// static int looping = 0;

// Initialize channels?
void I_SetChannels();

// Get raw data lump index for sound descriptor.
int I_GetSfxLumpNum(
    const sfxinfo_t *sfxinfo);

// Starts a sound in a particular sound channel.
int I_StartSound(
    const int id,
    const int vol,
    const int sep,
    const int pitch,
    __attribute__((unused)) const int priority);

// Stops a sound channel.
void I_StopSound(
    __attribute__((unused)) const int handle);

// Called by S_*() functions
//  to see if a channel is still playing.
// Returns 0 if no longer playing, 1 if playing.
int I_SoundIsPlaying(
    const int handle);

// Updates the volume, separation,
//  and pitch of a sound channel.
void I_UpdateSoundParams(
    __attribute__((unused)) const int handle,
    __attribute__((unused)) const int vol,
    __attribute__((unused)) const int sep,
    __attribute__((unused)) const int pitch);

//
//  MUSIC I/O
//
void I_InitMusic(void);
void I_ShutdownMusic(void);
// Volume.
void I_SetMusicVolume(int volume);
// PAUSE game handling.
void I_PauseSong(
    __attribute__((unused)) const int handle);
void I_ResumeSong(
    __attribute__((unused)) const int handle);
// Registers a song handle to song data.
int I_RegisterSong(
    __attribute__((unused)) const void *handle);
// Called by anything that wishes to start music.
//  plays a song, and when the song is done,
//  starts playing it again in an endless loop.
// Horrible thing to do, considering.
void I_PlaySong(
    __attribute__((unused)) const int handle);
// Stops a song over 3 seconds.
void I_StopSong(
    __attribute__((unused)) const int handle);
// See above (register), then think backwards
void I_UnRegisterSong(
    __attribute__((unused)) const int handle);

#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------
