#pragma once
#include "olcPixelGameEngine.h"

#define GENERATION_LIFESPAN 0.5

class GameOfLife :
    public olc::PixelGameEngine
{
private:
    bool isInGenerationMode;

    bool** field;
    float fRunningTime;
    float fGenerationRefreshTime;
    int nGeneration;

    int nFieldVerticalOffset = 8;
    int nCellSize = 2;
    int32_t nFieldWidth;
    int nFieldHeight;
public:
    GameOfLife();
public:
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
private:
    void initializeField();
    void drawField();
    void drawInfo();
    void refreshGeneration();
    bool refreshCell(bool** field, int x, int y);
};

