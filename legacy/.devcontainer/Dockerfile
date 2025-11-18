FROM ros:jazzy-perception

# Install packages
RUN apt update && \
    apt install -y ros-jazzy-turtlesim '~nros-jazzy-rqt*'

# Use default entrypoint from ROS image
CMD ["bash"]