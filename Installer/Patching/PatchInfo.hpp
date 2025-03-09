#pragma once

#include <sstream>

namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    class PatchInfo {
    public:
        FileValidation m_metadata;
        uint8_t[] m_data;

        PatchInfo() { }
        PatchInfo(BinaryReader reader);
        void writeTo(BinaryWriter writer);
        bool patchBytes(uint8_t[] inputBytes, FileValidation targetChk, out uint8_t[] outputBytes, out FileValidation outputChk);
        bool patchStream(std::ostringstream input, FileValidation targetChk, std::ostringstream output, out FileValidation outputChk);


/* VESTIGIAL MACRO
 *  #if LEGACY || DEBUG
 */
        static PatchInfo fromOldDiff(uint8_t[] diffData, FileValidation oldChk);

/* VESTIGIAL MACRO
 *  #endif
 */

    }
}
