#include <cstdio>
#include <cstring>
#include <string>

#include <rpos/rpos.h>
#include <rpos/robot_platforms/slamware_core_platform.h>
#include <rpos/features/location_provider/map.h>

#include "bitmap_image.hpp"						// C++ Bitmap library from http://www.partow.net 
#define DEFAULT_DUMPIMAGE_FILENAME "rom_home"
using namespace rpos::robot_platforms;
using namespace rpos::system::detail;

void displayMapInfo(rpos::features::location_provider::Map map);
void displayLocalizationInfo(const rpos::core::Pose &pose);
bool saveToTxt(const char *fliename, rpos::features::location_provider::Map map);
bool saveToBmp(const char *fliename, rpos::features::location_provider::Map map);

SlamwareCorePlatform sdp;

int main(int argc, char** argv)
{
	bool option_needShowHelp = false;
	bool option_noSave = false;
	bool option_txtFileMode = false;
	const char * option_sdpIPAddress = NULL;
	const char * option_dumpImageFilename = DEFAULT_DUMPIMAGE_FILENAME;
	
	// parse command line
	for(int currentArg = 1; currentArg < argc; ++currentArg)
	{
		if( strcmp(argv[currentArg], "-o") == 0 )
		{
			if(++currentArg < argc)						 			{ option_dumpImageFilename = argv[currentArg]; }
		}
		else if( strcmp( argv[currentArg], "-h"          ) == 0 )	{ option_needShowHelp = true; }
		else if( strcmp( argv[currentArg], "--txt-format") == 0 ) 	{ option_txtFileMode = true;  }
		else if( strcmp( argv[currentArg], "--info-only" ) == 0 )  	{ option_noSave = true; 	  }
		else if( strcmp( argv[currentArg], "-p"          ) == 0 )
		{
			if( ++currentArg < argc )								{ option_txtFileMode = true;  }
			else													{ option_needShowHelp= true;  }
		}
		else														{ option_sdpIPAddress= argv[currentArg]; } 	
	}

	try 
	{
		printf("Connecting to the SDP @ %s...\n", option_sdpIPAddress);
		sdp = rpos::robot_platforms::SlamwareCorePlatform::connect(option_sdpIPAddress, 1445);

		printf("Fetching Map info...\n");
		rpos::core::RectangleF knownArea = sdp.getKnownArea( rpos::features::location_provider::MapTypeBitmap8Bit, rpos::features::location_provider::EXPLORERMAP);
		rpos::features::location_provider::Map map = sdp.getMap( rpos::features::location_provider::MapTypeBitmap8Bit, knownArea, rpos::features::location_provider::EXPLORERMAP);
		displayMapInfo(map);

		printf("Fetching Localization info...\n");
		rpos::core::Pose robotPose = sdp.getPose();
		displayLocalizationInfo(robotPose);

		if( option_noSave ) 	{ return 0; }

		if( (option_txtFileMode)  && (!saveToTxt(option_dumpImageFilename, map)) ){ return -3;  }
		else { if( !saveToBmp(option_dumpImageFilename, map) ) 					  { return -3; }}
	}
	catch( rpos::system::detail::ExceptionBase& e ) {
		
	}

	return 0;
}

bool saveToBmp(const char *filename, rpos::features::location_provider::Map map)
{
    std::string finalFilename = filename;
    finalFilename += ".bmp";

    bitmap_image mapBitmap(map.getMapDimension().x(), map.getMapDimension().y());

    for (size_t posY = 0; posY < map.getMapDimension().y(); ++posY)
    {
        for (size_t posX = 0; posX < map.getMapDimension().x(); ++posX)
        {
            rpos::system::types::_u8 cellValue = ((rpos::system::types::_u8)128U) + map.getMapData()[posX + (map.getMapDimension().y()-posY-1) * map.getMapDimension().x()];
            mapBitmap.set_pixel(posX, posY, cellValue, cellValue, cellValue);
        }
    }
    mapBitmap.save_image(finalFilename);
    return true;
}

bool saveToTxt(const char *filename, rpos::features::location_provider::Map map)
{
    std::string finalFilename = filename;
    finalFilename += ".txt";

    FILE * fp = fopen(finalFilename.c_str(), "w");
    if (!fp) {
        fprintf(stderr, "Cannot create the file %s.\n", finalFilename.c_str());
        return false;
    }

    fprintf(fp, "# SLAMWARE MAP FILE\n");
    fprintf(fp, "Area: %.4f, %.4f, %.4f, %.4f\n", map.getMapArea().x(), map.getMapArea().y(), map.getMapArea().width(), map.getMapArea().height());
    fprintf(fp, "Cell Resolution: %.4f, %.4f\n", map.getMapResolution().x(), map.getMapResolution().y());
    fprintf(fp, "Cell Dimension: %lu, %lu\n", map.getMapDimension().x(), map.getMapDimension().y());


    for (size_t posY = 0; posY < map.getMapDimension().y(); ++posY)
    {
        for (size_t posX = 0; posX < map.getMapDimension().x(); ++posX)
        {
            fprintf(fp, "%d, ",  (char)map.getMapData()[posX + posY *map.getMapDimension().x() ]);

        }
        fprintf(fp, "\n");
    }

    fclose(fp);

    return true;
}

void displayMapInfo(rpos::features::location_provider::Map map)
{
    printf("> Map Area: (%.4f,%.4f,%.4f,%.4f)\n"
        , map.getMapArea().left()
        , map.getMapArea().top()
        , map.getMapArea().right()
        , map.getMapArea().bottom());

    printf("> Cell Dimension: (%u,%u)\n"
        , map.getMapDimension().x()
        , map.getMapDimension().y());

    printf("> Cell Resolution: (%.4f,%.4f)\n"
        , map.getMapResolution().x()
        , map.getMapResolution().y());

    printf("> Timestamp: %ull\n"
        , map.getMapTimestamp());
}

void displayLocalizationInfo(const rpos::core::Pose & pose)
{
    printf("> Position: (%.4f,%.4f,%.4f)\n"
        , pose.x(), pose.y(), pose.z());

    printf("> Heading: %.4f\n"
        , pose.yaw()*180.0f/M_PI);
}

