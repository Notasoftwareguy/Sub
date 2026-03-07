import rclpy
from rclpy.node import Node
import os

class VideoStreamer(Node):
    def __init__(self):
        super().__init__('video_streamer')
        self.get_logger().info('Starting video stream')
        self.start_video()
    
    def start_video(self):
        os.system("gst-launch-1.0 v4l2src device=/dev/video0 ! videoconvert ! x264enc tune=zerolatency bitrate=2000 speed-preset=ultrafast ! rtph264pay config-interval=1 pt=96 ! udpsink host=192.168.10.1 port=5000")

    def stop_video(self):
        pass




def main(args=None):
    rclpy.init(args=args)
    node = VideoStreamer()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.stop_video()
    finally:
        rclpy.shutdown()


if __name__ == '__main__':
    main()