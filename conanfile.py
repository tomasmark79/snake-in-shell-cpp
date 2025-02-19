from conan import ConanFile
from conan.tools.cmake import cmake_layout

class MarkWareVCMake(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def configure(self):
        self.options["*"].shared = False # this replaced shared flag from SolutionController and works


    def requirements(self):
        self.requires("zlib/1.2.11") # used to compress and decompress data
        self.requires("fmt/11.1.1") # required by cpm package
        self.requires("nlohmann_json/3.11.2") # is modern to have json support
    # it is on you to define rest of the dependencies

    # -------------------------------------- --
    # MarkWareVCMake is using self mechanism --
    # to define build ouput layout!          --
    # ├── Build
    #     ├── Artefacts - tarballs
    #     ├── Install - final installation
    #     ├── Library - library build
    #     └── Standalone - standalone build
    # ├── cmake
    # ├── include
    # ├── Source
    # └── Standalone
    # ------------------------keep-commented --
    # def layout(self):                      --
        # cmake_layout(self)                 --
    # -------------------------------------- --

       

        
