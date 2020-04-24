import glob
import os
import re
import pathlib
import subprocess
import sys
import tempfile

import setuptools
from setuptools import setup
from setuptools.extension import Extension


def has_flag(compiler, flag):
    """check if compiler has compatibility with the flag"""
    with tempfile.NamedTemporaryFile("w", suffix=".cpp") as f:
        f.write("int main (int argc, char** argv) { return 0; }")
        try:
            compiler.compile([f.name], extra_postargs=[flag])
        except setuptools.distutils.errors.CompileError:
            return False
    return True


def get_compile_flags():
    """get the compile flags"""
    cflags = ["-Wall", "-Wextra"]
    debug_env = os.getenv("PYGRAM11_DEBUG")
    if debug_env is None:
        cflags += ["-g0"]
    else:
        cflags += ["-g"]
    if sys.platform.startswith("darwin"):
        cflags += ["-std=c++17", "-fvisibility=hidden", "-stdlib=libc++"]
    else:
        cflags += ["-std=c++17", "-fvisibility=hidden"]
    return cflags


def get_link_flags():
    envPREFIX = os.getenv("PREFIX")
    lflags = []
    if sys.platform.startswith("darwin"):
        if envPREFIX is not None:
            lflags += ["-Wl,-rpath,{}/lib".format(envPREFIX)]
        lflags += ["-lc++fs"]
    else:
        lflags += ["-lstdc++fs"]
    return lflags


def get_extensions():
    cpp_cflags = get_compile_flags()
    cpp_lflags = get_link_flags()
    import pybind11
    return [
        Extension(
            "stdfs",
            [os.path.join("src", "stdfs.cpp")],
            language="c++",
            include_dirs=[pybind11.get_include()],
            extra_compile_args=cpp_cflags,
            extra_link_args=cpp_lflags,
        ),
    ]


def get_readme():
    project_root = pathlib.PosixPath(__file__).parent
    with (project_root / "README.md").open("rb") as f:
        return f.read().decode("utf-8")


setup(
    name="stdfs",
    version="0.0.1",
    author="Doug Davis",
    author_email="ddavis@ddavis.io",
    url="https://github.com/douglasdavis/stdfs",
    description="C++ std::filesystem API in Python",
    long_description=get_readme(),
    long_description_content_type="text/markdown",
    ext_modules=get_extensions(),
    python_requires=">=3.6",
    test_suite="tests",
    tests_require=["pytest>=4.0"],
    zip_safe=False,
    classifiers=[
        "Programming Language :: Python :: 3 :: Only",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: C++",
        "Operating System :: Unix",
        "Operating System :: MacOS",
        "Operating System :: POSIX",
        "Operating System :: POSIX :: Linux",
        "Development Status :: 4 - Beta",
    ],
)
