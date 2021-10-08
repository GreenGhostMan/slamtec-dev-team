#include <rpos/robot_platforms/slamware_core_platform.h>
#include <iostream>
#include <rpos/features/motion_planner/feature.h>
#include <rpos/core/pose.h>
#include <chrono>

using namespace std;
using namespace rpos::robot_platforms;

int main(int argc,const char **argv){
	SlamwareCorePlatform platform = SlamwareCorePlatform::connect(argv[1], 1445);
	rpos::features::motion_planner::MoveOptionFlag MoveOptionFlagWithYaw ;

	std::vector<rpos::core::Location> pointsToGo;
	pointsToGo.push_back(rpos::core::Location(0.5,0));

	rpos::actions::MoveAction moveAction = platform.getCurrentAction();

	if(moveAction.getStatus() == rpos::core::ActionStatusFinished){
		moveAction = platform.moveTo(pointsToGo, false, true);

		while(!(moveAction.getStatus()== rpos::core::ActionStatusFinished)){
			if(moveAction.getStatus() == rpos::core::ActionStatusWaitingForStart){
				cout<<"WAITING TO START"<<endl;
			}
			else if(moveAction.getStatus() == rpos::core::ActionStatusRunning){
				cout<<"RUNNING"<<endl;
			}
			else if(moveAction.getStatus() == rpos::core::ActionStatusPaused){
				cout<<"PAUSED"<<endl;
			}
			else if(moveAction.getStatus() == rpos::core::ActionStatusStopped){
				cout<<"STOPPED"<<endl;
			}
			else if(moveAction.getStatus() == rpos::core::ActionStatusError){
				cout<<"ERROR"<<endl;
			}		
		}
		cout<<"OK Finished"<<endl;
	}

	return 0;
}