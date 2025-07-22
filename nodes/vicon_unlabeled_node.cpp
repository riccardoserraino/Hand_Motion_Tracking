#include <ros/ros.h>
#include <visualization_msgs/MarkerArray.h>
#include <tf/transform_broadcaster.h>
#include "DataStreamClient.h"
#include <unistd.h>

using namespace ViconDataStreamSDK::CPP;

int main(int argc, char** argv) {
    ros::init(argc, argv, "vicon_unlabeled_node");
    ros::NodeHandle nh;

    ros::Publisher marker_pub = nh.advertise<visualization_msgs::MarkerArray>("vicon/unlabeled_markers", 10);
    static tf::TransformBroadcaster br;

    Client client;
    std::string hostname = "192.168.7.97:801";  // IP del Vicon PC

    std::cout << "Connecting to Vicon Tracker at " << hostname << "..." << std::endl;
    while (!client.IsConnected().Connected && ros::ok()) {
        client.Connect(hostname);
        sleep(1);
    }
    std::cout << "Connected to Vicon Tracker!" << std::endl;

    client.EnableUnlabeledMarkerData();
    client.EnableSegmentData();  // Per soggetti tracciati
    client.SetStreamMode(StreamMode::ClientPull);
    client.SetAxisMapping(Direction::Forward, Direction::Left, Direction::Up);

    ros::Rate rate(30);
    while (ros::ok()) {
        client.GetFrame();

        visualization_msgs::MarkerArray marker_array;

        // --- Unlabeled markers ---
        unsigned int count = client.GetUnlabeledMarkerCount().MarkerCount;
        for (unsigned int i = 0; i < count; ++i) {
            auto marker = client.GetUnlabeledMarkerGlobalTranslation(i);

            visualization_msgs::Marker m;
            m.header.frame_id = "vicon_world";
            m.header.stamp = ros::Time::now();
            m.ns = "unlabeled_markers";
            m.id = i;
            m.type = visualization_msgs::Marker::SPHERE;
            m.action = visualization_msgs::Marker::ADD;
            m.pose.position.x = marker.Translation[0] / 1000.0;
            m.pose.position.y = marker.Translation[1] / 1000.0;
            m.pose.position.z = marker.Translation[2] / 1000.0;
            m.pose.orientation.w = 1.0;
            m.scale.x = 0.01;
            m.scale.y = 0.01;
            m.scale.z = 0.01;
            m.color.r = 0.0;
            m.color.g = 1.0;
            m.color.b = 0.0;
            m.color.a = 1.0;
            m.lifetime = ros::Duration(0.1);
            marker_array.markers.push_back(m);
        }

        // --- Subjects (e.g. hand) ---
        unsigned int subjectCount = client.GetSubjectCount().SubjectCount;
        int marker_id_offset = 1000;

        for (unsigned int i = 0; i < subjectCount; ++i) {
            std::string subjectName = client.GetSubjectName(i).SubjectName;
            unsigned int segmentCount = client.GetSegmentCount(subjectName).SegmentCount;

            for (unsigned int j = 0; j < segmentCount; ++j) {
                std::string segmentName = client.GetSegmentName(subjectName, j).SegmentName;
                auto segmentPosition = client.GetSegmentGlobalTranslation(subjectName, segmentName);

                // tf only for "hand"
                if (subjectName == "hand") {
                    tf::Transform transform;
                    transform.setOrigin(tf::Vector3(
                        segmentPosition.Translation[0] / 1000.0,
                        segmentPosition.Translation[1] / 1000.0,
                        segmentPosition.Translation[2] / 1000.0));

                    auto rot = client.GetSegmentGlobalRotationQuaternion(subjectName, segmentName);
                    tf::Quaternion q(
                        rot.Rotation[0],
                        rot.Rotation[1],
                        rot.Rotation[2],
                        rot.Rotation[3]);
                    transform.setRotation(q);

                    br.sendTransform(tf::StampedTransform(
                        transform,
                        ros::Time::now(),
                        "vicon_world",
                        "hand"
                    ));
                }
            }
        }

        marker_pub.publish(marker_array);
        ros::spinOnce();
        rate.sleep();
    }

    client.Disconnect();
    return 0;
}
