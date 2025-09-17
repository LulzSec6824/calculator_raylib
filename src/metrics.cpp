#include "../includes/metrics.h"

#include <cstdio>

void PerformanceMetrics::startFrame() {
    frameStart = std::chrono::high_resolution_clock::now();
}

void PerformanceMetrics::endFrame() {
    const auto frameEnd = std::chrono::high_resolution_clock::now();
    frameTime = std::chrono::duration<double, std::milli>(frameEnd - frameStart)
                    .count();

    // Update running average with more efficient calculation
    ++frameCount;
    constexpr double smoothingFactor = 0.1;  // Use exponential moving average
    avgFrameTime = avgFrameTime + smoothingFactor * (frameTime - avgFrameTime);
}

int PerformanceMetrics::getFPS() const { return GetFPS(); }

double PerformanceMetrics::getFrameTime() const { return frameTime; }

double PerformanceMetrics::getAvgFrameTime() const { return avgFrameTime; }

std::string PerformanceMetrics::getPerformanceInfo() const {
    constexpr size_t bufferSize = 128;
    char buffer[bufferSize];
    std::snprintf(buffer, bufferSize, "FPS: %d | Frame: %.2f ms | Avg: %.2f ms",
                  getFPS(), frameTime, avgFrameTime);
    return std::string(buffer);
}