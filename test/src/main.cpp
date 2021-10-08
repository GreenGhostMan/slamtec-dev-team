#include <iostream>
#include <rpos/robot_platforms/slamware_core_platform.h>
#include <rpos/core/detail/geometry_rectangle.h>
//#include <Vector2.hpp>
//using namespace rpos::robot_platforms;

int main(int argc, char * argv[])
{
	//rpos::core::detail::RectangleF<float> rect1(Vector2f,Vector2f);
	rpos::core::RectangleF rect1(10.0, 10.0, 100.0, 50.0);
	auto position = rect1.position();
	auto size = rect1.size();
	float x = rect1.x(); float y = rect1.y(); float width = rect1.width(); float height = rect1.height();

	rpos::core::RectangleF rect2( 15.0, 15.0, 20, 20);
	if( rect1.contains(rect2) ) 
	{
		std::cout << "point or rectangle contains in this rect1" << std::endl;
	}

	return 0;
}
