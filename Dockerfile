FROM ros:jazzy-perception

# Install turtlesim
RUN sudo apt update
RUN sudo apt install ros-jazzy-turtlesim
RUN sudo apt install '~nros-jazzy-rqt*'

# turtlesim run
# ros2 run turtlesim turtlesim_node

# second terminal
# ./ros_entrypoint.sh
# ros2 run turtlesim turtle_teleop_key

# 
sudo apt update

# Entrypoint
ENTRYPOINT ./ros_entrypoint.sh
