#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: sealed class
    class FinalizedTextWriter : TextWriter {
    public:
        FinalizedTextWriter(string path);

        override void write(char[] buffer, int index, int count);

        override Encoding m_encoding;

    private:
        // NOTE: read-only block
        Crc32 _m_checksum = new Crc32();
        int _m_checksumSize;
        FileStream _m_stream;

        string m_checksum;

        byte[] updateChecksum(char[] s);

        bool _m_writingChecksum;
    }
}
