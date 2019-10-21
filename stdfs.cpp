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

  // std::filesystem::path binding

  py::class_<fs::path>(m, "path")
    .def(py::init<const std::string&>())
    .def("__str__", [](const fs::path& p) {
      return p.string();
    })
    .def("__repr__", [](const fs::path& p) {
      std::stringstream ss;
      ss << "path(\"" << p.string() << "\")";
      return ss.str();
    })
    .def("__iter__", [](const fs::path& p) {
      fs::directory_iterator di(p);
      return py::make_iterator(fs::begin(di), fs::end(di));
    }, py::keep_alive<0, 1>())
    .def(py::self / py::self)
    .def(py::self / std::string())
    .def("c_str", [](const fs::path& p) { return p.c_str(); })
    .def("native", [](const fs::path& p) { return p.native(); })
    .def("root_directory", [](const fs::path& p) { return p.root_directory(); })
    .def("root_name", [](const fs::path& p) { return p.root_name(); })
    .def("root_path", [](const fs::path& p) { return p.root_path(); });

  // std::filesystem::directory_entry binding

  py::class_<fs::directory_entry>(m, "directory_entry")
    .def(py::init<const fs::path&>())
    .def("__str__", [](fs::directory_entry& de) {
      return de.path().string();
    })
    .def("__repr__", [](fs::directory_entry& de) {
      std::stringstream ss;
      ss << "directory_entry(\"" << de.path().string() << "\")";
      return ss.str();
    })
    .def("exists", [](const fs::directory_entry& de) { return de.exists(); })
    .def("is_block_file", [](const fs::directory_entry& de) { return de.is_block_file(); })
    .def("is_character_file", [](const fs::directory_entry& de) { return de.is_character_file(); })
    .def("path", [](const fs::directory_entry& de) { return de.path(); });

  // free function bindings

  m.def("absolute", [](const fs::path& p) { return fs::absolute(p); });
  m.def("canonical", [](const fs::path& p) { return fs::canonical(p); });
  m.def("create_directory", [](const fs::path& p) { return fs::create_directory(p); });
  m.def("create_directories", [](const fs::path& p) { return fs::create_directories(p); });
  m.def("current_path", []() { return fs::current_path(); });
  m.def("equivalent", [](const fs::path& p1, const fs::path& p2) { return fs::equivalent(p1, p2); });
  m.def("exists", [](const fs::path& p) { return fs::exists(p); });
  m.def("file_size", [](const fs::path& p) { return fs::file_size(p); });
  m.def("proximate", [](const fs::path& p) { return fs::proximate(p); });
  m.def("relative", [](const fs::path& p) { return fs::relative(p); });
  m.def("temp_directory_path", []() { return fs::temp_directory_path(); });
  m.def("weakly_canonical", [](const fs::path& p) { return fs::weakly_canonical(p); });

  // iterator bindings

  py::class_<fs::directory_iterator>(m, "directory_iterator")
    .def(py::init<const fs::path&>())
    .def("__iter__", [](const fs::directory_iterator& di) {
      return py::make_iterator(fs::begin(di), fs::end(di));
    }, py::keep_alive<0, 1>());

  py::class_<fs::recursive_directory_iterator>(m, "recursive_directory_iterator")
    .def(py::init<const fs::path&>())
    .def("__iter__", [](const fs::recursive_directory_iterator& rdi) {
      return py::make_iterator(fs::begin(rdi), fs::end(rdi));
    }, py::keep_alive<0, 1>());

}
