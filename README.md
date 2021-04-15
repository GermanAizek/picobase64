# picobase64
Very fast and lightweight header encoder/decoder base64 and strlen.

**The fastest library for encoding and decoding base64!**

**THIS IS NOT A JOKE**

# Benchmarks

Additional information on benchmarks, tests and charts see below.

### Tests

*Less time (in microseconds) faster.*

The two fastest encoder implementations I found were Jouni Malinen's at

http://web.mit.edu/freebsd/head/contrib/wpa/src/utils/base64.c

and the Apache at

https://opensource.apple.com/source/QuickTimeStreamingServer/QuickTimeStreamingServer-452/CommonUtilitiesLib/base64.c.

```
#	Implementation	20	60	100	300	1000	2000	4000	9000	16000	30000	60000
1	adition	        0.066	0.091	0.114	0.233	0.645	1.303	2.469	5.397	9.465	17.900	35.913
2	polfosol_mod	0.071	0.095	0.139	0.276	0.816	1.548	3.390	8.021	12.415	24.797	45.845
3	polfosol	0.079	0.083	0.119	0.278	0.808	1.675	3.482	7.109	12.543	25.381	48.208
4	tomykaria	0.075	0.126	0.163	0.383	1.191	2.247	4.516	10.338	18.045	32.547	65.836
5	CppCodec	0.078	0.136	0.190	0.443	1.349	2.723	5.352	11.942	20.938	39.573	78.995
6	gnome	        0.091	0.152	0.212	0.512	1.509	2.892	5.857	12.630	22.295	42.216	84.956
7	apache	        0.094	0.180	0.264	0.607	1.850	3.600	7.097	15.923	28.017	52.350	104.498
```

![](https://raw.githubusercontent.com/GermanAizek/picobase64/main/time_encode.svg)

```

```

![](https://raw.githubusercontent.com/GermanAizek/picobase64/main/time_decode.svg)

# Copyrights

Thank you so much who developed the algorithms and who tested it.

The development of the *Base64 encoder* was done by **mohsenatigh and leguilc (ADITION technologies AG)**

The development of the *Base64 decoder* was done by **mohsenatigh and leguilc (ADITION technologies AG)**

Merge encoders, decoders, strlen in *picobase64* was done by maintainer **Herman Semenov (GermanAizek)**

https://github.com/GermanAizek/picobase64/

Repository *base64* with collection codes and tests was done by maintainer **Gaspard Petit (gaspardpetit)**

https://github.com/gaspardpetit/base64/

## License

PicoBase64 is licensed under the ([GNU General Public License 3.0](https://www.gnu.org/licenses/gpl-3.0.html))
