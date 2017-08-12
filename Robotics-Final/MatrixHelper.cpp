#include "MatrixHelper.h"


MatrixHelper::MatrixHelper() {

}

MatrixHelper::~MatrixHelper() {
}

Mat MatrixHelper::TranslateMat(Mat &mat, int offsetx, int offsety){
    Mat translationMat = (Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
    warpAffine(mat,mat,translationMat,mat.size(),INTER_LINEAR, BORDER_CONSTANT, Scalar(128,128,128));
    return translationMat;
}

Mat MatrixHelper::RotateMat(Mat &mat, double rotationAngle){
	Point center = Point( mat.cols/2, mat.rows/2 );
	Mat rotationMat = getRotationMatrix2D(center, rotationAngle , 1);
	warpAffine(mat, mat, rotationMat ,mat.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(128,128,128));
	return rotationMat;
}

