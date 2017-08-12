#include "Map.h"

using namespace std;
using namespace HamsterAPI;
using namespace cv;

typedef std::pair<NodeType,Vec3b> NodeTypeRGB;

//TODO : refactor it to colors

Vec3b BLUE = Vec3b(255, 0, 0);
Vec3b GREEN = Vec3b(0, 255, 0);
Vec3b RED = Vec3b(0, 0, 255);
Vec3b YELLOW = Vec3b(255, 255, 0);
Vec3b PINK = Vec3b(147, 0, 214);
Vec3b BLACK = Vec3b(0, 0, 0);
Vec3b WHITE = Vec3b(255, 255, 255);
Vec3b GRAY = Vec3b(128, 128, 128);

Map::Map(const OccupancyGrid &grid, int robot_size, float resolution) : 
	_originalOccupancyGrid(grid), _robot_size(robot_size), _resolution(resolution)
{
	// Calculate cell padding size by resolution and robot size
	_cube_padding_size = ceil((double)_robot_size / _resolution / 2);

	_rotatedOccupancyGrid = CreateRotatedGrid(_originalOccupancyGrid);
	_inflatedOccupancyGrid = CreateInflatedGrid(*_rotatedOccupancyGrid, _cube_padding_size);

	InitNodeTypeColors();
}

void Map::InitNodeTypeColors(){
	_node_type_color.insert(NodeTypeRGB(PARTICLE,BLUE));
	_node_type_color.insert(NodeTypeRGB(MAX_PARTICLE, GREEN));
	_node_type_color.insert(NodeTypeRGB(PATH, RED));
	_node_type_color.insert(NodeTypeRGB(WAYPOINT,YELLOW));
	_node_type_color.insert(NodeTypeRGB(ROBOT,PINK));
	_node_type_color.insert(NodeTypeRGB(PATH_START_POINT,BLUE));
	_node_type_color.insert(NodeTypeRGB(PATH_END_POINT,GREEN));
}

unsigned int Map::GetHeight()
{
	return _originalOccupancyGrid.getHeight();
}

unsigned int Map::GetWidth()
{
	return _originalOccupancyGrid.getWidth();
}

OccupancyGrid* Map::CreateRotatedGrid(OccupancyGrid grid)
{
	int height = grid.getHeight();
	int width = grid.getWidth();
	Mat matrix = ConvertGridToMatrix(grid);

	MatrixHelper::TranslateMat(matrix, 42, 42);
	MatrixHelper::RotateMat(matrix, -30);

	OccupancyGrid* rotatedGrid = new OccupancyGrid(height, width, grid.getResolution());

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			Vec3b color = matrix.at<Vec3b>(i,j);
			if(norm(color, BLACK, NORM_INF)==0){
				rotatedGrid->setCell(i, j, CELL_OCCUPIED);
			}
			else if (norm(color, WHITE, NORM_INF)==0){
				rotatedGrid->setCell(i, j, CELL_FREE);
			}
			else{
				rotatedGrid->setCell(i, j, CELL_UNKNOWN);
			}
		}
	}


	return rotatedGrid;
}

OccupancyGrid * Map::CreateInflatedGrid(const OccupancyGrid& ogrid, int cube_padding_size)
{
	// Duplicate grid
	OccupancyGrid *inflatedGrid = new OccupancyGrid(ogrid);

	for (unsigned int row = 0; row < ogrid.getHeight();
		 row++)
	{
		for (unsigned int col = 0; col < ogrid.getWidth();
			 col++)
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

				for (int i = minY; i <= maxY; i++)
				{
					for (int j = minX; j <= maxX; j++)
					{
						inflatedGrid->setCell(i, j, CELL_OCCUPIED);
					}
				}
			}
		}
	}

	return inflatedGrid;
}

Mat *Map::GetInflatedMatrix()
{
	return CopyToMat(_inflatedOccupancyGrid);
}

const OccupancyGrid *Map::GetRotatedGrid(){
	return _rotatedOccupancyGrid;
}

