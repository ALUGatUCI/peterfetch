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
  json-src = pkgs.fetchFromGitHub {
    owner = "nlohmann";
    repo = "json";
    rev = "v3.12.0";
    hash = "sha256-cECvDOLxgX7Q9R3IE86Hj9JJUxraDQvhoyPDF03B2CY=";
  };
in stdenv.mkDerivation {
  pname = "peterfetch";
  version = "unstable";
  src = ./..;

  enableParallelBuilding = true;

  cmakeFlags = [
    "-DFETCHCONTENT_SOURCE_DIR_CPR=${cpr-src}"
    "-DFETCHCONTENT_SOURCE_DIR_ARGS=${args-src}"
    "-DFETCHCONTENT_SOURCE_DIR_NLOHMANN_JSON=${json-src}"
  ];

  nativeBuildInputs = (commonPkgs pkgs);
}
