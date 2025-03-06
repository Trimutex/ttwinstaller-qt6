#pragma once

namespace TaleOfTwoWastelandsPatching {
    using Patch = Tuple<FileValidation, PatchInfo[]>;


    // NOTE: public class
    class PatchDict : Dictionary<string, Patch> {
    public:
        PatchDict(int size) : base(size) { };
        PatchDict(string file) : this(new BinaryReader(File.OpenRead(file))) { };
        void writeAll(Stream outStream);
#if LEGACY
        static PatchDict fromOldDatabase(IDictionary<string, string> oldDict, string prefix, Func<byte[], byte[]> convertPatch);
#endif

    private:
        PatchDict(BinaryReader reader) : this(reader, reader.ReadInt32()) { };
        PatchDict(BinaryReader reader, int size);
        Patch readPatches(BinaryReader reader);
        byte[] writePatches(Patch patch);

    }
}
