package=libsodium
$(package)_version=1.0.20
$(package)_download_path=https://download.libsodium.org/libsodium/releases/
$(package)_file_name=$(package)-$($(package)_version).tar.gz
$(package)_sha256_hash=8b84e4faabbde817eb8414b210cfba6ed9fa3a1eea933b2de25e514ca5ac96e9
$(package)_dependencies=
$(package)_config_opts=

define $(package)_preprocess_cmds
  echo "Skipping autogen.sh - using pre-generated configure"
endef

define $(package)_config_cmds
  $($(package)_autoconf) --enable-static --disable-shared
endef

define $(package)_build_cmds
  $(MAKE)
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef
