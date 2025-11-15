#include "game.h"
#include <random>

Game::Game()
{
    grid = Grid();
    blocks = {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    gameOver = false;
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
    currentBlock.Draw();
}

void Game::HandleInput()
{
    int keyPressed = GetKeyPressed();
    if(gameOver && keyPressed != 0)
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
        break;
    case KEY_W:
        RotateBlock();
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
            currentBlock = nextBlock; 
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
    grid.ClearFullRows();
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
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
}