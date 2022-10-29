from conans import ConanFile


class Moci(ConanFile):
    name = "moci"
    url = "https://github.com/tobanteAudio/moci"

    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_find_package", "markdown"

    def requirements(self):
        self.requires("assimp/5.2.2")
        self.requires("catch2/3.1.0")
        self.requires("freetype/2.12.1")
        self.requires("glew/2.2.0")
        self.requires("glfw/3.3.8")
        self.requires("glm/0.9.9.8")
        self.requires("ms-gsl/4.0.0")
        self.requires("spdlog/1.10.0")
        self.requires("stb/cci.20210910")
        self.requires("zlib/1.2.13", override=True)

    def configure(self):
        self.options["glfw"].shared = False
        self.options["glew"].shared = False
        self.options["freetype"].shared = False

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
