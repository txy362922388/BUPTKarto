#include  "stdlib.h"
#include  "stdio.h"

#include "Karto.h"
#include "Macros.h"
#include "Math.h"
#include "Types.h"
#include "Mapper.h"


using namespace karto;
using namespace std;

/**
* Sample code to demonstrate karto map creation
* Create a laser range finder device and three "dummy" range scans.
* Add the device and range scans to a karto Mapper.
*/
karto::Dataset* CreateMap(karto::Mapper* pMapper)
{
	karto::Dataset* pDataset = new karto::Dataset();

	/////////////////////////////////////
	// Create a laser range finder device - use SmartPointer to let karto subsystem manage memory.
	karto::Name name("laser0");
	karto::LaserRangeFinder* pLaserRangeFinder = karto::LaserRangeFinder::CreateLaserRangeFinder(karto::LaserRangeFinder_Custom, name);
	pLaserRangeFinder->SetOffsetPose(karto::Pose2(1.0, 0.0, 0.0));
	pLaserRangeFinder->SetAngularResolution(karto::math::DegreesToRadians(0.5));
	pLaserRangeFinder->SetRangeThreshold(12.0);

	pDataset->Add(pLaserRangeFinder);

	/////////////////////////////////////
	// Create three localized range scans, all using the same range readings, but with different poses. 
	karto::LocalizedRangeScan* pLocalizedRangeScan = NULL;

	// 1. localized range scan

	// Create a vector of range readings. Simple example where all the measurements are the same value.
	std::vector<kt_double> readings;
	for (int i = 0; i<361; i++)
	{
		readings.push_back(3.0);
	}

	// create localized range scan
	pLocalizedRangeScan = new karto::LocalizedRangeScan(name, readings);
	pLocalizedRangeScan->SetOdometricPose(karto::Pose2(0.0, 0.0, 0.0));
	pLocalizedRangeScan->SetCorrectedPose(karto::Pose2(0.0, 0.0, 0.0));

	// Add the localized range scan to the mapper
	pMapper->Process(pLocalizedRangeScan);
	std::cout << "Pose: " << pLocalizedRangeScan->GetOdometricPose() << " Corrected Pose: " << pLocalizedRangeScan->GetCorrectedPose() << std::endl;

	// Add the localized range scan to the dataset
	pDataset->Add(pLocalizedRangeScan);

	// 2. localized range scan

	// create localized range scan
	pLocalizedRangeScan = new karto::LocalizedRangeScan(name, readings);
	pLocalizedRangeScan->SetOdometricPose(karto::Pose2(1.0, 0.0, 1.57));
	pLocalizedRangeScan->SetCorrectedPose(karto::Pose2(1.0, 0.0, 1.57));

	// Add the localized range scan to the mapper
	pMapper->Process(pLocalizedRangeScan);
	std::cout << "Pose: " << pLocalizedRangeScan->GetOdometricPose() << " Corrected Pose: " << pLocalizedRangeScan->GetCorrectedPose() << std::endl;

	// Add the localized range scan to the dataset
	pDataset->Add(pLocalizedRangeScan);

	// 3. localized range scan

	// create localized range scan
	pLocalizedRangeScan = new karto::LocalizedRangeScan(name, readings);
	pLocalizedRangeScan->SetOdometricPose(karto::Pose2(1.0, -1.0, 2.35619449));
	pLocalizedRangeScan->SetCorrectedPose(karto::Pose2(1.0, -1.0, 2.35619449));

	// Add the localized range scan to the mapper
	pMapper->Process(pLocalizedRangeScan);
	std::cout << "Pose: " << pLocalizedRangeScan->GetOdometricPose() << " Corrected Pose: " << pLocalizedRangeScan->GetCorrectedPose() << std::endl;

	// Add the localized range scan to the dataset
	pDataset->Add(pLocalizedRangeScan);

	return pDataset;
}

/**
* Sample code to demonstrate basic occupancy grid creation and print occupancy grid.
*/
karto::OccupancyGrid* CreateOccupancyGrid(karto::Mapper* pMapper, kt_double resolution)
{
	std::cout << "Generating map..." << std::endl;

	// Create a map (occupancy grid) - time it
	karto::OccupancyGrid* pOccupancyGrid = karto::OccupancyGrid::CreateFromScans(pMapper->GetAllProcessedScans(), resolution);

	return pOccupancyGrid;
}

/**
* Sample code to print a basic occupancy grid
*/
void PrintOccupancyGrid(karto::OccupancyGrid* pOccupancyGrid)
{
	if (pOccupancyGrid != NULL)
	{
		// Output ASCII representation of map
		kt_int32s width = pOccupancyGrid->GetWidth();
		kt_int32s height = pOccupancyGrid->GetHeight();
		karto::Vector2<kt_double> offset = pOccupancyGrid->GetCoordinateConverter()->GetOffset();

		std::cout << "width = " << width << ", height = " << height << ", scale = " << pOccupancyGrid->GetCoordinateConverter()->GetScale() << ", offset: " << offset.GetX() << ", " << offset.GetY() << std::endl;
		for (kt_int32s y = height - 1; y >= 0; y--)
		{
			for (kt_int32s x = 0; x<width; x++)
			{
				// Getting the value at position x,y
				kt_int8u value = pOccupancyGrid->GetValue(karto::Vector2<kt_int32s>(x, y));

				switch (value)
				{
				case karto::GridStates_Unknown:
					std::cout << "*";
					break;
				case karto::GridStates_Occupied:
					std::cout << "X";
					break;
				case karto::GridStates_Free:
					std::cout << " ";
					break;
				default:
					std::cout << "?";
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}




int main()
{

	// use try/catch to catch karto exceptions that might be thrown by the karto subsystem. 
	/////////////////////////////////////
	// Get karto default mapper
	karto::Mapper* pMapper = new karto::Mapper();
	if (pMapper != NULL)
	{
		karto::OccupancyGrid* pOccupancyGrid = NULL;

		/////////////////////////////////////
		// sample code that creates a map from sample device and sample localized range scans

		std::cout << "Tutorial 1 ----------------" << std::endl << std::endl;

		// clear mapper
		pMapper->Reset();

		// create map from created dataset
		karto::Dataset* pDataset = CreateMap(pMapper);

		// create occupancy grid at 0.1 resolution and print grid
		pOccupancyGrid = CreateOccupancyGrid(pMapper, 0.1);
		PrintOccupancyGrid(pOccupancyGrid);
		delete pOccupancyGrid;

		// delete mapper
		delete pMapper;

		delete pDataset;
	}

	return 0;

	return 0;
}