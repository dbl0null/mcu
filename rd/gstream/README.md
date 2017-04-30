# Compiling
```
cd ~/git/apgar/gstream
gcc -g staticimage.c -o staticimage `pkg-config --cflags --libs gstreamer-1.0`
gdb ./staticimage
```

http://wiki.oz9aec.net/index.php/Gstreamer_cheat_sheet
