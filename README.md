MurmurInc
=========
MurmurHash3 incremental hash utility

Usage
-----

    #include "MurmurInc.h"
    murmur_inc h(seed);
    uint32_t out[4];

    h.hash(datum1, size1);
    h.hash(datum2, size2);
    // ...

    h.finalize(out);

Credits
-------
Based on https://code.google.com/p/smhasher/wiki/MurmurHash3
