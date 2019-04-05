// Copyright (C) 2018 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include <cstdio>
#include <string>

#include <cmath>
#include <utility>
#include <memory>
#include <map>
#include <vector>

#include "utils.hpp"

using namespace InferenceEngine;

namespace gaze_estimation {
void createPluginsPool(const std::vector<std::pair<std::string, std::string>>& cmdOptions,
                      std::map<std::string, InferencePlugin>& pluginsForDevices,
                      bool enablePCReport) {
    for (auto&& option : cmdOptions) {
        auto deviceName = option.first;
        auto networkName = option.second;

        if (deviceName == "" || networkName == "") {
            continue;
        }

        if (pluginsForDevices.find(deviceName) != pluginsForDevices.end()) {
            continue;
        }
        slog::info << "Loading plugin " << deviceName << slog::endl;
        InferencePlugin plugin = PluginDispatcher({"../../../lib/intel64", ""}).getPluginByDevice(deviceName);

        /** Printing plugin version **/
        printPluginVersion(plugin, std::cout);

        /** Loading extensions for the CPU plugin **/
        if ((deviceName.find("CPU") != std::string::npos))
            plugin.AddExtension(std::make_shared<Extensions::Cpu::CpuExtensions>());

        /** Enable per-layer performance metric is flag is set **/
        if (enablePCReport)
            plugin.SetConfig({{PluginConfigParams::KEY_PERF_COUNT, PluginConfigParams::YES}});

        pluginsForDevices[deviceName] = plugin;
    }
}

void gazeVectorToGazeAngles(const cv::Point3f& gazeVector, cv::Point2f& gazeAngles) {
    auto r = cv::norm(gazeVector);

    double v0 = static_cast<double>(gazeVector.x);
    double v1 = static_cast<double>(gazeVector.y);
    double v2 = static_cast<double>(gazeVector.z);

    gazeAngles.x = static_cast<float>(180.0 / M_PI * (M_PI_2 + std::atan2(v2, v0)));
    gazeAngles.y = static_cast<float>(180.0 / M_PI * (M_PI_2 - std::acos(v1 / r)));
}

void putTimingInfoOnFrame(cv::Mat& image, double overallTime, double inferenceTime) {
    auto frameHeight = image.rows;
    double fontScale = 1.6 * frameHeight / 640;
    auto fontColor = cv::Scalar(0, 0, 255);
    int thickness = 2;

    double overallFPS = 1000. / overallTime;
    double inferenceFPS = 1000. / inferenceTime;

    cv::putText(image,
                cv::format("Overall FPS: %0.0f, Inference FPS: %0.0f", overallFPS, inferenceFPS),
                cv::Point(10, static_cast<int>(30 * fontScale / 1.6)), cv::FONT_HERSHEY_PLAIN, fontScale, fontColor, thickness);
}
}  // namespace gaze_estimation
