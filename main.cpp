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

/**
 * GetKey()関数
 *
 * DxLib.hで定義されている
 * GetHitKeyStateAll()関数を実行し，
 * charKeyに格納される入力キーの情報を
 * intKeyに格納（int型に変換）する．
 *
 * @param なし
 * @return なし
 */

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

/**
 * KeyPressed()関数
 *
 * 1Fにおいて，任意キーが押されたかを検知する．
 * つまり，キーを押し続けても次フレームではfalseとなる．
 *
 * @param keyIndex DxLib.hで定義されたキー番号
 * @return true false 1Fにおいて，任意キーが押されたかの真偽
 */

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

/**
 * DrawBlock()関数
 *
 * 盤面の配列番号にブロックを描画する．
 * テトリスのブロック描画のひな型となる．
 *
 * @param x, y テトリスにおける盤面の配列番号
 * @return なし
 */

void DrawBlock(int x, int y)
{
    DrawBox(SOLID+x*(SIZE+SOLID), SOLID+y*(SIZE+SOLID), (x+1)*(SIZE+SOLID), (y+1)*(SIZE+SOLID), GetColor(255, 255, 255), true);
}

/**
 * Window_Init()関数
 *
 * ウィンドウに関する初期設定を行う．
 * ちゃっかり疑似乱数の初期化も
 * してるし結構ガバガバ．
 *
 * @param なし
 * @return なし
 */

void Window_Init()
{
    // 裏画面にて描画更新をする
    SetDrawScreen(DX_SCREEN_BACK);
    // ウィンドウタイトル
    SetWindowText("Tetris");
    // ウィンドウの大きさ
    SetGraphMode(MAX_X*(SOLID+SIZE), MAX_Y*(SOLID+SIZE), 32);
    // 疑似乱数の初期化
    srand((unsigned)time(NULL));
}

/**
 * Blockクラス
 *
 * ブロックの情報を持つ．
 * その場での回転を実現するため，
 * x,yには-2,-1,0,1のみが入る．
 */

class Block
{
public:
    int x, y;
    Block(int x, int y);
    void rot();
};

/**
 * Block()コンストラクタ
 *
 * 説明なし．
 *
 * @param x, y 原点におけるブロックの座標
 * @return なし
 */

Block::Block(int x, int y)
{
    this->x = x;
    this->y = y;
}

/**
 * rot()メソッド
 *
 * 右回転のみを実現する．
 * 回転行列より
 * x_next = xcosθ - ysinθ
 * y_next = xsinθ + ycosθ
 * θ = 90 のとき
 * x_next = -y
 * y_next = x
 *
 * @param なし
 * @return なし
 */

void Block::rot()
{
    int tmp_x = this->x;
    this->x = -1*this->y;
    this->y = tmp_x;
}

/**
 * Minoクラス
 *
 * テトロミノの情報を持つ．
 * x, y テトロミノの座標
 * rot 回転状態
 * shape 形状
 * block[NUM] Blockクラスのオブジェクトx4
 */

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

/**
 * Mino()コンストラクタ
 *
 * 説明なし．
 *
 * @param x, y, rot, shape テトロミノの座標，回転状態，形状
 * @return なし
 */

Mino::Mino(int x, int y, int rot, int shape)
{
    this->x = x;
    this->y = y;
    this->rot = rot;
    this->shape = shape;
}

/**
 * copyMino()メソッド
 *
 * オブジェクトの複製を作成する．
 *
 * @param なし
 * @return Minoオブジェクト オブジェクトの複製品
 */

Mino* Mino::copyMino()
{
    return new Mino(this->x, this->y, this->rot, this->shape);
}

/**
 * rotMino()メソッド
 *
 * テトロミノを回転させる．
 * 回転には行列計算を用いる．
 * 実行の度に，回転状態を計算する必要があるため，
 * 毎回Blockオブジェクトを生成（上書き）する．
 *
 * @param block[] Blockオブジェクトが格納されている配列
 * @return なし
 */

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

    //------------------------
    // 回転処理
    // 右回転->this->rot++;
    // 左回転->this->rot--;
    // マイナスの値にも対応するため，
    // (400 + this->rot) % 4を計算．
    // -> 出た値だけ右回転する．
    //------------------------
    for (int r = 0; r < (NUM * 100 + this->rot) % NUM; r++)
    {
        for (int i = 0; i < NUM; i++)
        {
            block[i]->rot();
        }
    }
}

/**
 * draw()メソッド
 *
 * テトロミノを描画する．
 * テトロミノの座標+各Blockオブジェクトの座標
 * で任意の位置にテトロミノを描画する．
 * 描画直前にテトロミノの回転処理をする．
 *
 * @param なし
 * @return なし
 */

void Mino::draw()
{
    this->rotMino(this->block);
    for (int i = 0; i < NUM; i++)
    {
        DrawBlock(this->block[i]->x+this->x, this->block[i]->y+this->y);
    }
}

/**
 * ~Mino()デストラクタ
 *
 * Blockオブジェクトを破棄する．
 *
 * @param なし
 * @return なし
 */

Mino::~Mino()
{
    for (int i = 0; i < NUM; i++)
    {
        delete block[i];
    }
}

/**
 * Boardクラス
 *
 * 盤面の情報を持つ．
 * ブロックの設置に加え，
 * 盤面のライン消去も行う．
 */

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

/**
 * Board()コンストラクタ
 *
 * 盤面の壁を生成する．
 *
 * @param なし
 * @return なし
 */

