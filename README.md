# Vehicle-Tracking

Object recognizing and tracking play a very crucial role in our daily life. Object tracking enables us to count and label each tracked object. Our goal is to detect, label, and track all individual vehicles in a given video.

//gif here

## Installation



## Methods

> Overall we want to extract feature points for each object from the background and keep tracking them (we used **canny edge detection** and **optical flow**) but we need to figure out a way to merge and separate blobs for identifying if the feature belongs to the same or different objects (by **clustering** the blobs using the relative translations and directions). 
>
>_The following picture is a flow chart representation of our logic._


<p align="center">
<a  href="https://www.vrpcommute.tk">
       <img align="center" height=85 src="https://github.com/kenanlv/Vehicle-Tracking/imgs/flowChart.jpg">
   </a>
 <p/>



[![bgslibrary](http://i.giphy.com/5A94AZahSIVOw.gif)]

### Canny Edge Detection

    Canny Edge Detection is one way for us to remove the background and extract objects/features from each individual frame. There are multiple stages in the Canny Edge Detection: noise reduction, finding intensity gradient of the image, non-maximum suppression, and Hysteresis thresholding. 

### Optical Flow with Corner Detection
    Optical flow or optic flow is the pattern of apparent motion of objects, surfaces, and edges in a visual scene caused by the relative motion between an observer and a scene. 
    By finding the strong corner points and comparing with the consecutive frame, we could sift out unwanted points and prevent us from missing potential candidates.

## Results


    


