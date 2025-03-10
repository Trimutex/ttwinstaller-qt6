#pragma once

#include <map>
#include <sstream>
#include <string>

namespace TaleOfTwoWastelandsPatching {
    using Patch = Tuple<FileValidation, PatchInfo[]>;


    // NOTE: public class
    class PatchDict : Dictionary<std::string, Patch> {
    public:
        PatchDict(int size) : base(size) { };
        PatchDict(std::string file) : this(new BinaryReader(File.OpenRead(file))) { };
        void writeAll(std::ostringstream outStream);

/* VESTIGIAL MACRO
 *  #if LEGACY
 */
        static PatchDict fromOldDatabase(std::map<std::string, std::string> oldDict, std::string prefix, Func<uint8_t[], uint8_t[]> convertPatch);

/* VESTIGIAL MACRO
 *  #endif
 */

    private:
        PatchDict(BinaryReader reader) : this(reader, reader.ReadInt32()) { };
        PatchDict(BinaryReader reader, int size);
        Patch readPatches(BinaryReader reader);
        uint8_t[] writePatches(Patch patch);

    }
}
