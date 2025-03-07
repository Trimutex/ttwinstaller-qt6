#pragma once


namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    class FileValidation : IDisposable, IEquatable<FileValidation> {
    public:
        enum ChecksumType : byte {
            Murmur128,
            Md5
        }
        uint m_filesize { get; private set; }
        byte[] m_checksum { get { return _computeChecksum.Value; } }
        ChecksumType m_type { get; private set; }

        // NOTE: read-only block
        Stream _stream;

        Lazy<byte[]> _computeChecksum;

        FileValidation(byte[] data, ChecksumType type = ChecksumType.Murmur128);
        FileValidation(Stream stream, ChecksumType type = ChecksumType.Murmur128);
        FileValidation(byte[] checksum, uint filesize, ChecksumType type = ChecksumType.Murmur128);

        FileValidation(string path, ChecksumType type = ChecksumType.Murmur128)
            : this(File.OpenRead(path), type) { };

        ~FileValidation();
        void dispose();
        override string toString();
        override bool equals(object obj);
        bool equals(FileValidation obj)
        static bool operator ==(FileValidation a, FileValidation b)
        static bool operator !=(FileValidation a, FileValidation b)
        static Dictionary<string, FileValidation> fromBSA(BSA bsa)
        static FileValidation fromBSAFile(BSAFile file, ChecksumType asType = ChecksumType.Murmur128)
        static FileValidation readFrom(BinaryReader reader)
        static void writeTo(BinaryWriter writer, FileValidation fv)

    protected:
        virtual void dispose(bool disposing);

    private:
/* VESTIGIAL MACRO
 *  [DllImport("msvcrt", CallingConvention = CallingConvention.Cdecl)]
 */
        static extern int memcmp(byte[] b1, byte[] b2, UIntPtr count);

        FileValidation(BinaryReader reader, byte typeByte);
        void setContents(Func<byte[]> getChecksum, uint filesize, ChecksumType type);
        void writeTo(BinaryWriter writer);
        HashAlgorithm getHash();
    }
}
