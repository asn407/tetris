#include <DxLib.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 20
#define MAX_X 12
#define MAX_Y 20
#define SOLID 2
#define NUM 4

int intKey[256] = {0};
char charKey[256] = {0};
bool pressFlag[256] = {0};

void GetKey()
{
    GetHitKeyStateAll(charKey);
    for (int i = 0; i < 256; i++)
    {
        if (charKey[i] != 0)
        {
            intKey[i]++;
        }
        else
        {
            intKey[i] = 0;
        }
    }
}

bool KeyPressed(int keyIndex)
{
    if (intKey[keyIndex] && !pressFlag[keyIndex])
    {
        pressFlag[keyIndex] = true;
        return true;
    }

    if (!intKey[keyIndex])
    {
        pressFlag[keyIndex] = false;
    }

    return false;
}

void DrawBlock(int x, int y)
{
    DrawBox(SOLID+x*(SIZE+SOLID), SOLID+y*(SIZE+SOLID), (x+1)*(SIZE+SOLID), (y+1)*(SIZE+SOLID), GetColor(255, 255, 255), true);
}

void Window_Init()
{
    SetDrawScreen(DX_SCREEN_BACK);
    SetWindowText("Tetris");
    SetGraphMode(MAX_X*(SOLID+SIZE), MAX_Y*(SOLID+SIZE), 32);
    srand((unsigned)time(NULL));
}

class Block
{
public:
    int x, y;
    Block(int x, int y);
    void rot();
};

/*
コンストラクタ

x, yはそれぞれ-1, 0, 1のみを格納する
rot()メソッドより行列計算を行いやすくするため
*/

Block::Block(int x, int y)
{
    this->x = x;
    this->y = y;
}

/*
rot()メソッド

回転行列より
x' = xcosθ - ysinθ
y' = xsinθ + ycosθ

θ = 90 のとき
x' = -y
y' = x
*/

void Block::rot()
{
    int tmp_x = this->x;
    this->x = -1*this->y;
    this->y = tmp_x;
}

class Mino
{
public:
    int x, y, rot, shape;
    Block* block[NUM];
    Mino(int x, int y, int rot, int shape);
    ~Mino();
    Mino* copyMino();
    void rotMino(Block* block[]);
    void draw();
};

Mino::Mino(int x, int y, int rot, int shape)
{
    this->x = x;
    this->y = y;
    this->rot = rot;
    this->shape = shape;
}

Mino* Mino::copyMino()
{
    return new Mino(this->x, this->y, this->rot, this->shape);
}

void Mino::rotMino(Block* block[])
{
    switch (this->shape)
    {
        case 0:// T mino
        block[0] = new Block(-1, 0);
        block[1] = new Block(0, 0);
        block[2] = new Block(0, -1);
        block[3] = new Block(1, 0);
        break;
        case 1:// Z mino
        block[0] = new Block(-1, -1);
        block[1] = new Block(0, -1);
        block[2] = new Block(0, 0);
        block[3] = new Block(1, 0);
        break;
        case 2:// S mino
        block[0] = new Block(-1, 0);
        block[1] = new Block(0, 0);
        block[2] = new Block(0, -1);
        block[3] = new Block(1, -1);
        break;
        case 3:// L mino
        block[0] = new Block(-1, -2);
        block[1] = new Block(-1, -1);
        block[2] = new Block(-1, 0);
        block[3] = new Block(0, 0);
        break;
        case 4:// J mino
        block[0] = new Block(0, -2);
        block[1] = new Block(0, -1);
        block[2] = new Block(-1, 0);
        block[3] = new Block(0, 0);
        break;
        case 5:// O mino
        block[0] = new Block(-1, -1);
        block[1] = new Block(-1, 0);
        block[2] = new Block(0, 0);
        block[3] = new Block(0, -1);
        break;
        case 6:// I mino
        block[0] = new Block(-2, 0);
        block[1] = new Block(-1, 0);
        block[2] = new Block(0, 0);
        block[3] = new Block(1, 0);
        break;
    }

    for (int r = 0; r < (NUM * 100 + this->rot) % NUM; r++)
    {
        for (int i = 0; i < NUM; i++)
        {
            block[i]->rot();
        }
    }
}

void Mino::draw()
{
    this->rotMino(this->block);
    for (int i = 0; i < NUM; i++)
    {
        DrawBlock(this->block[i]->x+this->x, this->block[i]->y+this->y);
    }
}

Mino::~Mino()
{
    for (int i = 0; i < NUM; i++)
    {
        delete block[i];
    }
}

class Board
{
public:
    Board();
    ~Board();
    void setBlock(int x, int y);
    int getBlock(int x, int y);
    int findLineFilled();
    void cutLine(int y);
    void draw();
private:
    Block* block[MAX_Y][MAX_X];
    int board[MAX_Y][MAX_X] = {
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1}
    };
};

Board::Board()
{
    for (int y = 0; y < MAX_Y; y++)
    {
        for (int x = 0; x < MAX_X; x++)
        {
            if (!board[y][x])
            {
                block[y][x] = NULL;
                continue;
            }

            block[y][x] = new Block(x, y);
        }
    }
}

void Board::setBlock(int x, int y)
{
    this->board[y][x] = 1;
}

int Board::getBlock(int x, int y)
{
    return this->board[y][x];
}

int Board::findLineFilled()
{
    for (int y = 0; y < MAX_Y - 1; y++)
    {
        bool isFilled = true;
        for (int x = 1; x < MAX_X - 1; x++)
        {
            if (!this->board[y][x])
            {
                isFilled = false;
            }
        }

        if (isFilled)
        {
            return y;
        }
    }

    return -1;
}

