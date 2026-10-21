//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//std:
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include<ctime>

//options

struct ArgumentList {
  std::string image_name;                   //!< image file name
  int wait_t;                               //!< waiting time
  int top;
  int left;
  int width;
  int height;
  int random_flag=0;
};

bool ParseInputs(ArgumentList& args, int argc, char **argv);

int main(int argc, char **argv)
{
  int frame_number = 0;
  char frame_name[256];
  bool exit_loop = false;
  int imreadflags = cv::IMREAD_COLOR; 
  srand(time(NULL));

  std::cout<<"Simple program."<<std::endl;

  //////////////////////
  //parse argument list:
  //////////////////////
  ArgumentList args;
  if(!ParseInputs(args, argc, argv)) {
    exit(0);
  }

  while(!exit_loop)
  {
    //generating file name
    //
    //multi frame case
    if(args.image_name.find('%') != std::string::npos)
      sprintf(frame_name,(const char*)(args.image_name.c_str()),frame_number);
    else //single frame case
      sprintf(frame_name,"%s",args.image_name.c_str());

    //opening file
    std::cout<<"Opening "<<frame_name<<std::endl;

    cv::Mat image = cv::imread(frame_name, imreadflags);
    if(image.empty())
    {
      std::cout<<"Unable to open "<<frame_name<<std::endl;
      return 1;
    }

    std::cout << "The image has " << image.channels() << 
      " channels, the size is " << image.rows << "x" << image.cols << " pixels " <<
      " the type is " << image.type() <<
      " the pixel size is " << image.elemSize() <<
      " and each channel is " << image.elemSize1() << (image.elemSize1()>1?" bytes":" byte") << std::endl;

    //////////////////////
    //processing code here

    //Esercizio 1
    unsigned char* data = image.data;

    for(int r = 0; r < image.rows; r++)
    {
        for(int c = 0; c < image.cols; c++)
        {
            int index = (r * image.cols + c) * image.elemSize();
            std::cout << (int)data[index] << " ";
        }

        std::cout << std::endl;
    }

    //Esercizio 2
    int new_width = image.cols / 2;
    int new_height = image.rows / 2;

    cv::Mat subsampled_image = cv::Mat::zeros(new_height, new_width, image.type());

    unsigned char* new_data = subsampled_image.data;

    int channels = image.channels();

    for(int r = 0; r < new_height; r++){
        for(int c = 0; c < new_width; c++){
          int old_index = (2 * r * image.cols + 2 * c) * channels;
          int new_index = (r * new_width + c) * channels;
          
          for(int ch = 0; ch < channels; ch++){
            new_data[new_index + ch] = data[old_index + ch];
          }
        }
      }

    cv::namedWindow("subsampled image", cv::WINDOW_NORMAL);
    cv::imshow("subsampled image", subsampled_image);
    
    //Esercizio 3
    cv::Mat subsampled_rows_image = cv::Mat::zeros(new_height, image.cols, image.type());

    unsigned char* subs_row_data = subsampled_rows_image.data;

    for(int r = 0; r < new_height; r++){
      for(int c = 0; c < image.cols; c++){
        int old_index = (2 * r * image.cols + c) * channels;
        int new_index = (r * image.cols + c) * channels;

        for(int ch = 0; ch < channels; ch++){
          subs_row_data[new_index + ch] = data[old_index + ch];
        }
      }

    }

    cv::namedWindow("subsampled rows image", cv::WINDOW_NORMAL);
    cv::imshow("subsampled rows image", subsampled_rows_image);


    //Esercizio 4
    cv::Mat subsampled_cols_image = cv::Mat::zeros(image.rows, new_width, image.type());

    unsigned char* subs_col_data = subsampled_cols_image.data;

    for(int r = 0; r < image.rows; r++){
      for(int c = 0; c < new_width; c++){
        int old_index = (r * image.cols + 2 * c) * channels;
        int new_index = (r * new_width + c) * channels;

        for(int ch = 0; ch < channels; ch++){
          subs_col_data[new_index + ch] = data[old_index + ch];
        }
      }

    }

    cv::namedWindow("subsampled cols image", cv::WINDOW_NORMAL);
    cv::imshow("subsampled cols image", subsampled_cols_image);
    
    //Esercizio 5
    cv::Mat cols_flip_image = cv::Mat::zeros(image.rows, image.cols, image.type());

    unsigned char* cols_flip_data = cols_flip_image.data;

    for(int r = 0; r < image.rows; r++){
      for(int c = 0; c < image.cols; c++){
        int old_index = (r * image.cols + c) * channels;
        int new_index = (r * image.cols + image.cols - 1 - c) * channels;

        for(int ch = 0; ch < channels; ch++){
          cols_flip_data[new_index+ch] = data[old_index + ch];
        }
      }
    }

    cv::namedWindow("cols flipped image", cv::WINDOW_NORMAL);
    cv::imshow("cols flipped image", cols_flip_image);

    //Esercizio 6
    cv::Mat rows_flip_image = cv::Mat::zeros(image.rows, image.cols, image.type());

    unsigned char* rows_flip_data = rows_flip_image.data;

    for(int r = 0; r < image.rows; r++){
      for(int c = 0; c < image.cols; c++){
        int old_index = (r * image.cols + c) * channels;
        int new_index = ((image.rows - 1 - r) * image.cols + c) * channels;

        for(int ch = 0; ch < channels; ch++){
          rows_flip_data[new_index + ch] = data[old_index+ch];
        }
      }
    }

    cv::namedWindow("rows flipped image", cv::WINDOW_NORMAL);
    cv::imshow("rows flipped image", rows_flip_image);

    //Esercizio 7
    if((image.cols < (args.left + args.width)) || (image.rows < (args.top + args.height))){
      std::cerr << "Error: cropping parameters are too big for image.\n";
      return -1;
    }

    if((args.left != 0) && (args.top != 0) && (args.width != 0) && (args.height != 0)){
      cv::Mat cropped_image = cv::Mat::zeros(args.height, args.width, image.type());

      unsigned char* cropped_image_data = cropped_image.data;

      for(int r = 0; r < args.height; r++){
        for(int c = 0; c < args.width; c++){
          int old_index = ((args.top + r) * image.cols + (args.left + c)) * channels;
          int new_index = (r * args.width + c) * channels;

          for(int ch = 0; ch < channels; ch++){
            cropped_image_data[new_index + ch] = data[old_index+ch];
          }
        }
      }

      cv::namedWindow("cropped image", cv::WINDOW_NORMAL); // cv::WINDOW_AUTOSIZE if you want the window to adapt to image size
      cv::imshow("cropped image", cropped_image);
    }
    

    //Esercizio 7bis
    if(args.random_flag){
      int top = rand() % image.rows;
      int left = rand() % image.cols;
      int width = rand() % (image.cols - left) + 1;
      int height = rand() % (image.rows - top) + 1;
      std::cout << "Random values: " << top << "," << left << "," << width << "," << height;
      cv::Mat random_cropped_image = cv::Mat::zeros(height, width, image.type());

      unsigned char* random_cropped_image_data = random_cropped_image.data;

      for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
          int old_index = ((top + r) * image.cols + (left + c)) * channels;
          int new_index = (r * width + c) * channels;

          for(int ch = 0; ch < channels; ch++){
            random_cropped_image_data[new_index + ch] = data[old_index+ch];
          }
        }
      }
      cv::namedWindow("random cropped image", cv::WINDOW_NORMAL); // cv::WINDOW_AUTOSIZE if you want the window to adapt to image size
      cv::imshow("random cropped image", random_cropped_image);
    }
    
    

    //Esercizio 8
    int padding_size = 50;
    cv::Mat padding_image = cv::Mat::zeros((image.rows+padding_size),(image.cols+padding_size), image.type());

    unsigned char* padding_image_data = padding_image.data;


    for(int r = 0; r < image.rows; r++){
      for(int c = 0; c < image.cols; c++){
        int old_index = (r * image.cols + c) * channels;
        int new_index = ((padding_size + r) * image.cols + (padding_size + c)) * channels;

        for(int ch = 0; ch < channels; ch++){
          padding_image_data[new_index + ch] = data[old_index+ch];
        }
      }
    }

    cv::namedWindow("padding image", cv::WINDOW_NORMAL); // cv::WINDOW_AUTOSIZE if you want the window to adapt to image size
    cv::imshow("padding image", padding_image);

    //esercizio 9

    //esercizio 10
    cv::Mat channel_shuffle_image = cv::Mat::zeros(image.rows, image.cols, image.type());

    unsigned char* channel_shuffle_data = channel_shuffle_image.data;

    for(int r = 0; r < image.rows; r++){
        for(int c = 0; c < image.cols; c++){
          int old_index = (r * image.cols + c) * channels;
          int new_index = (r * image.cols + c) * channels;

          new_data[new_index] = data[old_index + 2]; //B -> R
          new_data[new_index + 1] = data[old_index]; //G -> B
          new_data[new_index + 2] = data[old_index + 1]; //R -> G
          
        }
      }

    cv::namedWindow("channel shuffle image", cv::WINDOW_NORMAL);
    cv::imshow("channel shuffle image", channel_shuffle_image);

    

    /////////////////////

    //display image
    cv::namedWindow("original image", cv::WINDOW_NORMAL); // cv::WINDOW_AUTOSIZE if you want the window to adapt to image size
    cv::imshow("original image", image);

    //wait for key or timeout
    unsigned char key = cv::waitKey(args.wait_t);
    std::cout<<"key "<<int(key)<<std::endl;

    //here you can implement some looping logic using key value:
    // - pause
    // - stop
    // - step back
    // - step forward
    // - loop on the same frame

    switch(key)
    {
      case 'p':
	std::cout << "Mat = "<< std::endl << image << std::endl;
	break;
      case 'q':
	exit_loop = true;
	break;
      case 'a':
	std::cout << "SET UNCHANGED imread()" << std::endl;
	imreadflags = cv::IMREAD_UNCHANGED; // in such a case if  we have an alpha channel, we obtain 4 channels!
	break;
      case 'c':
	std::cout << "SET COLOR imread()" << std::endl;
	imreadflags = cv::IMREAD_COLOR;  // read RGB or, rather, BGR image
	break;
      case 'g':
	std::cout << "SET GREY  imread()" << std::endl;
	imreadflags = cv::IMREAD_GRAYSCALE; // Y = 0.299 R + 0.587 G + 0.114 B
	break;
    }

    frame_number++;
  }

  return 0;
}


