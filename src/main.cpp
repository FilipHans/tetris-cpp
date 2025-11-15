#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <iostream>

double lastUpdateTime = 0;
bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int main() 
{
    
    InitWindow(500, 620, "Raylib Tetris");
    SetTargetFPS(60);

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    Game Game;

    while(!WindowShouldClose())
    {
        UpdateMusicStream(Game.music);
        Game.HandleInput();
        
        if(EventTriggered(0.2))
        {
            Game.MoveBlockDown();
        }
        
        BeginDrawing();
        ClearBackground(darkBlue);
        DrawTextEx(font, "Score", {365, 15}, 38, 2, WHITE);
        DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, lightBlue);
        DrawTextEx(font, "Next", {365, 175}, 38, 2, WHITE);
        if (Game.gameOver)
        {
            DrawTextEx(font, "Game Over", {320, 450}, 38, 2, WHITE);
        }

        char scoreText[10];
        sprintf(scoreText, "%d", Game.score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
        
        DrawRectangleRounded({320, 215, 170, 180}, 0.3, 6, lightBlue);
        DrawTextEx(font, scoreText, {320 + (170 - textSize.x) / 2, 65}, 38, 2, WHITE);

        Game.Draw();
        EndDrawing();
    }
    CloseWindow();

    return 0;
}