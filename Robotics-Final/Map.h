#ifndef MAP_H_
#define MAP_H_

#include <HamsterAPIClientCPP/Hamster.h>
#include <HamsterAPICommon/Messages/OccupancyGrid.h>
#include <math.h>
#include <string>
#include <vector>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv/cvwimage.h>

#include "Models/Node.h"
#include "Models/Position.h"
#include "Models/Particle.h"

using HamsterAPI::OccupancyGrid;
using namespace cv;
using std::vector;

class Map {
public:
	Map(const OccupancyGrid& grid, double robot_size, float resolution);

	Mat* GetInflatedMatrix();
	const OccupancyGrid& GetRotatedGrid();

	HamsterAPI::Cell GetGridCell(int x, int y)const;

	bool IsInflatedOccupied(int x, int y);
	bool IsOccupiedInOriginalMap(int x, int y);

	void DrawPath(std::vector<Node*> nodes);
	void DrawParticles(vector<Particle*> particles);

	std::size_t GetHeight();
	std::size_t GetWidth();

	Position ConevrtMapPositionToGlobalPosition(Position p);
	Position ConevrtGlobalPositionToMapPosition(Position p);

private:
	const double _robotSize;
	const float _resolution;
	int _cubePaddingSize;
	std::map<NodeType, Vec3b> _nodeTypeColor;

	const OccupancyGrid _originalOccupancyGrid;
	const OccupancyGrid _rotatedOccupancyGrid;
	OccupancyGrid _inflatedOccupancyGrid;

	Mat ConvertGridToMatrix(OccupancyGrid ogrid);
	OccupancyGrid CreateRotatedGrid(OccupancyGrid ogrid);

	OccupancyGrid convertToCoarseGrid(OccupancyGrid& ogrid);

	void SetCellColorInMatrix(Mat &matrix, int row, int col, HamsterAPI::Cell cell);
	void SetColorInMatrixArea(Mat& matrix, int row, int col, int radius, Vec3b color);
	void ShowMap(std::string windowName, Mat mat);
	OccupancyGrid CreateInflatedGrid(const OccupancyGrid& ogrid, int cube_padding_size);
	Mat* CopyToMat(const OccupancyGrid& ogrid);
	std::map<NodeType, Vec3b> nodeTypeColors();
	void TranslateMat(Mat &mat, int offsetx, int offsety);
	void RotateMat(Mat &mat, double rotationAngle);
};

#endif /* MAP_H_ */
