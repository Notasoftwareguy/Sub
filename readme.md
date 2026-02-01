
# Sub Controller

## Assigning Static IPs (until its done automatically)

- Must be done every reboot for both master and slave
- Pi uses eth0 as the device, desktop uses enp5s0
- last digit of ip needs to be different

### Built in ethernet port

sudo ip addr add 192.168.10.1/24 dev enp5s0

sudo ip link set enp5s0 up

### USB hub

sudo ip addr add 192.168.10.1/24 dev enx00e04c1a1588

sudo ip link set enx00e04c1a1588 up

### Deck with crappy adapter

sudo ip addr add 192.168.10.1/24 dev enp4s0f3u1u1

sudo ip link set enp4s0f3u1u1 up

### Pi

sudo ip addr add 192.168.10.2/24 dev eth0

sudo ip link set eth0 up

## Starting stream on Pi (until its automatic)

gst-launch-1.0 v4l2src device=/dev/video0 ! videoconvert ! x264enc tune=zerolatency bitrate=5000 speed-preset=ultrafast ! rtph264pay config-interval=1 pt=96 ! udpsink host=192.168.10.1 port=5000


# Sub Computer

## Building 

* Pretty much everything has to be done from the computer folder. That is the "ROS2 workspace"
* To do a generic build, use `colcon build`
* After every restart or if new nodes are added you must source `computer/install/setup.bash`

