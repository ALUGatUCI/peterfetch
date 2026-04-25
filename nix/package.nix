{ pkgs, stdenv, commonPkgs, ... }:
stdenv.mkDerivation {
  pname = "peterfetch";
  version = "unstable";
  
  src = ./..;

  nativeBuildInputs = (commonPkgs pkgs);
}
