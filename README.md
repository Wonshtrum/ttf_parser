A primitive TTF font parser and renderer. Final goal is to understand and apply the TTF bytecode.

Current state (`A` with vtks font):

![A vtks](vtks_A.png)

With this project I'm also playing with header only code and derived macros for structures:
```C
#define CLASS NAME(Glyph)		\
	FIELD(GlyphHeader, header)	\
	FIELD(VEC(Point), points)	\
	FIELD(VEC(u16), contours)
#include "define_class.h"
#include "derive_debug.h"
#undef CLASS

// debug_fmt_Glyph(&glyph, FMT_NAMES | FMT_INLINE | FMT_CAP | FMT_NL | FMT_FNL );
debug_Glyph(&glyph);
free_Glyph(&glyph);
```

```
Glyph {
    header: GlyphHeader {
        n_contours: 23,
        x_min: -37,
        y_min: -194,
        x_max: 975,
        y_max: 1266,
    },
    points: vec(698, 698)[
        Point {
            x: 239,
            y: 1266,
            flags: 1,
        },
        Point {
            x: 243,
            y: 1262,
            flags: 1,
        },
        Point {
            x: 243,
            y: 1254,
            flags: 1,
        },
        ...
    ],
    contours: vec(23, 23)[ 337, 353, 370, 386, 399, 412, 423, 434, 448, 457, ... ],
}
```
