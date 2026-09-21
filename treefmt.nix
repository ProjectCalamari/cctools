{ lib, pkgs, ... }:
{
  projectRootFile = "flake.nix";

  programs = {
    clang-format.enable = true;
    nixfmt.enable = true;
    zig.enable = true;
  };
}
