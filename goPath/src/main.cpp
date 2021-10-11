#include <iostream>
#include <regex>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <rpos/robot_platforms/slamware_core_platform.h>
#include <rpos/features/location_provider/map.h>

std::string ipaddress = "";
const char *ipReg = "\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}";

bool parseCommandLine( int argc, const char* argv[] );
void showHelp(std::string appName);

int main(int argc, const char* argv[])
{   
    if( !parseCommandLine(argc,argv) ) { return 1; }

    std::vector<rpos::core::Location> pointsToGo;
    pointsToGo.push_back( rpos::core::Location(1,0) );
    pointsToGo.push_back( rpos::core::Location(2,0) );
    pointsToGo.push_back( rpos::core::Location(2,0) );
    pointsToGo.push_back( rpos::core::Location(4,0) );

    rpos::robot_platforms::SlamwareCorePlatform sdp;

    try{
        sdp = rpos::robot_platforms::SlamwareCorePlatform::connect( argv[1], 1445 );
        std::cout << "SDK Version: " << sdp.getSDKVersion() << std::endl;
        std::cout << "SDP Version: " << sdp.getSDPVersion() << std::endl;
    } catch( rpos::robot_platforms::ConnectionFailException& e) { 
        std::cout << e.what() << std::endl;
        return 1;
    } catch( rpos::robot_platforms::ConnectionTimeOutException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    std::cout << "Connection Successfully!" << std::endl;

    int i = 0;

    while(true)
    {
        try{
            rpos::actions::MoveAction move_action = sdp.getCurrentAction();

            if(move_action)
            {   // current အက်ရှင်ရှိလားစစ်မယ် ရှိရင် 2s စောင့်ပြီးမှ move မယ်။
                std::cout << (move_action.isEmpty()? "Empty" : "Non-Empty") << std::endl;
                std::cout << "Action ID: " << move_action.getActionId() << std::endl;
                std::cout << "Action Name: " << move_action.getActionName() << std::endl;

                boost::this_thread::sleep_for(boost::chrono::milliseconds(2000));

                if( move_action.getStatus()== rpos::core::ActionStatusFinished )
                {
                    move_action = sdp.moveTo( pointsToGo, false, true); // false appending and milestones true. ( location point as milestone)
                } 
                else { // current အက်ရှင် မရှိရင် လဲ 2s စောင့်ပြီးမှ move မယ်။
                    std::cout << "Empty" << std::endl;
                    std::cout << "Action Name: " << "EmptyAction" << std::endl;
                    boost::this_thread::sleep_for( boost::chrono::milliseconds(2000) );

                    move_action = sdp.moveTo( pointsToGo, false, true); 
                }
            }
            while(move_action) // action မပြီးသေးသရွေ့ ..
            {
                std::cout << (move_action.isEmpty()? "Empty" : "Non-Empty") << std::endl;
                std::cout << "Action ID: " << move_action.getActionId() << std::endl;
                std::cout << "Action Name: " << move_action.getActionName() << std::endl;
                boost::this_thread::sleep_for(boost::chrono::milliseconds(2000));

                if( i%4==2 )     {  move_action.cancel();  } // ? for remaining check path ? 
                else if ( i%4==3 )  
                {
                    std::cout << "Wait until done" << std::endl;
                    move_action.waitUntilDone();
                    std::cout << "Done" << std::endl;
                }

                boost::this_thread::sleep_for( boost::chrono::milliseconds(2000) );

                // get milestones
                rpos::features::motion_planner::Path milestones = move_action.getRemainingMilestones();
                std::vector<rpos::core::Location> points = milestones.getPoints();
                std::cout<< "Remaining Milestones: "<< std::endl;
                for( std::vector<rpos::core::Location>::const_iterator it=points.begin(); it != points.end(); it++) 
                {
                    std::cout << "(" << it->x() << ",";
                    std::cout << it->y() << ")" << std::endl;
                }

                boost::this_thread::sleep_for( boost::chrono::milliseconds(2000) );

                // get path
                rpos::features::motion_planner::Path path = move_action.getRemainingPath();
                std::vector<rpos::core::Location> locations = path.getPoints();
                if( locations.size() <= 0)  {  break;  }
                std::cout<< "Remaining Path: " << std::endl;
                for(std::vector<rpos::core::Location>::const_iterator it=locations.begin(); it!=locations.end(); it++)
                {
                    std::cout << "(" << it->x() << ",";
                    std::cout << it->y() << ")" << std::endl;
                }

                rpos::core::Location location = sdp.getLocation();
                std::cout << "Robot Location: " << std::endl;
                std::cout << "x: " << location.x() << ", ";
                std::cout << "y: " << location.y() << std::endl;

                rpos::core::Pose pose = sdp.getPose();
                std::cout << "Robot Pose: " << std::endl;
                std::cout << "x: " << pose.x() << ", ";
                std::cout << "y: " << pose.y() << ", ";
                std::cout << "yaw: " << pose.yaw() << std::endl;
                // location x,y နဲ့ Pose x,y တူ မတူ စစ်ရန်

            } // end while(move_action) loop
        } catch( rpos::robot_platforms::ConnectionFailException& e) { 
            std::cout << e.what() << std::endl;
            break;
        } catch( rpos::robot_platforms::RequestTimeOutException& e) {
            std::cout << e.what() << std::endl;
        }
        i++;
    }
    return 0;
}

bool parseCommandLine( int argc, const char* argv[] )
{
    bool option_show_help = false;
    for(int pos=1; pos<argc; ++pos)
    {
        const char* current = argv[pos];
        if( strcmp(current, "-h") == 0 )        { option_show_help = true; }
        else { ipaddress = current; }
    }

    std::regex reg(ipReg);
    if( !option_show_help  &&  std::regex_match(ipaddress, reg) )   { option_show_help = true; }
    if( option_show_help )  { showHelp( "goActionAndGetPath" );  return false; }
    return true;
}
void showHelp(std::string appName)
{
    std::cout << "SLAMWARE console demo." << std::endl;
    std::cout << "Usage: " << appName << "<slamware_ip_address>" << std::endl;
}
