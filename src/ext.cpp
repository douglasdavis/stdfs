#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/string.h>

#include <filesystem>
#include <string_view>
#include <sstream>

namespace nb = nanobind;

#define REGISTER_FREE_FUNC_PATH_OR_STR(NAME)                            \
  do {                                                                  \
    m.def(#NAME, [](const std::string& p) { return fs::NAME(p); });     \
    m.def(#NAME, [](const char* p) {return fs::NAME(p); });             \
    m.def(#NAME, [](const fs::path& p) { return fs::NAME(p); });        \
    m.def(#NAME, [](std::string_view s) { return fs::NAME(s); });       \
    m.def(#NAME, [](const fs::directory_entry& d) {                     \
      return fs::NAME(d.path());                                        \
    });                                                                 \
  } while (0)


NB_MODULE(ext, m) {
  m.attr("__version__") = "0.1";

  namespace fs = std::filesystem;

  // std::filesystem::file_type binding

  nb::enum_<fs::file_type>(m, "file_type")
    .value("none", fs::file_type::none)
    .value("not_found", fs::file_type::not_found)
    .value("regular", fs::file_type::regular)
    .value("directory", fs::file_type::directory)
    .export_values();

  nb::enum_<fs::copy_options>(m, "copy_options")
    .value("none", fs::copy_options::none)
    .value("skip_existing", fs::copy_options::skip_existing)
    .export_values();

  // std::filesystem::path binding

  nb::class_<fs::path>(m, "path")
    .def(nb::init<std::string_view>())
    .def(nb::init<const char*>())
    .def(nb::init<const fs::path&>())
    .def(nb::init<const std::string&>())
    .def("__str__", [](const fs::path& p) { return p.string(); })
    .def("__repr__", [](const fs::path& p) {
      std::stringstream ss;
      ss << "path(\"" << p.string() << "\")";
      return ss.str();
    })
    // .def("__iter__", [](const fs::path& p) {
    //   fs::directory_iterator di(p);
    //   return nb::iterator(fs::begin(di), fs::end(di));
    // }, nb::keep_alive<0, 1>())
    .def(nb::self / nb::self)
    .def(nb::self / std::string())
    .def("c_str", [](const fs::path& p) { return p.c_str(); })
    .def("native", [](const fs::path& p) { return p.native(); })
    .def("root_directory", [](const fs::path& p) { return p.root_directory(); })
    .def("root_name", [](const fs::path& p) { return p.root_name(); })
    .def("root_path", [](const fs::path& p) { return p.root_path(); })
    .def("relative_path", [](const fs::path& p) { return p.relative_path(); })
    .def("parent_path", [](const fs::path& p) { return p.parent_path(); })
    .def("filename", [](const fs::path& p) { return p.filename(); })
    .def("stem", [](const fs::path& p) { return p.stem(); })
    .def("extension", [](const fs::path& p) { return p.extension(); })
    .def("empty", [](const fs::path& p) { return p.empty(); })

    .def("clear", [](fs::path& p) { p.clear(); })
    .def("make_preferred", [](fs::path& p) { return p.make_preferred(); })
    .def("remove_filename", [](fs::path& p) { return p.remove_filename(); })
    .def("replace_filename", [](fs::path& p, const fs::path& r) { return p.replace_filename(r); })
    .def("replace_filename", [](fs::path& p, const std::string_view& r) { return p.replace_filename(r); })

    .def("has_root_path", [](const fs::path& p) { return p.has_root_path(); })
    .def("has_root_name", [](const fs::path& p) { return p.has_root_name(); })
    .def("has_root_directory", [](const fs::path& p) { return p.has_root_directory(); })
    .def("has_relative_path", [](const fs::path& p) { return p.has_relative_path(); })
    .def("has_parent_path", [](const fs::path& p) { return p.has_parent_path(); })
    .def("has_filename", [](const fs::path& p) { return p.has_filename(); })
    .def("has_stem", [](const fs::path& p) { return p.has_stem(); })
    .def("has_extension", [](const fs::path& p) { return p.has_extension(); })

    .def("is_absolute", [](const fs::path& p) { return p.is_absolute(); })
    .def("is_relative", [](const fs::path& p) { return p.is_relative(); });

  // std::filesystem::directory_entry binding

  nb::class_<fs::directory_entry>(m, "directory_entry")
    .def(nb::init<const fs::path&>())
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
  m.def("equivalent", [](const fs::path& p1, std::string_view p2) { return fs::equivalent(p1, p2); });
  m.def("equivalent", [](std::string_view p1, const fs::path& p2) { return fs::equivalent(p1, p2); });
  m.def("equivalent", [](std::string_view p1, std::string_view p2) { return fs::equivalent(p1, p2); });
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

  // nb::class_<fs::directory_iterator>(m, "direcory_iterator")
  //   .def(nb::init<const fs::path&>())
  //   .def(nb::init<std::string_view>())
  //   .def("__iter__", [](const fs::directory_iterator& di) {
  //     return nb::iterator(fs::begin(di), fs::end(di));
  //   }, nb::keep_alive<0, 1>());

  // nb::class_<fs::recursive_directory_iterator>(m, "recursive_directory_iterator")
  //   .def(nb::init<const fs::path&>())
  //   .def("__iter__", [](const fs::recursive_directory_iterator& rdi) {
  //     return nb::iterator(fs::begin(rdi), fs::end(rdi));
  //   }, nb::keep_alive<0, 1>());

}
