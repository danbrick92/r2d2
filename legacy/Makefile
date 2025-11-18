build:
	sudo docker build -t ros2

run:
	sudo xhost +local:root
	sudo docker run -it --rm -e DISPLAY=:10.0 -v /tmp/.X11-unix:/tmp/.X11-unix ros2

run-mac:
	xhost + 127.0.0.1
	socat TCP-LISTEN:6000,reuseaddr,fork UNIX-CLIENT:\"$DISPLAY\" &
	export IP=$(ipconfig getifaddr en0)
	docker run -it --rm --entrypoint bash -e DISPLAY=$IP:0 ros2


run-interactive:
	sudo xhost +local:root
	sudo docker run -it --rm --entrypoint bash -e DISPLAY=:10.0 -v /tmp/.X11-unix:/tmp/.X11-unix ros2

run-turtlesim-app:
	sudo xhost +local:root
	sudo docker run -it --rm --entrypoint bash -e DISPLAY=:10.0 -v /tmp/.X11-unix:/tmp/.X11-unix ros2 -c "source ./ros_entrypoint.sh && ros2 run turtlesim turtlesim_node"

run-turtlesim-control:
	sudo xhost +local:root
	sudo docker run -it --rm --entrypoint bash -e DISPLAY=:10.0 -v /tmp/.X11-unix:/tmp/.X11-unix ros2 -c "source ./ros_entrypoint.sh && ros2 run turtlesim turtle_teleop_key"
