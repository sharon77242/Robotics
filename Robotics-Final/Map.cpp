#include "Map.h"

using namespace std;
using namespace HamsterAPI;
using namespace cv;

typedef std::pair<NodeType,Vec3b> NodeTypeRGB;

static const Vec3b BLUE = Vec3b(255, 0, 0);
static const Vec3b GREEN = Vec3b(0, 255, 0);
static const Vec3b RED = Vec3b(0, 0, 255);
static const Vec3b YELLOW = Vec3b(255, 255, 0);
static const Vec3b PINK = Vec3b(147, 0, 214);
static const Vec3b BLACK = Vec3b(0, 0, 0);
static const Vec3b WHITE = Vec3b(255, 255, 255);
static const Vec3b GRAY = Vec3b(128, 128, 128);

Map::Map(const OccupancyGrid &grid, double robot_size, float resolution)
: _robotSize(robot_size)
, _resolution(resolution)
, _cubePaddingSize (ceil(robot_size / resolution / 2))
, _nodeTypeColor(nodeTypeColors())
, _originalOccupancyGrid(grid)
, _rotatedOccupancyGrid (CreateRotatedGrid(_originalOccupancyGrid))
, _inflatedOccupancyGrid(CreateInflatedGrid(_rotatedOccupancyGrid, _cubePaddingSize))
{}

map<NodeType, Vec3b> Map::nodeTypeColors()
{
	std::map<NodeType, Vec3b> temp_node_type_color;

	temp_node_type_color.insert(NodeTypeRGB(PARTICLE,BLUE));
	temp_node_type_color.insert(NodeTypeRGB(MAX_PARTICLE, GREEN));
	temp_node_type_color.insert(NodeTypeRGB(PATH, RED));
	temp_node_type_color.insert(NodeTypeRGB(WAYPOINT,YELLOW));
	temp_node_type_color.insert(NodeTypeRGB(ROBOT,PINK));
	temp_node_type_color.insert(NodeTypeRGB(PATH_START_POINT,BLUE));
	temp_node_type_color.insert(NodeTypeRGB(PATH_END_POINT,GREEN));

	return temp_node_type_color;
}

size_t Map::GetHeight()
{
	return _originalOccupancyGrid.getHeight();
}

size_t Map::GetWidth()
{
	return _originalOccupancyGrid.getWidth();
}

void Map::TranslateMat(Mat &mat, int offsetx, int offsety)
{
    Mat translationMat = (Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
    warpAffine(mat,mat,translationMat,mat.size(),INTER_LINEAR, BORDER_CONSTANT, Scalar(128,128,128));
}

void Map::RotateMat(Mat &mat, double rotationAngle)
{
	Point center = Point( mat.cols / 2, mat.rows / 2 );
	Mat rotationMat = getRotationMatrix2D(center, rotationAngle , 1);
	warpAffine(mat, mat, rotationMat ,mat.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(128,128,128));
}

OccupancyGrid Map::CreateRotatedGrid(OccupancyGrid grid)
{
	int height = grid.getHeight();
	int width = grid.getWidth();
	Mat matrix = ConvertGridToMatrix(grid);
	int offsetx = 42, offsety = 42;
	double rotationAngle = -30;

	TranslateMat(matrix, offsetx, offsety);
	RotateMat(matrix, rotationAngle);

	OccupancyGrid rotatedGrid(height, width, grid.getResolution());

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			Vec3b color = matrix.at<Vec3b>(i,j);
			if(norm(color, BLACK, NORM_INF)==0)
			{
				rotatedGrid.setCell(i, j, CELL_OCCUPIED);
			}
			else if (norm(color, WHITE, NORM_INF)==0)
			{
				rotatedGrid.setCell(i, j, CELL_FREE);
			}
			else
			{
				rotatedGrid.setCell(i, j, CELL_UNKNOWN);
			}
		}
	}


	return rotatedGrid;
}

