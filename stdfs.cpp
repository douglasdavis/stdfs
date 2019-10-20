#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <filesystem>
#include <string>
#include <sstream>

namespace py = pybind11;
namespace fs = std::filesystem;

namespace detail {
  struct directory_iterator_wrapper {
    fs::directory_iterator m_di;
    directory_iterator_wrapper(const fs::directory_iterator& di) : m_di(di) {}
    fs::directory_iterator begin() { return fs::begin(m_di); }
    fs::directory_iterator end() { return fs::end(m_di); }
  };
}

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

    .def("__iter__", [](const fs::path& p) {
      fs::directory_iterator di(p);
      detail::directory_iterator_wrapper diw(di);
      return py::make_iterator(diw.begin(), diw.end());
    }, py::keep_alive<0, 1>());

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
    .def("path", [](const fs::directory_entry& de) {
      return de.path();
    })
    .def("exists", [](const fs::directory_entry& de) {
      return de.exists();
    })
    .def("is_block_file", [](const fs::directory_entry& de) {
      return de.is_block_file();
    })
    .def("is_character_file", [](const fs::directory_entry& de) {
      return de.is_character_file();
    });

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
