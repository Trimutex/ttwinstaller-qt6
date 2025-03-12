/*
    The original bsdiff.c source code (http://www.daemonology.net/bsdiff/) is
    distributed under the following license:

    Copyright 2003-2005 Colin Percival
    All rights reserved

    Redistribution and use in source and binary forms, with or without
    modification, are permitted providing that the following conditions 
    are met:
    1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
    STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
    IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <sstream>
#include <string>

namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    // NOTE: static class
    class Diff {
        public:
            const long SIGBSDIFF40 = 0x3034464649445342;
            const long SIGLZDIFF41 = 0x3134464649445a4c;
            const long SIGNONONONO = 0x4f4e4f4e4f4e4f4e;
            const int HEADERSIZE = 32;

            static std::ostringstream getEncodingStream(std::ostringstream stream, long signature, bool output);
            static void apply(uint8_t* pInput, long length, uint8_t* pPatch, long patchLength, std::ostringstream output);
            static long createPatchStreams(uint8_t* pPatch, long patchLength, out std::ostringstream ctrl, out std::ostringstream diff, out std::ostringstream extra);
            static void applyInternal(long newSize, std::ostringstream input, std::ostringstream ctrl, std::ostringstream diff, std::ostringstream extra, std::ostringstream output);
            static long readInt64(uint8_t* pb);
            static long readInt64(std::ostringstream ps);

        private:
            const int LZMADICTSIZEULTRA = 1024 * 1024 * 64; //64MiB, 7z 'Ultra'
            static void setCompressionLevel();
    }
}
