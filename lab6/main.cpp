// std
#include <iostream>
#include <fstream>

// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>

// eigen
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

// utils
#include "utils.h"

// 
using namespace std;
using namespace Eigen;


int main(int argc, char **argv) {
    
    if (argc < 3) 
    {
        std::cerr << "Usage " << argv[0] << " <image_filename> <camera_params_filename>" << std::endl; 
        return 0;
    }
    
    // output image
    cv::Mat ipm_image(cv::Size(400, 400), CV_8UC3, cv::Scalar(0, 0, 0));
    
    // load image from file
    cv::Mat input;
    input = cv::imread(argv[1]);
    
    cv::namedWindow("Input", cv::WINDOW_AUTOSIZE );
    imshow("Input", input);
    cv::waitKey(10);
    
    // load camera params
    CameraParams params;
    LoadCameraParams(argv[2], params);
    
    Eigen::Matrix<float, 3, 4> K;
    K << params.ku,       0.0, params.u0, 0.0,
               0.0, params.kv, params.v0, 0.0,
               0.0,       0.0,       1.0, 0.0;
               
    Eigen::Matrix<float, 4, 4> RT;
    cv::Affine3f RT_inv = params.RT.inv();
    RT << RT_inv.matrix(0,0), RT_inv.matrix(0,1), RT_inv.matrix(0,2), RT_inv.matrix(0,3),
          RT_inv.matrix(1,0), RT_inv.matrix(1,1), RT_inv.matrix(1,2), RT_inv.matrix(1,3),
          RT_inv.matrix(2,0), RT_inv.matrix(2,1), RT_inv.matrix(2,2), RT_inv.matrix(2,3),
                           0,                  0,                  0,                  1;


    /*
     * YOUR CODE HERE: Choose a planar constraint and realize an inverse perspective mapping
     * 1) compute the perspective matrix P
     * 2) choose a planar constraint (ex. z=0) to be used during the inverse perspective mapping
     * 3) compute the inverse perspective matrix corresponding to the chosen constraint
     * 4) realize the inverse perspective mapping
     */ 
    
    //hint: partire da M = K*RT

    //considero come piano di riferimento y=0
    Eigen::Matrix<float, 3, 4> M = K * RT;

    Eigen::Matrix3f M_plane;
    M_plane.col(0) = M.col(0); //X
    M_plane.col(1) = M.col(2); //Z
    M_plane.col(2) = M.col(3); //1

    Eigen::Matrix3f M_plane_inverse = M_plane.inverse();

    const float meters_per_pixel = 0.05f;
    const float center_x = 0.0f;
    const float center_z = 0.0f;

    for (int y = 0; y < ipm_image.rows; ++y) {
        for (int x = 0; x < ipm_image.cols; ++x) {
            // Punto sul piano y = 0, in coordinate omogenee [X, Z, 1]
            Eigen::Vector3f plane_point(
                (x - ipm_image.cols / 2.0f) * meters_per_pixel,
                (ipm_image.rows - y) * meters_per_pixel,
                1.0f
            );

            // Proietta il punto del piano nell'immagine originale
            Eigen::Vector3f pixel = M_plane * plane_point;

            if (std::abs(pixel(2)) < 1e-6f) {
                continue;
            }

            int src_x = static_cast<int>(std::round(pixel(0) / pixel(2)));
            int src_y = static_cast<int>(std::round(pixel(1) / pixel(2)));

            if (src_x >= 0 && src_x < input.cols &&
                src_y >= 0 && src_y < input.rows) {
                ipm_image.at<cv::Vec3b>(y, x) =
                    input.at<cv::Vec3b>(src_y, src_x);
            }
        }
    }

    cv::namedWindow("IPM", cv::WINDOW_AUTOSIZE );
    imshow("IPM", ipm_image);
    
    std::cout << "Press any key to quit" << std::endl;
    
    cv::waitKey(0);
    
    return 0;
}
