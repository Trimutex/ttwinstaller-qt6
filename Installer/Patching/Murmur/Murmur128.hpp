#pragma once

namespace TaleOfTwoWastelandsPatchingMurmur {
    public abstract class Murmur128 : HashAlgorithm {
    public:
        uint Seed(void);
        static Murmur128 CreateMurmur(void);

    protected:
        Murmur128(uint seed) : _m_seed(seed);

    private:
        // TODO: read-only block
        private readonly uint _m_seed;

    };
}

