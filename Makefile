.PHONY: all
all: config build test

BUILD_DIR_BASE = build
CONFIG ?= Debug

BUILD_DIR ?= build
PYTHON_BIN ?= python3
CLANG_FORMAT_BIN ?= clang-format
CLANG_REPLACEMENTS_BIN ?= clang-apply-replacements
CLANG_TIDY_BIN ?= clang-tidy
RUN_CLANGTIDY_BIN ?= $(shell which run-clang-tidy)

CLANG_TIDY_ARGS =  ${PYTHON_BIN}
CLANG_TIDY_ARGS += ${RUN_CLANGTIDY_BIN}
CLANG_TIDY_ARGS += -clang-tidy-binary
CLANG_TIDY_ARGS += ${CLANG_TIDY_BIN}
CLANG_TIDY_ARGS += -clang-apply-replacements-binary
CLANG_TIDY_ARGS += ${CLANG_REPLACEMENTS_BIN}
CLANG_TIDY_ARGS += -j $(shell nproc)

CMAKE_GENERATOR ?= Ninja
CMAKE_EXTRA_FLAGS ?=
CMAKE_FLAGS = $(CMAKE_EXTRA_FLAGS)

.PHONY: config
config:
	cmake -S. -G$(CMAKE_GENERATOR) $(CMAKE_FLAGS) -B$(BUILD_DIR_BASE)_$(CONFIG) -DCMAKE_BUILD_TYPE=$(CONFIG) -DMOCI_API_OPENGL_LEGACY=ON

.PHONY: config-gles
config-gles:
	cmake -G$(CMAKE_GENERATOR) $(CMAKE_FLAGS) -B$(BUILD_DIR_BASE)_$(CONFIG) -DCMAKE_BUILD_TYPE=$(CONFIG) -DMOCI_API_OPENGL_LEGACY=ON -DMOCI_LINK_OPENGL_ES=ON .

.PHONY: config-vs
config-vs:
	cmake $(CMAKE_FLAGS) -B$(BUILD_DIR_BASE)_$(CONFIG) -DCMAKE_BUILD_TYPE=$(CONFIG) -DMOCI_API_OPENGL_LEGACY=OFF -DMOCI_API_OPENGL_MODERN=ON .

.PHONY: build
build:
	cmake --build $(BUILD_DIR_BASE)_$(CONFIG) --config $(CONFIG)


.PHONY: sanitize
sanitize:
	cmake -G$(CMAKE_GENERATOR) $(CMAKE_FLAGS) -B$(BUILD_DIR_BASE)_sanitize \
		-DCMAKE_BUILD_TYPE=$(CONFIG) \
		-DMOCI_API_OPENGL_LEGACY=ON \
		-DMOCI_BUILD_ASAN=ON \
		-DMOCI_BUILD_UBSAN=ON \
		.
	cmake --build $(BUILD_DIR_BASE)_sanitize --config $(CONFIG)
	cd $(BUILD_DIR_BASE)_sanitize && ctest -c

.PHONY: test
test:
	cd $(BUILD_DIR_BASE)_$(CONFIG) && ctest -c

.PHONY: tidy-check
tidy-check:
	${CLANG_TIDY_ARGS} -quiet -p $(BUILD_DIR) -header-filter $(shell realpath ./src) $(shell realpath ./src)

.PHONY: tidy-fix
tidy-fix:
	${CLANG_TIDY_ARGS} -fix -quiet -p $(BUILD_DIR) -header-filter $(shell realpath ./src) $(shell realpath ./src)

.PHONY: coverage
coverage:
	cmake -S. -GNinja -Bcmake-build-coverage -DCMAKE_BUILD_TYPE=Debug -D MOCI_BUILD_COVERAGE=TRUE
	cmake --build cmake-build-coverage
	ctest --test-dir cmake-build-coverage -C Debug --output-on-failure

.PHONY: coverage-html
coverage-html: coverage
	gcovr --html --html-details -e ".*test\.cpp" --exclude-unreachable-branches -r src -j ${shell nproc} -s cmake-build-coverage -o cmake-build-coverage/coverage.html

.PHONY: coverage-xml
coverage-xml: coverage
	gcovr --xml-pretty -e ".*test\.cpp" --exclude-unreachable-branches -r src -j ${shell nproc} -s cmake-build-coverage -o cmake-build-coverage/coverage.xml

.PHONY: docs
docs:
	cd docs && doxygen Doxyfile.in

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR_BASE)_$(CONFIG)

.PHONY: stats
stats:
	cloc --exclude-dir=3rd_party\
	,build_coverage\
	,build_sanitize\
	,build_Debug\
	,build_Release\
	,documentation\
	,venv\
	,playground .

.PHONY: format
format:
	find src -iname '*.h' -o -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
	find sandbox2D -iname '*.h' -o -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
	find sandbox3D -iname '*.h' -o -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
	find sandboxOpenGL -iname '*.h' -o -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
	find tools -iname '*.h' -o -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i

