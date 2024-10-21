FROM ros:jazzy-perception

# Install turtlesim
RUN sudo apt update
RUN sudo apt install ros-jazzy-turtlesim

# turtlesim run
# ros2 run turtlesim turtlesim_node

# second terminal
# ./ros_entrypoint.sh
# ros2 run turtlesim turtle_teleop_key
