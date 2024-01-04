from conan import ConanFile


class Corona(ConanFile):
    generators = ("CMakeToolchain", "CMakeDeps")
    settings = ("os", "compiler", "build_type", "arch")

    def requirements(self):
        self.requires("nlohmann_json/3.11.2")

    def build_requirements(self):
        # self.tool_requires("cmake/3.25.0")
        pass

    def configure(self):
        pass

    def layout(self):
        self.folders.generators = ""
