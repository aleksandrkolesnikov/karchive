from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps
from conans import tools
import os


class KArchiveConan(ConanFile):
    name = "KArchive"
    version = "5.96.0"
    license = "LGPL-2.1"
    url = "https://api.kde.org/frameworks/karchive/html/index.html"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "with_xz": [True, False]
    }
    default_options = {
        "shared": True,
        "with_xz": True
    }
    exports_sources = "*"
    scm = {
        "type": "git",
        "url": "auto",
        "revision": "auto"
    }

    def requirements(self):
        self.requires("zlib/1.2.12")
        self.requires("bzip2/1.0.8")
        self.requires("qt/5.15.2")
        self.requires("extra-cmake-modules/5.96.0@kde/testing")

        if self.options.with_xz:
            self.requires("xz_utils/5.2.5")

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.16.0]")

    def generate(self):
        toolchain = CMakeToolchain(self)
        qt = self.dependencies["qt"]
        ecm = self.dependencies["extra-cmake-modules"]
        toolchain.variables["REQUIRED_QT_VERSION"] = qt.ref.version
        toolchain.variables["QT_MAJOR_VERSION"] = tools.Version(qt.ref.version).major
        toolchain.variables["REQUIRED_ECM_VERSION"] = ecm.ref.version
        toolchain.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_find_mode", "none")
        self.cpp_info.builddirs = [os.path.join("lib", "cmake", "KF5Archive")]
