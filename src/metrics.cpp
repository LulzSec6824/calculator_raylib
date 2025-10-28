#include "../includes/metrics.h"

#include <cstdio>

void PerformanceMetrics::startFrame() { frameStart = std::chrono::high_resolution_clock::now(); }

void PerformanceMetrics::endFrame() {
    auto frameEnd = std::chrono::high_resolution_clock::now();
    frameTime     = std::chrono::duration<double, std::milli>(frameEnd - frameStart).count();

    // Update running average
    frameCount++;
    avgFrameTime = avgFrameTime + (frameTime - avgFrameTime) / frameCount;
}

int PerformanceMetrics::getFPS() const { return GetFPS(); }

double PerformanceMetrics::getFrameTime() const { return frameTime; }

double PerformanceMetrics::getAvgFrameTime() const { return avgFrameTime; }

std::string PerformanceMetrics::getPerformanceInfo() const {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "FPS: %d | Frame: %.2f ms | Avg: %.2f ms", getFPS(), frameTime, avgFrameTime);
    return buffer;
}