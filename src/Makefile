#$(EXECNAME): A Commander Keen Reimplementation
#Copyright (C) 2012 David Gow
#$(EXECNAME) is available at http://davidgow.net/keen/$(EXECNAME).html
#
#This program is free software; you can redistribute it and/or
#modify it under the terms of the GNU General Public License
#as published by the Free Software Foundation; either version 2
#of the License, or (at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program; if not, write to the Free Software
#Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

define HELP

This Makefile supports the following options:

  PLATFORM      the platform to build for
                  (unset) = native build for Linux or similar Unix-like OS
                  win32   = MinGW cross-build for 32-bit Windows
                  win64   = MinGW cross-build for 64-bit Windows
                  dos     = DJGPP cross-build for DOS (with DOS Extender)
                  gcw0    = OpenDingux cross-build for GCW Zero
		  serenity= SenerityOS build

  RENDERER      the type of video output API (not available for PLATFORM=dos)
		  sdl1    = SDL 1.2 (deprecated; no win64 support)
		  sdl2    = SDL 2.0 + SDL_Renderer (Windows default)
		  sdl2sw  = SDL 2.0 + Software SDL_Renderer
		  sdl2gl  = SDL 2.0 + OpenGL 2.0 (default)
		  sdl2vk  = SDL 2.0 + Vulkan (experimental)
		  null    = dummy renderer

  WITH_ALSA     whether to include ALSA support for real OPL2 hardware (Linux only)
  
  WITH_IEEE1284 whether to include support for the OPL2LPT parallel port soundcard
		  Linux only, requires libieee1284
		  Activate this at runtime with the /OPL2LPT argument. 

  STATIC        whether to build a static executable (0/1)
                  - defaults to 1 for Windows platforms, 0 otherwise

  DEBUG 	whether to include debug information (0/1; default: off)
  
  VANILLA	whether to disable $(EXECNAME)-only features (0/1; default: off)

  BUILDASCPP    whether to build as C++ code (0/1)
                  - defaults to 1 for Linux/Unix, 0 otherwise

  CLANG         whether to use GCC (0; default) or Clang (1) compilers

  LOCAL_SDL     whether to download and use a local copy of the SDL
                development headers (0/1; only available for
                PLATFORM=win32/64, and set as default there)

  KEEN6VER      version of Commander Keen Episode 6 to copy data for
                  - keen6e14 or keen6e15 (default)

  KEENPATH	set the default path to the Commander Keen data files
  		  - defaults to './data' -- the current data directory
		  - can be overridden at runtime with /KEENPATH

  USERPATH	set the default path for user savegames
  		  - defaults to '.' -- the current directory
		  - can be overridden at runtime with /USERPATH

  XDGUSERPATH	prefer XDG user paths to the default path above (0/1; default: off)
  		  - these are typically ~/.local/share/Commander Keen/$(EXECNAME)/*
		  - requires SDL >= 2.0.1 (SDL_GetPrefPath())
		  - uses a similar per-user path on other platforms
		  - this path is used as a fallback even if this setting is off
		    (see: FS_USER_XDG_FALLBACK in ck_config.h)

  SDL1_VERSION  version of SDL 1.x to download and use (if LOCAL_SDL=1)
  SDL2_VERSION  version of SDL 2.x to download and use (if LOCAL_SDL=1)

  COMPILER	explicit path of the compiler (overrides CLANG option)
  CXXFLAGS      additional options for the compiler
  LDFLAGS       additional options for the linker
  SYSFLAGS      additional options for both the compiler and the linker
  LIBS		additional libraries to link

  TOOLCHAIN_DIR	set path to toolchain for cross-compiling (e.g. serenity, gcw0, dos)
endef

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# platform selection

DEFAULT_STATIC = 0
DEFAULT_RENDERER = sdl2gl
DEFAULT_BUILDASCPP = 0
DEFAULT_LOCAL_SDL = 0
BINDIR =
LIBGL = -lGL
LIBVULKAN = -lvulkan
LIBASOUND = -lasound
LIBIEEE1284 = -lieee1284
SDL_CONFIG_PATH =
SDL_CONFIG_PREFIX =
OUTDIR_SUFFIX := INVALID
TOOLCHAIN_DIR =
KEENPATH = ./data
EXECNAME = omnispeak

ifeq ($(PLATFORM),)
	OUTDIR_SUFFIX :=
	DEFAULT_BUILDASCPP = 1
endif

ifneq (,$(findstring win,$(PLATFORM)))
	# common Windows stuff
	EXE_EXT = .exe
	DEFAULT_STATIC = 1
	DEFAULT_LOCAL_SDL = 1
	DEFAULT_RENDERER = sdl2
	LIBGL = -lopengl32
endif
ifeq ($(PLATFORM), win32)
	OUTDIR_SUFFIX := -win32
	TOOLSET = i686-w64-mingw32
endif
ifeq ($(PLATFORM), win64)
	OUTDIR_SUFFIX := -win64
	TOOLSET = x86_64-w64-mingw32
endif

ifeq ($(PLATFORM), dos)
	EXE_EXT = .exe
	OUTDIR_SUFFIX := -dos
	TOOLSET = i586-pc-msdosdjgpp
	TOOLDIR = /usr/local/djgpp/bin
	DEFAULT_RENDERER = dos
	DEFAULT_BUILDASCPP = 1
endif

ifeq ($(PLATFORM), gcw0)
        OUTDIR_SUFFIX := -gcw0
        TOOLCHAIN_DIR = /opt/gcw0-toolchain
        TOOLSET = mipsel-gcw0-linux-uclibc
        TOOLDIR = $(TOOLCHAIN_DIR)/usr/bin
        DEFAULT_RENDERER = sdl2sw
        SDL_CONFIG_PATH = $(TOOLCHAIN_DIR)/usr/$(TOOLSET)/sysroot/usr/bin/
        DEFAULT_BUILDASCPP = 1
        # HACK: gcw0 misel architecture doesn't define O_PATH
        CXXFLAGS += -DO_PATH=010000000
endif

ifeq ($(PLATFORM), serenity)
        OUTDIR_SUFFIX := -serenity
	TOOLSET = i686-pc-serenity
        TOOLDIR = $(TOOLCHAIN_DIR)/Local/i686/bin/
        DEFAULT_RENDERER = sdl2sw
        SDL_CONFIG_PATH = $(TOOLCHAIN_DIR)/../Build/i686/Root/usr/bin/
	# HACK: should not be need, but my sdl-config is broken
	SDL_CONFIG_PREFIX = $(TOOLCHAIN_DIR)/../Build/i686/Root/usr
        DEFAULT_BUILDASCPP = 1
	LIBS += -lgui -lgfx -lcore -lipc -lcompress -lpthread -lstdc++ -lm
	EXECNAME = KEEN
	CXXFLAGS = -std=c++2a
endif

ifeq ($(OUTDIR_SUFFIX),INVALID)
	DUMMY := $(error No valid platform selected)
endif

BINDIR = ../bin$(OUTDIR_SUFFIX)
OBJDIR = ../obj$(OUTDIR_SUFFIX)

# handle some special cases
ifeq ($(PLATFORM)-$(RENDERER),win32-sdl1)
	DEFAULT_STATIC = 0
endif

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# set configuration variables (with user overrides) and derived variables

RENDERER ?= $(DEFAULT_RENDERER)
BUILDASCPP ?= $(DEFAULT_BUILDASCPP)
WITH_ALSA ?= 0
WITH_IEEE1284 ?= 0
STATIC ?= $(DEFAULT_STATIC)
DEBUG ?= 0
VANILLA ?= 0
LOCAL_SDL ?= $(DEFAULT_LOCAL_SDL)
KEEN6VER ?= keen6e15
XDGUSERPATH ?= 0
SDL1_VERSION ?= 1.2.15
SDL2_VERSION ?= 2.0.14
SYSFLAGS ?=
CXX ?=
CXXFLAGS ?=
LDFLAGS ?=
LIBS ?=

# mandatory source files
IDOBJECTS = ck_quit.o id_mm.o id_fs.o id_ca.o id_in.o id_rf.o id_sd.o id_ti.o id_us_1.o id_us_2.o id_us_textscreen.o id_vh.o id_vl.o id_str.o
CKOBJECTS = ck_act.o ck_inter.o ck_keen.o ck_obj.o ck_map.o ck4_map.o ck5_map.o ck6_map.o ck4_obj1.o ck4_obj2.o ck4_obj3.o ck5_obj1.o ck5_obj2.o ck5_obj3.o ck6_obj1.o ck6_obj2.o ck6_obj3.o ck_phys.o ck_game.o ck_play.o ck_misc.o ck4_misc.o ck5_misc.o ck6_misc.o ck_main.o ck_text.o ck_cross.o icon.o
OPLOBJECTS = opl/dbopl.o opl/nuked_opl3.o

ifeq ($(PLATFORM), serenity)
CKOBJECTS += main.o
endif

# data files
DATAFILES = ACTION AUDINFOE AUDIODCT AUDIOHHD EGADICT EGAHEAD GFXINFOE MAPHEAD TILEINFO
K4DATA = $(foreach f, $(DATAFILES), $(BINDIR)/data/$(f).CK4)
K5DATA = $(foreach f, $(DATAFILES), $(BINDIR)/data/$(f).CK5)
K6DATA = $(foreach f, $(DATAFILES), $(BINDIR)/data/$(f).CK6)

# output binary file
OUTBIN = $(BINDIR)/$(EXECNAME)$(EXE_EXT)

# set BINPREFIX
ifneq ($(TOOLSET),)
	ifeq ($(TOOLDIR),)
		BINPREFIX = $(TOOLSET)-
	else
		BINPREFIX = $(TOOLDIR)/$(TOOLSET)-
	endif
endif

# pick GCC or Clang
ifeq ($(CLANG), 1)
	CXX = clang++
	CC = clang
	ifeq ($(BUILDASCPP), 1)
		# suppress "treating C input as C++ [...] is deprecated" warning
		CXXFLAGS += -Wno-deprecated
	endif
else
	CXX = g++
	CC = gcc
endif

# set C or C++ compiler
ifneq ($(COMPILER),)
	CXX := $(COMPILER)
else
	ifeq ($(BUILDASCPP), 1)
		CXX := $(BINPREFIX)$(CXX)
	else
		CXX := $(BINPREFIX)$(CC)
	endif
endif
ifneq ($(BUILDASCPP), 1)
	CXXFLAGS += -std=c99
endif

# set flags for debug info
ifeq ($(DEBUG), 1)
	CXXFLAGS += -g -O0 -DCK_DEBUG
	LDFLAGS += -g
	ifneq (,$(findstring win,$(PLATFORM)))
		LIBS += -mconsole
		# note: deliberately put into LIBS instead of LDFLAGS
		# because SDL_LIBS usually contains -mwindows
	endif
else
	CXXFLAGS += -O2
endif	

# set vanilla mode
ifeq ($(VANILLA), 1)
	CXXFLAGS += -DCK_VANILLA
endif

# set linker flags for static linking
ifeq ($(STATIC), 1)
	LDFLAGS += -static
	SDL_LIB_QUERY = --static-libs
else
	SDL_LIB_QUERY = --libs
endif

# set parameters that depend on the SDL version (1/2)
ifeq ($(RENDERER), sdl1)
	SDL_CONFIG_BIN = $(SDL_CONFIG_PATH)sdl-config
	SDL_VERSION = $(SDL1_VERSION)
	SDL_URL = http://libsdl.org/release/SDL-devel-$(SDL_VERSION)-mingw32.tar.gz
	SDL_LOCAL_PREFIX = SDL-$(SDL_VERSION)
else
	ifneq ($(SDL_CONFIG_PREFIX), )
	SDL_CONFIG_BIN = $(SDL_CONFIG_PATH)sdl2-config --prefix=$(SDL_CONFIG_PREFIX)
	else
	SDL_CONFIG_BIN = $(SDL_CONFIG_PATH)sdl2-config
	endif
	SDL_VERSION = $(SDL2_VERSION)
	SDL_URL = http://libsdl.org/release/SDL2-devel-$(SDL_VERSION)-mingw.tar.gz
	SDL_LOCAL_PREFIX = SDL2-$(SDL_VERSION)/$(TOOLSET)
endif

# set SDL specific parameters
ifneq (,$(findstring sdl,$(RENDERER)))
	ifeq ($(LOCAL_SDL),1)
		SDL_DIR = ../$(SDL_LOCAL_PREFIX)
		SDL_CONFIG = $(SDL_DIR)/bin/$(SDL_CONFIG_BIN) --prefix=$(SDL_DIR)
	else
		SDL_CONFIG = $(SDL_CONFIG_BIN)
	endif
	SDL_CFLAGS = `$(SDL_CONFIG) --cflags` -DWITH_SDL
	SDL_LIBS = `$(SDL_CONFIG) $(SDL_LIB_QUERY)`
endif

# set Windows-specific parameters
ifneq (,$(findstring win,$(PLATFORM)))
	BATFILES = $(BINDIR)/_keen4.cmd $(BINDIR)/_keen5.cmd $(BINDIR)/_keen6.cmd
	RES = $(OBJDIR)/windowsres.res
	# MS Bitfields has a bug with packed structures on some GCC versions:
	# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=52991
	CXXFLAGS += -mno-ms-bitfields
endif

# set GCW Zero-specific parameters
ifeq ($(PLATFORM), gcw0)
        SHELLFILES = $(BINDIR)/keen4.sh $(BINDIR)/keen5.sh $(BINDIR)/keen6.sh
        SHELLOPTS = /FULLSCREEN /NOBORDER /NOCOPY
endif

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# renderer-specific input file and library selection

ifeq ($(RENDERER), sdl1)
	RENDER_OBJS = id_vl_sdl12.o id_sd_sdl.o id_in_sdl.o
endif

ifeq ($(RENDERER), sdl2)
	RENDER_OBJS = id_vl_sdl2.o id_sd_sdl.o id_in_sdl.o
endif

ifeq ($(RENDERER), sdl2sw)
	RENDER_OBJS = id_vl_sdl2.o id_sd_sdl.o id_in_sdl.o
	CXXFLAGS += -DVL_SDL2_REQUEST_SOFTWARE
endif

ifeq ($(RENDERER), sdl2gl)
	RENDER_OBJS = id_vl_sdl2gl.o id_sd_sdl.o id_in_sdl.o
	LIBS += $(LIBGL)
endif

ifeq ($(RENDERER), sdl2vk)
	RENDER_OBJS = id_vl_sdl2vk.o id_sd_sdl.o id_in_sdl.o
	LIBS += $(LIBVULKAN)
	ifeq ($(PLATFORM),)
		LIBS += -lX11 -lX11-xcb
	endif
endif

ifeq ($(RENDERER), dos)
	RENDER_OBJS = id_vl_dos.o id_sd_dos.o id_in_dos.o
endif

ifeq ($(RENDERER), null)
	RENDER_OBJS = id_vl_null.o id_sd_null.o id_in_null.o
endif

IDOBJECTS += $(RENDER_OBJS)

ifeq ($(WITH_ALSA), 1)
	IDOBJECTS += id_sd_opl2alsa.o
	CXXFLAGS += -DSD_OPL2_WITH_ALSA
	LIBS += $(LIBASOUND)
endif

ifeq ($(WITH_IEEE1284), 1)
	IDOBJECTS += id_sd_opl2lpt.o
	CXXFLAGS += -DSD_OPL2_WITH_IEEE1284
	LIBS += $(LIBIEEE1284)
endif

ifneq ($(KEENPATH),)
	CXXFLAGS += -DFS_DEFAULT_KEEN_PATH=\"$(KEENPATH)\"
endif

ifneq ($(USERPATH),)
	CXXFLAGS += -DFS_DEFAULT_USER_PATH=\"$(USERPATH)\"
endif

ifeq ($(XDGUSERPATH),1)
	CXXFLAGS += -DFS_USER_PATH_PREFER_XDG=1
endif

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

OBJ := $(IDOBJECTS:%=$(OBJDIR)/%)
OBJ += $(CKOBJECTS:%=$(OBJDIR)/%)
OBJ += $(OPLOBJECTS:%=$(OBJDIR)/%)
DEPS := $(OBJ:%.o=%.d)

all: $(OUTBIN) binfiles batfiles shellfiles

help:
	$(info $(HELP))
	@true  # suppress "nothing to be done" message

dumpconfig:
	@echo PLATFORM = $(PLATFORM)
	@echo RENDERER = $(RENDERER)
	@echo WITH_ALSA = $(WITH_ALSA)
	@echo WITH_IEEE1284 = $(WITH_IEEE1284)
	@echo STATIC = $(STATIC)
	@echo VANILLA = $(VANILLA)
	@echo DEBUG = $(DEBUG)
	@echo BUILDASCPP = $(BUILDASCPP)
	@echo LOCAL_SDL = $(LOCAL_SDL)
	@echo KEEN6VER = $(KEEN6VER)
	@echo KEENPATH = $(KEENPATH)
	@echo USERPATH = $(USERPATH)
	@echo XDGUSERPATH = $(XDGUSERPATH)
	@echo SDL_VERSION = $(SDL_VERSION)
	@echo
	@echo BINPREFIX = $(BINPREFIX)
	@echo CXX = $(CXX)
	@echo SYSFLAGS = $(SYSFLAGS)
	@echo CXXFLAGS = $(CXXFLAGS)
	@echo LDFLAGS = $(LDFLAGS)
	@echo LIBS = $(LIBS)
	@echo
	@echo BINDIR = $(BINDIR)
	@echo OBJDIR = $(OBJDIR)
	@echo RENDER_OBJS = $(RENDER_OBJS)
	@echo SDL_URL = $(SDL_URL)
	@echo SDL_DIR = $(SDL_DIR)
	@echo SDL_CONFIG = $(SDL_CONFIG)
	@echo SDL_CFLAGS = $(SDL_CFLAGS)
	@echo SDL_LIBS = $(SDL_LIBS)

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR)/%.o: %.c* $(SDL_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(SYSFLAGS) $(CXXFLAGS) $(SDL_CFLAGS) -c -MMD -o $@ $<

$(OBJDIR)/windowsres.res: windowsres.rc
	mkdir -p $(dir $@)
	$(BINPREFIX)windres $< -O coff -o $@

$(OUTBIN): $(BINDIR) $(SDL_DIR) $(OBJ) $(RES)
	$(CXX) $(SYSFLAGS) $(LDFLAGS) -o $(OUTBIN) $(OBJ) $(RES) $(SDL_LIBS) -lm $(LIBS)

# rules to copy data files
binfiles: keen4data keen5data keen6data
keen4data: $(BINDIR) $(K4DATA)
keen5data: $(BINDIR) $(K5DATA)
keen6data: $(BINDIR) $(K6DATA)
$(BINDIR)/data/%.CK4: ../data/keen4/%.CK4
	mkdir -p $(BINDIR)/data
	cp -f $< $@ 
$(BINDIR)/data/%.CK5: ../data/keen5/%.CK5
	mkdir -p $(BINDIR)/data
	cp -f $< $@ 
$(BINDIR)/data/%.CK6: ../data/$(KEEN6VER)/%.CK6
	mkdir -p $(BINDIR)/data
	cp -f $< $@

# generate Windows batch files to quickly start a specific episode
batfiles: $(BATFILES)
$(BINDIR)/_keen4.cmd:
	echo -n "start $(EXECNAME).exe /EPISODE 4 /NOWAIT" >$@
$(BINDIR)/_keen5.cmd:
	echo -n "start $(EXECNAME).exe /EPISODE 5 /NOWAIT" >$@
$(BINDIR)/_keen6.cmd:
	echo -n "start $(EXECNAME).exe /EPISODE 6 /NOWAIT" >$@

# generate shell scripts to start specific episodes
shellfiles: $(SHELLFILES)
$(BINDIR)/keen4.sh:
	echo -n "./$(EXECNAME) /EPISODE 4 $(SHELLOPTS)" >$@
$(BINDIR)/keen5.sh:
	echo -n "./$(EXECNAME) /EPISODE 5 $(SHELLOPTS)" >$@
$(BINDIR)/keen6.sh:
	echo -n "./$(EXECNAME) /EPISODE 6 $(SHELLOPTS)" >$@

# auto-download of SDL
$(SDL_DIR):
	wget -O- $(SDL_URL) | tar xz -C ..

clean:
	rm -f $(OUTBIN) $(OBJ) $(OBJDIR)/windowsres.res $(DEPS) $(K4DATA) $(K5DATA) $(K6DATA) $(BATFILES) $(SHELLFILES)

RMDIR_ERRMSG = Note: Some of the 'bin' directories still contain user data. They have not been removed.
export RMDIR_ERRMSG
distclean:
	rm -rf ../obj*
	rm -f ../bin*/$(EXECNAME) ../bin-*/$(EXECNAME).exe ../bin-win*/_keen?.cmd
	rm -f $(foreach f, $(DATAFILES), ../bin*/data/$(f).CK?)
	rmdir ../bin* || echo $$RMDIR_ERRMSG

-include $(DEPS)

.PHONY: all help dumpconfig binfiles batfiles shellfiles keen4data keen5data keen6data clean distclean
