#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <filesystem>
#include <string>
#include <sstream>

namespace py = pybind11;
namespace fs = std::filesystem;

PYBIND11_MODULE(stdfs, m) {
  m.doc() = "std::filesystem in Python";

  py::class_<fs::path>(m, "path")
    .def(py::init<const std::string&>())
    .def("__repr__", [](const fs::path& p) {
      std::stringstream ss;
      ss << "path(\"" << p.string() << "\")";
      return ss.str();
    })
    .def("__str__", [](const fs::path& p) {
      return p.string();
    })
    .def(py::self / py::self)
    .def(py::self / std::string())

    .def("c_str", [](const fs::path& p) {
      return p.c_str();
    })

    .def("native", [](const fs::path& p) {
      return p.native();
    })

    .def("root_name", [](const fs::path& p) {
      return p.root_name();
    })

    .def("root_directory", [](const fs::path& p) {
      return p.root_directory();
    })

    .def("root_path", [](const fs::path& p) {
      return p.root_path();
    })

    .def("ls", [](const fs::path& p, bool ignore_hidden = true) {
      py::list ret;
      for (const auto& ip : fs::directory_iterator(p)) {
        if (ignore_hidden) {
          if (ip.path().filename().string()[0] == '.') {
            continue;
          }
        }
        ret.append(ip.path());
      }
      return ret;
    }, py::arg("ignore_hidden") = true);

  m.def("exists", [](const fs::path& p) {
    return fs::exists(p);
  });
  m.def("create_directory", [](const fs::path& p) {
    return fs::create_directory(p);
  });
  m.def("create_directories", [](const fs::path& p) {
    return fs::create_directories(p);
  });
  m.def("temp_directory_path", []() {
    return fs::temp_directory_path();
  });

}
