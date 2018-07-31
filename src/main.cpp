#include <snakeHW.h>
#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <iostream>

int main(int argc, char **argv)
{
	ros::init(argc, argv, "snakeHW");
	ros::NodeHandle snake_nh;

	SnakeHW snakeHW(snake_nh);

	ros::NodeHandle cm_nh;
	ros::CallbackQueue cm_callback_queue;
	cm_nh.setCallbackQueue(&cm_callback_queue);

	controller_manager::ControllerManager cm(&snakeHW, cm_nh);

	ROS_INFO("%f",snakeHW.getFreq());
	ros::Rate rate(snakeHW.getFreq());

	// ros::AsyncSpinner hw_spinner(1, snakeHW.getCallbackQueue());
	ros::AsyncSpinner cm_spinner(1, &cm_callback_queue);
	// hw_spinner.start();
	cm_spinner.start();


	bool index = 0;
	int i = 0;
	while (ros::ok())
	{
		ros::Time currentTime = ros::Time::now();

		snakeHW.read(currentTime, ros::Duration(1 / snakeHW.getFreq()));
		cm.update(currentTime, ros::Duration(1 / snakeHW.getFreq()));
		snakeHW.write(currentTime, ros::Duration(1 / snakeHW.getFreq()), index);

		index = !index;
		std::cout << "round " << ++i << ", index " << index << std::endl;
		
		rate.sleep();
	}
	cm_spinner.stop();
	return 0;
}
