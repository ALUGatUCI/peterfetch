{ pkgs, stdenv, commonPkgs, ... }:
stdenv.mkDerivation {
  pname = "peterfetch";
  version = "unstable";
  src = ./..;

  enableParallelBuilding = true;

  cmakeFlags = [
    "-GNinja"
  ];

  nativeBuildInputs = (commonPkgs pkgs);
}
