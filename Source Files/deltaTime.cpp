#include "deltaTime.h"

std::chrono::high_resolution_clock timer;

DeltaTime deltaTime;

DeltaTime::DeltaTime() {
    oldTime_ = timer.now();
    newTime_ = timer.now();
}

float DeltaTime::fps() {
    return fps_;
}

void DeltaTime::updateTime() {
    oldTime_ = newTime_;
    newTime_ = newTime_ = timer.now();
    value_ = std::chrono::duration_cast<std::chrono::microseconds>(newTime_ - oldTime_).count() / 1000000.f;
    fpsList_.push_back(1. / value_);
    fpsList_.pop_front();
    for (auto i : fpsList_) {
        fps_ += i;
    }
    fps_ /= 10.f;
}

DeltaTime::operator float() const{
    return value_;
}