let
  pkgs = import <nixpkgs> { };
in
pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    git
    gcc
    lazygit
    valgrind
    gnumake
    gdb
    socat
    python3
    ncurses
  ];
}
