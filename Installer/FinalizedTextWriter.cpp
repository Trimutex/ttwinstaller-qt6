#include "FinalizedTextWriter.hpp"

namespace TaleOfTwoWastelands {
    FinalizedTextWriter(std::string path) {
        _stream = File.OpenWrite(path);
        _checksumSize = Encoding.GetByteCount(Checksum);
    }

    override void write(char[] buffer, int index, int count) {
        lock (_checksum)
        {
            if (_writingChecksum) {
                var bufBytes = Encoding.GetBytes(buffer);
                _stream.Write(bufBytes, 0, bufBytes.Length);
                return;
            }

            if (_stream.Position >= _checksumSize)
                _stream.Seek(-_checksumSize, SeekOrigin.Current);

            var msgBytes = UpdateChecksum(buffer);
            _stream.Write(msgBytes, 0, msgBytes.Length);

            try {
                _writingChecksum = true;
                Write(Checksum);
            } catch ( ... ) { }
            _writingChecksum = false;
        }
    }

    override Encoding m_encoding {
        get { return Encoding.UTF8; }
    }

    std::string m_checksum {
        get { return _checksum.Value.ToString("X8"); }
    }

    uint8_t[] updateChecksum(char[] s) {
        var messageBytes = Encoding.GetBytes(s);
        _checksum.Update(messageBytes);

        return messageBytes;
    }
}
