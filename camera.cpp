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
    else 
    {
        //cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        //cap.set(cv::CAP_PROP_FRAME_HEIGHT,480);
        //cap.set(cv::CAP_PROP_FPS, 30);
        //cap.set(cv::CAP_PROP_BRIGHTNESS, 100); 
        //cap.set(cv::CAP_PROP_CONTRAST, 0);   
        //cap.set(cv::CAP_PROP_SATURATION, 70);
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

    if (frame.empty()) 
    {
        cerr << "No Image!" << endl;
        return false;
    }

    if (!cv::imwrite(image_path, frame)) 
    {
        cerr << "Error Saving Image!" << endl;
        return false;
    }
    return true;

}