#ifndef MATRIXHELPER_H_
#define MATRIXHELPER_H_

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv/cvwimage.h>

using namespace cv;

class MatrixHelper {
public:
	MatrixHelper();
	virtual ~MatrixHelper();
	static Mat TranslateMat(Mat &mat, int offsetx, int offsety);
	static Mat RotateMat(Mat &mat, double rotationAngle);
};

#endif /* MATRIXHELPER_H_ */
