# OpenCV template	

## CMakeList

```cmake
############################################################################################
##                                          常用宏
##                                      ${PROJECT_NAME} 
##										${PROJECT_SOURCE_DIR} 本文件地址
##										${PROJECT_BINARY_DIR})  编译地址
##											输出信息
##										message(STATUS "xxx ${xxx}")
############################################################################################
cmake_minimum_required(VERSION 3.0.2)
##需和xml文件中的name一致
project(mypack)
#set( CMAKE_BUILD_TYPE "Release" )  #release模式
#set( CMAKE_CXX_FLAGS "-std=c++11 -O3" ) #c++11   o3优化


############################################################################################
##                                      non-ros packages:具体路径宏可以查询<库名Config.cmake>查看定义
## <name>_FOUND
## <name>_INCLUDE_DIRS  /  <name>_DIRS
## <name>_LIBRARIES  /  <name>_LIBS  /  <name>_LIBRARY
############################################################################################
##                          当前目录下放的头文件
#####################################################
#添加头文件目录 
include_directories(''./include'')  #相对于CMakeLists.txt
#添加库文件目录  
link_directories(" ./lib")  #相对于执行CMake的目录或路径一般是在
#添加所有源文件名到SRC_DIRS中
aux_source_directory(./src  SRCS_FILES)
#EXECUTABLE_OUTPUT_PATH(./bin)
#LIBRARY_OUTPUT_PATH(./lib)



##                          Eigen
#find_package(Eigen3  REQUIRED)
#include_directories(${Eigen3_INCLUDE_DIRS})
#list(APPEND ALL_LIBS ${Eigen3_LIBRARIES})

##                          Boost
#find_package(Boost REQUIRED COMPONENTS
# system  # format 
#)
#include_directories(${Boost_INCLUDE_DIRS})
#list(APPEND ALL_LIBS ${Boost_LIBRARIES})

##                          OpenCV
find_package(OpenCV  REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
list(APPEND ALL_LIBS ${OpenCV_LIBRARIES})

##                          Sophus
#find_package(Sophus REQUIRED) 
#include_directories(${Sophus_INCLUDE_DIRS}) #link  ${Sophus_LIBRARIES}
#list(APPEND ALL_LIBS ${Sophus_LIBRARIES})

##							Ceres
#find_package( Ceres REQUIRED )
#include_directories( ${CERES_INCLUDE_DIRS} ) #link  ${CERES_LIBRARIES}
#list(APPEND ALL_LIBS ${CERES_LIBRARIES})

##                         G2O
#find_package(G2O REQUIRED)
#include_directories(${G2O_INCLUDE_DIRS}) 
#list(APPEND ALL_LIBS ${G2O_LIBRARIES})

##                         glog
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/cmake)
#find_package(Glog REQUIRED)
#include_directories(${GLOG_INCLUDE_DIRS}) #link   ${GLOG_LIBRARIES}
#list(APPEND ALL_LIBS ${GLOG_LIBRARIES})

##                         gtest
#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS}) #link  ${GTEST_BOTH_LIBRARIES}
#list(APPEND ALL_LIBS ${GTEST_LIBRARIES})

##                         gflags
#find_package(GFlags REQUIRED)
#include_directories(${GFLAGS_INCLUDE_DIRS}) #link  ${GFLAGS_LIBRARIES}
#list(APPEND ALL_LIBS ${GFLAGS_LIBRARIES})

############################################################################################
##                                       build
############################################################################################
##                     generate libs like .s .so
#####################################################
MESSAGE(STATUS "This is BINARY dir " ${HELLO_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir " ${HELLO_SOURCE_DIR})
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/mypackage.cpp
# )
##库内有用到自定义消息类型时启用,否则无法生成消息头文件
#add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

#####################################################
##                          node
#####################################################
#add_executable(${PROJECT_NAME}_node src/main.cpp)
###可执行文件内有用到自定义消息类型时启用,否则无法生成消息头文件
##add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
#target_link_libraries(${PROJECT_NAME}_node
#        ${catkin_LIBRARIES}
#        )

#####################################################
##                         non-node
#####################################################
add_executable(${PROJECT_NAME}_node ${SRCS_FILES})
target_link_libraries(${PROJECT_NAME}_node
        ${ALL_LIBS}
        )

```

## note

* 图像原点在左上角、→x、 ↓y、从相机看物体方向(图像往内)为z。
* GPU加速https://blog.csdn.net/kelvin_yan/article/details/41804357

* clone是全部克隆过去，有点像引用。而copyto是复制过去，只复制roi。但是copy后的原图消失，目标图像不会变化，而clone也会消失。





## Core-Code

```cpp
#include <opencv2/opencv.hpp>
using namespace cv;
int main(int argc, char **argv)
{
    VideoCapture capture;
    capture.open(0);
    Mat frame;
    while(capture.isOpened())
    {
        capture.read(frame);
        imshow("origin",frame);
        if(waitKey(1)=='q')
            break;
    }
    return 0;
}
```

### Mat

