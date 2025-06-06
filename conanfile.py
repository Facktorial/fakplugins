from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy
import os


class FakstdConan(ConanFile):
    name = "fakplugins"
    version = "0.0.1"
    license = "WTFPL"
    author = "Your Name <you@example.com>"
    url = "https://github.com/Facktorial/fakplugins"
    description = "My custom library for support plugins in my gamedev."
    topics = ("utilities", "modern-cpp", "plugins")
    package_type = "library"

    settings = "os", "arch", "compiler", "build_type"
    exports_sources = (
        "CMakeLists.txt",
        "include/*",
        "src/*",
        "cmake/*",
        "examples/*",
        "docs/*",
    )
    no_copy_source = True

    # options = {"shared": [True, False]}
    options = {
        "shared": [True, False],
        "BUILD_DOC": [True, False],
    }

    default_options = {"shared": False, "BUILD_DOC": True}
    # default_options = {"shared": False}

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        # self.requires("fakstd/0.0.1")
        self.requires("spdlog/1.13.0")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        tc = CMakeToolchain(self)
        tc.generate()

    def package(self):
        copy(
            self,
            "*.h",
            dst=os.path.join(self.package_folder, "include"),
            src=os.path.join(self.source_folder, "include"),
        )

        copy(
            self,
            "*.a",
            dst=os.path.join(self.package_folder, "lib"),
            src=self.build_folder,
            keep_path=False,
        )
        copy(
            self,
            "*.lib",
            dst=os.path.join(self.package_folder, "lib"),
            src=self.build_folder,
            keep_path=False,
        )
        copy(
            self,
            "*.so",
            dst=os.path.join(self.package_folder, "lib"),
            src=self.build_folder,
            keep_path=False,
        )
        copy(
            self,
            "*.dll",
            dst=os.path.join(self.package_folder, "bin"),
            src=self.build_folder,
            keep_path=False,
        )

    def package_info(self):
        self.cpp_info.libs = ["fakplugins"]
        self.cpp_info.includedirs = ["include"]

    def configure(self):
        # This will make Conan respect the C++ standard set in CMakeLists.txt
        # by not forcing a specific standard from Conan side
        if hasattr(self.settings, "compiler") and hasattr(
            self.settings.compiler, "cppstd"
        ):
            # Let CMake handle the C++ standard instead of Conan
            del self.settings.compiler.cppstd
