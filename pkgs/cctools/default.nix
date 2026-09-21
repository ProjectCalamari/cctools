{
  lib,
  stdenv,
  buildPackages,
  flakever,
  mkShell,
  zig,
  xnu,
}:
stdenv.mkDerivation (finalAttrs: {
  pname = "cctools";
  inherit (flakever) version;

  src = lib.cleanSource ../../.;

  nativeBuildInputs = [
    zig
  ];

  zigBuildFlags = [
    "-Dxnu-src=${xnu}"
  ];

  passthru.shell = mkShell {
    name = "cctools-dev-shell";
    packages = [
      zig
    ];
  };
})
