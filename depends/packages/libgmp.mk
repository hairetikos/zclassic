package=libgmp
$(package)_version=6.3.0
$(package)_download_path=https://gmplib.org/download/gmp/
$(package)_file_name=gmp-$($(package)_version).tar.bz2
$(package)_sha256_hash=a8f40f5e4aa7f7c643c286e6749a8437cffc0d9666a907b6b11ed3be48caa20e
$(package)_dependencies=
$(package)_config_opts=--enable-cxx --disable-shared
$(package)_config_opts_arm_darwin=--disable-assembly

define $(package)_config_cmds
  $($(package)_autoconf) --host=$(host) --build=$(build)
endef

define $(package)_build_cmds
  $(MAKE) CPPFLAGS='-fPIC'
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install ; echo '=== staging find for $(package):' ; find $($(package)_staging_dir)
endef
