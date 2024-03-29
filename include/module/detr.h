#pragma once
#include "module/module.h"
#include "functional/functional.h"
#include "functional/detection.h"

class Detr :
    public Module
{
public:
    Detr(int w = 224, int h = 224,
        int max_w = 800, int max_h = 800,
        const std::vector<float>& mean = { 0.485, 0.456, 0.406 },
        const std::vector<float>& std = { 0.229, 0.224, 0.225 },
        bool fixed_input = true, 
        float nms_thresh = 0.5, float confidence_thresh = 0.25);

    std::vector<cv::Mat> preprocess(cv::Mat &mat, transforms::DataFormat data_format);
    std::vector<Ort::Value> forward(std::string& img_path, Ort::MemoryInfo& mem_info, Ort::RunOptions&,
        transforms::DataFormat data_format = transforms::DataFormat::CHW);
    std::vector<Ort::Value> forward(cv::Mat& mat, Ort::MemoryInfo& mem_info, Ort::RunOptions&,
        transforms::DataFormat data_format = transforms::DataFormat::CHW);
    std::vector<Ort::Value> forward(std::vector<cv::Mat>& input_cv, Ort::MemoryInfo& mem_info, Ort::RunOptions& run_info,
        transforms::DataFormat data_format = transforms::DataFormat::CHW);
    std::vector<Box>  postprocess(std::vector<Ort::Value>&);
    void adjust_par(int h = 640, int w = 640, float nms_thresh = 0.45, float confidence_thresh = 0.2);

    cv::Mat call(cv::Mat& img, Ort::MemoryInfo& mem_info, Ort::RunOptions& run_options,
        transforms::DataFormat data_format = transforms::DataFormat::CHW) {
        auto result = forward(img, mem_info, run_options, data_format);
        std::vector<Box> pred_classes = postprocess(result);
        img *= 255;
        img.convertTo(img, CV_8UC3);
        functional::draw_box(img, pred_classes, 80);
        return img;
    }
private:
    int h, w, max_h, max_w;
    std::vector<float> mean, std;
    bool fixed_input;
    std::vector <int64_t> shape;
    std::vector<int64_t> mask_shape;
    // 这个用于解析mat的数据
    std::vector<float> tensor_data;
    // for detect
    float ratio;
    float nms_thresh;
    float confidence_thresh;
};