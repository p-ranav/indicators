from conans import CMake, ConanFile, tools
from conans.errors import ConanException
from contextlib import contextmanager
import os


class IndicatorsConan(ConanFile):
    name = "indicators"
    description = "Activity Indicators for Modern C++"
    topics = ("indicators", "progress", "activity", "indicator", "header-only", "c++11")
    homepage = "https://github.com/p-ranav/indicators"
    url = "https://github.com/p-ranav/indicators"
    license = ("MIT", "BSD-3-Clause")
    exports_sources = "demo/**", "include/**", "samples/**", "CMakeLists.txt", "LICENSE*", \
                      "indicators.pc.in", "indicatorsConfig.cmake.in", "README*", "img/**"
    exports = "LICENSE"
    no_copy_source = True
    settings = "os", "arch", "compiler", "build_type"
    generators = "cmake"

    def set_version(self):
        import re
        m = re.search(r"project\(.*VERSION ([0-9a-zA-Z.-]+)[ )]",
                      open(os.path.join(self.recipe_folder, "CMakeLists.txt")).read())
        if not m:
            raise ConanException("Could not extract version from CMakeLists.txt")
        self.version = m.group(1)

    _cmake = None

    def _configure_cmake(self):
        if self._cmake:
            return self._cmake
        generator = None
        if self.settings.compiler == "Visual Studio":
            generator = "NMake Makefiles"
        self._cmake = CMake(self, generator=generator)
        if tools.get_env("CONAN_RUN_TESTS", default=False):
            self._cmake.definitions["INDICATORS_DEMO"] = True
            self._cmake.definitions["INDICATORS_SAMPLES"] = True
        self._cmake.configure()
        return self._cmake

    @property
    def _test_programs(self):
        programs = []
        import re
        for subdir in ("demo", "samples", ):
            for match in re.finditer(r"add_executable\((\S+)",
                                     open(os.path.join(self.source_folder, subdir, "CMakeLists.txt")).read()):
                programs.append(os.path.join(self.build_folder, "bin", match.group(1)))
        return programs

    @contextmanager
    def _build_context(self):
        with tools.vcvars(self.settings) if self.settings.compiler == "Visual Studio" else tools.no_op():
            yield

    def build(self):
        with self._build_context():
            cmake = self._configure_cmake()
            cmake.build()
            cmake.build(target="package_source")
            # if tools.get_env("CONAN_RUN_TESTS", default=False):
            #     for program in self._test_programs:
            #         self.output.info("Running program '{}'".format(program))
            #         self.run(program, run_environment=True)

    def package(self):
        with self._build_context():
            cmake = self._configure_cmake()
            cmake.install()

    def package_id(self):
        self.info.header_only()

    def package_info(self):
        self.cpp_info.includedirs.append(os.path.join("include", "indicators"))
