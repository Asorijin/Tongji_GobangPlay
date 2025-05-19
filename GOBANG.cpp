#include <iostream>
#include <string>
#include <ctime>
#include<vector>
#include<queue>
using namespace std;

// board information
#define BOARD_SIZE 12
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define BETA 99999999
#define ALPHA -99999999

// command information
const string START = "START";
const string PLACE = "PLACE";
const string TURN = "TURN";
const string END = "END";

int myFlag;
int enemyFlag;
constexpr int BOARD_MIDDLE_1 = (BOARD_SIZE + 1) / 2 - 1;
constexpr int BOARD_MIDDLE_2 = BOARD_SIZE / 2;
int my_x= BOARD_MIDDLE_1-1, my_y= BOARD_MIDDLE_2+1;


constexpr int MAXDEPTH = 11;//奇数
constexpr int MAXNODES = 3;

//落子点类，存储位置信息和当前落子得分
class PtPos {
public:
    int x;
    int y;
    int flag;
    long long alpha=ALPHA;
    long long beta=BETA;
    long long score=0;

    PtPos(int x,int y) {
        this->x = x;
        this->y = y;
    }
    bool operator<(const PtPos& other) const {
        return this->score < other.score;
    }
};
constexpr int SCORE[4] = {10000000000,1000000,1000,1};
int DEFENSE_K = 2;
enum NUM {
    FIVE,
    FOUR,
    THREE,
    TWO,
};
static class tools {
public:
    static long long SumCul(int com,long long sum=0) {
        if (com + 1 >= 5)
            sum += SCORE[FIVE];
        else if (com + 1 == 4)
            sum += SCORE[FOUR];
        else if (com + 1 == 3)
            sum += SCORE[THREE];
        else if (com + 1 == 2)
            sum += SCORE[TWO];
        return sum;
    }
    static void AddPt(int b[][BOARD_SIZE], PtPos p) {
        b[p.x][p.y] = p.flag;
    }
    static void RemovePt(int b[][BOARD_SIZE], PtPos p) {
        b[p.x][p.y] = 0;
    }
    static vector<PtPos> PtPosEvaluate(int b[][BOARD_SIZE],int whoFlag) {
        vector<PtPos> nowPts;
        priority_queue<PtPos> priorPts;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int k = 0; k < BOARD_SIZE; k++) {
                long long sum = 0;
                int right=0, left=0, up=0, down=0, rightUp=0, rightDown=0, leftUp=0, leftDown=0;
                if (b[i][k] == 0) {
                    //横向
                    long long sum=0;
                    int emp = 0,com=0;
                    //offense
                    for (int j = k-1; j >= 0; j--) {
                        if (b[i][j] == 0) {
                            emp++;
                        }
                        if (b[i][j] == 3 - whoFlag) {
                            com += k - j - emp-1;
                            break;
                        }
                        if (emp >= 2) {
                            com += k - j - emp;
                            if (b[i][j] != b[i][j + 1])
                                com--;
                            if (com < 0)
                                com = 0;
                            break;
                        }
                    }
                    if (com != 0)
                        right = com;
                    emp = 0;
                    for (int j = k + 1; j < BOARD_SIZE; j++) {
                        if (b[i][j] == 0) {
                            emp++;
                        }
                        if (b[i][j] == 3 - whoFlag) {
                            com += j - k - emp - 1;
                            break;
                        }
                        if (emp >= 2) {
                            com += j - k - emp;
                            if (b[i][j] != b[i][j - 1])
                                com--;
                            if (com < 0)
                                com = 0;
                            break;
                        }
                    }
                    if (right != com)
                        left = 1;
                    
                    if (left && right)
                        sum += SumCul(com)*3;
                    else
                        sum += SumCul(com);
                    com = 0;
                    emp = 0;
                    //defense(0.5rate)
                    right = 0,left = 0;
                    for (int j = k - 1; j >= 0; j--) {
                        if (b[i][j] == 0) {
                            emp++;
                        }
                        if (b[i][j] == whoFlag) {
                            com += k - j - emp - 1;
                            
                            DEFENSE_K *= 2;
                            break;
                        }
                        if (emp >= 2) {
                            com += k - j - emp;
                            if (b[i][j] != b[i][j + 1])
                                com--;
                            if (com < 0)
                                com = 0;
                            break;
                        }
                    }
                    if (com)
                        right = com;
                    emp = 0;
                    for (int j = k + 1; j < BOARD_SIZE; j++) {
                        if (b[i][j] == 0) {
                            emp++;
                        }
                        if (b[i][j] == whoFlag) {
                            com += j - k - emp - 1;
                            
                            DEFENSE_K *= 2;
                            break;
                        }
                        if (emp >= 2) {
                            com += j - k - emp;
                            if (b[i][j] != b[i][j - 1])
                                com--;
                            if (com < 0)
                                com = 0;
                            break;
                        }
                    }
                    if (right != com)
                        left = 1;
                    if (left && right) {
                        sum += SumCul(com) / DEFENSE_K*3;
                    }
                    else {
                        sum += SumCul(com) / DEFENSE_K;
                    }
                    com = 0;
                    emp = 0;
                    DEFENSE_K = 2;
                    //纵向
                    //OFFENSE
                    for (int j = i + 1; j < BOARD_SIZE; j++) {
                        if (b[j][k] == 0) {
                            emp++;
                        }
                        if (b[j][k] == 3 - whoFlag) {
                            com += j - i - emp - 1;
                            break;
                        }
                        if (emp >= 2) {
                            com += j - i - emp;
                            if (b[j][k] != b[j - 1][k])
                                com--;
                            if (com < 0)
                                com = 0;
                            break;
                        }
                    }
                    if (com)
                        up = com;
                    emp = 0;
                    for (int j = i - 1; j>=0; j--) {
                        if (b[j][k] == 0) {
                            emp++;
                        }
                        if (b[j][k] == 3 - whoFlag) {
                            com += i - j - emp - 1;
                            break;
                        }
                        if (emp >= 2) {
                            com += i - j - emp;
                            if (b[j][k] != b[j + 1][k])
                                com--;
                            if (com < 0)
                                com = 0;
                            break;
                        }
                    }
                    if (up != com)
                        down = 1;
                    if (up && down) {
                        sum += SumCul(com)*3;
                    }
                    else {
                        sum += SumCul(com);
                    }
                    emp = 0;
                    com = 0;
                    //DEFENSE
                    up = 0, down = 0;
                    for (int j = i + 1; j < BOARD_SIZE; j++) {
                        if (b[j][k] == 0) {
                            emp++;
                        }
                        if (b[j][k] == whoFlag) {
                            com += j - i - emp - 1;
                            DEFENSE_K *= 2;
                            break;
                        }
                        if (emp >= 2) {
                            com += j - i - emp;
                            if (b[j][k] != b[j - 1][k])
                                com--;
                            if (com < 0)
                                com = 0;
                            break;
                        }
                    }
                    if (com) {
                        up = com;
                    }
                    emp = 0;
                    for (int j = i - 1; j >= 0; j--) {
                        if (b[j][k] == 0) {
                            emp++;
                        }
                        if (b[j][k] == whoFlag) {
                            com += i - j - emp - 1;
                            DEFENSE_K *= 2;
                            break;
                        }
                        if (emp >= 2) {
                            com += i - j - emp;
                            if (b[j][k] != b[j + 1][k])
                                com--;
                            if (com < 0)
                                com = 0;
                            break;
                        }
                    }
                    if (up != com)
                        down = 1;
                    emp = 0;
                    if (up && down) {
                        sum += SumCul(com) / DEFENSE_K*3;
                    }
                    else {
                        sum += SumCul(com) / DEFENSE_K;
                    }
                    com = 0;
                    DEFENSE_K = 2;
                    //两对角
                    //OFFENSE
                    for (int j = 1; j + i < BOARD_SIZE && j + k < BOARD_SIZE; j++) {
                        if (b[i+j][j+k] == 0) {
                            emp++;
                        }
                        if (b[i + j][j + k] == 3 - whoFlag) {
                            com += j - emp - 1;
                            break;
                        }
                        if (emp >= 2) {
                            com += j - emp;
                            if (b[i+j][j+k] != b[i+j-1][j+k-1])
                                com--;
                            if (com < 0)
                                com = 0;
                            break;
                        }
                    }
                    if (com)
                        rightDown = com;
                    emp = 0;
                    for (int j = 1;  i-j >=0 &&  k-j >=0; j++) {
                        if (b[i - j][k-j] == 0) {
                            emp++;
                        }
                        if (b[i - j][k - j] == 3 - whoFlag) {
                            com += j - emp - 1;
                            break;
                        }
                        if (emp >= 2) {
                            com += j - emp;
                            if (b[i - j][k-j] != b[i - j - 1][k-j - 1])
                                com--;
                            if (com < 0)
                                com = 0;
                            break;
                        }
                    }
                    com = 0;
                    if (rightDown != com) {
                        rightUp = 1;
                    }
                    if (rightUp && rightDown) {
                        sum += SumCul(com) * 3;
                    }
                    else {
                        sum += SumCul(com);
                    }
                    emp = 0,rightDown=0,rightUp=0;
                    //DEFENSE
                    for (int j = 1; j + i < BOARD_SIZE && j + k < BOARD_SIZE; j++) {
                        if (b[i + j][j + k] == 0) {
                            emp++;
                        }
                        if (b[i + j][j + k] == whoFlag) {
                            DEFENSE_K *= 2;
                            com += j - emp - 1;
                            break;
                        }
                        if (emp >= 2) {
                            com += j - emp;
                            if (b[i + j][j + k] != b[i + j - 1][j + k - 1])
                                com--;
                            if (com < 0)
                                com = 0;
                            break;
                        }
                    }
                    if (com) {
                        rightDown = com;
                    }
                    emp = 0;
                    for (int j = 1; i - j  >= 0 && k - j >=0 ; j++) {
                        if (b[i - j][k - j] == 0) {
                            emp++;
                        }
                        if (b[i - j][k - j] == whoFlag) {
                            DEFENSE_K *= 2;
                            com += j - emp - 1;
                            break;
                        }
                        if (emp >= 2) {
                            com += j - emp;
                            if (b[i - j][k - j] != b[i - j - 1][k - j - 1])
                                com--;
                            if (com < 0)
                                com = 0;
                            break;
                        }
                    }
                    if (rightDown != com)
                        rightUp = 1;
                    if (rightUp && rightDown) {
                        sum += SumCul(com) / DEFENSE_K * 3;
                    }
                    else {
                        sum += SumCul(com) / DEFENSE_K;
                    }
                    com = 0;
                    emp = 0;
                    DEFENSE_K = 2;
                    //OFFENSE
                    for (int j = 1; i - j >= 0 && j + k < BOARD_SIZE; j++) {
                        if (b[i - j][j + k] == 0) {
                            emp++;
                        }
                        if (b[i - j][j + k] == 3 - whoFlag) {
                            com += j - emp - 1;
                            break;
                        }
                        if (emp >= 2) {
                            com += j - emp;
                            if (b[i - j][j + k] != b[i - j - 1][j + k - 1])
                                com--;
                            if (com < 0)
                                com = 0;
                            break;
                        }
                    }
                    if (com)
                        leftUp = com;
                    emp = 0;
                    for (int j = 1; j + i < BOARD_SIZE &&  k-j >=0; j++) {
                        if (b[i + j][k - j] == 0) {
                            emp++;
                        }
                        if (b[i + j][k - j] == 3 - whoFlag) {
                            com += j - emp - 1;
                            break;
                        }
                        if (emp >= 2) {
                            com += j - emp;
                            if (b[i + j][k - j] != b[i + j - 1][k - j - 1])
                                com--;
                            if (com < 0)
                                com = 0;
                            break;
                        }
                    }
                    if (leftUp != com)
                        leftDown = 1;
                    if (leftDown && leftUp) {
                        sum += SumCul(com) * 3;
                    }
                    else {
                        sum += SumCul(com);
                    }
                    com = 0;
                    emp = 0;
                    leftDown = 0, leftUp = 0;
                    //DEFENSE
                    for (int j = 1; i - j >= 0 && j + k < BOARD_SIZE; j++) {
                        if (b[i - j][j + k] == 0) {
                            emp++;
                        }
                        if (b[i - j][j + k] == whoFlag) {
                            DEFENSE_K *= 2;
                            com += j - emp - 1;
                            break;
                        }
                        if (emp >= 2) {
                            com += j - emp;
                            if (b[i - j][j + k] != b[i - j - 1][j + k - 1])
                                com--;
                            if (com < 0)
                                com = 0;
                            break;
                        }
                    }
                    if (com) {
                        leftDown = com;
                    }
                    emp = 0;
                    for (int j = 1; j + i < BOARD_SIZE && k - j >= 0; j++) {
                        if (b[i + j][k - j] == 0) {
                            emp++;
                        }
                        if (b[i + j][k - j] == whoFlag) {
                            com += j - emp - 1;
                            DEFENSE_K *= 2;
                            break;
                        }
                        if (emp >= 2) {
                            com += j - emp;
                            if (b[i + j][k - j] != b[i + j - 1][k - j - 1])
                                com--;
                            if (com < 0)
                                com = 0;
                            break;
                        }
                    }
                    if (leftDown != com)
                        leftUp = 1;
                    if (leftUp && leftDown) {
                        sum += SumCul(com) / DEFENSE_K*3;
                    }
                    else{
                        sum += SumCul(com) / DEFENSE_K;
                    }
                    com = 0;
                    emp = 0;

                    DEFENSE_K = 2;
                    PtPos p = PtPos(i,k);
                    p.score = sum;
                    p.flag = whoFlag;
                    priorPts.push(p);
                }
            }
        }
        for (int i = 0; i < priorPts.size() && i < MAXNODES; i++) {
            nowPts.push_back(priorPts.top());
            priorPts.pop();
        }
        for (int i = nowPts.size()-1; i  >=0; i--) {
            if (nowPts[0].score / nowPts[i].score > 3)
                nowPts.pop_back();
        }
        return nowPts;
    }

    //alphabeta递归函数，用于查找最优落子
    static int AlphaBeta(int b[][BOARD_SIZE],int depth,int whoFlag, int beta = BETA, int alpha = ALPHA) {
        vector<PtPos> BestPts;
        BestPts = PtPosEvaluate(b, whoFlag);
        

        if (depth == 11) {
            //最深层，处理获得一个最小值后返回最小值作为β
            long long minScore=BETA;

            for (auto& a : BestPts) {
                minScore = min(minScore, a.score);
            }

            return minScore;
        }
        //对手分支
        if (depth % 2 == 0) {
            
            for (auto& a:BestPts) {
                AddPt(b, a);
                a.alpha = alpha;
                a.beta = beta;
                
                
                a.beta = min(AlphaBeta(b,depth+1,myFlag,a.beta,a.alpha),beta);

                RemovePt(b, a);
                //这里可以改为>=，作为可调参数
                if (a.alpha >= a.beta) {
                    break;
                }
                alpha=a.beta;
                
            }
            return alpha;
        }
        //我方分支
        else {
            for (auto& a : BestPts) {
                AddPt(b, a);
                a.alpha = alpha;
                a.beta = beta;
                a.alpha = max(AlphaBeta(b, depth + 1,enemyFlag,a.beta,a.alpha),alpha);
                RemovePt(b, a);
                if (a.alpha >= a.beta) {
                    break;
                }
                beta = a.alpha;
                
            }
            if (depth == 1) {
                //根据alpha决策落子点，在这里给位置静态全局变量赋值后退出函数
                //取当前α值最大的BestPts元素
                long long maxAlpha = 0;
                
                for (auto& a : BestPts) {
                    if (maxAlpha <= a.alpha) {
                        maxAlpha = a.alpha;
                        my_x = a.x;
                        my_y = a.y;
                    }
                }
            }
            return beta;
        }
        
    }
};