```c++
Mat a = Mat_<double>(3,2)<< 1,2,3,4,5,6;

hconcat(A, B, C);//把B合并在A的右边，把结果保留在C中。
```



### 矩阵拷贝

```c++
Mat A=B
Mat A=B(Rect(1,2,3,4));
//赋值和构造函数出来的都是共享内存的。

//而一下两者是重开内存。
roi=B.clone() //但是有差别，clone无论roi是否已经开过内存了，直接指向新开的clone内存。
B.copyTo(roi) //如果roi共享了别的图像roi内存，则不再分配新内存，直接修改原图roi位置。

//比如一下操作就是真正的roi操作
Mat roi=inputImage(Rect(Point(50,50),Point(100,100)));
Mat blue=Mat::zeros(50,50,CV_8UC(3));
blue.copyTo(roi);
//会把inputimage中roi对应位置涂成黑色。
    
//惊奇的发现std::vector 的.pushback()是调用构造函数的。而Mat的构造函数是共享数据内存，新建Mat头部而已。
        vector<Mat> rois;
        rois.push_back(inputImage(Rect(50,50,50,50)));
        rois[0].setTo(Scalar(255,0,0));
//结果也可以吧inputImage的5050区域涂蓝色
```

![这里写图片描述](https://img-blog.csdn.net/20170228174132012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhaXBwMDYwNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里就可以看出，共享内存的构造和复制都是很快的。而copyto非常慢，因为需要重写大块内存。

### roi

```
img2=img(Rect(sx, sy, width, height));
img2=img(Rect(Point(lu.x,lu.y),Point(rb.x,rb.y)));
img3=img(Range(100, 200), Range(100,200));
```





### threshold

动态阈值？大津法？

```cpp
	cv::threshold(grayImage, grayImage, NULL, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

```

### contours/rectangle detect

```c++
		vector<vector<cv::Point>> contours;
		vector<cv::Point> maxAreaContour;
		
		cv::findContours(grayImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		//cv::drawContours(frame, contours, -1, cv::Scalar(0, 0, 255), 2, 8);
		
		// 提取面积最大轮廓
		double maxArea = 0;
		for (size_t i = 0; i < contours.size(); i++) {
			double area = fabs(cv::contourArea(contours[i]));			
			if (area > maxArea) {
				maxArea = area;
				maxAreaContour = contours[i];
			}
		}
		// 轮廓外包正矩形
		cv::Rect rect = cv::boundingRect(maxAreaContour);
		cv::rectangle(frame, cv::Point(rect.x, rect.y), cv::Point(rect.x + rect.width, rect.y + rect.height), cv::Scalar(255, 0, 0), 2, 8)
```

### put text

```c++
cv::putText(frame, disH, cv::Point(5, 40), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0, 255, 0), 1, 8);
```





## ORB feature detect

```cpp
keypoints_2[good_matches[i].trainIdx].pt //访问第二帧中匹配序列的第i个特征点的像素坐标(u,v)
```

其中匹配器 中存放匹配对DMatch。DMatch包含该匹配对在第一图和第二图关键点集合中的索引号queryIdx与trainIdx。

然后KeyPoint集合中索引这个值得到该匹配对对应的关键点。然后用KeyPoint的属性pt得到关键点的像素点坐标。

```c++
#include <opencv2/opencv.hpp>
#include  <iostream>
using namespace cv;
using namespace std;

void detectORB(Mat &im1,Mat &im2,
               std::vector<KeyPoint>&keypoints_1,
               std::vector<KeyPoint>&keypoints_2,
               vector<DMatch>& good_matches){
    Mat descriptors_1,descriptors_2;
    Ptr<ORB> orb = ORB::create(500,1.2f);//8,31,0,2,ORB::HARRIS_SCORE,31,20
    orb->detect(im1,keypoints_1);
    orb->detect(im2,keypoints_2);

    orb->compute(im1,keypoints_1,descriptors_1);
    orb->compute(im2,keypoints_2,descriptors_2);

    Mat output1;
    drawKeypoints(im1,keypoints_1,output1);
    imshow("orb feature detect",output1);
    std::vector<DMatch> matches;
    BFMatcher matcher(NORM_HAMMING);//BruteForceMatcher 暴力匹配
    matcher.match(descriptors_1,descriptors_2,matches);

    double min_dist=INT_MAX,max_dist=0;
    for(int i=0;i<descriptors_1.rows;i++) {
        double distance = matches[i].distance;
        if (distance < min_dist) min_dist = distance;
        if (distance > max_dist) max_dist = distance;
    }
    cout<<"max dist:"<<max_dist<<endl;
    cout<<"min dist:"<<min_dist<<endl;

    double threshold = 30;//min_dist + 0.15*(max_dist-min_dist);
    for(int i=0;i<descriptors_1.rows;i++)
    {
        if(matches[i].distance<=threshold)
        {
            good_matches.push_back(matches[i]);
        }
    }
    cout<<"good matches:"<<good_matches.size()<<"pairs"<<endl;
    Mat img_match;
    drawMatches(im1,keypoints_1,im2,keypoints_2,good_matches,img_match);
    imshow("筛选后匹配结果",img_match);
}

int main(int argc, char **argv)
{
    Mat im1=imread("../../doc/1.png"),im2=imread("../../doc/2.png");
    std::vector<KeyPoint> keypoints_1,keypoints_2;
    vector<DMatch> good_matches;
    detectORB(im1,im2,keypoints_1,keypoints_2,good_matches);


    while (1)
    {

        waitKey(1);
    }
    return 0;
}

```

## essential/fundamental/homography_matrix

可以参考https://blog.csdn.net/xiaoxiaowenqiang/article/details/79278884

```c++
double internal_parameter[]=
        {520.9,		0,			325.1,
          		0,		 521.0,	 247.7,
          		0,		 0,		 	 1};
```



根据匹配点对，计算基础矩阵、本质矩阵、单应矩阵并从本质矩阵中恢复R和t。

```c++
void pose_estimation_2d2d(
        std::vector<KeyPoint>&keypoints_1,
        std::vector<KeyPoint>&keypoints_2,
        vector<DMatch>& matches,
        Mat&R,Mat&t
        ){
    vector<Point2f> points1,points2;
    for (int i = 0; i < matches.size(); ++i) {
        points1.push_back(keypoints_1[matches[i].queryIdx].pt);
        points2.push_back(keypoints_2[matches[i].trainIdx].pt);
    }
    //入口参数差不多都是  按匹配对pairs序号存放在2f点集point1和point2中。
    Mat fundamental_matrix;
    fundamental_matrix=findFundamentalMat(points1,points2,CV_FM_8POINT);
    cout<<"fundamental matrix is:"<<endl<<fundamental_matrix<<endl;

    Point2d  principal_point(325.1,249.7);//光心xy	
    int focal_length=521;//焦距
    Mat essential_matrix;
    essential_matrix=findEssentialMat(points1,points2,focal_length,principal_point,RANSAC);
    cout<<"essential matrix is:"<<endl<<essential_matrix<<endl;

    Mat homography_matrix;
    //这里只是计算，没有正确结果，因为单应矩阵要求points1和points2应该在同一平面上，这里特征提取无法保证这点。
    homography_matrix=findHomography(points1,points2,RANSAC,3,noArray(),2000,0.99);
    //可以配合warpPerspective 矫正投影使用。
    cout<<"homography matrix is:"<<endl<<homography_matrix<<endl;
    
    //求解出来的[R;T]是{T_21} 即x_2 = T_21 · x_1
    recoverPose(essential_matrix,points1,points2,R,t,focal_length,principal_point);
    cout<<"R is:"<<endl<<R<<endl;
    cout<<"t is:"<<endl<<t<<endl;
}
```



## triangulation

```cpp
void triangulation(
        std::vector<KeyPoint>&keypoints_1,
        std::vector<KeyPoint>&keypoints_2,
        vector<DMatch>& matches,
        Mat&R,Mat&t,
        vector<Point3d> &points
        ){
    Mat T1 = (Mat_<float> (3,4) <<
                                1,0,0,0,
            0,1,0,0,
            0,0,1,0);
    Mat T2 = (Mat_<float> (3,4) <<
                                R.at<double>(0,0), R.at<double>(0,1), R.at<double>(0,2), t.at<double>(0,0),
            R.at<double>(1,0), R.at<double>(1,1), R.at<double>(1,2), t.at<double>(1,0),
            R.at<double>(2,0), R.at<double>(2,1), R.at<double>(2,2), t.at<double>(2,0)
    );
    Mat K = ( Mat_<double> ( 3,3 ) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1 );
    vector<Point2f> pts_1, pts_2;
    for ( DMatch m:matches )
    {
        // 将像素坐标转换至相机坐标
        pts_1.push_back ( pixel2cam( keypoints_1[m.queryIdx].pt, K) );
        pts_2.push_back ( pixel2cam( keypoints_2[m.trainIdx].pt, K) );
    }

    Mat pts_4d;
    //参数1,2：第一帧相机在世界坐标姿态，第二帧相机世界坐标系姿态， [R|t^T]
    //参数3,4：第一帧像素点归一化坐标集，第二帧对应像素点归一化坐标集，
    //输出像素点集合在空间坐标系下的坐标
    //若第一帧世界坐标系定为I,则认为是第一帧相机坐标系下。输出的点空间坐标也是相对于第一帧的。
    //注意需要传入的数据类型要是基于float的，不能是double的
    cv::triangulatePoints( T1, T2, pts_1, pts_2, pts_4d );

    // 转换成非齐次坐标
    for ( int i=0; i<pts_4d.cols; i++ )
    {
        Mat x = pts_4d.col(i);
        x /= x.at<float>(3,0); // 归一化
        Point3d p (
                x.at<float>(0,0),
                x.at<float>(1,0),
                x.at<float>(2,0)
        );
        points.push_back( p );
    }
}
```



## 帧数计算

```c++
#include <chrono>

chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
//doing somethings
chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>( t2-t1 );
cout<<"solve time cost = "<<time_used.count()<<" seconds. "<<endl
```





