#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "util.h"
#include "levels.h"

const char* UNLOADED = "ERROR";
const char* PROJECT_HEADER = "HAXAGON1.0";
const char* PROJECT_FOOTER = "ENDHAXAGON";
const char* LEVEL_HEADER = "LEVEL2.0";
const char* LEVEL_FOOTER = "ENDLEVEL";
const char* PATTERN_HEADER = "PATTERN1.0";
const char* PATTERN_FOOTER = "ENDPATTERN";
const char* BGM_PATH = "sdmc:/3ds/data/haxagon/";

const int MIN_WALL_HEIGHT = 8;
const int MIN_PATTERN_SIDES = 3;

int compare(FILE* file, const char* string) {
	int len = strlen(string);
	char* buff = malloc(sizeof(char) * (len + 1)); //for '/0'
	check(!buff, "Cannot check file string!", DEF_DEBUG, ftell(file));
	fread(buff, sizeof(char), len, file); //no '/0' in file, only len
	buff[len] = '\0'; //add '/0'
	int result = strcmp(buff, string);
	free(buff);
	return result;
}

void* getMalloc(FILE* file, size_t size, int* length, int extra, char* error) {
	if(extra < 0) extra = 0;
	fread(length, sizeof(int), 1, file);
	if(*length > 300) check(0, "Huge alloc found!", DEF_DEBUG, ftell(file));
	void* address = malloc(size * (*length + extra));
	check(!address, error, DEF_DEBUG, ftell(file));
	return address;
}

FileString getString(FILE* file) {
	FileString string;
	string.str = getMalloc(file, sizeof(char), &string.len, 1, "Cannot load string from file!");
	fread(string.str, sizeof(char), string.len, file);
	string.str[string.len] = '\0';
	return string;
}

FileString getStringPrefix(const char* prefix, FILE* file) {
	FileString string;
	int prefixlen = strlen(prefix);
	string.str = getMalloc(file, sizeof(char), &string.len, prefixlen + 1, "Cannot load string from file!");
	memcpy(string.str, prefix, prefixlen);
	fread(&(string.str[prefixlen]), sizeof(char), string.len, file); //potentially dangerous?
	string.len += prefixlen;
	return string;
}

Wall getWall(FILE* file, int maxSide) {
	Wall wall;
	
	//wall data
	fread(&wall.distance, sizeof(short), 1, file);
	fread(&wall.height, sizeof(short), 1, file);
	fread(&wall.side, sizeof(short), 1, file);
	
	//check wall data
	if(wall.height < MIN_WALL_HEIGHT) wall.height = MIN_WALL_HEIGHT;
	if(wall.side < 0) wall.side = 0;
	if(wall.side >= maxSide) wall.side = maxSide - 1;
	
	//exit
	return wall;
} 

Color getColor(FILE* file) {
	Color color;
	fread(&color.r, sizeof(char), 1, file);
	fread(&color.g, sizeof(char), 1, file);
	fread(&color.b, sizeof(char), 1, file);
	return color;
}

Pattern getPattern(FILE* file) {
	Pattern pattern;
	
	//pattern (file) name
	pattern.name = getString(file);
	
	//header
	check(compare(file, PATTERN_HEADER), "Pattern header incorrect!", DEF_DEBUG, ftell(file));
	
	//number of sides
	fread(&pattern.sides, sizeof(int), 1, file);
	if(pattern.sides < MIN_PATTERN_SIDES) pattern.sides = MIN_PATTERN_SIDES;
	
	//walls
	pattern.walls = getMalloc(file, sizeof(Wall), &pattern.numWalls, 0, "Cannot alloc walls!");
	check(!pattern.numWalls, "Pattern must have at least one wall!", DEF_DEBUG, ftell(file));
	for(int i = 0; i < pattern.numWalls; i++) pattern.walls[i] = getWall(file, pattern.sides);
	
	//footer
	check(compare(file, PATTERN_FOOTER), "Pattern header incorrect!", DEF_DEBUG, ftell(file));
	
	//exit
	return pattern;
}

