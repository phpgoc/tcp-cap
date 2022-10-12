default: debug

debug:
	scripts/install_dependencies.sh
	build_type=debug scripts/build.sh

release:
	scripts/install_dependencies.sh
	build_type=release scripts/build.sh

config:
	test -d Debug && cp config.example.toml Debug/config.toml || true
	test -d Debug && cp server_config.example.toml Debug/server_config.toml || true
	test -d Release && cp config.example.toml Release/config.toml || true
	test -d Release && cp server_config.example.toml Release/server_config.toml || true

clean:
	rm -rf Debug Release