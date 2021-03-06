dream engine

released under the MIT license (read LICENSE)

disclaimer: this api was created for personal projects, without intention of being distribuited, it is not documented and it isnt ready for releases.

file structure
all the source files (.c, .h) must go in the src folder, the makefile scans for all pairs of foo.h and foo.c and compiles an object foo.o or foo.win.o out of them.
main.c is at the root of src by default (you can change the location from the makefile) and must contain int SDL_main(int argc, char** argv) which will be the entry point to the c code from your android app.
src/de contains all the engine's src.
by default, src/game contains your game source, you can change this on the makefile.
all assets you plan to pack should be in the assets folder.

clone de_android git into root folder.

de_android contains links to assets and src, internal android project files, names, icons, etc.
and, inside the jni folder, the sources for every library used, and Android.mk makefiles to compile those libraries.
renaming android project consists of editing plenty of files, described in de_android/README.
_todo a script to automate project creation and renaming_


usage:
include de.h
the global De struct contains most engine parameters, it's defined on src/de/de_struct.h

de_init(int x, int y, int flags) initializes the engine modules and creates a window of size (x,y) with the specified SDL window flags
	x: width, 0 defaults to screen size
	y: height, 0 defaults to screen size
	flags: any valid SDL_WINDOW_... flag, 0 defaults to SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN.
	on android, x,y and flags is always set to default




modules:
window: sdl initialization, glcontext setup

vao: Vertex Array Object, glVertexAttribPointer (vap), glVertexBufferObject (vbo), Index Buffer Object
vertex format parser handles string like "type dim [name] [',' or ';']"
up to 16 attributes, which is gl standard.
type is 'f', 'i', 's' or 'c'. with optionally 'u' or 'n' at any point
these stand for the first letter in float, int, short, char, unsigned, normalize
normalize tells gl to map integers to [1,-1], according to their type



installing MXE:
clone mxe repo `git clone https://github/com/mxe/mxe.git`
and run
`make glew sdl2 sdl2_image sdl2_mixer sdl2_net sdl2_ttf ogg mpg123 [libmikmod|modplug]`
note this will download packages, needs 2GiB of ram to link gcc and 700MB of disk space
then `export PATH=/path/to/mxe/usr/bin:$PATH`




quirks:
on android and windows SDL_main must be used instead of main and must have standard type

input & main thread:
DE_GAME_LOOP calls SDL_PumpEvents which can only be called from the main thread
without pumping events the engine cant see them
SDL_PollEvents calls pump too
keyboard events with repeat will be ignored on keyboard state
mouse relative state gets reset each call to dinput_update

opengl:
the attribute parser is weak so dont use names that appear on the header or anywhere before the actual variable name
ie dont use attribute def; since it will be found on "#define ..." segment;
either this or enforce " name;" to minimize the problem //todo

glsl:
only consts can be used in a global initializer, glsl v100 error, some androids
every float number in glsl must have an explicit decimal
if you dont use a variable in glsl it gets optimized away, if this is an attribute or uniform, it will change the index and cause graphic bugs, note that it is not enough to assign a variable to an unused variable.

gl ibo ext:
	GL_UNSIGNED_INT and GL_OES_element_index_uint extension:
	even tho this should work on gles2 the only device i have to try it gives GL_INVALID_ENUM
	as if the extension wasn't aviable despite it being in glGetString(GL_EXTENSIONS)
	so element buffer object is made with shorts for the moment ...


makefile:
	it doesnt detect included headers so it will not update every file and it's buggy
	the simplest way to fix it is `make rm && make`
	the android makefile handles this correctly


audio:
	when using sdl mixer don't call the following sdl functions:
	SDL_OpenAudio, SDL_CloseAudio, SDL_PauseAudio, SDL_LockAudio, SDL_UnlockAudio.
	use Mix_Pause(-1) and Mix_PauseMusic or Mix_Resume(-1) and Mix_ResumeMusic instead.
	it is also a BAD idea to call SDL_mixed and SDL audio functions from a callback.
	Callbacks include Effects functions and other SDL_mixer audio hooks


android:
	SDL_WINDOW_RESIZABLE required for apps with both portrait and landscape orientations



TODO
handle sdl event android background, text composition, dropfile, mouse wheel

