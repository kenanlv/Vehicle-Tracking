using namespace std;
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "Blob.h"
#include <fstream>
#include <string>
#include <iomanip>
#pragma warning(disable : 4996)
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

#include<iostream>
#include<conio.h>  // remove this line if not using Windows OS
#define SHOW_STEPS // un-comment | comment this line to show steps or not



#include <stdio.h>  /* defines FILENAME_MAX */
#include <direct.h>
#include <map>
#define GetCurrentDir _getcwd

using namespace cv;
// const global variables
const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_YELLOW = cv::Scalar(0.0, 255.0, 255.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 200.0, 0.0);
const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);

// function prototypes
void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &existingBlobs, std::vector<Blob> &currentFrameBlobs);
void addBlobToExistingBlobs(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs, int &intIndex);
void addNewBlob(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs);
double distanceBetweenPoints(cv::Point point1, cv::Point point2);
void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName);
void drawAndShowContours(cv::Size imageSize, std::vector<Blob> blobs, std::string strImageName);
bool checkIfBlobsCrossedTheLineRight(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCountRight);
bool checkIfBlobsCrossedTheLineLeft(std::vector<Blob> &blobs, int &intHorizontalLinePositionLeft, int &carCountLeft);
void drawBlobInfoOnImage(std::vector<Blob>& blobs, cv::Mat& imgFrame2Copy, vector<Scalar> color);
void drawCarCountOnImage(int &carCountRight, cv::Mat &imgFrame2Copy);

void findBlob(std::vector<Blob>& currentFrameBlobs, cv::Mat img, cv::Mat imgFrame2, cv::Mat imgFrame2Copy, cv::Mat imgThresh, map<double, vector<Point>> groups);

// global variables
std::stringstream date;
int carCountLeft, intVerticalLinePosition, carCountRight = 0;

