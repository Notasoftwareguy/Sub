from launch import LaunchDescription
from launch_ros.actions import Node

import os

def generate_launch_description():
    os.system("sudo ip addr add 192.168.10.2/24 dev eth0")
    os.system("sudo ip addr add 192.168.10.2/24 dev eth0")
    os.system("sudo ip addr add 192.168.10.2/24 dev eth0")
    os.system("sudo ip link set eth0 up")

    return LaunchDescription([
        Node(
            package='sub_computer',
            executable='surface_publisher',
            name='surface_publisher',
        ),
        Node(
            package='sub_computer',
            executable='move_motors',
            name='move_motors',
        ),
        Node(
            package='sub_computer',
            executable='talk_to_micro',
            name='talk_to_micro',
        ),
        Node(
            package='sub_computer',
            executable='video_streamer',
            name='video_streamer',
        ),
    ])