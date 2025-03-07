#include "FileValidation.hpp"

namespace TaleOfTwoWastelandsPatching {
        FileValidation(byte[] data, ChecksumType type = ChecksumType.Murmur128) {
            if (data == null)
                throw new ArgumentNullException("data");

            SetContents(() => {
                using (var hash = GetHash())
                    return hash.ComputeHash(data);
            }, (uint)data.LongLength, type);
        }

        FileValidation(Stream stream, ChecksumType type = ChecksumType.Murmur128) {
            if (stream == null)
                throw new ArgumentNullException("stream");

            _stream = stream;
            SetContents(() => {
                using (stream)
                using (var hash = GetHash())
                    return hash.ComputeHash(stream);
            }, (uint)stream.Length, type);
        }

        FileValidation(byte[] checksum, uint filesize, ChecksumType type = ChecksumType.Murmur128) {
            if (checksum == null)
                throw new ArgumentNullException("checksum");
            if (checksum.Length != 16)
                throw new ArgumentException("checksum must be 128bit");
            if (filesize == 0)
                filesize = uint.MaxValue;
            //throw new ArgumentException("filesize must have a value");

            SetContents(() => checksum, filesize, type);
        }

        ~FileValidation() {
            Dispose(false);
        }

        void dispose() {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        override string toString() {
            return string.Format("({0}, {1} bytes, {2})", BitConverter.ToString(Checksum), Filesize, Enum.GetName(typeof(ChecksumType), Type));
        }

        override bool equals(object obj) {
            return Equals(obj as FileValidation);
        }

        bool equals(FileValidation obj) {
            if (obj == null)
                return false;

            Debug.Assert(Type == obj.Type);

            if (Filesize != obj.Filesize && Filesize != uint.MaxValue && obj.Filesize != uint.MaxValue)
                return false;

            return Checksum.Length == obj.Checksum.Length && memcmp(Checksum, obj.Checksum, (UIntPtr)Checksum.Length) == 0;
        }

        static bool operator ==(FileValidation a, FileValidation b) {
            bool
                nullA = ReferenceEquals(a, null),
                nullB = ReferenceEquals(b, null);
            if (nullA || nullB) {
                return nullA && nullB;
            }

            return a.Equals(b);
        }

        static bool operator !=(FileValidation a, FileValidation b) {
            return !(a == b);
        }

        static Dictionary<string, FileValidation> fromBSA(BSA bsa) {
            return bsa
                .SelectMany(folder => folder)
                .ToDictionary(file => file.Filename, file => FromBSAFile(file));
        }

        static FileValidation fromBSAFile(BSAFile file, ChecksumType asType = ChecksumType.Murmur128) {
            return new FileValidation(file.GetContents(true), asType);
        }
        
        // NOTE: originally internal
        static FileValidation readFrom(BinaryReader reader) {
            var typeByte = reader.ReadByte();
            if (typeByte != byte.MaxValue)
                return new FileValidation(reader, typeByte);

            return null;
        }

        // NOTE: originally internal
        static void writeTo(BinaryWriter writer, FileValidation fv) {
            if (fv != null)
                fv.WriteTo(writer);
            else
                writer.Write(byte.MaxValue);
        }

        virtual void dispose(bool disposing) {
            if (disposing) {
                if (_stream != null)
                    _stream.Dispose();
            }
        }

/* VESTIGIAL MACRO
 *  [DllImport("msvcrt", CallingConvention = CallingConvention.Cdecl)]
 */
        static extern int memcmp(byte[] b1, byte[] b2, UIntPtr count);

        FileValidation(BinaryReader reader, byte typeByte) {
            Debug.Assert(typeByte != byte.MaxValue);

            var type = (ChecksumType)typeByte;
            var filesize = reader.ReadUInt32();
            var checksum = reader.ReadBytes(16);

            SetContents(() => checksum, filesize, type);
        }

        void setContents(Func<byte[]> getChecksum, uint filesize, ChecksumType type) {
            _computeChecksum = new Lazy<byte[]>(getChecksum);
            Filesize = filesize;
            Type = type;
        }

        void writeTo(BinaryWriter writer) {
            writer.Write((byte)Type);
            writer.Write(Filesize);
            writer.Write(Checksum);
        }

        HashAlgorithm getHash() {
            switch (Type) {
                case ChecksumType.Murmur128:
                    return Murmur128.CreateMurmur();
                case ChecksumType.Md5:
                    return MD5.Create();
                default:
                    throw new NotImplementedException("Unknown checksum type: " + Type);
            }
        }
    }
}
