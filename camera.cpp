#include "camera.h"

//#define CAMERA_DIR "/dev/video0"
#define CAMERA_DIR "v4l2src device=/dev/video0 ! video/x-raw ! videoconvert ! appsink"
#define IMAGE_HEIGHT 640
#define IMAGE_WIDTH 640

using namespace std;

static cv::VideoCapture cap(CAMERA_DIR);


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

    if (frame.empty()) 
    {
        cerr << "No Image!" << endl;
        return false;
    }

    //Resizing image to the desired format
    cv::Mat resized_img;
    cv::Size new_size(IMAGE_WIDTH, IMAGE_HEIGHT);

    cv::resize(frame, resized_img, new_size);

    if (!cv::imwrite(image_path, resized_img)) 
    {
        cerr << "Error Saving Image!" << endl;
        return false;
    }
    return true;

}