OccupancyGrid Map::CreateInflatedGrid(const OccupancyGrid& ogrid, int cube_padding_size)
{
	// Duplicate grid
	OccupancyGrid inflatedGrid(ogrid);

	for (unsigned int row = 0; row < ogrid.getHeight(); ++row)
	{
		for (unsigned int col = 0; col < ogrid.getWidth(); ++col)
		{
			Cell currentCell = ogrid.getCell(row, col);

			// Check if the cell is occupied
			if (currentCell == CELL_OCCUPIED)
			{
				// Calculate the cells range to inflate
				int maxX =
					col + cube_padding_size >= ogrid.getWidth() ? ogrid.getWidth() - 1 : col + cube_padding_size;
				int maxY =
					row + cube_padding_size >= ogrid.getHeight() ? ogrid.getHeight() - 1 : row + cube_padding_size;
				int minX =
					col - cube_padding_size < 0 ? 0 : col - cube_padding_size;
				int minY =
					row - cube_padding_size < 0 ? 0 : row - cube_padding_size;

				for (int i = minY; i <= maxY; ++i)
				{
					for (int j = minX; j <= maxX; ++j)
					{
						inflatedGrid.setCell(i, j, CELL_OCCUPIED);
					}
				}
			}
		}
	}

	return inflatedGrid;
	//return convertToCoarseGrid(inflatedGrid);
}

OccupancyGrid Map::convertToCoarseGrid(OccupancyGrid& grid)
{
	std::cout << "out of your mind" << std::endl;

	const int robotSizeInPixels = _robotSize / _resolution;
	std::cout << "out of your mind" << _originalOccupancyGrid.getResolution() << std::endl;
	std::cout << "out of your mind" << robotSizeInPixels << std::endl;
	std::cout << "grid.getHeight()" << grid.getHeight() << std::endl;
	std::cout << "grid.getHeight()" << grid.getWidth() << std::endl;

	int rows = grid.getHeight() / robotSizeInPixels;
	int cols = grid.getWidth() / robotSizeInPixels;
	double resolution = _resolution * robotSizeInPixels;
	std::cout << "fuck that shit" << std::endl;
	OccupancyGrid coarseGrid (rows, cols, resolution);
	for (int i  = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			int row = i * robotSizeInPixels;
			int col = j * robotSizeInPixels;

			bool isOccupied = false;
			for (int k = row; k < row + robotSizeInPixels && !isOccupied; k++)
			{
				for (int m = col; m < col + robotSizeInPixels; m++)
				{
					if (grid.getCell(k, m) != CELL_FREE)
					{
						isOccupied = true;
						break;
					}
				}
			}
			if (isOccupied)
				coarseGrid.setCell(i, j, CELL_OCCUPIED);
			else
				coarseGrid.setCell(i, j, CELL_FREE);
		}
	}

	return coarseGrid;
}

Mat* Map::GetInflatedMatrix()
{
	return CopyToMat(_inflatedOccupancyGrid);
}

const OccupancyGrid& Map::GetRotatedGrid(){
	return _rotatedOccupancyGrid;
}

void Map::DrawParticles(vector<Particle *> particles)
{
	static const string PARTICLES_VIEW ("Particles-View");
	Mat matrix = ConvertGridToMatrix(_rotatedOccupancyGrid);
	int size = particles.size();
	int best = 5;
	int i = size-1;

	// Draw the low-belief particles in red color
	for (; i > best; i--) {
		int x = particles[i]->mPosition.X();
		int y = particles[i]->mPosition.Y();

		SetColorInMatrixArea(matrix, y, x, 1, RED);
	}

	// Drawing the top 5 particles in green color
	for (; i >= 0; i--) {
		int x = particles[i]->mPosition.X();
		int y = particles[i]->mPosition.Y();

		SetColorInMatrixArea(matrix, y, x, 1, GREEN);
	}

	ShowMap(PARTICLES_VIEW, matrix);
}

