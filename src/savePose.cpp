#include <tf/transform_broadcaster.h>
#include <yaml-cpp/yaml.h>
#include <execution>
#include <fstream>

#include "ros/ros.h"
#include <nav_msgs/Odometry.h>

class savePose 
{

private:
    ros::NodeHandle nh;
    ros::Subscriber odom_pose;

    std::ofstream PoseSaveFile;
    std::string filepath, filename;
    ros::Time ROSTIME_STAMP;

public:

    savePose() // 생성자 sub or pub 정보 넣음.
    {
        odom_pose = nh.subscribe<nav_msgs::Odometry>("/Odometry",5, &savePose::PoseCallBack, this, ros::TransportHints().tcpNoDelay());
       // ROS에서 제공하는 subscribe 함수.
       // nav_msgs::Odometry : 템플릿 매개변수. 메세지 타입을 넣으면 된다. 
       // "/Odometry" : 구독하려는 토픽의 이름
       // 5: queue의 크기
       // &savePose::PoseCallBack : 메세지를 받았을 때 호출될 콜백 함수의 주소를 나타냄. 메세지를 받으면 이 함수가 호출되어 처리됨. 앞의 savePose는 클래스의 이름임(생성자x) 클래스의 멤버 함수를 참조함.
       // this : 클래스의 현재 인스턴스. 이를 통해 콜백 함수에 클래스 인스턴스를 전달할 수 있음. 항상 있는듯. 
       // ros::TransportHints().tcpNodelay(): 통신 방식에 대한 힌트를 제공. tcpNoDelay()는 TCP소켓에서 Nagle알고리즘을 비활성화하는 옵션. 이를 통해 메시지이 전송 지연을 최소화할수있음.

    }
    void PoseCallBack(const nav_msgs::Odometry::ConstPtr& Odom)
    // const nav_msgs::Odometry::ConstPtr& >> "매개변수 타입"은 nav_msgs::Odometry 메세지의 상수 포인터 참조. 항상 이렇게 포인터 참조를 하는듯. 포인터 참조를 해야 -> 로 정보 접근이 가능함.
    // Odom : 함수의 매개변수 이름. Odom은 받은 nav_msgs::Odometry 메세지를 나타내며, 이를 통해 pose 정보와 속도 정보를 추출할 수 있음.
    {
        ROSTIME_STAMP = Odom -> header.stamp; 
        // ROSTIME_STAMP를 사용하는 이유 : 가독성과 일관성 유지를 위함. 또한 시간 정보 처리와 관련된 ROS 기능과의 상호 작용을 쉽게 할 수 있음. 
        // 예를 들어 시간 정보를 다른 함수로 전달하거나, 시간 연산을 수행하는 경우에도 ros::Time 이편함.
    

        // FILE PATH//
        filepath = "/home/eunseong/catkin_ws/src/faster-lio/";
    

        filename = "result";

        filepath = filepath + filename + ".txt";

        float p_x = Odom->pose.pose.position.x;
        float p_y = Odom->pose.pose.position.y;
        float p_z = Odom->pose.pose.position.z;
        float r_x = Odom->pose.pose.orientation.x;
        float r_y = Odom->pose.pose.orientation.y;
        float r_z = Odom->pose.pose.orientation.z;
        float r_w = Odom->pose.pose.orientation.w;
    
        
        PoseSaveFile.open (filepath, std::ios::out |std::ios::app);
        
        PoseSaveFile << ROSTIME_STAMP
                     << " " << p_x
                     << " " << p_y
                     << " " << p_z
                     << " " << r_x
                     << " " << r_y
                     << " " << r_z
                     << " " << r_w << std::endl;
        
        
        PoseSaveFile.close();
    }

};


int main(int argc, char** argv)
{
    ros::init(argc, argv, "pose_saver");
  
    savePose pose; 
    
    ROS_INFO("\033[1;32m---->\033[0m Pose Started.");
    ros::spin(); 
    // ros::spin() : 메세지가 도착할 때마다 콜백 함수를 호출함, 이를 통해 메시지 처리를 계속 수행할 수 있다. 
    return 0;
}
