from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake, CMakeToolchain, CMakeDeps
from conan.tools.files import copy
import os
from os.path import join


class BasicConanfile(ConanFile):
    name = "hypodermic"
    version = "2.5.3"
    description = "Hypodermic is an IoC container for C++. It provides dependency injection to your existing design."
    license = "MIT"
    homepage = "github.com/ybainier/Hypodermic.git"
    settings = "build_type"
    default_options = {"boost/*:shared": True}
    exports_sources = "Hypodermic/*", "Hypodermic.Tests/*"

    def requirements(self):
        self.requires("boost/1.81.0")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = True
        tc.generate()

    def layout(self):
        cmake_layout(self)

    def build(self):
        if not self.conf.get("tools.build:skip_test", default=False):
            cmake = CMake(self)
            cmake.configure(build_script_folder="Hypodermic.Tests")
            cmake.build()
            self.run(join(self.cpp.build.bindir, "Hypodermic.Tests"))

    def package(self):
        copy(
            self,
            "*.h",
            join(self.source_folder, "Hypodermic"),
            join(self.package_folder, "include/Hypodermic"),
            keep_path=False,
        )

    def package_id(self):
        self.info.clear()