int main(void) {				    
	cv::VideoCapture capVideo;
    cv::Mat imgFrame1;
    cv::Mat imgFrame2;
    std::vector<Blob> blobs;	
    cv::Point crossingLine[2];
	cv::Point crossingLineLeft[2];	
	char cCurrentPath[FILENAME_MAX];

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
        {
        return errno;
        }
    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
    cout<<cCurrentPath<<endl;
	cout << "this is the directory" << endl;

	
    //capVideo.open("..\\..\\src\\test1.mp4");
    capVideo.open("MultipleCar.mp4");
	//MultipleCar
	//RedTruck
	//RunWay
	//OliveTruck
	//MultipleEO
	//GreenTruck
	//SilverAndRed
    if (!capVideo.isOpened()) {                                                 // if unable to open video file
        std::cout << "error reading video file  " << cCurrentPath <<  std::endl << std::endl;      // show error message
        _getch();																// remove this line if not using Windows OS
        return(0);                                                              // and exit program
    }

    if (capVideo.get(cv::CAP_PROP_FRAME_COUNT) < 2) {
        std::cout << "error: video file must have at least two frames";
        _getch();																// remove this line if not using Windows OS
        return(0);
    }

    capVideo.read(imgFrame1);
    capVideo.read(imgFrame2);

    char chCheckForEscKey = 0;
    bool blnFirstFrame = true;
    int frameCount = 2;

    // Create some random colors
	vector<Scalar> colors;
	RNG rng;
	for (int i = 0; i < 100; i++)
	{
		int r = rng.uniform(0, 256);
		int g = rng.uniform(0, 256);
		int b = rng.uniform(0, 256);
		colors.push_back(Scalar(r, g, b));
	}
    cv::Mat old_frame, old_gray;
    std::vector<Point2f> p0, p1;
    // capVideo >> old_frame;
    old_frame = imgFrame1.clone();
	cvtColor(old_frame, old_gray, COLOR_BGR2GRAY);
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    int thresh = 170;
    Mat dst;
    
    goodFeaturesToTrack(old_gray, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);
	// Create a mask image for drawing purposes
	Mat mask = Mat::zeros(old_frame.size(), old_frame.type());

    while (capVideo.isOpened() && chCheckForEscKey != 27) {
        // optical flow----------------

		Mat frame, frame_gray;
		frame = imgFrame2.clone();;
		if (frame.empty())
			break;
		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		// calculate optical flow
		vector<uchar> status;
		vector<float> err;
		TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
		calcOpticalFlowPyrLK(old_gray, frame_gray, p0, p1, status, err, Size(15, 15), 2, criteria);
		vector<Point2f> good_new;
		map<double, vector<Point>> groups;
		for (uint i = 0; i < p0.size(); i++)
		{
			// Select good points
			if (status[i] == 1) {
				good_new.push_back(p1[i]);
				// draw the tracks
				//line(mask, p1[i], p0[i], colors[i], 2);
				circle(frame, p1[i], 15, colors[i], -1);
				double distance = distanceBetweenPoints(p0[i], p1[i]);
				bool flg = false;
				for (auto iter = groups.begin(); iter != groups.end(); iter++) {
					if (abs(iter->first - distance) <= 0.3) {
						iter->second.push_back(p1[i]);
						flg = true;
					}
				}
				if (!flg) {
					groups.insert(pair<double, vector<Point>>(distance, vector<Point>()));
					groups[distance].push_back(p1[i]);
				}
			}
		}
		// Add opical flow to contours 
		int idx = 0;
		for (auto iter = groups.begin(); iter != groups.end(); iter++) {
			//contours.push_back(iter->second);
			circle(frame, iter->second[iter->second.size() - 1], 15, colors[rand() % colors.size()], -1);

		}

		Mat img;
		add(frame, mask, img);
		imshow("Frame", img);

		// Now update the previous frame and previous points
		old_gray = frame_gray.clone();
		p0 = good_new;


        // bolb detection-------------------
        std::vector<vector<Blob>> farmArr;
		cv::Mat imgThresh;
		cv::Mat imgFrame2Copy = imgFrame2.clone();
		std::vector<Blob> currentFrameBlobs;
		findBlob(currentFrameBlobs, img, imgFrame2, imgFrame2Copy, imgThresh, groups);
        if (blnFirstFrame == true) {
            for (auto &currentFrameBlob : currentFrameBlobs) {
                blobs.push_back(currentFrameBlob);
            }
        } 
		else {
            matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
        }
         imgFrame2Copy = imgFrame2.clone();	// get another copy of frame 2 since we changed the previous frame 2 copy in the processing above

         drawBlobInfoOnImage(blobs, imgFrame2Copy, colors);
		 drawCarCountOnImage(carCountRight, imgFrame2Copy);
         cv::imshow("imgFrame2Copy", imgFrame2Copy);  
		
        // now we prepare for the next iteration

         currentFrameBlobs.clear();

        imgFrame1 = imgFrame2.clone();	// move frame 1 up to where frame 2 is

        if ((capVideo.get(cv::CAP_PROP_POS_FRAMES) + 1) < capVideo.get(cv::CAP_PROP_FRAME_COUNT)) {
            capVideo.read(imgFrame2);
        }
        else {
            std::cout << "end of video\n";
            break;
        }

        blnFirstFrame = false;
        frameCount++;
        chCheckForEscKey = cv::waitKey(1);
        farmArr.clear();

        // cv::Mat imgFrame1Copy = imgFrame1.clone();
        
        
    }
    int keyboard = waitKey(30);
    if (keyboard == 'q' || chCheckForEscKey != 27) {               // if the user did not press esc (i.e. we reached the end of the video)
        cv::waitKey(0);                         // hold the windows open to allow the "end of video" message to show
		std::cout << "end of video\n";
    }
	std::cout << "end of video\n";
    // note that if the user did press esc, we don't need to hold the windows open, we can simply let the program end which will close the windows
    return(0);
}

