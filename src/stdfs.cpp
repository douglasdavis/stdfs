#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <filesystem>
#include <string>
#include <sstream>

namespace py = pybind11;

#define REGISTER_FREE_FUNC_PATH_OR_STR(NAME)                            \
  do {                                                                  \
    m.def(#NAME, [](const fs::path& p) { return fs::NAME(p); });        \
    m.def(#NAME, [](const std::string& s) { return fs::NAME(s); });     \
  } while (0)


PYBIND11_MODULE(stdfs, m) {
  m.doc() = "std::filesystem in Python";

  namespace fs = std::filesystem;

  // std::filesystem::file_type binding

  py::enum_<fs::file_type>(m, "file_type")
    .value("none", fs::file_type::none)
    .value("not_found", fs::file_type::not_found)
    .value("regular", fs::file_type::regular)
    .value("directory", fs::file_type::directory)
    .export_values();

  // std::filesystem::path binding

  py::class_<fs::path>(m, "path")
    .def(py::init<const std::string&>())
    .def("__str__", [](const fs::path& p) { return p.string(); })
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
    .def("root_path", [](const fs::path& p) { return p.root_path(); })

    .def("clear", [](fs::path& p) { p.clear(); })
    .def("make_preferred", [](fs::path& p) { return p.make_preferred(); })
    .def("remove_filename", [](fs::path& p) { return p.remove_filename(); })
    .def("replace_filename", [](fs::path& p, const fs::path& r) { return p.replace_filename(r); })
    .def("replace_filename", [](fs::path& p, const std::string& r) { return p.replace_filename(r); });


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
  m.def("current_path", []() { return fs::current_path(); });
  m.def("temp_directory_path", []() { return fs::temp_directory_path(); });
  m.def("equivalent", [](const fs::path& p1, const fs::path& p2) { return fs::equivalent(p1, p2); });
  m.def("equivalent", [](const fs::path& p1, const std::string& p2) { return fs::equivalent(p1, p2); });
  m.def("equivalent", [](const std::string& p1, const fs::path& p2) { return fs::equivalent(p1, p2); });
  m.def("equivalent", [](const std::string& p1, const std::string& p2) { return fs::equivalent(p1, p2); });

  REGISTER_FREE_FUNC_PATH_OR_STR(absolute);
  REGISTER_FREE_FUNC_PATH_OR_STR(canonical);
  REGISTER_FREE_FUNC_PATH_OR_STR(create_directory);
  REGISTER_FREE_FUNC_PATH_OR_STR(create_directories);
  REGISTER_FREE_FUNC_PATH_OR_STR(exists);
  REGISTER_FREE_FUNC_PATH_OR_STR(file_size);
  REGISTER_FREE_FUNC_PATH_OR_STR(proximate);
  REGISTER_FREE_FUNC_PATH_OR_STR(relative);
  REGISTER_FREE_FUNC_PATH_OR_STR(weakly_canonical);

  REGISTER_FREE_FUNC_PATH_OR_STR(is_block_file);
  REGISTER_FREE_FUNC_PATH_OR_STR(is_character_file);
  REGISTER_FREE_FUNC_PATH_OR_STR(is_directory);
  REGISTER_FREE_FUNC_PATH_OR_STR(is_empty);
  REGISTER_FREE_FUNC_PATH_OR_STR(is_fifo);
  REGISTER_FREE_FUNC_PATH_OR_STR(is_other);
  REGISTER_FREE_FUNC_PATH_OR_STR(is_regular_file);
  REGISTER_FREE_FUNC_PATH_OR_STR(is_socket);
  REGISTER_FREE_FUNC_PATH_OR_STR(is_symlink);

  // iterator bindings

  py::class_<fs::directory_iterator>(m, "directory_iterator")
    .def(py::init<const fs::path&>())
    .def(py::init<const std::string&>())
    .def("__iter__", [](const fs::directory_iterator& di) {
      return py::make_iterator(fs::begin(di), fs::end(di));
    }, py::keep_alive<0, 1>());

  py::class_<fs::recursive_directory_iterator>(m, "recursive_directory_iterator")
    .def(py::init<const fs::path&>())
    .def("__iter__", [](const fs::recursive_directory_iterator& rdi) {
      return py::make_iterator(fs::begin(rdi), fs::end(rdi));
    }, py::keep_alive<0, 1>());

}
