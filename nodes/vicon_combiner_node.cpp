#include <ros/ros.h>
#include <visualization_msgs/MarkerArray.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Point.h>

ros::Publisher combined_pub;
visualization_msgs::MarkerArray last_unlabeled;

void unlabeledCallback(const visualization_msgs::MarkerArray::ConstPtr& msg)
{
    last_unlabeled = *msg;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "vicon_combiner_node");
    ros::NodeHandle nh;

    ros::Subscriber unlabeled_sub = nh.subscribe("/vicon/unlabeled_markers", 1, unlabeledCallback);
    combined_pub = nh.advertise<visualization_msgs::MarkerArray>("/vicon/combined_markers", 1);

    tf::TransformListener tf_listener;

    ros::Rate rate(50);
    while (ros::ok())
    {
        ros::spinOnce();

        visualization_msgs::MarkerArray combined;

        // Copia i marker non etichettati (verdi)
        for (auto marker : last_unlabeled.markers)
        {
            marker.ns = "unlabeled_markers";
            marker.color.r = 0.0;
            marker.color.g = 1.0;
            marker.color.b = 0.0;
            marker.color.a = 1.0;
            combined.markers.push_back(marker);
        }

        // Aggiungi il marker "hand" (blu)
        tf::StampedTransform transform;
        try
        {
            tf_listener.lookupTransform("vicon_world", "hand", ros::Time(0), transform);

            visualization_msgs::Marker hand_marker;
            hand_marker.header.frame_id = "vicon_world";
            hand_marker.header.stamp = ros::Time::now();
            hand_marker.ns = "hand";
            hand_marker.id = 1000;
            hand_marker.type = visualization_msgs::Marker::SPHERE;
            hand_marker.action = visualization_msgs::Marker::ADD;
            hand_marker.pose.position.x = transform.getOrigin().x();
            hand_marker.pose.position.y = transform.getOrigin().y();
            hand_marker.pose.position.z = transform.getOrigin().z();
            hand_marker.pose.orientation.x = 0.0;
            hand_marker.pose.orientation.y = 0.0;
            hand_marker.pose.orientation.z = 0.0;
            hand_marker.pose.orientation.w = 1.0;
            hand_marker.scale.x = 0.01;
            hand_marker.scale.y = 0.01;
            hand_marker.scale.z = 0.01;
            hand_marker.color.r = 0.0;
            hand_marker.color.g = 0.0;
            hand_marker.color.b = 1.0;
            hand_marker.color.a = 1.0;
            hand_marker.lifetime = ros::Duration(0.1);

            combined.markers.push_back(hand_marker);
        }
        catch (tf::TransformException &ex)
        {
            ROS_WARN_THROTTLE(1.0, "TF exception for 'hand': %s", ex.what());
        }

        combined_pub.publish(combined);
        rate.sleep();
    }

    return 0;
}