Pattern getLoadedPattern(FILE* file, Pattern* patterns, int numPatterns) {
	FileString search = getString(file);
	int i = 0;
	for(i = 0; i < numPatterns; i++) {
		if(!strcmp(patterns[i].name.str, search.str)) {
			break;
		}
	}
	
	//could not find pattern
	check(i == numPatterns, "Could not locate pattern!", DEF_DEBUG, ftell(file));
	
	//copy pattern if located.
	Pattern located;
	memcpy(&located, &patterns[i], sizeof(Pattern));
	return located;
}

Level getLevel(FILE* file, Pattern* patterns, int numPatterns) {
	Level level;
	
	//header
	check(compare(file, LEVEL_HEADER), "Level header incorrect!", DEF_DEBUG, ftell(file));
	
	//strings
	level.name = getString(file);
	level.difficulty = getStringPrefix("DIFFICULTY: ", file);
	level.mode = getStringPrefix("MODE: ", file);
	level.creator = getStringPrefix("CREATOR: ", file);
	level.music = getStringPrefix(BGM_PATH, file);
	
	//colors
	level.colorsBG1 = getMalloc(file, sizeof(Color), &level.numBG1, 0, "Cannot alloc BG1 colors!");
	check(!level.numBG1, "Level must have at least one bg1 color!", DEF_DEBUG, ftell(file));
	for(int i = 0; i < level.numBG1; i++) level.colorsBG1[i] = getColor(file);
	level.colorsBG2 = getMalloc(file, sizeof(Color), &level.numBG2, 0, "Cannot alloc BG2 colors!");
	check(!level.numBG2, "Level must have at least one bg2 color!", DEF_DEBUG, ftell(file));
	for(int i = 0; i < level.numBG2; i++) level.colorsBG2[i] = getColor(file);
	level.colorsFG = getMalloc(file, sizeof(Color), &level.numFG, 0, "Cannot alloc FG colors!");
	check(!level.numFG, "Level must have at least one fg color!", DEF_DEBUG, ftell(file));
	for(int i = 0; i < level.numFG; i++) level.colorsFG[i] = getColor(file);
	
	//floats
	fread(&level.speedWall, sizeof(float), 1, file);
	fread(&level.speedRotation, sizeof(float), 1, file);
	fread(&level.speedHuman, sizeof(float), 1, file);
	
	//int
	fread(&level.speedPulse, sizeof(int), 1, file);
	
	//linked patterns (a copy of loaded patterns)
	level.patterns = getMalloc(file, sizeof(Pattern), &level.numPatterns, 0, "Cannot alloc patterns!");
	check(!level.numPatterns, "Level must have at least one pattern!", DEF_DEBUG, ftell(file));
	for(int i = 0; i < level.numPatterns; i++) level.patterns[i] = getLoadedPattern(file, patterns, numPatterns);
	
	check(compare(file, LEVEL_FOOTER), "Level header incorrect!", DEF_DEBUG, ftell(file));
	
	//exit
	return level;
}

GlobalData getData(FILE* file) {
	GlobalData data;
	
	//header
	check(compare(file, PROJECT_HEADER), "Wrong file type/format/version!", DEF_DEBUG, ftell(file));
	
	//patterns
	data.patterns = getMalloc(file, sizeof(Pattern), &data.numPatterns, 0, "Cannot alloc patterns!");
	check(!data.numPatterns, "Must load at least one pattern!", DEF_DEBUG, ftell(file));
	for(int i = 0; i < data.numPatterns; i++) data.patterns[i] = getPattern(file);
	
	//levels
	data.levels = getMalloc(file, sizeof(Level), &data.numLevels, 0, "Cannot alloc levels!");
	check(!data.numLevels, "Must load at least one level!", DEF_DEBUG, ftell(file));
	for(int i = 0; i < data.numLevels; i++) data.levels[i] = getLevel(file, data.patterns, data.numPatterns);
	
	//footer
	check(compare(file, PROJECT_FOOTER), "Project footer incorrect!", DEF_DEBUG, ftell(file));
	return data;
}