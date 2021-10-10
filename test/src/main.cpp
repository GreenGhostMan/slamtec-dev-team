#include <rpos/robot_platforms/slamware_core_platform.h>
#include <iostream>
#include <rpos/features/motion_planner/feature.h>
#include <rpos/core/pose.h>
#include <chrono>
#include <rpos/core/geometry.h>
#include <regex>

std::string ip_address = "";
const char *ip_regex ="\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}"; //192.168.100.61 ip
const double pi = 3.1415927;

void Help(std::string application_name) {
	std::cout<<"Slamware console"<<std::endl << 
		"Usage"<<application_name<<"<slamware_address>"<<std::endl;
	
}
bool ParseCommandLine(int argc, const char *argv[])
{
	bool show_help = false;
	for(int pos = 1; pos<argc; pos++) {
		const char *current = argv[pos];
		if (strcmp(current, "-h") == 0)
			show_help = true;
	    else 
			ip_address = current;		
	}
	std::regex reg(ip_regex);
	if (!show_help && !std::regex_match(ip_address, reg)) 
		show_help = true;
	if (show_help) {
		Help("move_to_spot");
		return false;
	}
	return true;
}
int main(int argc, const char* argv[]) {
	if(!ParseCommandLine(argc,argv))
	return 1;
	std::cout<<"Connecting to "<<ip_address<<">>>>"<<std::endl;
	try {
		rpos::robot_platforms::SlamwareCorePlatform sdp = rpos::robot_platforms::SlamwareCorePlatform::connect(ip_address,1445);
		std::cout<<"SDK Version"<<sdp.getSDKVersion()<<std::endl;
		std::cout<<"SDP Version"<<sdp.getSDPVersion()<<std::endl;
		rpos::actions::MoveAction action = sdp.getCurrentAction();
		if(action)
			action.cancel();
		rpos::features::motion_planner::MoveOptions options;
		options.flag = rpos::features::motion_planner::MoveOptionFlag( rpos::features::motion_planner::MoveOptionFlagMilestone |  rpos::features::motion_planner::MoveOptionFlagPrecise);
		action = sdp.moveTo(rpos::core::Location(2,0),options); // a loot g twr ml :3
		action.waitUntilDone();
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