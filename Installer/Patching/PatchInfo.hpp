#pragma once

namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    class PatchInfo {
    public:
        FileValidation Metadata { get; set; }
        byte[] Data { get; set; }

        PatchInfo() { }
        PatchInfo(BinaryReader reader);
        void WriteTo(BinaryWriter writer);
        bool PatchBytes(byte[] inputBytes, FileValidation targetChk, out byte[] outputBytes, out FileValidation outputChk);
        bool PatchStream(Stream input, FileValidation targetChk, Stream output, out FileValidation outputChk);

#if LEGACY || DEBUG
        static PatchInfo FromOldDiff(byte[] diffData, FileValidation oldChk);
#endif

    }
}