void findBlob(std::vector<Blob> &currentFrameBlobs, cv::Mat img, cv::Mat imgFrame2, cv::Mat imgFrame2Copy, cv::Mat imgThresh, map<double, vector<Point>> groups) {
    	double thres = 40;
        cv::Mat imgFrame1Copy = img.clone();
        //cv::Mat imgFrame2Copy = imgFrame2.clone();
        cv::Mat imgDifference;
        //cv::Mat imgThresh;
        cv::cvtColor(imgFrame1Copy, imgFrame1Copy, cv::COLOR_BGR2GRAY);
        cv::cvtColor(imgFrame2Copy, imgFrame2Copy, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(5, 5), 0);
        cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(5, 5), 0);
        cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference); //changed
		//[1 1 1]
		//[1 2 1]
		//[2 1 1]
        cv::threshold(imgDifference, imgThresh, thres, 255.0, cv::THRESH_BINARY);
        cv::imshow("imgThresh", imgThresh);
        cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
        cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
        cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
        cv::Mat structuringElement15x15 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

        for (unsigned int i = 0; i < 2; i++) {
            cv::dilate(imgThresh, imgThresh, structuringElement7x7);
            cv::dilate(imgThresh, imgThresh, structuringElement7x7);
            cv::erode(imgThresh, imgThresh, structuringElement7x7);
        }

        cv::Mat imgThreshCopy = imgThresh.clone();
        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(imgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        std::vector<std::vector<cv::Point> > convexHulls(contours.size());

        for (unsigned int i = 0; i < contours.size(); i++) {
            cv::convexHull(contours[i], convexHulls[i]);
        }
	
        for (auto &convexHull : convexHulls) {
            Blob possibleBlob(convexHull);

            if (possibleBlob.currentBoundingRect.area() > 400 &&
                possibleBlob.dblCurrentAspectRatio > 0.2 &&
                possibleBlob.dblCurrentAspectRatio < 4.0 &&
                possibleBlob.currentBoundingRect.width > 30 &&
                possibleBlob.currentBoundingRect.height > 30 &&
                possibleBlob.dblCurrentDiagonalSize > 60.0 &&
                (cv::contourArea(possibleBlob.currentContour) / (double)possibleBlob.currentBoundingRect.area()) > 0.50) { // 0.5
					currentFrameBlobs.push_back(possibleBlob);
            }
        }
}


void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &existingBlobs, std::vector<Blob> &currentFrameBlobs) {
    for (auto &existingBlob : existingBlobs) {
        existingBlob.blnCurrentMatchFoundOrNewBlob = false;
        existingBlob.predictNextPosition();
    }

    for (auto &currentFrameBlob : currentFrameBlobs) {
        int intIndexOfLeastDistance = 0;
        double dblLeastDistance = 100000.0;

        for (unsigned int i = 0; i < existingBlobs.size(); i++) {

            if (existingBlobs[i].blnStillBeingTracked == true) {
                double dblDistance = distanceBetweenPoints(currentFrameBlob.centerPositions.back(), existingBlobs[i].predictedNextPosition);

                if (dblDistance < dblLeastDistance) {
                    dblLeastDistance = dblDistance;
                    intIndexOfLeastDistance = i;
                }
            }
        }

        if (dblLeastDistance < currentFrameBlob.dblCurrentDiagonalSize * 1.0) { // 0.5
            addBlobToExistingBlobs(currentFrameBlob, existingBlobs, intIndexOfLeastDistance);
        }
        else {
            addNewBlob(currentFrameBlob, existingBlobs);
        }

    }

    for (auto &existingBlob : existingBlobs) {
        if (existingBlob.blnCurrentMatchFoundOrNewBlob == false) {
            existingBlob.intNumOfConsecutiveFramesWithoutAMatch++;
        }
        if (existingBlob.intNumOfConsecutiveFramesWithoutAMatch >= 5) {
            existingBlob.blnStillBeingTracked = false;
        }
    }
}


