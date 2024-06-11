from conan import ConanFile

class TransceiverToolRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("nlohmann_json/[^3.11.3]")

        self.requires("fmt/[^10]")

        self.requires("cppcodec/0.2")

    def build_requirements(self):
        pass