struct coordinate {
    int x = -1;
    int y = -1;
};

class Game {
public:
    int board[BOARD_SIZE][BOARD_SIZE] = { {0} };
    int state = 0;

    Game() {
        board[BOARD_MIDDLE_1][BOARD_MIDDLE_1] = WHITE;
        board[BOARD_MIDDLE_2][BOARD_MIDDLE_2] = WHITE;
        board[BOARD_MIDDLE_2][BOARD_MIDDLE_1] = BLACK;
        board[BOARD_MIDDLE_1][BOARD_MIDDLE_2] = BLACK;
    }

public: 
    void arr_input(int x, int y, int playerround) {
        board[x][y] = playerround;
    }
    
    void start(int b[][BOARD_SIZE]) {
        tools t;
        t.AlphaBeta(b,1,myFlag);
    }
};

Game game;

void loop() {
    string tag;
    int status;
    coordinate command;

    while (true) {
        cin >> tag;
        if (tag == START) {
            cin >> myFlag;
            enemyFlag = 3 - myFlag;
            cout << "OK" << endl;
        }
        else if (tag == PLACE) {
            cin >> command.x >> command.y;
            game.arr_input(command.x, command.y, enemyFlag);
            game.start(game.board);
        }
        else if (tag == TURN) {
            // 轮到己方落子
            // 计算my_x, my_y
            
            game.arr_input(my_x, my_y, myFlag);
            /*cout << "************"<<endl;
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int k = 0; k < BOARD_SIZE; k++) {
                    cout << game.board[i][k] << " ";
                }
                cout << endl;
            }
            cout << "**************"<<endl;*/
            cout << my_x << " " << my_y << endl;
        }
        else if (tag == END) {
            cin >> status;
            return;
        }
    }
}

int main() {
    loop();
    return 0;
}