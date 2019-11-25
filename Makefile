#!/bin/sh

NAN_INCLUDE_DIRECTORY := node_modules/nan

configure:
	([ -e compile_commands.json ] && unlink compile_commands.json) && \
	mkdir -pv build/cmake-vscode-debug && \
	cd build/cmake-vscode-debug && \
	cmake \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
		-DNAN_INCLUDE_DIRECTORY=$(NAN_INCLUDE_DIRECTORY) \
		-DNODE_PATH=$(NODE_PATH) \
		-DCMAKE_BUILD_TYPE=Debug ../..

create_compile_commands_link: configure
	ln -sv build/cmake-vscode-debug/compile_commands.json compile_commands.json

release:
	npx tsc