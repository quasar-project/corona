import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout, CMakeDeps
from conan.tools.build import check_min_cppstd
from conan.tools.files import rmdir
try:
    from termcolor import colored
except ImportError:
    def colored(text, color, on_color=None, attrs=None):
        return text


class CoronaBootstrapGeoserviceRecipe(ConanFile):
    name = "corona-bootstrap-geoservice"
    version = "0.1.0"
    description = "Geoservice plugin for Corona"
    author = "whs31 <whs31@github.io>, spoo0k <mukhinva2@gmail.com>"
    topics = ("utility", "qt", "qml", "geoservice")

    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False],
        "dev": [True, False],
    }
    default_options = {
        "shared": False,
        "dev": False
    }

    exports_sources = "*"

    @property
    def _min_cppstd(self):
        return "20" 

    def requirements(self):
        self.requires("floppy/[^1.3.0]", transitive_headers=True, transitive_libs=True)
        self.requires("magic_enum/[^0.9.0]")
        self.requires("reflect-cpp/0.11.1")

    def layout(self):
        cmake_layout(self)

    def validate(self):
        if self.settings.get_safe("compiler.cppstd"):
            check_min_cppstd(self, self._min_cppstd)

    def configure(self):
        self.options["reflect-cpp"].with_json = True
        self.options["reflect-cpp"].with_yaml = True

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.cache_variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.cache_variables["QNX_DEV"] = self.options.dev
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))
        rmdir(self, os.path.join(self.package_folder, "lib", "pkgconfig"))
        rmdir(self, os.path.join(self.package_folder, "res"))
        rmdir(self, os.path.join(self.package_folder, "share"))

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "corona-bootstrap-geoservice")
        self.cpp_info.set_property("cmake_target_name", "corona::bootstrap-geoservice")
        self.cpp_info.libs = ["corona-bootstrap-geoservice"]
        self.cpp_info.requires = ["floppy::floppy", "magic_enum::magic_enum", "reflect-cpp::reflect-cpp"]
