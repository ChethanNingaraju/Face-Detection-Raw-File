# FaceDetectionRawFile
Face Detection on raw video. Uses OpenCV to detect face directly on raw video in BGR 24 bit format.

Input: RGB24 bit video format
Output: Face recognized video raw file, 
	with co-ordinated making the face dumped in text file
	A binary map file, with every byte specifying the ROI = true/false in raster scan order

Version changes [v1_1]
---------------
[x] Added function to dump mapfile of ROI region in the frame
