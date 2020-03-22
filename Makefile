.PHONY: default
default: config build test

BUILD_DIR_BASE = build
CONFIG ?= Debug

CMAKE_GENERATOR ?= Ninja
CMAKE_EXTRA_FLAGS ?= 
CMAKE_FLAGS = -Wno-dev $(CMAKE_EXTRA_FLAGS)

.PHONY: config
config:
	cmake -S. -G$(CMAKE_GENERATOR) $(CMAKE_FLAGS) -B$(BUILD_DIR_BASE)_$(CONFIG) -DCMAKE_BUILD_TYPE=$(CONFIG) -DMOCI_API_OPENGL=ON 

.PHONY: config-gles
config-gles:
	cmake -G$(CMAKE_GENERATOR) $(CMAKE_FLAGS) -B$(BUILD_DIR_BASE)_$(CONFIG) -DCMAKE_BUILD_TYPE=$(CONFIG) -DMOCI_API_OPENGL_LEGACY=ON -DMOCI_API_OPENGL_ES=ON .

.PHONY: config-vs
config-vs:
	cmake $(CMAKE_FLAGS) -B$(BUILD_DIR_BASE)_$(CONFIG) -DCMAKE_BUILD_TYPE=$(CONFIG) .

.PHONY: build
build:
	cmake --build $(BUILD_DIR_BASE)_$(CONFIG) --config $(CONFIG)


.PHONY: sanitize
sanitize:
	cmake -G$(CMAKE_GENERATOR) $(CMAKE_FLAGS) -B$(BUILD_DIR_BASE)_sanitize \
		-DCMAKE_BUILD_TYPE=$(CONFIG) \
		-DMOCI_BUILD_TESTS=ON \
		-DMOCI_BUILD_ASAN=ON \
		-DMOCI_BUILD_UBSAN=ON \
		.
	cmake --build $(BUILD_DIR_BASE)_sanitize --config $(CONFIG)
	cd $(BUILD_DIR_BASE)_sanitize && ctest -c

.PHONY: test
test:
	cd $(BUILD_DIR_BASE)_$(CONFIG) && ctest -c

.PHONY: tidy
tidy:
	cmake -S. -G"Ninja" $(CMAKE_FLAGS) -B $(BUILD_DIR_BASE)_tidy -DMOCI_CLANG_TIDY=ON
	cmake --build $(BUILD_DIR_BASE)_tidy

.PHONY: tidy-fix
tidy-fix:
	cmake -S. -G"Unix Makefiles" $(CMAKE_FLAGS) -B $(BUILD_DIR_BASE)_tidy-fix -DMOCI_CLANG_TIDY_FIX=ON
	cd $(BUILD_DIR_BASE)_tidy-fix && make -j1

.PHONY: coverage
coverage:
	cmake -S. -G"Unix Makefiles" $(CMAKE_FLAGS) -B $(BUILD_DIR_BASE)_coverage -DMOCI_ENABLE_COVERAGE=ON
	cd $(BUILD_DIR_BASE)_coverage && make -j3
	cd $(BUILD_DIR_BASE)_coverage && lcov -c -i -d . --base-directory . -o base_cov.info
	cd $(BUILD_DIR_BASE)_coverage && ctest
	cd $(BUILD_DIR_BASE)_coverage && lcov -c -d . --base-directory . -o test_cov.info
	cd $(BUILD_DIR_BASE)_coverage && lcov -a base_cov.info -a test_cov.info -o cov.info
	cd $(BUILD_DIR_BASE)_coverage && lcov --remove cov.info "*3rd_party/*" -o cov.info
	cd $(BUILD_DIR_BASE)_coverage && lcov --remove cov.info "*c++*" -o cov.info
	cd $(BUILD_DIR_BASE)_coverage && lcov --remove cov.info "*v1*" -o cov.info
	cd $(BUILD_DIR_BASE)_coverage && lcov --remove cov.info "*Xcode.app*" -o cov.info

.PHONY: report
report:
	cd $(BUILD_DIR_BASE)_coverage && genhtml cov.info --output-directory lcov

.PHONY: docs
docs:
	cd docs && doxygen Doxyfile.in

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR_BASE)_$(CONFIG)

.PHONY: stats
stats:
	@cloc --exclude-dir=3rd_party,build_coverage,build_sanitize,build_Debug,build_Release,venv .

.PHONY: format
format:
	find moci -iname '*.h' -o -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
	find sandbox -iname '*.h' -o -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
	find sandbox3D -iname '*.h' -o -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i