void addBlobToExistingBlobs(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs, int &intIndex) {
    existingBlobs[intIndex].currentContour = currentFrameBlob.currentContour;
    existingBlobs[intIndex].currentBoundingRect = currentFrameBlob.currentBoundingRect;
    existingBlobs[intIndex].centerPositions.push_back(currentFrameBlob.centerPositions.back());
    existingBlobs[intIndex].dblCurrentDiagonalSize = currentFrameBlob.dblCurrentDiagonalSize;
    existingBlobs[intIndex].dblCurrentAspectRatio = currentFrameBlob.dblCurrentAspectRatio;
    existingBlobs[intIndex].blnStillBeingTracked = true;
    existingBlobs[intIndex].blnCurrentMatchFoundOrNewBlob = true;
}


void addNewBlob(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs) {
    currentFrameBlob.blnCurrentMatchFoundOrNewBlob = true;
    existingBlobs.push_back(currentFrameBlob);
}


double distanceBetweenPoints(cv::Point point1, cv::Point point2) {    
    int intX = abs(point1.x - point2.x);
    int intY = abs(point1.y - point2.y);

    return(sqrt(pow(intX, 2) + pow(intY, 2)));
}


void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName) {    
	cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);
    cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);
    cv::imshow(strImageName, image);
}


void drawAndShowContours(cv::Size imageSize, std::vector<Blob> blobs, std::string strImageName) {    
    cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);
    std::vector<std::vector<cv::Point> > contours;

    for (auto &blob : blobs) {
        if (blob.blnStillBeingTracked == true) {
            contours.push_back(blob.currentContour);
        }
    }

    cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);
    cv::imshow(strImageName, image);
}


bool checkIfBlobsCrossedTheLineRight(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCountRight) {    
	bool blnAtLeastOneBlobCrossedTheLine = false;

    for (auto blob : blobs) {
        if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
            int prevFrameIndex = (int)blob.centerPositions.size() - 2;
            int currFrameIndex = (int)blob.centerPositions.size() - 1;

			// Left way
			if (blob.centerPositions[prevFrameIndex].y > intHorizontalLinePosition && blob.centerPositions[currFrameIndex].y <= intHorizontalLinePosition && blob.centerPositions[currFrameIndex].x > 350) {
                carCountRight++;												
                blnAtLeastOneBlobCrossedTheLine = true;
            }
        }
    }

    return blnAtLeastOneBlobCrossedTheLine;
}


bool checkIfBlobsCrossedTheLineLeft(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCountLeft) {	
	bool blnAtLeastOneBlobCrossedTheLineLeft = false;

	for (auto blob : blobs) {
		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;

			// Left way
			if (blob.centerPositions[prevFrameIndex].y <= intHorizontalLinePosition && blob.centerPositions[currFrameIndex].y > intHorizontalLinePosition && blob.centerPositions[currFrameIndex].x < 350 && blob.centerPositions[currFrameIndex].x > 0) {
				carCountLeft++;					
				blnAtLeastOneBlobCrossedTheLineLeft = true;
			}
		}
	}

	return blnAtLeastOneBlobCrossedTheLineLeft;
}


void drawBlobInfoOnImage(std::vector<Blob> &blobs, cv::Mat &imgFrame2Copy, vector<Scalar>  color) {
    for (unsigned int i = 0; i < blobs.size(); i++) {
        if (blobs[i].blnStillBeingTracked == true) {
            cv::rectangle(imgFrame2Copy, blobs[i].currentBoundingRect, color[i], 2);
            
            int intFontFace = cv::FONT_HERSHEY_SIMPLEX;
            double dblFontScale = (imgFrame2Copy.rows * imgFrame2Copy.cols) / 300000.0;
            int intFontThickness = (int)std::round(dblFontScale * 1.0);

            cv::putText(imgFrame2Copy, std::to_string(i+1), blobs[i].centerPositions.back(), intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);
        }
    }
}



void drawCarCountOnImage(int &carCountRight, cv::Mat &imgFrame2Copy) {
    int intFontFace = cv::FONT_HERSHEY_SIMPLEX;
    double dblFontScale = (imgFrame2Copy.rows * imgFrame2Copy.cols) / 450000.0;
    int intFontThickness = (int)std::round(dblFontScale * 2.5);
}


