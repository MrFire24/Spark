#pragma once
#include <chrono>
#include <list>

extern std::chrono::high_resolution_clock timer;

class DeltaTime{
public:
    void updateTime();
    operator float() const;
    float fps();

    DeltaTime();
private:
    float value_ = 0.1f, fps_ = 0;
    std::chrono::steady_clock::time_point oldTime_, newTime_;
    std::list<float> fpsList_ = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f};
};

extern DeltaTime deltaTime;