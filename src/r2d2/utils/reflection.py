import importlib
from typing import Any, Dict, Optional


def load_fqcn(
    fqcn: str, kwargs: Optional[Dict] = None, as_instance: bool = True
) -> Any:
    module_name, class_name = fqcn.rsplit(".", 1)

    # Get the module
    try:
        module = importlib.import_module(module_name)
    except ImportError as e:
        raise ImportError(f"Could not import module: {module_name}") from e

    # Get the class
    try:
        cls = getattr(module, class_name)
    except AttributeError as e:
        raise AttributeError(
            f"Could not find class '{class_name}' in module '{module_name}'"
        ) from e

    if not as_instance:
        return cls

    if kwargs is None:
        kwargs = {}
    return cls(**kwargs)
