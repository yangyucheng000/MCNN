/*
 *   d
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ============================================================================
 */

#ifndef MCNN_H
#define MCNN_H
#include <dirent.h>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "MxBase/Log/Log.h"
#include "MxBase/DvppWrapper/DvppWrapper.h"
#include "MxBase/ModelInfer/ModelInferenceProcessor.h"
#include "MxBase/Tensor/TensorContext/TensorContext.h"
#include "MxBase/DeviceManager/DeviceManager.h"

struct InitParam {
    uint32_t deviceId;
    bool checkTensor;
    std::string modelPath;
    std::string srPath;
    std::string gtPath;
};


class Mcnn {
 public:
    APP_ERROR Init(const InitParam &initParam);
    APP_ERROR DeInit();
    APP_ERROR ReadImage(const std::string &imgPath, cv::Mat *imageMat);
    APP_ERROR PadImage(const cv::Mat &img, cv::Mat *imgPad);
    APP_ERROR CVMatToTensorBase(const cv::Mat &imageMat, MxBase::TensorBase *tensorBase);
    APP_ERROR Inference(const std::vector<MxBase::TensorBase> &inputs, std::vector<MxBase::TensorBase> *outputs);
    APP_ERROR PostProcess(std::vector<MxBase::TensorBase> outputs, const std::string &imgName);
    APP_ERROR Process(const std::string &imgPath, const std::string &imgName);
    // get infer time
    double GetInferCostMilliSec() const {return inferCostTimeMilliSec;}
    double GetPSNR() const {return psnr_;}
    float getmae() const {return mae;}
    float getmse() const {return mse;}

 private:
    std::shared_ptr<MxBase::DvppWrapper> dvppWrapper_;
    std::shared_ptr<MxBase::ModelInferenceProcessor> model_;
    std::string srPath_;
    std::string gtPath_;
    MxBase::ModelDesc modelDesc_;
    double_t psnr_ = 0;
    uint32_t deviceId_ = 0;
    // infer time
    double inferCostTimeMilliSec = 0.0;
    float *mat_data_ = new float[1048576];
    cv::Mat imgPad_ = cv::Mat(1024, 1024, CV_32FC1);
    float mae = 0.0;
    float mse = 0.0;
};


#endif
