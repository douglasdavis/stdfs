from skbuild import setup
import nanobind

setup(
    name="stdfs",
    version="0.0.1",
    author="Doug Davis",
    author_email="ddavis@ddavis.io",
    description="C++ std::filesystem",
    url="https://github.com/douglasdavis/stdfs",
    license="BSD",
    packages=["stdfs"],
    package_dir={"": "src"},
    cmake_install_dir="src/stdfs",
    include_package_data=True,
    python_requires=">=3.8",
)
