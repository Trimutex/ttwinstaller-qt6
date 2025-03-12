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

namespace PatchMaker {
    // NOTE: internal class
    static class MakeDiff {
    public:
        static void create(uint8_t[] oldBuf, uint8_t[] newBuf, long signature, std::ostringstream output);

    private:
        const int HEADERSIZE = Diff.HEADERSIZE;

        static long readInt64(uint8_t* buf);
        static std::ostringstream getEncodingStream(std::ostringstream stream, long signature, bool output);
        static uint8_t[] convertPatch(uint8_t* pPatch, long length, long inputSig, long outputSig);
        static int compareBytes(uint8_t* left, int leftLength, uint8_t* right, int rightLength);
        static int matchLength(uint8_t* oldData, int oldLength, uint8_t* newData, int newLength);
        static int search(uint8_t* I, uint8_t* oldData, int oldLength, uint8_t* newData, int newLength, int newOffset, int start, int end, out int pos);
        static void writeInt64(long y, uint8_t* pb);
    }
}
