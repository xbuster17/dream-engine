.SILENT:
EXEC = de

# settings
CC = $(CROSS)gcc
PKG_CONFIG = $(CROSS)pkg-config
SDL2_CONFIG = $(CROSS)sdl2-config
CPPC = $(CROSS)g++
# CCFLAG = -pg -ffast-math -funroll-loops -fPIC -D _DEFAULT_SOURCE -D _POSIX_C_SOURCE
CCFLAG = -std=gnu99 -Wall -Wextra -g -O3 -Wno-psabi -ffast-math -funroll-loops
CPPCFLAG = -std=gnu++11 -Wall -Wextra -Wno-unused-parameter -g -O2

LFLAG = $(CCFLAG)

# glew gl SDL2_mixer SDL2_net SDL2_image SDL2_ttf libpng
LIBS = -lm \
       -DGLEW_STATIC \
       $(shell $(PKG_CONFIG) --cflags --libs \
        glew gl SDL2_mixer SDL2_net SDL2_image SDL2_ttf libpng)\
       $(WIN_LIBS)

       # $(shell $(SDL2_CONFIG) --cflags --libs)\


#sources_______________________________________________________________________
MAIN = ./src/main.c
GAME_DIR = src/game
GAME_SRC = \
  $(wildcard $(GAME_DIR)/*.cpp)       $(wildcard $(GAME_DIR)/*.c)\
  $(wildcard $(GAME_DIR)/*/*.cpp)     $(wildcard $(GAME_DIR)/*/*.c)\
  $(wildcard $(GAME_DIR)/*/*/*.cpp)   $(wildcard $(GAME_DIR)/*/*/*.c)

ENGINE_SRC = \
  $(wildcard ./src/de/*.c)\
  $(wildcard ./src/de/*/*.c)\
  $(wildcard ./src/de/*/*/*.c)\
  $(wildcard ./src/de/*/*/*/*.c)

ENGINE_OBJ = $(subst .c,.$(CROSSOBJ)o,$(ENGINE_SRC))
GAME_OBJ = $(subst .c,.$(CROSSOBJ)o,$(GAME_SRC))



#main__________________________________________________________________________
all: notify $(ENGINE_SRC) $(GAME_SRC) $(MAIN) $(EXEC)

$(EXEC): $(MAIN) $(ENGINE_OBJ) $(GAME_OBJ)
	$(call color_green,"linking $(EXEC)")
	$(CC) $(LFLAG) $(MAIN) $(ENGINE_OBJ) $(GAME_OBJ) $(LIBS)  -o $(EXEC)

%.$(CROSSOBJ)o: %.c %.h
	$(call color_gold,"$(CC) $@:")
	$(CC) -c $(CCFLAG) $< $(LIBS) -o $@

%.$(CROSSOBJ)opp: %.cpp %.hpp
	$(call color_gold,"$(CPPC) $@:")
	$(CPPC) -c $(CPPCFLAG) $< $(LIBS) -o $@



notify:
	$(call color_cyan,"make:")





# delete_______________________________________________________________________
rm: rm_all
rm_all: clean_engine clean
clean:
	$(call color_red,"rm:") $(GAME_OBJ)
	rm $(GAME_OBJ)

clean_engine:
	$(call color_red,"rm:") $(ENGINE_OBJ)
	rm $(ENGINE_OBJ)





# run__________________________________________________________________________
run:
	@make --silent --no-print-directory
	$(call color_yellow,"running $(EXEC):")
	./$(EXEC)

rund:
	@make --silent --no-print-directory
	$(call color_yellow,"running gdb $(EXEC):")
	gdb -ex run ./$(EXEC)



















#______________________________________________________________________________
# WINDOWS
#______________________________________________________________________________

WIN_EXEC = $(EXEC).exe
WIN_FLAG = CROSSOBJ=win CROSS=i686-w64-mingw32.static- EXEC=$(WIN_EXEC) WIN_LIBS=-liphlpapi
# WIN_FLAG = CROSSOBJ=win CROSS=x86_64-w64-mingw32.static- EXEC=$(WIN_EXEC)
# -liphlpapi is requiered for sdl_net to link properly

win:
	make $(WIN_FLAG) -j4

win-run:
	make $(WIN_FLAG) && ./$(WIN_EXEC)

win-rm:
	make rm $(WIN_FLAG)

win-clean:
	make clean $(WIN_FLAG)


# installing MXE:
# git clone and `make glew sdl2 sdl2_image sdl2_mixer sdl2_net sdl2_ttf
# ogg mpg123`
# and `export PATH=/path/to/mxe/usr/bin:$PATH` into .bashrc

















#______________________________________________________________________________
# android
#______________________________________________________________________________
droid:
	$(call color_red, "-link assets folder:")
	ln -f -s -r ./assets ./de_android/
	$(call color_red, "-write file android makefile $(ANDROID_MK_PATH)/makefile :")
	echo "#auto generated from root makefile ( $(shell pwd) )" > $(ANDROID_MK_PATH)
	@printf $(ANDROID_MK_PREFIX) >> $(ANDROID_MK_PATH)
	@echo LOCAL_SRC_FILES := $(ANDROID_MK_SOURCES) >> $(ANDROID_MK_PATH)
	@printf $(ANDROID_MK_SUFFIX) >> $(ANDROID_MK_PATH)

	$(call color_red, "-clear makefile's timestamp so it doesnt recompile everything:")
	touch -d "1970-01-01 00:00:00.000000000 +0000" $(ANDROID_MK_PATH)

	cd de_android && make

	$(call color_red, "android makefile done")


ANDROID_MK_PATH = ./de_android/jni/src/Android.mk

ANDROID_MK_SOURCES = $(addprefix ../../../,$(GAME_SRC) $(MAIN) $(ENGINE_SRC))



# android makefile
# droid's makefile prefix
# add libraries to compile on android here
define ANDROID_MK_PREFIX
'\n\
LOCAL_PATH := $$(call my-dir)\n\
include $$(CLEAR_VARS)\n\
LOCAL_MODULE := main\n\
SDL_PATH := ../SDL2\n\
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



#droid's makefile suffix
define ANDROID_MK_SUFFIX
'\n\
LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_mixer SDL2_net SDL2_ttf \n\
LOCAL_LDLIBS := -lm -ldl -lGLESv1_CM -lGLESv2 -llog -lz\n\
LOCAL_CFLAGS += $(LFLAG) -DGL_GLEXT_PROTOTYPES -Wno-unused-parameter -Wno-missing-braces \n\
LOCAL_NEON_CFLAGS += $(LFLAG) -mfloat-abi=softfp -mfpu=neon -march=armv7 \n\
APP_CFLAGS += $(LFLAG) -O3 -ffast-math -funroll-loops \n\
include $$(BUILD_SHARED_LIBRARY)\n'
endef
























#______________________________________________________________________________
#eye-candy
#______________________________________________________________________________
color_red    =@echo -e "\e[91m"$(1)"\e[0m"$(2)
color_green  =@echo -e "\e[92m"$(1)"\e[0m"$(2)
color_gold   =@echo -e "\e[93m"$(1)"\e[0m"$(2)
color_blue   =@echo -e "\e[94m"$(1)"\e[0m"$(2)
color_purple =@echo -e "\e[95m"$(1)"\e[0m"$(2)
color_cyan   =@echo -e "\e[96m"$(1)"\e[0m"$(2)

color_yellow =@echo -e "\e[33m"$(1)"\e[0m"$(2)

# more colors
# common
# \e[0;30m	Black
# \e[0;31m	Red
# \e[0;32m	Green
# \e[0;33m	Yellow
# \e[0;34m	Blue
# \e[0;35m	Purple
# \e[0;36m	Cyan
# \e[0;37m	White
# high intensity
# \e[0;90m	Black
# \e[0;91m	Red
# \e[0;92m	Green
# \e[0;93m	Yellow
# \e[0;94m	Blue
# \e[0;95m	Purple
# \e[0;96m	Cyan
# \e[0;97m	White

## TODO :
# rm: filter third-party libraries from objects (.o)
