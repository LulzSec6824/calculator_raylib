#include "../includes/metrics.h"

#include <cstdio>

PerformanceMetrics::PerformanceMetrics()
    : frameTime(0.0), frameCount(0), avgFrameTime(0.0) {}

void PerformanceMetrics::startFrame() {
    frameStart = std::chrono::high_resolution_clock::now();
}

void PerformanceMetrics::endFrame() {
    auto frameEnd = std::chrono::high_resolution_clock::now();
    frameTime = std::chrono::duration<double, std::milli>(frameEnd - frameStart)
                    .count();

    // Update running average
    frameCount++;
    avgFrameTime = avgFrameTime + (frameTime - avgFrameTime) / frameCount;
}

int PerformanceMetrics::getFPS() const { return GetFPS(); }

double PerformanceMetrics::getFrameTime() const { return frameTime; }

double PerformanceMetrics::getAvgFrameTime() const { return avgFrameTime; }

std::string PerformanceMetrics::getPerformanceInfo() const {
    char perfInfo[64];
    sprintf(perfInfo, "FPS: %d | Frame: %.2f ms | Avg: %.2f ms", getFPS(),
            frameTime, avgFrameTime);
    return std::string(perfInfo);
}