#include <rpos/robot_platforms/slamware_core_platform.h>
#include <iostream>
#include <rpos/features/motion_planner/feature.h>
#include <rpos/core/pose.h>
#include <chrono>
#include <rpos/core/geometry.h>


using namespace rpos::robot_platforms;

int main(int argc,const char **argv){
	SlamwareCorePlatform platform = SlamwareCorePlatform::connect(argv[1], 1445);
	//rpos::features::motion_planner::MoveOptionFlag MoveOptionFlagWithYaw ;

	 std::vector<rpos::core::Location> pointsToGo;
     pointsToGo.push_back(rpos::core::Location(2,0));
    
	rpos::actions::MoveAction moveAction = platform.getCurrentAction();
	
	if(moveAction.getStatus() == rpos::core::ActionStatusFinished){
        
		//rpos::actions::MoveAction moveBy = platform.moveBy(direction);
		//moveAction = platform.moveBy(start, false, true);
		moveAction = platform.moveTo(pointsToGo, false, true);
		
		//moveAction = platform.moveTo(pointsToGo, false, true);
		//moveAction = platform.moveTo(pointsToGo, false, true);

		while(!(moveAction.getStatus()== rpos::core::ActionStatusFinished)){
			if(moveAction.getStatus() == rpos::core::ActionStatusWaitingForStart){
				std::cout<<"WAITING TO START"<<std::endl;
			}
			else if(moveAction.getStatus() == rpos::core::ActionStatusRunning){
				rpos::core::Location location = platform.getLocation();
                std::cout << "Robot Location: " << std::endl;
                std::cout << "x: " << location.x() << ", ";
                std::cout << "y: " << location.y() << std::endl;

                rpos::core::Pose pose = platform.getPose();
                std::cout << "Robot Pose: " << std::endl;
                std::cout << "x: " << pose.x() << ", ";
                std::cout << "y: " << pose.y() << ", ";
                std::cout << "yaw: " << pose.yaw() << std::endl;
				std::cout<<"RUNNING"<<std::endl;
			}
			else if(moveAction.getStatus() == rpos::core::ActionStatusPaused){
				std::cout<<"PAUSED"<<std::endl;
			}
			else if(moveAction.getStatus() == rpos::core::ActionStatusStopped){
				std::cout<<"STOPPED"<<std::endl;
			}
			else if(moveAction.getStatus() == rpos::core::ActionStatusError){
				std::cout<<"ERROR"<<std::endl;
			}		
		}
		std::cout<<"OK Finished"<<std::endl;
	}

	return 0;
}