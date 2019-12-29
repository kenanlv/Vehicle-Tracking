# Vehicle-Tracking

Object recognizing and tracking play a very crucial role in our daily life. Object tracking enables us to count and label each tracked object. Our goal is to detect, label, and track all individual vehicles in a given video.
 
<p align="center">
       <img align="center" height=350 src="https://github.com/kenanlv/Vehicle-Tracking/blob/master/imgs/demo.gif">
<p/>


## Installation

**Building the project using CMake from the command-line:**

_Linux:_

    export OpenCV_DIR="~/OpenCV/build"
    mkdir build
    cd build
    cmake -D OpenCV_DIR=$OpenCV_DIR ..
    make 

_MacOSX (Xcode):_

    export OpenCV_DIR="~/OpenCV/build"
    mkdir build
    cd build
    cmake -G Xcode -D OpenCV_DIR=$OpenCV_DIR ..    

_Windows (MS Visual Studio):_

    set OpenCV_DIR="C:\OpenCV\build"
    mkdir build
    cd build
    cmake -G "Visual Studio 12 2019" -D OpenCV_DIR=%OpenCV_DIR% ..  


## Methods

> Overall we want to extract feature points for each object from the background and keep tracking them (we used **canny edge detection** and **optical flow**) but we need to figure out a way to merge and separate blobs for identifying if the feature belongs to the same or different objects (by **clustering** the blobs using the relative translations and directions). 
>
>_The following picture is a flow chart representation of our logic._


<p align="center">
<a  href="https://www.vrpcommute.tk">
       <img align="center" height=600 src="https://github.com/kenanlv/Vehicle-Tracking/blob/master/imgs/flowChart.JPG">
   </a>
 <p/>

### Canny Edge Detection

> Canny Edge Detection is one way for us to remove the background and extract objects/features from each individual frame. 
>
> There are multiple stages in the Canny Edge Detection: noise reduction, finding intensity gradient of the image, non-maximum suppression, and Hysteresis thresholding. 

### Optical Flow with Corner Detection

> Optical flow or optic flow is the pattern of apparent motion of objects, surfaces, and edges in a visual scene caused by the relative motion between an observer and a scene. 
> 
> By finding the strong corner points and comparing with the consecutive frame, we could sift out unwanted points and prevent us from missing potential candidates.

## Results

### Canny Edge Detection
    
<p align="center">
<a  href="https://www.vrpcommute.tk">
       <img align="center" height=400 src="https://github.com/kenanlv/Vehicle-Tracking/blob/master/imgs/cornerDetection.JPG">
   </a>
 <p/>
    
### Optical Flow with Clustering
    
<p align="center">
<a  href="https://www.vrpcommute.tk">
       <img align="center" height=400 src="https://github.com/kenanlv/Vehicle-Tracking/blob/master/imgs/opticalFlowCluster.JPG">
   </a>
 <p/>
 
### Final Result

<p align="center">
<a  href="https://www.vrpcommute.tk">
       <img align="center" height=400 src="https://github.com/kenanlv/Vehicle-Tracking/blob/master/imgs/result.JPG">
   </a>
 <p/>


## Reference

[1]"Canny Edge Detector — OpenCV 2.4.13.7 documentation", Docs.opencv.org, 2019. [Online]. [Available Here](https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/canny_detector/canny_detector.html)

[2]A. Consulting, "Histogram of Oriented Gradients | Learn OpenCV", Learnopencv.com, 2019. [Online]. [Available Here](https://www.learnopencv.com/histogram-of-oriented-gradients/)

[3]T. Lindeberg, "Detecting salient blob-like image structures and their scales with a scale-space primal sketch: A method for focus-of-attention", International Journal of Computer Vision, vol. 11, no. 3, pp. 283-318, 1993. Available: 10.1007/bf01469346.

[4]"OpenCV: Optical Flow", Docs.opencv.org, 2019. [Online]. [Available Here](https://docs.opencv.org/3.4/d4/dee/tutorial_optical_flow.html)

[5] Opencv-python-tutroals.readthedocs.io. (2019). Harris Corner Detection — OpenCV-Python Tutorials 1 documentation. [Available Here](https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_feature2d/py_features_harris/py_features_harris.html)

[6]"Harris corner detector — OpenCV 2.4.13.7 documentation", Docs.opencv.org, 2019. [Online]. [Available Here](https://docs.opencv.org/2.4/doc/tutorials/features2d/trackingmotion/harris_detector/harris_detector.html)
