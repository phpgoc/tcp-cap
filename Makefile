default: debug

debug:
	scripts/install_dependencies.sh
	build_type=debug scripts/build.sh

release:
	scripts/install_dependencies.sh
	build_type=release scripts/build.sh

clean:
	rm -rf Debug Release