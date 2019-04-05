// Copyright (C) 2018 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <string>
#include <vector>
#include <map>

#include <inference_engine.hpp>
#include <opencv2/core/core.hpp>

#include "face_inference_results.hpp"
#include "ie_wrapper.hpp"

namespace gaze_estimation {
class FaceDetector {
public:
    FaceDetector(const std::string& modelPath,
                 InferenceEngine::InferencePlugin& devicePlugin,
                 double detectionConfidenceThreshold,
                 bool enableReshape);
    std::vector<FaceInferenceResults> detect(const cv::Mat& image);
    void printPerformanceCounts() const;
    ~FaceDetector();

private:
    IEWrapper ieWrapper;
    double detectionThreshold;
    bool enableReshape;

    void adjustBoundingBox(cv::Rect& boundingBox) const;
};
}  // namespace gaze_estimation