void Map::DrawParticles(vector<Particle *> particles)
{
	static const string PARTICLES_VIEW ("Particles-View");
	Mat matrix = ConvertGridToMatrix(*_rotatedOccupancyGrid);
	int size = particles.size();
	int best = 5;
	int i = size-1;

	// Draw the low-belief particles in red color
	for (; i > best; i--) {
		int x = particles[i]->mPosition->X();
		int y = particles[i]->mPosition->Y();

		SetColorInMatrixArea(matrix, y, x, 1, RED);
	}

	// Drawing the top 5 particles in green color
	for (; i >= 0; i--) {
		int x = particles[i]->mPosition->X();
		int y = particles[i]->mPosition->Y();

		SetColorInMatrixArea(matrix, y, x, 1, GREEN);
	}

	ShowMap(PARTICLES_VIEW, matrix);
}

Mat *Map::CopyToMat(const OccupancyGrid *ogrid)
{
	int width = ogrid->getWidth();
	int height = ogrid->getHeight();
	Mat *inflatedMatrix = new Mat(width, height, CV_8UC1);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			SetCellColorInMatrix(*inflatedMatrix, i, j, ogrid->getCell(i, j));
		}
	}
	return inflatedMatrix;
}

HamsterAPI::Cell Map::GetGridCell(int x, int y) const
{
	return _inflatedOccupancyGrid->getCell(y, x);
}

bool Map::IsInflatedOccupied(int x, int y)
{
	return _inflatedOccupancyGrid->getCell(y, x) == CELL_OCCUPIED;
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
	for (int i = row - radius; i < row + radius && i >= 0 && i < GetHeight(); i++)
	{
		for (int j = col - radius; j < col + radius && j >= 0 && i < GetWidth(); j++)
		{
			matrix.at<Vec3b>(i, j) = color;
		}
	}
}

Mat Map::ConvertGridToMatrix(OccupancyGrid ogrid)
{
	Mat matrix = Mat(ogrid.getHeight(),
					 ogrid.getWidth(), CV_8UC3);

	for (unsigned int row = 0; row < ogrid.getHeight(); row++)
	{
		for (unsigned int col = 0; col < ogrid.getWidth(); col++)
		{
			SetCellColorInMatrix(matrix, row, col, ogrid.getCell(row, col));
		}
	}

	return matrix;
}

void Map::DrawPath(vector<Node *> nodes)
{
	static const string PATH_MAP_VIEW ("Path-View");
	Mat mat = ConvertGridToMatrix(*_inflatedOccupancyGrid);
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		Position *p = nodes[i]->location;
		SetColorInMatrixArea(mat, p->Y(), p->X(), _cube_padding_size,
							_node_type_color[nodes[i]->type]);
	}

	if(nodes.size()!=0){
	Position *start = nodes[0]->location;
	SetColorInMatrixArea(mat, start->Y(), start->X(), _cube_padding_size, BLUE);

	Position *end = nodes[nodes.size() - 1]->location;
	SetColorInMatrixArea(mat, end->Y(), end->X(), _cube_padding_size, GREEN);
	}

	ShowMap(PATH_MAP_VIEW, mat);
}

Position* Map::ConevrtMapPositionToGlobalPosition(Position * p)
{
	float newX = (p->X() - (double)(GetWidth() / 2)) * _originalOccupancyGrid.getResolution();
	float newY = -(p->Y() - (double)(GetHeight() / 2)) * _originalOccupancyGrid.getResolution();
	return new Position(newX, newY, p->Yaw());

}

Position* Map::ConevrtGlobalPositionToMapPosition(Position * p)
{
	float newX = p->X() / _originalOccupancyGrid.getResolution() + (double)(GetWidth() / 2);
	float newY = -p->Y() / _originalOccupancyGrid.getResolution() + (double)(GetHeight() / 2);
	return new Position(newX, newY, p->Yaw());
}

void Map::ShowMap(string windowName, Mat matrix)
{
	imshow(windowName, matrix);
	waitKey(100);
}

float Map::convertPixelToMeter(float inPixel)
{
	return inPixel*_resolution;
}

float Map::convertMeterToPixel(float inMeter)
{
	return inMeter/_resolution;
}
