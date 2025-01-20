build:
	sudo docker build -t foo

run:
	sudo xhost +local:root
	sudo docker run -it --rm -e DISPLAY=:10.0 -v /tmp/.X11-unix:/tmp/.X11-unix foo

run-interactive:
	sudo xhost +local:root
	sudo docker run -it --rm --entrypoint bash -e DISPLAY=:10.0 -v /tmp/.X11-unix:/tmp/.X11-unix foo

run-turtlesim-app:
	sudo xhost +local:root
	sudo docker run -it --rm --entrypoint bash -e DISPLAY=:10.0 -v /tmp/.X11-unix:/tmp/.X11-unix foo -c "source ./ros_entrypoint.sh && ros2 run turtlesim turtlesim_node"

run-turtlesim-control:
	sudo xhost +local:root
	sudo docker run -it --rm --entrypoint bash -e DISPLAY=:10.0 -v /tmp/.X11-unix:/tmp/.X11-unix foo -c "source ./ros_entrypoint.sh && ros2 run turtlesim turtle_teleop_key"

