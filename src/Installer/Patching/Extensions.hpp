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

namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    // NOTE: static class
    class Extensions {
    public:
        // WARN: prob needs to be protected and using friend classes
        // due to vigorous usage of internal originally 
        static unsigned toUInt32(this uint8_t[] data, int start = 0);
        static unsigned toUInt32(this uint8_t[] data, int start = 0);
        static unsigned long toUInt64(this uint8_t[] data, int start = 0);
        static unsigned rotateLeft(this unsigned x, uint8_t r);
        static unsigned long rotateLeft(this unsigned long x, uint8_t r);
        static unsigned fMix(this unsigned h);
        static unsigned long fMix(this unsigned long h);

    }
}
