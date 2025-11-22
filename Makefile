install-mosquito-mac:
	brew install mosquito

run-mosquito:
	/opt/homebrew/opt/mosquitto/sbin/mosquitto -c /opt/homebrew/etc/mosquitto/mosquitto.conf

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
