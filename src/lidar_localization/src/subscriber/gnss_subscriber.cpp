/*
 * @Description: 订阅GNSS信息，并解析数据
 * @Author: Xiangmy
 * @Date: 2021-08-31
 */

#include "lidar_localization/subscriber/gnss_subscriber.hpp"

#include "glog/logging.h"

namespace lidar_localization
{
    GNSSSubscriber::GNSSSubscriber(ros::NodeHandle &nh, std::string topic_name, size_t buff_size)
        : nh_(nh)
    {
        subscriber_ = nh_.subscribe(topic_name, buff_size, &GNSSSubscriber::msg_callback, this);
    }

    // 接收数据
    void GNSSSubscriber::msg_callback(const sensor_msgs::NavSatFixConstPtr &nav_sat_fix_ptr)
    {
        GNSSData gnss_data;
        gnss_data.time = nav_sat_fix_ptr->header.stamp.toSec();
        gnss_data.latitude = nav_sat_fix_ptr->latitude;
        gnss_data.longitude = nav_sat_fix_ptr->longitude;
        gnss_data.altitude = nav_sat_fix_ptr->altitude;
        gnss_data.status = nav_sat_fix_ptr->status.status;
        gnss_data.service = nav_sat_fix_ptr->status.service;

        new_gnss_data_.push_back(gnss_data);
    }

    // 解析数据
    void GNSSSubscriber::ParseData(std::deque<GNSSData> &gnss_data_buff)
    {
        if (new_gnss_data_.size() > 0)
        {
            gnss_data_buff.insert(gnss_data_buff.end(), new_gnss_data_.begin(), new_gnss_data_.end());
            new_gnss_data_.clear();
        }
    }
}