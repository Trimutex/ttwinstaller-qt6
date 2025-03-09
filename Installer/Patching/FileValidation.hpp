#pragma once

#include <map>
#include <sstream>
#include <string>

namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    class FileValidation : IDisposable, IEquatable<FileValidation> {
    public:
        enum ChecksumType : byte {
            Murmur128,
            Md5
        };

        unsigned m_filesize { get; private set; }
        uint8_t[] m_checksum { get { return _computeChecksum.Value; } }
        ChecksumType m_type { get; private set; }

        // NOTE: read-only block
        std::ostringstream _stream;

        Lazy<uint8_t[]> _computeChecksum;

        FileValidation(uint8_t[] data, ChecksumType type = ChecksumType.Murmur128);
        FileValidation(std::ostringstream stream, ChecksumType type = ChecksumType.Murmur128);
        FileValidation(uint8_t[] checksum, unsigned filesize, ChecksumType type = ChecksumType.Murmur128);

        FileValidation(std::string path, ChecksumType type = ChecksumType.Murmur128)
            : this(File.OpenRead(path), type) { };

        ~FileValidation();
        void dispose();
        override std::string toString();
        override bool equals(object obj);
        bool equals(FileValidation obj);
        static bool operator ==(FileValidation a, FileValidation b);
        static bool operator !=(FileValidation a, FileValidation b);
        static std::map<std::string, FileValidation> fromBSA(BSA bsa);
        static FileValidation fromBSAFile(BSAFile file, ChecksumType asType = ChecksumType.Murmur128);
        static FileValidation readFrom(BinaryReader reader);
        static void writeTo(BinaryWriter writer, FileValidation fv);

    protected:
        virtual void dispose(bool disposing);

    private:
/* VESTIGIAL MACRO
 *  [DllImport("msvcrt", CallingConvention = CallingConvention.Cdecl)]
 */
        static extern int memcmp(byte[] b1, byte[] b2, UIntPtr count);

        FileValidation(BinaryReader reader, uint8_t typeByte);
        void setContents(Func<uint8_t[]> getChecksum, unsigned filesize, ChecksumType type);
        void writeTo(BinaryWriter writer);
        HashAlgorithm getHash();
    }
}
