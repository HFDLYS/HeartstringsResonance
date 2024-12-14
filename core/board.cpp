#include "board.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <unistd.h>



Board::Board() {
    // std::cerr << "Into second constructor\n";
    Gem::SetMaxType(4);
    // std::cout << difficulty << std::endl;
    add_tools = 0;
    cnt_ = 0;
    combo_times = 0;
    stop_ = 0;
    point_ = 0;
    // std::cerr << "generate start" << std::endl;
    generate(1);
    // std::cerr << "generate end" << std::endl;
}

void Board::SetGemManager(GemManager *gem_manager) { gem_manager_ = gem_manager; }

void Board::initBoard() {
    #ifdef _WIN32
        Sleep(100);
    #else
        usleep(100000);
    #endif


    gem_manager_->Init(8, 8);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int ret = gem_manager_->Generate(gems_[i][j].GetId(), i, j, gems_[i][j].GetType(),
                                                              300 + rand() % 500);
            if (ret != GemManager::kSuccess) std::cout << i << " " << j << " " << ret << std::endl;
        }
    }
}

std::pair<int, int> Board::GetChosen() { return chosen_; }


/* 生成 */
void Board::generate(bool start) {
    if (!start) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                gem_manager_->Remove(gems_[i][j].GetId(), true);
            }
        }
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            gems_[i][j] = Gem(++cnt_);
            if (!start) gem_manager_->Generate(cnt_, i, j, gems_[i][j].GetType());
        }
    }
    if (start) {
        while (check()) {
            for (auto match : matches_) {
                gems_[match.first][match.second].SetType(rand() % Gem::GetMaxType() + 1);
            }
            add_tools = 0;
        }
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                gems_[i][j].SetId(++cnt_);
            }
        }
    }
    hint_[0] = hint_[1] = chosen_ = {-1, -1};
}

/* 交换两个宝石 */
void Board::swap(Gem &a, Gem &b) {
    Gem tmp = a;
    a = b;
    b = tmp;
}

/* 检查是否有可以消除的宝石，若有则将位置记录在matches_中 */
bool Board::check() {
    add_tools = 0;
    matches_.clear();
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            int now = j - 1, type = gems_[i][j].GetType();
            int sum = 1;
            while (now >= 0 && gems_[i][now].GetType() == type) {
                now--;
                sum++;
            }
            now = j + 1;
            while (now < 8 && gems_[i][now].GetType() == type) {
                now++;
                sum++;
            }
            if (sum >= 3) {
                matches_.push_back({i, j});
                if (sum >= 5) {
                    add_tools = 1;
                }
                continue;
            }
            sum = 1;
            now = i - 1;
            while (now >= 0 && gems_[now][j].GetType() == type) {
                now--;
                sum++;
            }
            now = i + 1;
            while (now < 8 && gems_[now][j].GetType() == type) {
                now++;
                sum++;
            }
            if (sum >= 3) {
                matches_.push_back({i, j});
                if (sum >= 5) {
                    add_tools = 1;
                }
                continue;
            }
        }
    }
    if (!matches_.empty()) {
        return 1;
    }
    return 0;
}

/* 鼠标点击坐标(x,y) */
void Board::clicked(int x, int y) {
    if (stop_) return;
    if (gem_manager_->isPlayingAnimation()) return;

    std::cout << "Board:Clicked" << x << " " << y << "\n";
    for (int j = 0; j < 8; ++j) {
        for (int i = 0; i < 8; ++i) {
            std::cerr << gems_[i][j].GetType() << " ";
        }
        std::cerr << "\n";
    }
    std::cerr << "\n";

    int chosen_x = x, chosen_y = y;

    // std::cerr << x << " " << y << " " << chosen_x << " " << chosen_y << std::endl;
    if (chosen_x == -1) {
        if (chosen_.first != -1) {
            chosen_ = {-1, -1};
            gem_manager_->SetRotate(gems_[chosen_.first][chosen_.second].GetId(),
                                                     GemManager::kRotate);
        }
        return;
    }
    cancelHint();
    
    if (chosen_x == chosen_.first && chosen_y == chosen_.second) {
        gem_manager_->SetRotate(gems_[chosen_.first][chosen_.second].GetId(),
                                                 GemManager::kRotate);
        chosen_ = {-1, -1};
        return;
    }
    if (abs(chosen_x - chosen_.first) + abs(chosen_y - chosen_.second) <= 1) {
        swap(gems_[chosen_x][chosen_y], gems_[chosen_.first][chosen_.second]);
        if (!check()) {
            swap(gems_[chosen_x][chosen_y], gems_[chosen_.first][chosen_.second]);
            gem_manager_->SetRotate(gems_[chosen_.first][chosen_.second].GetId(),
                                                     GemManager::kRotate);
            chosen_ = {chosen_x, chosen_y};
            // animation setRotate
            gem_manager_->SetRotate(gems_[chosen_.first][chosen_.second].GetId(),
                                                     GemManager::kRotateFastInverse);

            return;
        } else {
            gem_manager_->SetRotate(gems_[chosen_.first][chosen_.second].GetId(),
                                                     GemManager::kRotate);
            gem_manager_->SetRotate(gems_[chosen_x][chosen_y].GetId(),
                                                     GemManager::kRotate);
            gem_manager_->SwapStone(gems_[chosen_.first][chosen_.second].GetId(),
                                                     gems_[chosen_x][chosen_y].GetId());

            std::cerr << "swap:" << chosen_.first << " " << chosen_.second << " " << chosen_x << " " << chosen_y
                      << "\n";
            chosen_ = {-1, -1};
            refresh();
        }
        return;
    }
    gem_manager_->SetRotate(gems_[chosen_.first][chosen_.second].GetId(),
                                             GemManager::kRotate);
    chosen_ = {chosen_x, chosen_y};
    gem_manager_->SetRotate(gems_[chosen_.first][chosen_.second].GetId(),
                                             GemManager::kRotateFastInverse);
}

