#!/bin/sh

NAN_INCLUDE_DIRECTORY := $(PWD)/node_modules/nan

configure:
	mkdir -pv build/cmake-vscode-debug && \
	cd build/cmake-vscode-debug && \
	cmake \
		-DNODE_INSTALL_DIR="$(NODE_INSTALL_DIR)" \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
		-DNAN_INCLUDE_DIRECTORY="$(NAN_INCLUDE_DIRECTORY)" \
		-DCMAKE_BUILD_TYPE=Debug ../..

create_compile_commands_link: configure
	ln -sv build/cmake-vscode-debug/compile_commands.json compile_commands.json

test:
	npx sarg \
		--require ts-node/register \
		keylogger/test.ts

release:
	npx tsc -b typescript

publish: test release
	npm publish
