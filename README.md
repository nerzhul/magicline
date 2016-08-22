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

Just copy the following line at the end of your __~/.zshrc__, __/etc/zsh/zshrc__ or __/usr/local/etc/zshrc__ and enjoy.

```
source /usr/local/share/magicline/zshrc
```

## Tmux configuration

Just source the followine line to __~/.tmux.conf__, __/etc/tmux.conf__ or __/usr/local/etc/tmux.conf__.

```
source /usr/local/share/magicline/tmux.conf
```

If you have tmux v1 you should use
```
source /usr/local/share/magicline/tmux-v1.conf
```

Ensure you current shell has a LANG environment variable with UTF-8 support.

## Screenshots

![powerline+mux](https://lut.im/DmzvDlaOxd/cQenonLp95Yfnn55.png "Powerline + tmux")
