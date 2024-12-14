#ifndef BOARD_H
#define BOARD_H

#include <math.h>

#include <cstdlib>
#include <ctime>
#include <utility>
#include <vector>


#include "gemmanager.h"
#include "gem.h"

class Board {
   private:
    int stop_;
    bool add_tools;
    int combo_times;
    int point_;                                      // 分数
    int cnt_;                                        // 记录宝石id编号
    double combo_base;                               // combo对分数的加成
    std::pair<int, int> chosen_;                     // 鼠标点击的宝石位置
    std::pair<int, int> hint_[2];                    // 提示：可交换的两个宝石的位置
    Gem gems_[8][8];                             // 宝石矩阵
    std::vector<std::pair<int, int>> matches_;       // 可消除的宝石
    GemManager* gem_manager_;

   public:
    explicit Board();
    void SetGemManager(GemManager* gem_manager);

    void initBoard();
    void generate(bool start);      // 生成. start :true:开始游戏时生成 false:游戏中途生成
    void swap(Gem& a, Gem& b);  // 交换两个宝石
    void clicked(int x, int y);     // 点击坐标(x,y)
    bool check();                   // 检查是否有可以消除的宝石，若有则将位置记录在matches_中
    bool isGameOver();
    void remove();              // 消除matches_中的宝石
    void fall();                // 宝石掉落并补齐
    void fall2();               // 宝石掉落并补齐(无动画和加分)
    bool showHint(bool show);   // 提示
    void remove(int i, int j);  // 移除位置(i,j)上的宝石
    void refresh();
    std::pair<int, int> GetChosen();  // 获取点击的宝石位置
    void clickedOnHint();             // 提示
    int getScore();                   // 获取分数
    void clickedOnStop();             // 暂停

    void cancelHint();                // 取消暂停


};

#endif  // BOARD_H
