#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: sealed class
    class FinalizedTextWriter : TextWriter {
    public:
        FinalizedTextWriter(string path);

        override void Write(char[] buffer, int index, int count);

        override Encoding Encoding;

    private:
        // NOTE: read-only block
        Crc32 _checksum = new Crc32();
        int _checksumSize;
        FileStream _stream;

        string Checksum;

        byte[] UpdateChecksum(char[] s);

        bool _writingChecksum;
    }
}
