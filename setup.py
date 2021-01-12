import os
import pathlib
import sys

from setuptools import setup
from distutils.sysconfig import get_config_vars
from pybind11.setup_helpers import Pybind11Extension


def get_macOS_deployment_target():
        macos_target = get_config_vars()["MACOSX_DEPLOYMENT_TARGET"]
        macos_target = tuple(int(x) for x in macos_target.split("."))
        if macos_target < (10, 15):
            macos_target = (10, 15)
        return ".".join(str(x) for x in macos_target)


def get_extensions():
    # on macOS std::filesystem requires Catalina (10.15) or later.
    if sys.platform.startswith("darwin"):
        os.environ["MACOSX_DEPLOYMENT_TARGET"] = get_macOS_deployment_target()
    return [
        Pybind11Extension(
            "stdfs",
            [os.path.join("src", "stdfs.cpp")],
            cxx_std=17,
        ),
    ]


setup(
    version="0.1",
    ext_modules=get_extensions(),
    zip_safe=False,
)
