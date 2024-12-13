#include "gem.h"

#include <cassert>
#include <cmath>
#include <iostream>

namespace Graphics {

Gem::Gem() {}

Gem::Gem(int x, int y, int z, int angle, int type)
    : x_(x), y_(y), z_(z), angle_(angle), type_(type), pausing_(false), active_(true) {
    falling_target_y_ = 0;
    swaping_timer_ = 0;
    rotating_speed_ = 0;
    removing_acceleration_ = 0;
}
// 基础属性
int Gem::x() const { return x_; }
int Gem::y() const { return y_; }
int Gem::z() const { return z_; }
float Gem::angle() const { return angle_; }
int Gem::type() const { return type_; }
// 暂停
bool Gem::is_pausing() const { return pausing_; }
void Gem::set_pausing(int pausing) { pausing_ = pausing; }
// 可用
bool Gem::is_active() const { return active_; }
void Gem::set_active(bool active) { active_ = active; }
// 下落
bool Gem::is_falling() const { return falling_target_y_ != 0; }
float Gem::falling_speed() const { return falling_speed_; }
float Gem::falling_acceleration() const { return falling_acceleration_; }
void Gem::set_falling(float speed, int falling_target_y) {
    if (std::abs(speed - kFallingSpeedRandom) <= 1e-6) {
        speed = kFallingSpeed + (rand() % 5) - 2;
    }
    falling_speed_ = speed;
    falling_target_y_ = falling_target_y;
}
// 旋转
bool Gem::is_rotating() const { return rotating_speed_ != 0; }
float Gem::rotating_speed() const { return rotating_speed_; }
void Gem::set_rotating_speed(float speed) { rotating_speed_ = speed; }
// 交换
bool Gem::is_swaping() const { return swaping_timer_ > 0; }
void Gem::set_swaping(int target_x, int target_y, float swaping_speed) {
    swaping_timer_ = 180;
    swaping_speed_ = swaping_speed;
    swaping_start_x_ = x_;
    swaping_start_y_ = y_;
    swaping_target_x_ = target_x;
    swaping_target_y_ = target_y;
    std::cerr << "set swap: "
              << "(" << x_ << ", " << y_ << ") -> (" << target_x << ", " << target_y << ")" << std::endl;
}

// 删除
bool Gem::is_removing() const { return removing_acceleration_ != 0; }
void Gem::set_removing(float removing_speed, float removing_acceleration) {
    removing_speed_ = removing_speed;
    removing_acceleration_ = removing_acceleration;
}

// 更新
void Gem::UpdateRotating() {
    // rotating
    if (is_rotating()) {
        angle_ += rotating_speed_;
    }
}

void Gem::UpdateFalling() {
    // falling
    if (is_falling()) {
        // falling_speed_ += falling_acceleration_;
        y_ -= falling_speed_;
        if (y_ < falling_target_y_) {
            y_ = falling_target_y_;
            falling_speed_ = 0;
            falling_acceleration_ = 0;
            falling_target_y_ = 0;
        }
    }
}

void Gem::UpdateSwaping() {
    // swaping
    if (is_swaping()) {
        swaping_timer_ -= swaping_speed_;

        if (swaping_timer_ <= 0) {
            x_ = swaping_target_x_;
            y_ = swaping_target_y_;
            z_ = 0;

            swaping_timer_ = 0;
            swaping_speed_ = 0;
            swaping_start_x_ = 0;
            swaping_start_y_ = 0;
            swaping_target_x_ = 0;
            swaping_target_y_ = 0;
        } else {
            const int& ed_x = swaping_start_x_;
            const int& ed_y = swaping_start_y_;
            const int& st_x = swaping_target_x_;
            const int& st_y = swaping_target_y_;
            const int st_z = 0;
            const int ed_z = 0;
            const float& theta = swaping_timer_ / 180.0 * 3.1415926;

            if (st_y == ed_y) {  // alone Ox
                float r = (st_x - ed_x) / 2.f;
                x_ = (st_x + ed_x) / 2.f + r * std::cos(theta);
                y_ = st_y;
                z_ = (st_z + ed_z) / 2.f + r * std::sin(theta);
            } else if (st_x == ed_x) {  // alone Oy
                float r = (st_y - ed_y) / 2.f;
                x_ = st_x;
                y_ = (st_y + ed_y) / 2.f + r * std::cos(theta);
                z_ = (st_z + ed_z) / 2.f + r * std::sin(theta);
            } else {
                assert(false);
            }
        }
    }
}

void Gem::UpdataRemoving() {
    if (is_removing()) {
        removing_speed_ += removing_acceleration_;
         //std::cerr << "removing: " << z_ << std::endl;
        z_ += removing_speed_;
        if (z_ < kRemovingEndZ) {
            set_active(false);
            removing_acceleration_ = 0;
        }
    }
}

}  // namespace Hypercube
