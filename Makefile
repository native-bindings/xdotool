#!/bin/sh

NAN_INCLUDE_DIRECTORY := node_modules/nan

configure:
	mkdir -pv build/cmake-vscode-debug && \
	cd build/cmake-vscode-debug && \
	cmake \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
		-DNAN_INCLUDE_DIRECTORY=$(NAN_INCLUDE_DIRECTORY) \
		-DNODE_PATH=$(NODE_PATH) \
		-DCMAKE_BUILD_TYPE=Debug ../..

create_compile_commands_link: configure
	ln -sv build/cmake-vscode-debug/compile_commands.json compile_commands.json

test:
	npx sarg --require ts-node/register test/test.ts

release:
	npx tsc