Mat *Map::CopyToMat(const OccupancyGrid& ogrid)
{
	int width = ogrid.getWidth();
	int height = ogrid.getHeight();
	Mat *inflatedMatrix = new Mat(width, height, CV_8UC1);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			SetCellColorInMatrix(*inflatedMatrix, i, j, ogrid.getCell(i, j));
		}
	}
	return inflatedMatrix;
}

HamsterAPI::Cell Map::GetGridCell(int x, int y) const
{
	return _inflatedOccupancyGrid.getCell(y, x);
}

bool Map::IsInflatedOccupied(int x, int y)
{
	return _inflatedOccupancyGrid.getCell(y, x) == CELL_OCCUPIED;
}

bool Map::IsOccupiedInOriginalMap(int x, int y)
{
	return _originalOccupancyGrid.getCell(y, x) == CELL_OCCUPIED;
}

void Map::SetCellColorInMatrix(Mat &matrix, int row, int col, Cell cell)
{
	switch (cell)
	{
	case CELL_FREE:
		matrix.at<Vec3b>(row, col) = WHITE;
		break;
	case CELL_OCCUPIED:
		matrix.at<Vec3b>(row, col) = BLACK;
		break;
	default:
		matrix.at<Vec3b>(row, col) = GRAY;
	}
}

void Map::SetColorInMatrixArea(Mat &matrix, int row, int col, int radius, Vec3b color)
{
	for (size_t i = row - radius; i < row + radius && i >= 0 && i < GetHeight(); ++i)
	{
		for (size_t j = col - radius; j < col + radius && j >= 0 && i < GetWidth(); ++j)
		{
			matrix.at<Vec3b>(i, j) = color;
		}
	}
}

Mat Map::ConvertGridToMatrix(OccupancyGrid ogrid)
{
	Mat matrix = Mat(ogrid.getHeight(),
					 ogrid.getWidth(), CV_8UC3);

	for (size_t row = 0; row < ogrid.getHeight(); ++row)
	{
		for (size_t col = 0; col < ogrid.getWidth(); ++col)
		{
			SetCellColorInMatrix(matrix, row, col, ogrid.getCell(row, col));
		}
	}

	return matrix;
}

void Map::DrawPath(vector<Node *> nodes)
{
	static const string PATH_MAP_VIEW ("Path-View");
	Mat mat = ConvertGridToMatrix(_inflatedOccupancyGrid);
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		Position p = nodes[i]->location;
		SetColorInMatrixArea(mat, p.Y(), p.X(), _cubePaddingSize,
							_nodeTypeColor[nodes[i]->type]);
	}

	if(nodes.size()!=0){
	Position start = nodes[0]->location;
	SetColorInMatrixArea(mat, start.Y(), start.X(), _cubePaddingSize, BLUE);

	Position end = nodes[nodes.size() - 1]->location;
	SetColorInMatrixArea(mat, end.Y(), end.X(), _cubePaddingSize, GREEN);
	}

	ShowMap(PATH_MAP_VIEW, mat);
}

Position Map::ConevrtMapPositionToGlobalPosition(Position p)
{
	float newX = (p.X() - (double)(GetWidth() / 2)) * _originalOccupancyGrid.getResolution();
	float newY = -(p.Y() - (double)(GetHeight() / 2)) * _originalOccupancyGrid.getResolution();
	return Position(newX, newY, p.Heading());

}

Position Map::ConevrtGlobalPositionToMapPosition(Position p)
{
	float newX = p.X() / _originalOccupancyGrid.getResolution() + (double)(GetWidth() / 2);
	float newY = -p.Y() / _originalOccupancyGrid.getResolution() + (double)(GetHeight() / 2);
	return Position(newX, newY, p.Heading());
}

void Map::ShowMap(string windowName, Mat matrix)
{
	imshow(windowName, matrix);
	waitKey(100);
}
