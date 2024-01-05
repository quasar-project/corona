from conan import ConanFile


class Corona(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    name = "Corona"
    version = "1.0.0"

    def requirements(self):
        self.requires("spdlog/1.11.0")
        self.requires("yaml-cpp/0.8.0")
