#ifndef CAMERA_H_
#define CAMERA_H_

#include <opencv2/opencv.hpp>
#include "logger.h"

#define CAMERA_INDEX 0 // /dev/video0

using namespace std;

class Camera{

    private:
        int camera_index;
        Logger camera_logger;

    public:
        Camera(int camera_index = CAMERA_INDEX);
        ~Camera();
        bool open_camera();
        void close_camera();
        bool is_opened();
        bool capture_image(cv::Mat& frame);
        bool save_image(const string& image_path, const cv::Mat& frame);
};

#endif