bool ParseInputs(ArgumentList& args, int argc, char **argv);

#if NATIVE_OPTS
// cumbersome, it requires to use "=" for args, i.e. -i=../images/Lenna.pgm
bool ParseInputs(ArgumentList& args, int argc, char **argv) {
  args.wait_t=0;

  cv::CommandLineParser parser(argc, argv,
      "{input   i|../images/Lenna.png|input image, Use %0xd format for multiple images.}"
      "{wait    t|0                  |wait before next frame (ms)}"
      "{help    h|<none>             |produce help message}"
      );

  if(parser.has("help"))
  {
    parser.printMessage();
    return false;
  }

  args.image_name = parser.get<std::string>("input");
  args.wait_t     = parser.get<int>("wait");

  return true;
}

#else

#include <unistd.h>
bool ParseInputs(ArgumentList& args, int argc, char **argv) {
  int c;
  args.wait_t = 0;

  while ((c = getopt (argc, argv, "hi:t:c:r")) != -1)
    switch (c)
    {
      case 't':
        args.wait_t = atoi(optarg);
        break;
      case 'c':
        args.top = atoi(optarg);
        args.left = atoi(argv[optind++]);
        args.width = atoi(argv[optind++]);
        args.height = atoi(argv[optind++]);
        break;
      /*  
      case 'r':
        args.random_flag = 1;
        break;
        */
      case 'i':
        args.image_name = optarg;
        break;
      case 'h':
      default:
        std::cout<<"usage: " << argv[0] << " -i <image_name>"<<std::endl;
        std::cout<<"exit:  type q"<<std::endl<<std::endl;
        std::cout<<"Allowed options:"<<std::endl<<
          "   -h                       produce help message"<<std::endl<<
          "   -i arg                   image name. Use %0xd format for multiple images."<<std::endl<<
          "   -t arg                   wait before next frame (ms)"<<std::endl<<std::endl<<
          "   -c <top> <left> <width> <height> crop image" <<std::endl<<std::endl<<
          "   -r (use with -i flag)    crop random area of the image"<<std::endl<<std::endl;
        return false;
    }
  return true;
}

#endif


