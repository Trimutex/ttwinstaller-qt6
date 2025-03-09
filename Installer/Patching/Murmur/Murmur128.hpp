#pragma once

namespace TaleOfTwoWastelandsPatchingMurmur {

    // NOTE: originally public
    // NOTE: originally abstract
    class Murmur128 : HashAlgorithm {
    public:
        unsigned seed(void);
        static Murmur128 createMurmur(void);

    protected:
        Murmur128(unsigned seed) : _m_seed(seed);

    private:
        // TODO: read-only block
        unsigned _m_seed;

    };
}

