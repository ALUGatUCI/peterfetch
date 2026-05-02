{ pkgs, stdenv, commonPkgs, ... }:
stdenv.mkDerivation {
  pname = "peterfetch";
  version = "unstable";
  src = ./..;

  enableParallelBuilding = true;
  doCheck = true;

  cmakeFlags = [
    "-GNinja"
  ];

  checkPhase = ''
  ctest -j$NIX_BUILD_CORES --output-on-failure
  '';

  nativeBuildInputs = (commonPkgs pkgs);
}
