#
# Conftest module (e2e python test).
#

import subprocess
import time
import pytest

@pytest.fixture(scope="session", autouse=True)
def backend_server():
    process = subprocess.Popen([
        "~/roomsched/build/tests/backend_tests"
    ])

    time.sleep(2)

    yield

    process.terminate()
