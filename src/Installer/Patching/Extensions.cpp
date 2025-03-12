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

#include "Extensions.hpp"

namespace TaleOfTwoWastelandsPatching {

    /* VESTIGIAL MACRO
     *  #if NET45
     *      [MethodImpl(MethodImplOptions.AggressiveInlining)]
     *  #endif
     */
    // NOTE: originally internal
    static unsigned toUInt32(this uint8_t[] data, int start = 0) {
        return BitConverter.IsLittleEndian
            ? (uint)(data[start] | data[start + 1] << 8 | data[start + 2] << 16 | data[start + 3] << 24)
            : (uint)(data[start] << 24 | data[start + 1] << 16 | data[start + 2] << 8 | data[start + 3]);
    }

    /* VESTIGIAL MACRO
     *  #if NET45
     *      [MethodImpl(MethodImplOptions.AggressiveInlining)]
     *  #endif
     */
    // NOTE: originally internal
    static unsigned long toUInt64(this uint8_t[] data, int start = 0) {
        if (BitConverter.IsLittleEndian) {
            var i1 = (uint)(data[start] | data[start + 1] << 8 | data[start + 2] << 16 | data[start + 3] << 24);
            var i2 = (ulong)(data[start + 4] | data[start + 5] << 8 | data[start + 6] << 16 | data[start + 7] << 24);
            return (i1 | i2 << 32);
        } else {
            var i1 = (ulong)(data[start] << 24 | data[start + 1] << 16 | data[start + 2] << 8 | data[start + 3]);
            var i2 = (uint)(data[start + 4] << 24 | data[start + 5] << 16 | data[start + 6] << 8 | data[start + 7]);
            return (i2 | i1 << 32);

            //int i1 = (*pbyte << 24) | (*(pbyte + 1) << 16) | (*(pbyte + 2) << 8) | (*(pbyte + 3));
            //int i2 = (*(pbyte + 4) << 24) | (*(pbyte + 5) << 16) | (*(pbyte + 6) << 8) | (*(pbyte + 7));
            //return (uint)i2 | ((long)i1 << 32);
        }
    }

    /* VESTIGIAL MACRO
     *  #if NET45
     *      [MethodImpl(MethodImplOptions.AggressiveInlining)]
     *  #endif
     */
    // NOTE: originally internal
    static unsigned rotateLeft(this unsigned x, uint8_t r) {
        return (x << r) | (x >> (32 - r));
    }

    /* VESTIGIAL MACRO
     *  #if NET45
     *      [MethodImpl(MethodImplOptions.AggressiveInlining)]
     *  #endif
     */
    // NOTE: originally internal
    static unsigned long rotateLeft(this unsigned long x, uint8_t r) {
        return (x << r) | (x >> (64 - r));
    }

    /* VESTIGIAL MACRO
     *  #if NET45
     *      [MethodImpl(MethodImplOptions.AggressiveInlining)]
     *  #endif
     */
    // NOTE: originally internal
    static unsigned fMix(this unsigned h) {
        // pipelining friendly algorithm
        h = (h ^ (h >> 16)) * 0x85ebca6b;
        h = (h ^ (h >> 13)) * 0xc2b2ae35;
        return h ^ (h >> 16);
    }

    /* VESTIGIAL MACRO
     *  #if NET45
     *      [MethodImpl(MethodImplOptions.AggressiveInlining)]
     *  #endif
     */
    // NOTE: originally internal
    static unsigned long fMix(this unsigned long h) {
        // pipelining friendly algorithm
        h = (h ^ (h >> 33)) * 0xff51afd7ed558ccd;
        h = (h ^ (h >> 33)) * 0xc4ceb9fe1a85ec53;

        return (h ^ (h >> 33));
    }
}
