{ ... }:
{
  # Worktrees can cause issues
  projectRootFile = "flake.nix";

  programs.nixfmt.enable = true;
  programs.clang-format.enable = true;
}
