FROM ros:jazzy-perception

# Install turtlesim
RUN sudo apt update
RUN sudo apt install ros-jazzy-turtlesim
RUN sudo apt install -y '~nros-jazzy-rqt*'

# turtlesim run
# ros2 run turtlesim turtlesim_node

# second terminal
# ./ros_entrypoint.sh
# ros2 run turtlesim turtle_teleop_key

# 
# RUN sudo apt update

# Entrypoint
ENTRYPOINT ./ros_entrypoint.sh

## TO BUILD: sudo docker build -t foo
## TO RUN INTERACTIVE: sudo docker run -it --rm --entrypoint bash -e DISPLAY=:10.0 -v /tmp/.X11-unix:/tmp/.X11-unix foo
## IF QT ISSUES: sudo xhost +local:root
## TO RUN TURTLESIM APP: source ./ros_entrypoint.sh && ros2 run turtlesim turtlesim_node
## TO RUN TURTLESIM CONTROLLER: source ./ros_entrypoint.sh && ros2 run turtlesim turtle_teleop_key
