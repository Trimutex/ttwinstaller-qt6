#pragma once

namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    class PatchInfo {
    public:
        FileValidation m_metadata { get; set; }
        byte[] m_data { get; set; }

        PatchInfo() { }
        PatchInfo(BinaryReader reader);
        void writeTo(BinaryWriter writer);
        bool patchBytes(byte[] inputBytes, FileValidation targetChk, out byte[] outputBytes, out FileValidation outputChk);
        bool patchStream(Stream input, FileValidation targetChk, Stream output, out FileValidation outputChk);

#if LEGACY || DEBUG
        static PatchInfo fromOldDiff(byte[] diffData, FileValidation oldChk);
#endif

    }
}
