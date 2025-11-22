import asyncio
import logging
import time
from typing import Any, Callable, Dict, Optional

from r2d2.utils.logging import LoggerMixin


class LoopOperationMixin(LoggerMixin):
    async def loop(
        self,
        async_func: Callable,
        rate: float,
        catchup: bool = False,
        kwargs: Optional[Dict[str, Any]] = None,
    ) -> None:
        if kwargs is None:
            kwargs = {}

        while True:
            start = time.time()
            await async_func(**kwargs)
            elapsed = time.time() - start
            to_sleep = rate - elapsed
            if to_sleep > 0:
                await asyncio.sleep(to_sleep)
            else:
                self.log(
                    "Rate too fast",
                    details={"rate": rate, "to_sleep": to_sleep},
                    level=logging.WARNING,
                )
                if catchup:
                    await self.catchup()

    async def catchup(self) -> None:
        raise NotImplementedError("catchup used but not implemented")
