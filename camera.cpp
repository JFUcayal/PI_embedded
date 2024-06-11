#include "camera.h"

#define CAMERA_DIR "/dev/video2"
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
    else 
    {      
        //cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        //cap.set(cv::CAP_PROP_FRAME_HEIGHT,640);
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