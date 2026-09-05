# SPDX-FileCopyrightText: 2024-2026 Robin Christ
# SPDX-License-Identifier: MPL-2.0

from conan import ConanFile

class TransceiverToolRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"
    options = {"schema_tests": [True, False]}
    default_options = {"schema_tests": False}

    def requirements(self):
        self.requires("nlohmann_json/[^3.11.3]", force=bool(self.options.schema_tests))

        self.requires("fmt/[^10]")

        self.requires("cppcodec/0.2")

        if self.settings.os != "Emscripten":
            self.requires("cli11/2.4.2")

        if self.options.schema_tests:
            self.test_requires("json-schema-validator/2.3.0")

    def build_requirements(self):
        pass
