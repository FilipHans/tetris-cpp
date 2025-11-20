#include "game.h"
#include <random>

Game::Game()
{
    grid = Grid();
    blocks = {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    gameOver = false;
    score = 0;
    InitAudioDevice();
    music = LoadMusicStream("Sounds/tetris-converted.mp3");
    clearSound = LoadSound("Sounds/Sounds_clear.mp3");
    rotateSound = LoadSound("Sounds/Sounds_rotate.mp3");
    PlayMusicStream(music);
}

Game::~Game()
{
    UnloadMusicStream(music);
    UnloadSound(rotateSound);
    UnloadSound(clearSound);
    CloseAudioDevice();
}

Block Game::GetRandomBlock()
{
    // If all blocks have been used and removed from vector it refreshes it
    if(blocks.empty())
    {
        blocks = GetAllBlocks();
    }
    // Randomizes number based on vector size, returns and deletes the randomly selected block to prevent getting same back to back
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}

void Game::Draw()
{
    grid.Draw();
    currentBlock.Draw(11, 11);
    nextBlock.Draw(270, 270);
}

void Game::HandleInput()
{
    int keyPressed = GetKeyPressed();
    if(gameOver && keyPressed == KEY_R)
    {
        gameOver = false;
        Reset();
    }
    switch (keyPressed)
    {
    case KEY_A:
        MoveBlockLeft();
        break;
    case KEY_D:
        MoveBlockRight();
        break;        
    case KEY_S:
        MoveBlockDown();
        PointCalculator(0, 5);
        break;
    case KEY_W:
        RotateBlock();
        PlaySound(rotateSound);
    default:
        break;
    }
}

void Game::MoveBlockLeft()
{
    
    if (!gameOver)
    {
        currentBlock.Move(0, -1);
        if(IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, 1);
        }
    }
}
void Game::MoveBlockRight()
{
        if (!gameOver)
    {
        currentBlock.Move(0, 1);
        if(IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, -1);
        }
    }
}
void Game::MoveBlockDown()
{
        if (!gameOver)
    {
        currentBlock.Move(1, 0);
        if(IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(-1, 0);
            LockBlock();
        }        
    }
}

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = currentBlock.GetCellPosition();

    for(Position item: tiles)
    {
        if(grid.isCellOutside(item.row, item.column))
        {
            return true;
        }
    }
    return false;
}

void Game::RotateBlock()
{
    if (!gameOver)
    {
        currentBlock.Rotate();
        if(IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.UndoRotation();
        }
    }
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPosition();
    for(Position item: tiles)
    {
        grid.grid[item.row][item.column] = currentBlock.id;
    }
    currentBlock = nextBlock;
    if(BlockFits() == false)
    {
        gameOver = true;
    }
    nextBlock = GetRandomBlock();
    int clearedRows = grid.ClearFullRows();
    if (clearedRows > 0)
    {
        PlaySound(clearSound);
        PointCalculator(clearedRows, 0);
    }
}

bool Game::BlockFits()
{
    std::vector<Position> tiles = currentBlock.GetCellPosition();
    for(Position item: tiles)
    {
        if(grid.IsCellEmpty(item.row, item.column) == false)
        {
            return false;
        }
    }
    return true;
}

void Game::Reset()
{
    score = 0;
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
}

void Game::PointCalculator(int rowsCleared, int movedDownPoints)
{
    extern double intervals;
    switch(rowsCleared)
    {
        case 1:
            score += 100;
            intervals -= 0.01;
        break;

        case 2:
            score += 300;
            intervals -= 0.02;
        break;
        case 3:
            score += 500;
            intervals -= 0.04;
        break;
        case 4:
            score += 1000;
            intervals -= 0.06;
        break;
    }
    score += movedDownPoints;
}