int Board::getScore() { return point_; }

void Board::clickedOnStop() {
    if (stop_ == 1) {
        // hypercube->stop(0)
        gem_manager_->SetPause(false);
        stop_ = 0;
    } else if (stop_ == 0) {
        // hypercube->stop(1)
        gem_manager_->SetPause(true);
        stop_ = 1;
    }
    // std::cout << "set stop: " << (stop_ ? "true" : "false") << std::endl;
}


void Board::refresh() {
    cancelHint();
    combo_base = 1.0;
    double accelerate_base = 0.2;

    combo_times = 0;
    while (!matches_.empty() || check()) {
        if (add_tools) {
            add_tools = 0;
        }
        combo_times++;

        // BGM::GetInstance()->PlayFall();
        remove();
        fall();
        combo_base += accelerate_base;
        accelerate_base += 0.1;
        matches_.clear();
    };
}

/*消除matches_中的宝石*/
void Board::remove() {
    for (const auto &match : matches_) {
        remove(match.first, match.second);
        // std::cerr << "match: " << match.first << " " << match.second << "\n";
    }
    matches_.clear();
    while (false && gem_manager_->isPlayingAnimation()) {
        #ifdef _WIN32
            usleep(20000); // usleep takes sleep time in microseconds (1 second = 1,000,000 microseconds)
        #else
            usleep(20000); // usleep takes sleep time in microseconds (1 second = 1,000,000 microseconds)
        #endif
    };
}

void Board::remove(int x, int y) {
    if (x < 0 || y < 0 || x > 7 || y > 7) return;
    if (gems_[x][y].Empty()) return;
    int difficulty_base = 1;
    point_ += 2.0 * combo_base * difficulty_base;
    gems_[x][y].SetEmpty(1);
    // animation Remove
    gem_manager_->Remove(gems_[x][y].GetId(), true);
    return;
}

void Board::clickedOnHint() { showHint(1); }


// 提示
bool Board::showHint(bool show) {
    bool get_hint = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (i < 7) {
                bool flag = 0;
                swap(gems_[i][j], gems_[i + 1][j]);
                flag = check();
                swap(gems_[i][j], gems_[i + 1][j]);
                if (flag) {
                    hint_[0] = {i, j};
                    hint_[1] = {i + 1, j};
                    get_hint = 1;
                    break;
                }
            }
            if (j < 7) {
                bool flag = 0;
                swap(gems_[i][j], gems_[i][j + 1]);
                flag = check();
                swap(gems_[i][j], gems_[i][j + 1]);
                if (flag) {
                    hint_[0] = {i, j};
                    hint_[1] = {i, j + 1};
                    get_hint = 1;
                    break;
                }
            }
        }
        if (get_hint) break;
    }

    if (show) {
        if (get_hint == 0) return 0;
        if (point_ < 100) {
            return 0;
        }
        point_ -= 100;
    }

    if (show) {
        if (chosen_.first != -1) {
            gem_manager_->SetRotate(gems_[chosen_.first][chosen_.second].GetId(),
                                                     GemManager::kRotate);
        }
        chosen_ = {-1, -1};
        gem_manager_->SetRotate(gems_[hint_[0].first][hint_[0].second].GetId(),
                                                 GemManager::kRotateFast);
        gem_manager_->SetRotate(gems_[hint_[1].first][hint_[1].second].GetId(),
                                                 GemManager::kRotateFast);
    }
    return get_hint;
}

void Board::cancelHint() {
    if (hint_[0].first == -1) return;
    gem_manager_->SetRotate(gems_[hint_[0].first][hint_[0].second].GetId(),
                                             GemManager::kRotate);
    gem_manager_->SetRotate(gems_[hint_[1].first][hint_[1].second].GetId(),
                                             GemManager::kRotate);
    hint_[0] = hint_[1] = {-1, -1};
    return;
}

bool Board::isGameOver() {
    if (gem_manager_->isPlayingAnimation()) return 0;
    return 0;
}

/* 宝石掉落并补齐 */
void Board::fall() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 7; j >= 0; --j) {
            if (gems_[i][j].Empty()) continue;
            int now = j + 1;
            while (now < 8 && gems_[i][now].Empty()) {
                now++;
            }
            now--;
            if (now == j) continue;

            gem_manager_->FallTo(gems_[i][j].GetId(), now);
            swap(gems_[i][j], gems_[i][now]);
        }
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (!gems_[i][j].Empty()) break;
            // animation generate

            gems_[i][j] = Gem(++cnt_);
            gems_[i][j].SetEmpty(0);
            gem_manager_->Generate(gems_[i][j].GetId(), i, j, gems_[i][j].GetType());
        }
    }
    while (false && gem_manager_->isPlayingAnimation()) {
        usleep(20000); // usleep takes sleep time in microseconds (1 second = 1,000,000 microseconds)
    };
}
