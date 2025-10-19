# picobase64
Very fast and lightweight header-only base64 library. Without excess SIMD, dependencies. Include file and work with it.

Simple just coding.

Have fun!

### Many thanks:
- 2025 [vogma Marco](https://github.com/vogma) compilation fixes

# Example

```cpp
#include <iostream>
#include <string>
#include "picobase64.h"

int main() {
    std::string str = "John Doe with IBM 5100";
    std::cout << b64encode(str); // Sm9obiBEb2Ugd2l0aCBJQk0gNTEwMA==
    std::string base64_str = b64encode(str);
    std::cout << b64decode(base64_str); // John Doe with IBM 5100
    return 0;
}
```

### Tests and benchmarks

Repository *base64* with collection codes and tests was done by maintainer **Gaspard Petit (gaspardpetit)**

https://github.com/gaspardpetit/base64/

# Copyrights

Thank you so much who developed the algorithms and who tested it.

The development of the *Base64 encoder* was done by **mohsenatigh and leguilc (ADITION technologies AG)**

The development of the *Base64 decoder* was done by **mohsenatigh and leguilc (ADITION technologies AG)**

Merge encoders, decoders, strlen in *picobase64* was done by maintainer **Herman Semenov (GermanAizek)**

https://github.com/GermanAizek/picobase64/

## License

PicoBase64 is licensed under the ([GNU General Public License 3.0](https://www.gnu.org/licenses/gpl-3.0.html))
