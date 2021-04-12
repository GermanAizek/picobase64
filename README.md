# picobase64
Very fast and lightweight header encoder/decoder base64 and strlen.

**The fastest library for encoding and decoding base64!**

**THIS IS NOT A JOKE**

# Benchmarks

More info about benchmarks here:

https://github.com/gaspardpetit/base64/

### Tests

*Less time (in microseconds) faster.*

The two fastest encoder implementations I found were Jouni Malinen's at

http://web.mit.edu/freebsd/head/contrib/wpa/src/utils/base64.c

and the Apache at

https://opensource.apple.com/source/QuickTimeStreamingServer/QuickTimeStreamingServer-452/CommonUtilitiesLib/base64.c.

```
FASTEST ENCODERS: Here is the time (in microseconds) to encode 32K of data
using the different algorithms I have tested up to now:

jounimalinen                25.1544
apache                      25.5309
NibbleAndAHalf              38.4165
internetsoftwareconsortium  48.2879
polfosol                    48.7955
wikibooks_org_c             51.9659
gnome                       74.8188
elegantdice                 118.899
libb64                      120.601
manuelmartinez              120.801
arduino                     126.262
daedalusalpha               126.473
CppCodec                    151.866
wikibooks_org_cpp           343.200
adp_gmbh                    381.523
LihO                        406.693
libcurl                     3246.39
user152949                  4828.21
```

![](https://raw.githubusercontent.com/GermanAizek/picobase64/main/time_encode.svg)

```
FASTEST DECODERS: Here are the decoding results and I must admit that
I am a bit surprised:

polfosol                    45.2335
wikibooks_org_c             74.7347
apache                      77.1438
libb64                      100.332
gnome                       114.511
manuelmartinez              126.579
elegantdice                 138.514
daedalusalpha               151.561
jounimalinen                206.163
arduino                     335.950
wikibooks_org_cpp           350.437
CppCodec                    526.187
internetsoftwareconsortium  862.833
libcurl                     1280.27
LihO                        1852.40
adp_gmbh                    1934.43
user152949                  5332.87
```

![](https://raw.githubusercontent.com/GermanAizek/picobase64/main/time_decode.svg)
