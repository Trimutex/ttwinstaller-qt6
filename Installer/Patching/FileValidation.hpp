#pragma once


namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    class FileValidation : IDisposable, IEquatable<FileValidation> {
    public:
        enum ChecksumType : byte {
            Murmur128,
            Md5
        }
        uint Filesize { get; private set; }
        byte[] Checksum { get { return _computeChecksum.Value; } }
        ChecksumType Type { get; private set; }

        // NOTE: read-only block
        Stream _stream;

        Lazy<byte[]> _computeChecksum;

        FileValidation(byte[] data, ChecksumType type = ChecksumType.Murmur128);
        FileValidation(Stream stream, ChecksumType type = ChecksumType.Murmur128);
        FileValidation(byte[] checksum, uint filesize, ChecksumType type = ChecksumType.Murmur128);

        FileValidation(string path, ChecksumType type = ChecksumType.Murmur128)
            : this(File.OpenRead(path), type) { };

        ~FileValidation();
        void Dispose();
        override string ToString();
        override bool Equals(object obj);
        bool Equals(FileValidation obj)
        static bool operator ==(FileValidation a, FileValidation b)
        static bool operator !=(FileValidation a, FileValidation b)
        static Dictionary<string, FileValidation> FromBSA(BSA bsa)
        static FileValidation FromBSAFile(BSAFile file, ChecksumType asType = ChecksumType.Murmur128)
        static FileValidation ReadFrom(BinaryReader reader)
        static void WriteTo(BinaryWriter writer, FileValidation fv)

    protected:
        virtual void Dispose(bool disposing);

    private:
/* VESTIGIAL MACRO
 *  [DllImport("msvcrt", CallingConvention = CallingConvention.Cdecl)]
 */
        static extern int memcmp(byte[] b1, byte[] b2, UIntPtr count);

        FileValidation(BinaryReader reader, byte typeByte);
        void SetContents(Func<byte[]> getChecksum, uint filesize, ChecksumType type);
        void WriteTo(BinaryWriter writer);
        HashAlgorithm GetHash();
    }
}
