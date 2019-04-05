// Copyright (C) 2018 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <cstdio>
#include <string>

#include <map>
#include <vector>

#include <inference_engine.hpp>


#include <ie_iextension.h>
#include <ext_list.hpp>

#include <samples/ocv_common.hpp>
#include <samples/slog.hpp>
#include <samples/common.hpp>

namespace gaze_estimation {
class IEWrapper {
public:
    IEWrapper(const std::string& modelPath,
                  InferenceEngine::InferencePlugin& devicePlugin);
    // For setting input blobs containing images
    void setInputBlob(const std::string& blobName, const cv::Mat& image);
    // For setting input blobs containing vectors of data
    void setInputBlob(const std::string& blobName, const std::vector<float>& data);

    // Version if there are more than one output blob
    void getOutputBlob(const std::string& blobName, std::vector<float>& output);
    // Version if there is one output blob
    void getOutputBlob(std::vector<float>& output);

    void printPerlayerPerformance() const;

    const std::map<std::string, std::vector<unsigned long>>& getIputBlobDimsInfo() const;
    const std::map<std::string, std::vector<unsigned long>>& getOutputBlobDimsInfo() const;

    void reshape(const std::map<std::string, std::vector<unsigned long>>& newBlobsDimsInfo);

    void infer();

private:
    InferenceEngine::InferencePlugin& plugin;
    std::string modelPath;
    InferenceEngine::CNNNetReader netReader;
    InferenceEngine::CNNNetwork network;
    InferenceEngine::ExecutableNetwork executableNetwork;
    InferenceEngine::InferRequest request;
    std::map<std::string, std::vector<unsigned long>> inputBlobsDimsInfo;
    std::map<std::string, std::vector<unsigned long>> outputBlobsDimsInfo;

    void setExecPart();
};
}  // namespace gaze_estimation
