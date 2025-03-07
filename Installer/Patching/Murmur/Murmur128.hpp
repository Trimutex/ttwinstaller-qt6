#pragma once

namespace TaleOfTwoWastelandsPatchingMurmur {

    // NOTE: originally public
    // NOTE: originally abstract
    class Murmur128 : HashAlgorithm {
    public:
        uint seed(void);
        static Murmur128 createMurmur(void);

    protected:
        Murmur128(uint seed) : _m_seed(seed);

    private:
        // TODO: read-only block
        uint _m_seed;

    };
}

