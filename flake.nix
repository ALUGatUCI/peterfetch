{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
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
      pkg-config
      git
    ];
  in {
    packages = forEachSystem (pkgs: rec {
      default = peterfetch;
      peterfetch = (pkgs.callPackage ./nix/package.nix { inherit commonPkgs; });
    });

    devShells = forEachSystem (pkgs: {
      default = pkgs.mkShell {
        packages = (commonPkgs pkgs) ++ [
          pkgs.clang-tools
        ];
      };
    });
  };
}
