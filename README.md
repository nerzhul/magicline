# Magicline

## Compilation instructions

magicline compilation uses cmake and c++11. Please ensure you have gcc 4.9 or greater or clang 3.4 and greater

Clone project and then follow the build step mentioned under.

```shell
mkdir build
cd build
cmake ..
make
sudo make install
```

## Tmux configuration

You will find an example configuration for tmux in __examples/__ directory.

Just copy it to __~/.tmux.conf__, __/etc/tmux.conf__ or __/usr/local/etc/tmux.conf__ and enjoy.
