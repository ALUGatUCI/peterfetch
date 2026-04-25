{ pkgs, stdenv, commonPkgs, ... }:
let
  cpr-src = pkgs.fetchFromGitHub {
    owner = "libcpr";
    repo = "cpr";
    rev = "1.14.2";
    hash = "sha256-fglJNQzf+5c5nJysxqTxE4EWSQO0GVauLV8yLypQMPs=";
  };
  args-src = pkgs.fetchFromGitHub {
    owner = "Taywee";
    repo = "args";
    rev = "6.4.12";
    hash = "sha256-RW+n6pX8rCqbOqMOBHh1B+JZZXzQRj2CIZh4+RyjMAM=";
  };
in stdenv.mkDerivation {
  pname = "peterfetch";
  version = "unstable";
  src = ./..;

  enableParallelBuilding = true;

  cmakeFlags = [
    "-DFETCHCONTENT_SOURCE_DIR_CPR=${cpr-src}"
    "-DFETCHCONTENT_SOURCE_DIR_ARGS=${args-src}"
  ];

  nativeBuildInputs = (commonPkgs pkgs);
}
