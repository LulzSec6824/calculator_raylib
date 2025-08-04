#pragma once
#include <chrono>
#include <string>

#include "../raylib_v5/src/raylib.h"

class PerformanceMetrics {
   private:
    double frameTime;     // Current frame time in milliseconds
    int frameCount;       // Total number of frames processed
    double avgFrameTime;  // Running average frame time
    std::chrono::high_resolution_clock::time_point
        frameStart;  // Start time of current frame

   public:
    PerformanceMetrics();

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