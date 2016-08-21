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

## ZSH configuration

You will find the shell configuration for zsh in __examples__ directory.

Just copy the content at the end of your __~/.zshrc__, __/etc/zsh/zshrc__ or __/usr/local/etc/zshrc__ and enjoy.

## Tmux configuration

You will find an example configuration for tmux in __examples/__ directory.

Just copy it to __~/.tmux.conf__, __/etc/tmux.conf__ or __/usr/local/etc/tmux.conf__.

Ensure you current shell has a LANG environment variable with UTF-8 support.

## Screenshots

![powerline+mux](https://lut.im/DmzvDlaOxd/cQenonLp95Yfnn55.png "Powerline + tmux")
