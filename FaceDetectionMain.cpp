#define _CRT_SECURE_NO_DEPRECATE
#include <fstream>
#include <iostream>
#include <cmath>
#include <stdio.h>

#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	Mat img;
	ifstream raw_file;
	ofstream outfile, statfile;
	int width, height;


	if (argc != 4)
	{
		printf("Enter the image file name");
		exit(0);
	}
	raw_file.open(argv[1], ios::in | ios::binary);//open the raw video file and read frame by frame
	statfile.open("E:/Sequence/stat1.txt", ios::out | ios::trunc);
	width = atoi(argv[2]);
	height = atoi(argv[3]);

	//outfile to store output file with face detected
	outfile.open("E:/Sequence/out1.raw", ios::out | ios::binary | ios::trunc);
	int frame_size = width * height * 3;//assuming rgb24 format
	//char framebuf[width * height * 3], bgr_buf[width * height * 3];
	char * framebuf = (char *)malloc(frame_size);
	char * bgr_buf = (char *)malloc(frame_size);
	//img = cvLoadImage("E:/Sequence/Lena.png"); //change the name (image.jpg) according to your Image filename.


	//vector<Mat> channels(3);


	// Load Face cascade (.xml file)
	CascadeClassifier face_cascade;
	face_cascade.load("C:/library/opencv/sources/data/haarcascades_GPU/haarcascade_frontalface_default.xml");
	for (int frame_num = 0;; frame_num++)
	{
		img.create(height, width, CV_8UC(3));
		Mat channels[3];
		split(img, channels);
		//read single frame
		raw_file.read(framebuf, frame_size);
		//GBR format needs to be interleaved
		for (int i = 0; i < frame_size / 3; i++)
		{
			bgr_buf[i] = framebuf[i * 3];
			bgr_buf[width * height + i] = framebuf[i * 3 + 1];
			bgr_buf[width * height * 2 + i] = framebuf[i * 3 + 2];
		}

		//img.create(3,imageSize, CV_8UC(3));
		memcpy(channels[2].data, bgr_buf, (width * height));
		memcpy(channels[1].data, &bgr_buf[(width * height)], (width * height));
		memcpy(channels[0].data, &bgr_buf[(width * height * 2)], (width * height));
		merge(channels, 3, img);

		//Mat myuv(height + height / 2, 340, CV_8UC1,framebuf);
		//cvtColor(myuv, img, CV_YCrCb2BGR, 3);

		// Detect faces
		std::vector<Rect> faces;
		face_cascade.detectMultiScale(img, faces, 1.1, 2, 0, Size(60, 80));
		if (faces.size() > 0)
		{
			statfile << faces[0].x << " " << faces[0].y << " " << faces[0].height << " " << faces[0].width << "\n";
		}
		else
		{
			statfile << "NA\n";
		}
		// Draw circles on the detected faces
		for (size_t i = 0; i < ((faces.size() > 1) ? faces.size() : faces.size()); i++)
		{
			//Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
			//ellipse(img, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(128, 128, 128), 4, 8, 0);
			Point dia1(faces[i].x, faces[i].y);
			Point dia2(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
			rectangle(img, dia1, dia2, Scalar(0, 0, 255), 2, 8, 0);
		}
		outfile.write((char *)img.data, frame_size);
		img.release();
		//imshow("Detected Face", img);
		//cvWaitKey(0);
		//cvDestroyWindow("Detected Face");
		if (raw_file.eof())
			break;
	}
	free(framebuf);
	free(bgr_buf);
	outfile.close();
	raw_file.close();
	/*
	img = cvLoadImage(argv[1]);
	imshow("Detected Face", img);*/

	system("pause");
	return 0;
}