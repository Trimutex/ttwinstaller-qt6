#pragma once

#include <string>

namespace TaleOfTwoWastelands {
    // NOTE: sealed class
    class FinalizedTextWriter : TextWriter {
    public:
        FinalizedTextWriter(std::string path);

        override void write(char[] buffer, int index, int count);

        override Encoding m_encoding;

    private:
        // NOTE: read-only block
        Crc32 _m_checksum = new Crc32();
        int _m_checksumSize;
        FileStream _m_stream;

        string m_checksum;

        uint8_t[] updateChecksum(char[] s);

        bool _m_writingChecksum;
    }
}
