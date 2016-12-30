#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <3ds.h>
#include <sf2d.h>

#include "types.h"
#include "util.h"
#include "draw.h"
#include "sound.h"



/** INTERNAL
 * Panics the game. This version appends to a file and immediately quits the game.
 * TODO: Change this to a graphical error message if possible. This method should
 * hang the program until the user quits via  home or homebrew exit.
 */
void panic(const char* message, const char* file, const char* function, int line, int error) {
	FILE* panic = fopen("sdmc:/haxapanic.txt", "a");
	if(panic) {
		fprintf(panic, "Compilation date: %s %s\n", __DATE__, __TIME__);
		fprintf(panic, "Sorry! There was a problem during runtime.\n");
		fprintf(panic, "Message: %s\n", message);
		fprintf(panic, "...in file: %s.\n", file);
		fprintf(panic, "...in function: %s.\n", function);
		fprintf(panic, "...on line: %d.\n", line);
		fprintf(panic, "...with error code: 0x%08x.\n", error);
		fprintf(panic, "For more information, see https://github.com/RedInquisitive/Super-Haxagon\n");
		fprintf(panic, "If a file failed to load, see https://github.com/RedInquisitive/Super-Haxagon/tree/master/sdmc\n");
		fprintf(panic, "The game has quit.\n\n");
		fclose(panic);
	}
	while(aptMainLoop()) {
		if(getButton() == QUIT) break;
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		drawPanic(message, file, function, line, error);
		sf2d_end_frame();
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		drawPanicBot();
		sf2d_end_frame();
		sf2d_swapbuffers();
	}
	sf2d_fini();
	gfxExit();	
	romfsExit();	
	sdmcExit();
	ndspExit();	
	exit(1);
}

//EXTERNAL
double linear(double start, double end, double percent) {
    return (end - start) * percent + start;
}

//EXTERNAL
int check(int result, const char* message, const char* file, const char* function, int line, int error) {
	if(result) panic(message, file, function, line, error);
	return result;
}

//EXTERNAL
Color interpolateColor(Color one, Color two, double percent) {
	Color new;
	new.r = (int)linear((double)one.r, (double)two.r, percent);
	new.g = (int)linear((double)one.g, (double)two.g, percent);
	new.b = (int)linear((double)one.b, (double)two.b, percent);
	new.a = (int)linear((double)one.a, (double)two.a, percent);
	return new;
}

//EXTERNAL
Point calcPointWall(Point focus, double rotation, double offset, double distance, int side, double sides)  {
	Point point = {0,0};
	double width = (double)side * TAU/sides + offset;
	if(width > TAU + OVERFLOW_OFFSET) width = TAU + OVERFLOW_OFFSET;
	point.x = (int)((distance * cos(rotation + width) + (double)(focus.x)) + 0.5);
	point.y = (int)((distance * sin(rotation + width) + (double)(focus.y)) + 0.5);
	return point;
}

//EXTERNAL
Point calcPoint(Point focus, double rotation, double offset, double distance)  {
	Point point = {0,0};
	point.x = (int)(distance * cos(rotation + offset) + focus.x + 0.5);
	point.y = (int)(distance * sin(rotation + offset) + focus.y + 0.5);
	return point;
}

//EXTERNAL
ButtonState getButton() {
	hidScanInput();
	u32 kDown = hidKeysDown();
	u32 kHold = hidKeysHeld();
	if(kDown & KEY_A ) {
		return SELECT;
	} 
	if(kDown & KEY_START ) {
		return QUIT;
	} 
	if(kDown & KEY_B ) {
		return BACK;
	} 
	if(kHold & (KEY_R | KEY_ZR | KEY_CSTICK_RIGHT | KEY_CPAD_RIGHT | KEY_DRIGHT | KEY_X)) {
		return DIR_RIGHT;
	} 
	if(kHold & (KEY_L | KEY_ZL | KEY_CSTICK_LEFT | KEY_CPAD_LEFT | KEY_DLEFT | KEY_Y)) {
		return DIR_LEFT;
	} 
	return NOTHING;
}

 char* getScoreText(int score) {
	if(score < 10 * 60) return "SPACE";
	if(score < 20 * 60) return "POINT";
	if(score < 30 * 60) return "LINE";
	if(score < 40 * 60) return "TRIANGLE";
	if(score < 50 * 60) return "SQUARE";
	if(score < 60 * 60) return "PENTAGON";
	return "HEXAGON";
 }