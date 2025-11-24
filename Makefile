PORT ?= 1883

install-mqtt-broker:
	brew install mosquito

run-mqtt-broker:
	/opt/homebrew/opt/mosquitto/sbin/mosquitto -c /opt/homebrew/etc/mosquitto/mosquitto.conf

kill-mqtt-broker:
	@PID=$$(lsof -t -i tcp:$(PORT)); \
	if [ -z "$$PID" ]; then \
		echo "No process is running on port $(PORT)."; \
	else \
		echo "Process running on port $(PORT): PID $$PID"; \
		echo "Killing process..."; \
		kill -9 $$PID; \
		echo "Process $$PID terminated."; \
	fi

install-ffmpeg:
	brew install ffmpeg

create-venv:
	python3 -m venv venv && source venv/bin/activate && pip install -e ".[test]"

activate-venv:
	source venv/bin/activate

lint:
	echo "Running isort" && source venv/bin/activate && isort src/r2d2
	echo "Running black" && source venv/bin/activate && black src/r2d2
	echo "Running ruff" && source venv/bin/activate && ruff check src/r2d2
	echo "Running pylint" && source venv/bin/activate && pylint src/r2d2
	echo "Running mypy" && source venv/bin/activate && mypy src/r2d2

run:
	source venv/bin/activate && cd src/r2d2 && python main.py
