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

#include "MatrixHelper.h"
#include "Models/Node.h"
#include "Models/Position.h"
#include "Models/Particle.h"

#define PATH_MAP_VIEW "Path-View"
#define PARTICLES_VIEW "Particles-View"

using HamsterAPI::OccupancyGrid;

using namespace HamsterAPI;
using namespace cv;

class Map {

private:
	const OccupancyGrid _originalOccupancyGrid;
	const OccupancyGrid* _inflatedOccupancyGrid;
	const OccupancyGrid* _rotatedOccupancyGrid;

	const int _robot_size;
	const float _resolution;
	int _cube_padding_size;
	map<NodeType, Vec3b> _node_type_color;

	Mat ConvertGridToMatrix(OccupancyGrid ogrid);
	OccupancyGrid* CreateRotatedGrid(OccupancyGrid ogrid);

	void CopyToMat(OccupancyGrid* ogrid);
	void SetCellColorInMatrix(Mat &matrix, int row, int col, Cell cell);
	void SetColorInMatrixArea(Mat& matrix, int row, int col, int radius, Vec3b color);
	void ShowMap(std::string windowName, Mat mat);
	OccupancyGrid* CreateInflatedGrid(const OccupancyGrid& ogrid, int cube_padding_size);
	Mat * CopyToMat(const OccupancyGrid* ogrid);
	void InitNodeTypeColors();

public:
	Map(const OccupancyGrid & grid, int robot_size, float resolution);

	Mat * GetInflatedMatrix();
	const OccupancyGrid * GetRotatedGrid();

	HamsterAPI::Cell GetGridCell(int x, int y)const;

	bool IsInflatedOccupied(int x, int y);
	bool IsOccupiedInOriginalMap(int x, int y);

	void DrawPath(std::vector<Node*> nodes);
	void DrawParticles(vector<Particle *> particles);

	unsigned int GetHeight();
	unsigned int GetWidth();

	Position* ConevrtMapPositionToGlobalPosition(Position * p);
	Position* ConevrtGlobalPositionToMapPosition(Position * p);

	float convertPixelToMeter(float inPixel);
	float convertMeterToPixel(float inMeter);
};

#endif /* MAP_H_ */
