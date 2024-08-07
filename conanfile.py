import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout, CMakeDeps
from conan.tools.build import check_min_cppstd


class CoronaRecipe(ConanFile):
    name = "corona"
    version = "2.0.0"
    description = "Frontend and interop library for QuaSAR hardware"
    author = "whs31 <whs31@github.io>"
    topics = ("qt", "qml", "js", "api", "plugin", "application")
    settings = "os", "arch", "compiler", "build_type"
    exports_sources = "*"
    options = {
        "shared": [True, False],
        "standalone": [True, False],
        "test": [True, False],
        "self_sufficient": [True, False]
    }
    default_options = {
        "shared": True,
        "standalone": False,
        "test": False,
        "self_sufficient": False
    }

    @property
    def _min_cppstd(self):
        return "23"

    def requirements(self):
        self.requires("floppy/[>=1.2.4]", transitive_headers=True, transitive_libs=True)
        self.requires("magic_enum/0.9.5", transitive_headers=True, transitive_libs=True)
        self.requires("reflect-cpp/0.11.1")
        if self.options.standalone:
            self.requires("tomlplusplus/[>=3.0.1]")
        if self.options.test:
            self.requires("gtest/[>=1.10.0]")
        if self.options.self_sufficient:
            self.requires("boost/[>=1.85.0]", transitive_headers=True, transitive_libs=True)
            # qt6

    def configure(self):
        self.options["reflect-cpp"].with_json = True
        self.options["reflect-cpp"].with_yaml = True

    def layout(self):
        cmake_layout(self)

    def validate(self):
        if self.settings.get_safe("compiler.cppstd"):
            check_min_cppstd(self, self._min_cppstd)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.cache_variables["CORONA_STANDALONE"] = self.options.standalone
        tc.cache_variables["CORONA_TEST"] = self.options.test
        tc.cache_variables["CORONA_SELF_SUFFICIENT"] = self.options.self_sufficient
        tc.cache_variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    #  def package(self):
    #         cmake = CMake(self)
    #         cmake.install()
    #         rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))
    #         rmdir(self, os.path.join(self.package_folder, "lib", "pkgconfig"))
    #         rmdir(self, os.path.join(self.package_folder, "res"))
    #         rmdir(self, os.path.join(self.package_folder, "share"))
    #
    #     def package_info(self):
    #         self.cpp_info.set_property("cmake_file_name", "flavor")
    #         self.cpp_info.set_property("cmake_target_name", "flavor::flavor")
    #         self.cpp_info.libs = ["flavor"]
    #         self.cpp_info.requires = ["magic_enum::magic_enum", "leaf::leaf", "fmt-qt::fmt-qt", "qmlbind20::qmlbind20", "tomlplusplus::tomlplusplus"]
