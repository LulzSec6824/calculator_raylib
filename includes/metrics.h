#pragma once
#include <chrono>
#include <string>

#include "../raylib/src/raylib.h"

class PerformanceMetrics {
   private:
    double frameTime{0.0};                                      // Current frame time in milliseconds
    int frameCount{0};                                          // Total number of frames processed
    double avgFrameTime{0.0};                                   // Running average frame time
    std::chrono::high_resolution_clock::time_point frameStart;  // Start time of current frame

   public:
    PerformanceMetrics() = default;

    // Start timing a new frame
    void startFrame();

    // End timing the current frame and update metrics
    void endFrame();

    // Get current FPS from raylib
    int getFPS() const;

    // Get current frame time in milliseconds
    double getFrameTime() const;

    // Get average frame time in milliseconds
    double getAvgFrameTime() const;

    // Get formatted performance info string
    std::string getPerformanceInfo() const;
};