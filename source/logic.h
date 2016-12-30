#pragma once

/**
 * Logic for working the main menu, along  with a selection sound.
 * Modifys the address of level to the currently selected level upon return.
 */
GameState doMainMenu(GlobalData data, LoadedState loaded, Track select, int* level);

/** 
 * Plays a level.
 */
GameState doPlayGame(Level level, LiveLevel* gameOver);

/**
 * Shows the game over screen and frees resources.
 */
GameState doGameOver(Level level, LiveLevel gameOver);