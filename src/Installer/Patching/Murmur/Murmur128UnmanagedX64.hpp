/*  Copyright 2012 Darren Kopp
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///    http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
*/

#pragma once
namespace TaleOfTwoWastelandsPatchingMurmur {
    // NOTE: originally internal
    class Murmur128UnmanagedX64 : Murmur128 {
    public:
        // NOTE: originally internal
        internal Murmur128UnmanagedX64(unsigned seed = 0);

        override void initialize(void);

    protected:
        override void hashCore(uint8_t[] array, int ibStart, int cbSize);
        override uint8_t[] hashFinal(void);

    private:
        // NOTE: originally internal
        const unsigned long m_c1 = 0x87c37b91114253d5UL;
        const unsigned long m_c2 = 0x4cf5ad432745937fUL;
        unsigned long m_h1 { get; set; }
        unsigned long m_h2 { get; set; }
        int m_length { get; set; }

/* VESTIGIAL MACRO
 *  #if NET45
 *      [MethodImpl(MethodImplOptions.AggressiveInlining)]
 *  #endif
 */
        // NOTE: originally unsafe
        void tail(uint8_t* tail, int remaining);
        void reset(void);

/* VESTIGIAL MACRO
 *  #if NET45
 *      [MethodImpl(MethodImplOptions.AggressiveInlining)]
 *  #endif
 */
        void body(uint8_t[] data, int start, int length);

    }
}
