from conan import ConanFile


class Corona(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    name = "Corona"
    version = "1.0.0"

    def requirements(self):
        # self.requires("qt/6.6.1")
        self.requires("spdlog/1.11.0")
        self.requires("yaml-cpp/0.8.0")
        self.requires("tl-expected/1.0.0")


        # self.default_options = {
        #     "qt:shared": False,
        #     "qt:qtsvg": True,
        #     "qt:qtquickcontrols2": True,
        #     "qt:qtnetwork": True,
        #     "qt:qtlocation": True,
        #     "qt:qtdeclarative": True,
        #     "qt:gui": True,
        #     "qt:qtpositioning": True,
        #     "qt:qtimageformats": True,
        #
        #     "qt:with_harfbuzz": False,
        #     "qt:with_glib": False,
        # }
