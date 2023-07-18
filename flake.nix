{
  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable";
  };
  outputs = { self, nixpkgs }: let
    pkgs = nixpkgs.legacyPackages.x86_64-linux.pkgs;
  in {
    devShells.x86_64-linux.default = pkgs.mkShell {
      buildInputs = with pkgs; [
        ccls
        platformio
        python3
        gcc-arm-embedded
        picotool
        # This is a serial monitor
        # This isn't named after the pico
        # Just a coincidence
        picocom
      ];
    };
  };
}
