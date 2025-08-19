{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      flake-utils,
      nixpkgs,
      ...
    }@inputs:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages."${system}";
      in
      {
        # Override is needed to install clang19 instead of gcc (which is default for mkShell since it builds on mkDerivation)
        devShell = pkgs.mkShell.override { stdenv = pkgs.clang19Stdenv; } {
          packages = [
            # Provides many clang utility binaries such as `clangd`
            # See: nix-locate bin/clang | grep llvmPackages_<version>.clang-tools
            pkgs.llvmPackages_19.clang-tools
            # Provides lldb and lldb-dap debuggers
            pkgs.lldb
            # General
            pkgs.cmake
            pkgs.libGL
            # GLFW for wayland dependencies
            pkgs.pkg-config
            pkgs.wayland
            pkgs.libxkbcommon
            pkgs.gtk3
          ];
        };
      }
    );
}
