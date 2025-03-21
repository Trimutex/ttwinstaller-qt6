﻿#include "PatchInfo.hpp"

namespace TaleOfTwoWastelandsPatching {
    /// <summary>
    /// Deserialize a PatchInfo
    /// </summary>
    /// <param name="reader">A reader aligned to a serialized PatchInfo</param>
    PatchInfo(BinaryReader reader) {
        //reading a FV (metadata) now
        Metadata = FileValidation.ReadFrom(reader);

        //reading data now
        var dataSize = reader.ReadUInt32();
        Debug.Assert((int)dataSize == dataSize);
        if (dataSize > 0)
            Data = reader.ReadBytes((int)dataSize);
    }

    void writeTo(BinaryWriter writer) {
        FileValidation.WriteTo(writer, Metadata);

        if (Data != null) {
            writer.Write((uint)Data.LongLength);
            if (Data.Length > 0)
                writer.Write(Data);
        } else
            writer.Write(0U);
    }

    bool patchBytes(uint8_t[] inputBytes, FileValidation targetChk, out uint8_t[] outputBytes, out FileValidation outputChk) {
        using (var output = new MemoryStream()) {
            // NOTE: originally unsafe
            fixed (uint8_t* pInput = inputBytes)
                fixed (uint8_t* pPatch = Data)
                Diff.Apply(pInput, inputBytes.LongLength, pPatch, Data.LongLength, output);

            outputBytes = output.ToArray();

            output.Seek(0, SeekOrigin.Begin);
            outputChk = new FileValidation(outputBytes, targetChk.Type);

            return targetChk == outputChk;
        }
    }

    bool patchStream(std::ostringstream input, FileValidation targetChk, std::ostringstream output, out FileValidation outputChk) {
        // NOTE: originally unsafe
        fixed (uint8_t* pPatch = Data)
            Diff.Apply(input, pPatch, Data.LongLength, output);

        output.Seek(0, SeekOrigin.Begin);
        outputChk = new FileValidation(output, targetChk.Type);

        return targetChk == outputChk;
    }

/* VESTIGIAL MACRO
 *  #if LEGACY || DEBUG
 */
    static PatchInfo FromOldDiff(byte[] diffData, FileValidation oldChk) {
        return new PatchInfo {
            Metadata = oldChk,
                     Data = diffData
        };
    }
/* VESTIGIAL MACRO
 *  #endif
 */
}
