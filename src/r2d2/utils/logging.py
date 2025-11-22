import logging
import json
import sys
from typing import Dict, Any, Optional


class JsonFormatter(logging.Formatter):
    def format(self, record: logging.LogRecord) -> str:
        log_record = {
            "level": record.levelname,
            "logger": record.name,
            "time": self.formatTime(record, self.datefmt),
        }

        message = record.getMessage()
        try:
            message_dict = json.loads(message)
            log_record.update(message_dict)
        except json.JSONDecodeError:
            log_record["message"] = message

        if record.exc_info:
            log_record["exception"] = self.formatException(record.exc_info)

        return json.dumps(log_record)


def setup_logging(level: int = logging.INFO) -> logging.Logger:
    logger = logging.getLogger()
    logger.setLevel(level)

    # Prevent duplicate handlers if called multiple times
    if not logger.handlers:
        console_handler = logging.StreamHandler(sys.stdout)
        console_handler.setLevel(level)
        console_handler.setFormatter(JsonFormatter())
        logger.addHandler(console_handler)

    return logger


class LoggerMixin:

    def whoami(self) -> str:
        return self.__class__.__name__

    def log(
        self,
        message: str,
        details: Optional[Dict[str, Any]] = None,
        error: Optional[Exception] = None,
        level: int = logging.INFO
    ) -> None:
        if not hasattr(self, "logger"):
            raise ValueError(f"{self.whoami()} must have self.logger variable")
        logger: logging.Logger = self.logger

        if details is None:
            details = {}

        details["message"] = message
        details["class"] = self.whoami()
        if error:
            details["error"] = error
        return logger.log(level=level, msg=details)
