#include <rpos/robot_platforms/slamware_core_platform.h>
#include <iostream>
#include <rpos/features/motion_planner/feature.h>
#include <rpos/core/pose.h>
#include <chrono>
#include <rpos/core/geometry.h>
#include <regex>

double pi = 3.1415927;

int main(int argc, const char* argv[]) {

	try {
		rpos::robot_platforms::SlamwareCorePlatform sdp = rpos::robot_platforms::SlamwareCorePlatform::connect(argv[1],1445);
		std::cout<<"SDK Version"<<sdp.getSDKVersion()<<std::endl;
		std::cout<<"SDP Version"<<sdp.getSDPVersion()<<std::endl;

		rpos::core::Pose pose = sdp.getPose();
		std::cout << "Robot Before Pose: " << std::endl;
		std::cout << "x: " << pose.x() << ", ";
		std::cout << "y: " << pose.y() << ", ";
		std::cout << "yaw: " << pose.yaw() << std::endl;
        
		rpos::actions::MoveAction action = sdp.getCurrentAction();
		if(action)
			action.cancel();
		rpos::features::motion_planner::MoveOptions options;
		options.flag = rpos::features::motion_planner::MoveOptionFlag( rpos::features::motion_planner::MoveOptionFlagMilestone |  rpos::features::motion_planner::MoveOptionFlagPrecise);
		std::cout<<"Ready!!! "<<std::endl;
        double target=0;
		// action = sdp.moveTo(rpos::core::Location(-0.3,0),options,0); // a loot g twr ml :3
		 action = sdp.rotateTo(rpos::core::Rotation(target,0,0));

		std::cout<<" Rotating "<<std::endl;
		action.waitUntilDone();
		std::cout<<" Finished "<<std::endl;

		rpos::core::Pose pose2 = sdp.getPose();
		std::cout << "Robot after Pose: " << std::endl;
		std::cout << "x: " << pose2.x() << ", ";
		std::cout << "y: " << pose2.y() << ", ";
		std::cout << "yaw: " << pose2.yaw() << std::endl;

		float error=target-pose2.yaw();
        if(!(error<0.2 && error>-0.2)){
			if(error>=0.2){
			  action = sdp.rotateTo(rpos::core::Rotation(target+0.54,0,0));
			}
			else if(error<=-0.2){
			  action = sdp.rotateTo(rpos::core::Rotation(target-0.54,0,0));
			}
            
		}
			action.waitUntilDone();
		rpos::core::Pose pose3 = sdp.getPose();
		std::cout << "Robot After Correction: " << std::endl;
		std::cout << "x: " << pose3.x() << ", ";
		std::cout << "y: " << pose3.y() << ", ";
		std::cout << "yaw: " << pose3.yaw() << std::endl;

		if(action.getStatus()==rpos::core::ActionStatusError)
			std::cout<<"Action Failed: "<<action.getReason()<<std::endl;
		// //draw a virtual track from (0, 0) to (2, 0), then move to (0, 0) via virtual track
		// rpos::core::Line line(rpos::core::Point(0,0),rpos::core::Point(2,0));
		// sdp.addLine(rpos::features::artifact_provider::ArtifactUsageVirtualTrack, line);
		// options.flag =  rpos::features::motion_planner::MoveOptionFlag( rpos::features::motion_planner::MoveOptionFlagKeyPoints |  rpos::features::motion_planner::MoveOptionFlagPrecise);
		// action = sdp.moveTo(rpos::core::Location(0, 0), options);
		// action.waitUntilDone();
		// if (action.getStatus() == rpos::core::ActionStatusError)
		// 	std::cout << "Action Failed: " << action.getReason() << std::endl;
	}
	catch (const rpos::system::detail::ExceptionBase& e) {		
		std::cout << e.what() << std::endl;
		return 1;	
	}
	return 0;
} 