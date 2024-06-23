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
        "standalone": [True, False],
        "test": [True, False]
    }
    default_options = {
        "standalone": False,
        "test": False
    }

    @property
    def _min_cppstd(self):
        return "23"

    def requirements(self):
        self.requires("floppy/[>=1.2.1]", transitive_headers=True, transitive_libs=True)
        self.requires("magic_enum/0.9.5", transitive_headers=True, transitive_libs=True)
        self.requires("boost/[>=1.85.0]", transitive_headers=True, transitive_libs=True)
        self.requires("reflect-cpp/0.11.1")
        if self.options.test:
            self.requires("gtest/[>=1.10.0]")

    def layout(self):
        cmake_layout(self)

    def validate(self):
        if self.settings.get_safe("compiler.cppstd"):
            check_min_cppstd(self, self._min_cppstd)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["CORONA_STANDALONE"] = self.options.standalone
        tc.variables["CORONA_TEST"] = self.options.test
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
