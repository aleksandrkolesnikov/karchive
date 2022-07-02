from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps
import os


class KArchiveTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("KArchive/5.96.0@kde/testing")

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.16.0]")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.so*", dst = "lib", src = "bin")
        #self.copy("*.dll", dst="bin", src="bin")
        #self.copy("*.dylib*", dst="bin", src="lib")

    def test(self):
        os.chdir(self.build_folder)
        self.run(".%sexample" % os.sep)
