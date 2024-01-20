################################################################
#
# $Id:$
#
# $Log:$
#
# CC = gcc  # gcc or g++

# CFLAGS = -g -Wall -Wextra -Wpedantic -DNORMALUNIX -DLINUX # -DUSEASM 

CXX = g++
CXXSTANDARD = -std=c++11
OPTFLAGS = -O3 -flto
MFLAGS = -mcpu=native
WFLAGS = -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wformat-security -Winline -Wformat=2 -Wattributes \
-Wbuiltin-macro-redefined -Wcast-align -Wdiv-by-zero -Wdouble-promotion -Wfloat-equal -Wint-to-pointer-cast -Wlogical-op -Woverflow \
-Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wwrite-strings -Wimplicit-fallthrough=5 -Wstringop-overflow=4 -Wstrict-aliasing=3
EXTRA_CXXFLAGS = -DNORMALUNIX -DLINUX
# LDFLAGS = -L/usr/X11R6/lib
LDFLAGS = -L/usr/lib/X11/
LIBS = -lXext -lX11 -lnsl -lm
CXXFLAGS = $(CXXSTANDARD) $(OPTFLAGS) $(MFLAGS) $(PFLAGS) $(WFLAGS) $(EXTRA_CXXFLAGS) $(INCLUDE)

# subdirectory for objects
O = linux

# not too sophisticated dependency
OBJS =						\
		$(O)/doomdef.o		\
		$(O)/doomstat.o		\
		$(O)/dstrings.o		\
		$(O)/i_system.o		\
		$(O)/i_sound.o		\
		$(O)/i_video.o		\
		$(O)/i_net.o		\
		$(O)/tables.o		\
		$(O)/f_finale.o		\
		$(O)/f_wipe.o 		\
		$(O)/d_main.o		\
		$(O)/d_net.o		\
		$(O)/d_items.o		\
		$(O)/g_game.o		\
		$(O)/m_menu.o		\
		$(O)/m_misc.o		\
		$(O)/m_argv.o  		\
		$(O)/m_bbox.o		\
		$(O)/m_fixed.o		\
		$(O)/m_swap.o		\
		$(O)/m_cheat.o		\
		$(O)/m_random.o		\
		$(O)/am_map.o		\
		$(O)/p_ceilng.o		\
		$(O)/p_doors.o		\
		$(O)/p_enemy.o		\
		$(O)/p_floor.o		\
		$(O)/p_inter.o		\
		$(O)/p_lights.o		\
		$(O)/p_map.o		\
		$(O)/p_maputl.o		\
		$(O)/p_plats.o		\
		$(O)/p_pspr.o		\
		$(O)/p_setup.o		\
		$(O)/p_sight.o		\
		$(O)/p_spec.o		\
		$(O)/p_switch.o		\
		$(O)/p_mobj.o		\
		$(O)/p_telept.o		\
		$(O)/p_tick.o		\
		$(O)/p_saveg.o		\
		$(O)/p_user.o		\
		$(O)/r_bsp.o		\
		$(O)/r_data.o		\
		$(O)/r_draw.o		\
		$(O)/r_main.o		\
		$(O)/r_plane.o		\
		$(O)/r_segs.o		\
		$(O)/r_sky.o		\
		$(O)/r_things.o		\
		$(O)/w_wad.o		\
		$(O)/wi_stuff.o		\
		$(O)/v_video.o		\
		$(O)/st_lib.o		\
		$(O)/st_stuff.o		\
		$(O)/hu_stuff.o		\
		$(O)/hu_lib.o		\
		$(O)/s_sound.o		\
		$(O)/z_zone.o		\
		$(O)/info.o			\
		$(O)/sounds.o

all:	 $(O)/linuxxdoom

# default:
# 	$(CXX) $(CXXFLAGS) *.C -o linuxxdoom

clean:
	rm -f *.o *~ *.flc
	rm -f linux/*
	rm -rf $(HOME)/DOOM/linux/

$(O)/linuxxdoom:	$(OBJS) $(O)/i_main.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) $(O)/i_main.o \
	$(LIBS) -o $(O)/linuxxdoom

install:
	cp -r linux/ $(HOME)/DOOM/

$(O)/%.o:	%.C
	$(CXX) $(CXXFLAGS) -c $< -o $@

#############################################################
#
#############################################################