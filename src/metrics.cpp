#include "../includes/metrics.h"

#include <cstdio>

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
    return "FPS: " + std::to_string(getFPS()) +
           " | Frame: " + std::to_string(frameTime) +
           " ms | Avg: " + std::to_string(avgFrameTime) + " ms";
}