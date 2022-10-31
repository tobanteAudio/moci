from conans import ConanFile, CMake


class Moci(ConanFile):
    name = "moci"
    url = "https://github.com/tobanteAudio/moci"

    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_find_package", "markdown"

    def requirements(self):
        self.requires("assimp/5.2.2")
        self.requires("catch2/3.1.0")
        self.requires("freetype/2.12.1")
        self.requires("glm/0.9.9.8")
        self.requires("imgui/cci.20220621+1.88.docking")
        self.requires("spdlog/1.10.0")
        self.requires("stb/cci.20210910")
        self.requires("zlib/1.2.13", override=True)

        if self.settings.os != "Emscripten":
            self.requires("glew/2.2.0")
            self.requires("glfw/3.3.8")

    def configure(self):
        self.options["freetype"].shared = False
        self.options["imgui"].shared = False

        self.options["assimp"].with_fbx = True
        self.options["assimp"].with_obj = True
        self.options["assimp"].with_3d = False
        self.options["assimp"].with_3ds = False
        self.options["assimp"].with_3ds_exporter = False
        self.options["assimp"].with_3mf = False
        self.options["assimp"].with_3mf_exporter = False
        self.options["assimp"].with_ac = False
        self.options["assimp"].with_amf = False
        self.options["assimp"].with_ase = False
        self.options["assimp"].with_assbin = False
        self.options["assimp"].with_assbin_exporter = False
        self.options["assimp"].with_assxml_exporter = False
        self.options["assimp"].with_assjson_exporter = False
        self.options["assimp"].with_b3d = False
        self.options["assimp"].with_blend = False
        self.options["assimp"].with_bvh = False
        self.options["assimp"].with_ms3d = False
        self.options["assimp"].with_cob = False
        self.options["assimp"].with_collada = False
        self.options["assimp"].with_collada_exporter = False
        self.options["assimp"].with_csm = False
        self.options["assimp"].with_dxf = False
        self.options["assimp"].with_fbx_exporter = False
        self.options["assimp"].with_gltf = False
        self.options["assimp"].with_gltf_exporter = False
        self.options["assimp"].with_hmp = False
        self.options["assimp"].with_ifc = False
        self.options["assimp"].with_irr = False
        self.options["assimp"].with_irrmesh = False
        self.options["assimp"].with_lwo = False
        self.options["assimp"].with_lws = False
        self.options["assimp"].with_md2 = False
        self.options["assimp"].with_md3 = False
        self.options["assimp"].with_md5 = False
        self.options["assimp"].with_mdc = False
        self.options["assimp"].with_mdl = False
        self.options["assimp"].with_mmd = False
        self.options["assimp"].with_ndo = False
        self.options["assimp"].with_nff = False
        self.options["assimp"].with_obj_exporter = False
        self.options["assimp"].with_off = False
        self.options["assimp"].with_ogre = False
        self.options["assimp"].with_opengex = False
        self.options["assimp"].with_opengex_exporter = False
        self.options["assimp"].with_ply = False
        self.options["assimp"].with_ply_exporter = False
        self.options["assimp"].with_q3bsp = False
        self.options["assimp"].with_q3d = False
        self.options["assimp"].with_raw = False
        self.options["assimp"].with_sib = False
        self.options["assimp"].with_smd = False
        self.options["assimp"].with_step = False
        self.options["assimp"].with_step_exporter = False
        self.options["assimp"].with_stl = False
        self.options["assimp"].with_stl_exporter = False
        self.options["assimp"].with_terragen = False
        self.options["assimp"].with_x = False
        self.options["assimp"].with_x_exporter = False
        self.options["assimp"].with_x3d = False
        self.options["assimp"].with_x3d_exporter = False
        self.options["assimp"].with_xgl = False

        if self.settings.os != "Emscripten":
            self.options["glfw"].shared = False
            self.options["glew"].shared = False

    def imports(self):
        src = "res/bindings"
        dest = "bindings"

        self.copy("imgui_impl_opengl3.h", dst=dest, src=src)
        self.copy("imgui_impl_opengl3.cpp", dst=dest, src=src)
        self.copy("imgui_impl_opengl3_loader.h", dst=dest, src=src)

        if self.settings.os == "Emscripten":
            self.copy("imgui_impl_sdl.h", dst=dest, src=src)
            self.copy("imgui_impl_sdl.cpp", dst=dest, src=src)
        else:
            self.copy("imgui_impl_glfw.h", dst=dest, src=src)
            self.copy("imgui_impl_glfw.cpp", dst=dest, src=src)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
