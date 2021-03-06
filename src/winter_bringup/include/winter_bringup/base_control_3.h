#ifndef BASE_CONTROL_H
#define BASE_CONTROL_H

#include <unistd.h>
#include <math.h>
#include <string.h> //for memset

#include <iostream>
#include <string> //for string
#include <queue>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h> // for tf

//角度数据常数
#define ANGLE_K M_PI/(180.0*10)

/*
一个脉冲走多远的距离(m/pulse)：
编码器一圈的脉冲数目：pulse=600
电机和编码器的转速比 4:3
轮子的半径：2 * M_PI * R = 0.4823
odom_k = 2*M_PI*R
*/
#define ODOM_K (0.4823/600.0)

using namespace std;
using namespace boost::asio;

enum Wheel{
    LeftWheel,
    RightWheel
};

//全局变量
bool sig_flag;

class BaseControl{

private:

    /*服务*/
    io_service iosev;

    /*串口对象*/
    serial_port sp;

    /*定时器*/
    deadline_timer timer;

    ros::NodeHandle nh;

    ros::Publisher odom_pub;

    ros::Subscriber cmd_sub;

    ros::Time current_time, last_time;

    tf::TransformBroadcaster odom_broadcaster;
        
    geometry_msgs::Twist cmd_msg;

    string str;

    boost::asio::streambuf rec_buf;

    float velocity[2];//m/s

    int encoder_curr[2];

    int encoder_last[2];

    float angle_curr;

    float angle_last;

    bool FistTime;

    bool data_ready_flag;

    float x_pos;

    float y_pos;

    float th_pos;

    std::string usb_device_;

    vector<char> rec_vec;

    std::size_t bytes_read;

    //速度发布50HZ的情况下，应低于20ms
    //数据类型由launch文件决定，是std::string,double,int，不能用unsigned int ,float,etc
    int deadline_time_;

    int baudrate_;

    int pulse_per_rotation_;

    int ratio_;

    int loop_rate_;

    double wheel_radius_;

    double wheels_separation_;

    float left_temp, right_temp;

public:

    /*构造函数*/
    BaseControl();

    ~BaseControl(){}

    /*初始化串口*/
    void InitSerial();

    /*设置速度*/
    void SetSpeed();

    /*获取推到位置姿态的数据（编码器脉冲数和角度）*/
    void GetPoseData();

    /*解析串口数据*/
    void ParseSerial();

    /*发布odom*/
    void PublishOdom();

    /*读取串口处理函数*/
    void HandleRead(boost::system::error_code ec, std::size_t bytes_transferred);

    void CmdCallback(const geometry_msgs::TwistConstPtr& cmd_msg);

    static void SigHandler(int sig);

    void ShutdownRobot();

};
#endif 
