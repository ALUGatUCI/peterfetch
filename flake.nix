{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

    # Since Git submodules are used to vendor dependencies, they must be explicitly allowed.
    # This does require a Nix version >= 2.27.0
    self.submodules = true;
  };

  outputs = { self, nixpkgs }: let
    forEachSystem = func: nixpkgs.lib.genAttrs [
      "x86_64-linux"
      "aarch64-linux"
    ] ( system: func (import nixpkgs { inherit system; }) );

    commonPkgs = pkgs: with pkgs; [
      cmake
      ninja
      curl
      git
      clang
    ];
  in {
    packages = forEachSystem (pkgs: rec {
      default = peterfetch;
      peterfetch = (pkgs.callPackage ./nix/package.nix { inherit commonPkgs; });
    });

    devShells = forEachSystem (pkgs: {
      default = pkgs.mkShell {
        packages = (commonPkgs pkgs) ++ (with pkgs; [
          clang-tools
          lldb
        ]);
        CMAKE_GENERATOR = "Ninja";
        CLICOLOR_FORCE = 1;
        CTEST_OUTPUT_ON_FAILURE = 1;
      };
    });
  };
}
