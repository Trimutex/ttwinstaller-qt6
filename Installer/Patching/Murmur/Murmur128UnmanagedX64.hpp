#pragma once
namespace TaleOfTwoWastelandsPatchingMurmur {
    // NOTE: originally internal
    class Murmur128UnmanagedX64 : Murmur128 {
    public:
        // NOTE: originally internal
        internal Murmur128UnmanagedX64(uint seed = 0);

        override void initialize(void);

    protected:
        override void hashCore(byte[] array, int ibStart, int cbSize);
        override byte[] hashFinal(void);

    private:
        // NOTE: originally internal
        const ulong m_c1 = 0x87c37b91114253d5UL;
        const ulong m_c2 = 0x4cf5ad432745937fUL;
        ulong m_h1 { get; set; }
        ulong m_h2 { get; set; }
        int m_length { get; set; }

/* VESTIGIAL MACRO
 *  #if NET45
 *      [MethodImpl(MethodImplOptions.AggressiveInlining)]
 *  #endif
 */
        // NOTE: originally unsafe
        void tail(byte* tail, int remaining);
        void reset(void);

/* VESTIGIAL MACRO
 *  #if NET45
 *      [MethodImpl(MethodImplOptions.AggressiveInlining)]
 *  #endif
 */
        void body(byte[] data, int start, int length);

    }
}
