#pragma once
#include <chrono>
#include <string>

#include "../raylib/src/raylib.h"

class PerformanceMetrics {
   private:
    double frameTime{0.0};
    int frameCount{0};
    double avgFrameTime{0.0};
    std::chrono::high_resolution_clock::time_point frameStart;

   public:
    PerformanceMetrics() = default;
    void startFrame();
    void endFrame();
    int getFPS() const;
    double getFrameTime() const;
    double getAvgFrameTime() const;
    std::string getPerformanceInfo() const;
};