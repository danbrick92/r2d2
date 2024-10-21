# Update 
sudo apt update

# Install docker
sudo apt install docker.io

# Pull the correct ROS2 images
sudo docker pull ros:jazzy-perception

# Allow docker to run xhost
xhost +local:docker

# Run the docker container
sudo docker run -it --rm --env="DISPLAY=$DISPLAY" --env="QT_X11_NO_MITSHM=1" --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" ros:jazzy-perception