Board::Board()
{
    for (int y = 0; y < MAX_Y; y++)
    {
        for (int x = 0; x < MAX_X; x++)
        {
            if (!this->board[y][x])
            {
                block[y][x] = NULL;
                continue;
            }

            block[y][x] = new Block(x, y);
        }
    }
}

/**
 * setBlock()メソッド
 *
 * 任意の座標にブロックを設置する．
 *
 * @param x, y 盤面の配列番号
 * @return なし
 */

void Board::setBlock(int x, int y)
{
    this->board[y][x] = 1;
}

/**
 * getBlock()メソッド
 *
 * 任意の座標の情報を取得する．
 *
 * @param x, y 盤面の配列番号
 * @return 0 1 指定したマスがブロックなら1，空なら0
 */

int Board::getBlock(int x, int y)
{
    return this->board[y][x];
}

/**
 * findLineFilled()メソッド
 *
 * 盤面のすべてを探索し，
 * 消去可能なラインが見つかれば
 * その行番号を返却する．
 *
 * @param なし
 * @return y, -1 消去可能な行が見つからなければ-1
 */

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

/**
 * cutLine()メソッド
 *
 * 指定した行番号の消去を実現する．
 * 一つずつ全ての上段のブロックを
 * 下げることで再現する．
 *
 * @param y 行番号
 * @return なし
 */

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

/**
 * draw()メソッド
 *
 * 現在の盤面のブロックを描画する．
 *
 * @param なし
 * @return なし
 */

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

/**
 * ~Board()デストラクタ
 *
 * 盤面上のBlockオブジェクトを破棄する．
 *
 * @param なし
 * @return なし
 */

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

/**
 * Tetrisクラス
 *
 * 本プログラムのメインクラス．
 */

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

/**
 * Tetris()コンストラクタ
 *
 * もろもろの初期設定を行う．
 *
 * @param なし
 * @return なし
 */

Tetris::Tetris()
{
    // ゲーム実行時にLog.txtを生成しない
    SetOutApplicationLogValidFlag(false);
    // フルスクリーンに設定しない
    ChangeWindowMode(true);
    // DxLibの初期化
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

/**
 * mainLoop()メソッド
 *
 * 説明なし．
 *
 * @param なし
 * @return なし
 */

void Tetris::mainLoop()
{
    // エラー or Escキー入力で終了
    while (!ProcessMessage() && !KeyPressed(KEY_INPUT_ESCAPE))
    {
        // 画面を初期化
        ClearDrawScreen();
        // テトリスゲームを更新
        this->update();
        // スクリーンに反映
        ScreenFlip();
    }
}

/**
 * update()メソッド
 * 
 * テトリスゲームの更新手続きを行う．
 * 衝突判定を実装しやすくするため，
 * futureMinoオブジェクトや
 * minoVxVyVrVDrop変数を使用し，
 * 条件を満たしたときのみ
 * テトロミノの更新を行う．
 *
 * @param なし
 * @return なし
 */

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

/**
 * isMinoMovable()メソッド
 *
 * テトロミノが移動できるかを判定する．
 * 移動後（未来）のテトロミノの一部が
 * 盤面上のブロックと接触してる場合，
 * 移動不可とみなす．
 *
 * @param futureMino 仮移動したMinoオブジェクト
 * @return true false 移動可能ならtrue，そうでなければfalse
 */

bool Tetris::isMinoMovable(Mino* futureMino)
{
    futureMino->rotMino(futureMino->block);
    for (int i = 0; i < NUM; i++)
    {
        // 移動先の盤面が1（ブロック）ならfalseを返却
        if (this->board->getBlock(futureMino->block[i]->x+futureMino->x, futureMino->block[i]->y+futureMino->y))
        {
            return false;
        }
    }

    return true;
}

/**
 * action()メソッド
 *
 * プレイヤーの操作を受け付ける．
 * また，テトロミノの自動落下も判定する．
 *
 * @param なし
 * @return なし
 */

void Tetris::action()
{
    if (KeyPressed(KEY_INPUT_LEFT))  this->minoVx = -1;
    if (KeyPressed(KEY_INPUT_RIGHT)) this->minoVx = 1;
    if (KeyPressed(KEY_INPUT_DOWN))  this->minoVy = 1;
    if (KeyPressed(KEY_INPUT_Z))     this->minoVr = -1;
    if (KeyPressed(KEY_INPUT_X))     this->minoVr = 1;
    if (KeyPressed(KEY_INPUT_C))     this->minoDrop = true;
    // 20Fに1回落下する
    if ((this->frameCounter%20)==19) this->minoVy = 1;
}

/**
 * makeNewMino()メソッド
 *
 * 新しいテトロミノを作成する．
 *
 * @param なし
 * @return Minoオブジェクト 新規Minoオブジェクト
 */

Mino* Tetris::makeNewMino()
{
    return new Mino(5, 2, 0, rand() % 7);
}

/**
 * ~Tetris()デストラクタ
 *
 * Minoオブジェクト，Boardオブジェクトを破棄する．
 * また，DxLibの使用終了関数も呼び出す．
 *
 * @param なし
 * @return なし
 */
Tetris::~Tetris()
{
    delete mino;
    delete board;
    // DxLib終了
    DxLib_End();
}

/**
 * WinMain()関数
 *
 * Tetrisクラスのコンストラクタとデストラクタを実行する．
 *
 * @param https://learn.microsoft.com/ja-jp/windows/win32/learnwin32/winmain--the-application-entry-point
 * @return 0 メイン関数正常終了
 */

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Tetris* tetris = new Tetris;
    delete tetris;
    return 0;
}
