#include "camera.h"

using namespace std;

Camera::Camera(int camera_index):camera_index(camera_index){
 
}

Camera::~Camera(){
    close_camera();
}

bool Camera::open_camera(){

    cap.open(camera_index);

    if (!cap.isOpened()) {
        cerr << "Error opening Camera!" << endl;
        return false;
    }
    return true;
}

void Camera::close_camera(){

    if (cap.isOpened()) 
    {
        cap.release();
    }
}

bool Camera::is_opened(){

    return cap.isOpened();
}

bool Camera::capture_image(cv::Mat& frame){

    if (!cap.isOpened()) 
    {
        cerr << "Camera isnt opened!" << endl;
        return false;
    }
    
    cap >> frame;
    
    if (frame.empty()) 
    {
        cerr << "Error capturing image!" << endl;
        return false;
    }
    return true;
}

bool Camera::save_image(const string& image_path, const cv::Mat& frame){

    if (frame.empty()) {
        cerr << "No Image!" << endl;
        return false;
    }
    if (!cv::imwrite(image_path, frame)) {
        cerr << "Error Saving Image!" << endl;
        return false;
    }
    return true;

}