.SILENT:
EXEC = de
MAIN = ./src/main.cpp
GAME_DIR = src/game

CCFLAG = -std=gnu99 -g -O3 -Wall -Wextra -Wno-psabi -ffast-math -funroll-loops -D _DEFAULT_SOURCE
CPPCFLAG = -std=gnu++11 -Wall -Wextra -g -O3
LFLAG = $(CPPCFLAG)
LCC=$(CPPC)

CC = $(CROSS)gcc
CPPC = $(CROSS)g++
PKG_CONFIG = $(CROSS)pkg-config
SDL2_CONFIG = $(CROSS)sdl2-config

# glew gl SDL2 SDL2_mixer SDL2_net SDL2_image SDL2_ttf libpng
    # -DGLEW_STATIC
    # glew gl SDL2_mixer SDL2_net SDL2_image SDL2_ttf libpng

LIBS = -lm -ldl \
       $(WIN_LIBS) \
       $(shell $(PKG_CONFIG) --cflags --libs \
        gl SDL2_mixer SDL2_net SDL2_image SDL2_ttf) \
       $(shell $(SDL2_CONFIG) --cflags --libs)\



GAME_SRC = \
  $(wildcard $(GAME_DIR)/*.cpp)       $(wildcard $(GAME_DIR)/*.c)\
  $(wildcard $(GAME_DIR)/*/*.cpp)     $(wildcard $(GAME_DIR)/*/*.c)\
  $(wildcard $(GAME_DIR)/*/*/*.cpp)   $(wildcard $(GAME_DIR)/*/*/*.c)

ENGINE_SRC = \
  $(wildcard ./src/de/*.c) $(wildcard ./src/de/*/*.c)\
  $(wildcard ./src/de/*/*/*.c) $(wildcard ./src/de/*/*/*/*.c)

ENGINE_OBJ = $(subst .c,.$(CROSSOBJ)o,$(ENGINE_SRC))
GAME_OBJ = $(subst .c,.$(CROSSOBJ)o,$(GAME_SRC))



all: @notify $(ENGINE_SRC) $(GAME_SRC) $(MAIN) $(EXEC)

$(EXEC): $(MAIN) $(ENGINE_OBJ) $(GAME_OBJ)
	$(call color_green,"$(LCC) $(EXEC)")
	$(LCC) $(LFLAG) $(MAIN) $(ENGINE_OBJ) $(GAME_OBJ) $(LIBS)  -o $(EXEC)

%.$(CROSSOBJ)o: %.c %.h
	$(call color_green,"$(CC) $@")
	$(CC) -c $(CCFLAG) $< $(LIBS) -o $@

%.$(CROSSOBJ)opp: %.cpp %.hpp
	$(call color_green,"$(CPPC) $@")
	$(CPPC) -c $(CPPCFLAG) $< $(LIBS) -o $@













rm: rm-all
rm-all: clean-engine clean-game
clean-game:
	$(call color_red,"rm:") $(GAME_OBJ)
	rm $(GAME_OBJ)

clean-engine:
	$(call color_red,"rm:") $(ENGINE_OBJ)
	rm $(ENGINE_OBJ)

run:
	@make --silent --no-print-directory
	$(call color_green,"running $(EXEC):")
	./$(EXEC)

rund:
	@make --silent --no-print-directory
	$(call color_green,"gdb -ex run ./$(EXEC)")
	gdb -ex run ./$(EXEC)










#______________________________________________________________________________
# WINDOWS - mxe
WIN_EXEC = $(EXEC).exe
WIN_FLAG = CROSSOBJ=win. CROSS=i686-w64-mingw32.static- EXEC=$(WIN_EXEC) WIN_LIBS="-liphlpapi -DGLEW_STATIC -lGLEW"
# WIN_FLAG = CROSSOBJ=win CROSS=x86_64-w64-mingw32.static- EXEC=$(WIN_EXEC)
# -liphlpapi is requiered for sdl_net to link properly
# required libs: sdl2 sd2_* glew dlfcn-win32

win:
	make $(WIN_FLAG) -j4

win-run:
	make $(WIN_FLAG) && ./$(WIN_EXEC)

win-rm:
	make rm $(WIN_FLAG)

win-clean:
	make clean $(WIN_FLAG)












#______________________________________________________________________________
# android - ndk
ANDROID_MK_PATH = ./de_android/jni/src/Android.mk
ANDROID_MK_SOURCES = $(addprefix ../../../,$(GAME_SRC) $(MAIN) $(ENGINE_SRC))

android:
	$(call color_green,"ln -f -s -r ./assets ./de_android/")
	@ln -f -s -r ./assets ./de_android/

	$(call color_green,"writing android makefile $(ANDROID_MK_PATH):")
	@echo "#auto generated from root makefile ( $(shell pwd) )" > $(ANDROID_MK_PATH)
	@printf $(ANDROID_MK_PREFIX) >> $(ANDROID_MK_PATH)
	@echo LOCAL_SRC_FILES := $(ANDROID_MK_SOURCES) >> $(ANDROID_MK_PATH)
	@printf $(ANDROID_MK_SUFFIX) >> $(ANDROID_MK_PATH)
	# clear makefile's timestamp so it doesnt recompile everything
	$(call color_green,"touch -d \"1970-01-01 00:00:00.000000000 +0000\"" $(ANDROID_MK_PATH) )
	@touch -d "1970-01-01 00:00:00.000000000 +0000" $(ANDROID_MK_PATH)

	cd de_android && make
	$(call color_green,"done")


# add libraries to compile on android here
define ANDROID_MK_PREFIX
'\n\
LOCAL_PATH := $$(call my-dir)\n\
include $$(CLEAR_VARS)\n\
LOCAL_MODULE := main\n\
SDL_PATH       := ../SDL2\n\
SDL_MIXER_PATH := ../SDL2_mixer                 \n\
SDL_NET_PATH   := ../SDL2_net                   \n\
SDL_IMAGE_PATH := ../SDL2_image                 \n\
SDL_TTF_PATH   := ../SDL2_ttf                   \n\
LOCAL_C_INCLUDES := \
  $$(LOCAL_PATH)/$$(SDL_PATH)/include             \
  $$(LOCAL_PATH)/$$(SDL_MIXER_PATH)               \
  $$(LOCAL_PATH)/$$(SDL_NET_PATH)                 \
  $$(LOCAL_PATH)/$$(SDL_IMAGE_PATH)               \
  $$(LOCAL_PATH)/$$(SDL_TTF_PATH)                 \
\n'
endef

define ANDROID_MK_SUFFIX
'\n\
LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_mixer SDL2_net SDL2_ttf \n\
LOCAL_LDLIBS := -lm -ldl -lGLESv1_CM -lGLESv2 -llog -lz\n\
LOCAL_CFLAGS += $(LFLAG) -DGL_GLEXT_PROTOTYPES \n\
LOCAL_NEON_CFLAGS += $(LFLAG) -mfloat-abi=softfp -mfpu=neon -march=armv7 \n\
APP_CFLAGS += $(LFLAG) \n\
include $$(BUILD_SHARED_LIBRARY)\n'
endef















#______________________________________________________________________________
@notify:
	$(call color_green,"make")

color_red    =@echo -e "\e[91m"$(1)"\e[0m"$(2)
color_green  =@echo -e "\e[92m"$(1)"\e[0m"$(2)
color_gold   =@echo -e "\e[93m"$(1)"\e[0m"$(2)
color_blue   =@echo -e "\e[94m"$(1)"\e[0m"$(2)
color_purple =@echo -e "\e[95m"$(1)"\e[0m"$(2)
color_cyan   =@echo -e "\e[96m"$(1)"\e[0m"$(2)
color_yellow =@echo -e "\e[33m"$(1)"\e[0m"$(2)