void Board::cutLine(int y)
{
    for (int dy = y; dy > 0; dy--)
    {
        for (int x = 1; x < MAX_X - 1; x++)
        {
            this->board[dy][x] = this->board[dy-1][x];
        }
    }
}

void Board::draw()
{
    for (int y = 0; y < MAX_Y; y++)
    {
        for (int x = 0; x < MAX_X; x++)
        {
            if (board[y][x])
            {
                DrawBlock(x, y);
            }
        }
    }
}

Board::~Board()
{
    for (int y = 0; y < MAX_Y; y++)
    {
        for (int x = 0; x < MAX_X; x++)
        {
            delete block[y][x];
        }
    }
}

class Tetris
{
public:
    Tetris();
    ~Tetris();
private:
    Mino* mino;
    Board* board;
    int minoVx;
    int minoVy;
    int minoVr;
    bool minoDrop;
    int frameCounter;
    void mainLoop();
    void update();
    bool isMinoMovable(Mino* futureMino);
    void action();
    Mino* makeNewMino();
};

Tetris::Tetris()
{
    SetOutApplicationLogValidFlag(false);
    ChangeWindowMode(true);
    DxLib_Init();
    Window_Init();

    this->board = new Board;
    this->mino = this->makeNewMino();
    this->minoVx = 0;
    this->minoVy = 0;
    this->minoVr = 0;
    this->minoDrop = false;
    this->frameCounter = 0;

    this->mainLoop();
}

void Tetris::mainLoop()
{
    while (!ProcessMessage() && !KeyPressed(KEY_INPUT_ESCAPE))
    {
        ClearDrawScreen();
        this->update();
        ScreenFlip();
    }
}

void Tetris::update()
{
    GetKey();
    this->action();

    //------------------------
    // 落下（急降下）
    //------------------------
    if (this->minoDrop)
    {
        Mino* futureMino = this->mino->copyMino();
        int skipHeight = 0;

        while (this->isMinoMovable(futureMino))
        {
            futureMino->y++;
            skipHeight++;
        }

        this->mino->y += skipHeight - 1;

        //------------------------
        // 設置
        //------------------------
        for (int i = 0; i < NUM; i++)
        {
            this->mino->rotMino(this->mino->block);
            this->board->setBlock(this->mino->block[i]->x+this->mino->x, this->mino->block[i]->y+this->mino->y);
        }

        this->mino = this->makeNewMino();
        this->minoDrop = false;
        frameCounter = 0;
        delete futureMino;
    }

    //------------------------
    // 落下（コマ送り）
    //------------------------
    if (this->minoVy)
    {
        Mino* futureMino = this->mino->copyMino();
        futureMino->y += this->minoVy;
        
        if (this->isMinoMovable(futureMino))
        {
            this->mino->y += this->minoVy;
        }
        else
        {
            //------------------------
            // 設置
            //------------------------
            for (int i = 0; i < NUM; i++)
            {
                this->mino->rotMino(this->mino->block);
                this->board->setBlock(this->mino->block[i]->x+this->mino->x, this->mino->block[i]->y+this->mino->y);
            }

            this->mino = this->makeNewMino();
        }

        int y;
        while ((y = this->board->findLineFilled()) != -1)
        {
            this->board->cutLine(y);
        }

        this->minoVy = 0;
        delete futureMino;
    }

    //------------------------
    // 左右移動
    //------------------------
    if (this->minoVx)
    {
        Mino* futureMino = this->mino->copyMino();
        futureMino->x += this->minoVx;

        if (this->isMinoMovable(futureMino))
        {
            this->mino->x += this->minoVx;
        }

        this->minoVx = 0;
        delete futureMino;
    }

    //------------------------
    // 回転
    //------------------------
    if (this->minoVr)
    {
        Mino* futureMino = this->mino->copyMino();
        futureMino->rot += this->minoVr;

        if (this->isMinoMovable(futureMino))
        {
            this->mino->rot += this->minoVr;
        }

        this->minoVr = 0;
        delete futureMino;
    }

    this->mino->draw();
    this->board->draw();

    this->frameCounter++;
}

bool Tetris::isMinoMovable(Mino* futureMino)
{
    futureMino->rotMino(futureMino->block);
    for (int i = 0; i < NUM; i++)
    {
        if (this->board->getBlock(futureMino->block[i]->x+futureMino->x, futureMino->block[i]->y+futureMino->y))
        {
            return false;
        }
    }

    return true;
}

void Tetris::action()
{
    if (KeyPressed(KEY_INPUT_LEFT))  this->minoVx = -1;
    if (KeyPressed(KEY_INPUT_RIGHT)) this->minoVx = 1;
    if (KeyPressed(KEY_INPUT_DOWN))  this->minoVy = 1;
    if (KeyPressed(KEY_INPUT_Z))     this->minoVr = -1;
    if (KeyPressed(KEY_INPUT_X))     this->minoVr = 1;
    if (KeyPressed(KEY_INPUT_C))     this->minoDrop = true;
    if ((this->frameCounter%20)==19) this->minoVy = 1;
}

Mino* Tetris::makeNewMino()
{
    return new Mino(5, 2, 0, rand() % 7);
}

Tetris::~Tetris()
{
    delete mino;
    delete board;
    DxLib_End();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Tetris* tetris = new Tetris;
    delete tetris;
    return 0;
}
