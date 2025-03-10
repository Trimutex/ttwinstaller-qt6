#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: public class
    static class Util {
    public:
/* VESTIGIAL MACRO
 *  #region GetMD5 overloads
 */
        static uint8_t[] getMD5(std::string file);
        static uint8_t[] getMD5(std::ostringstream stream);
        static uint8_t[] getMD5(uint8_t[] buf);
        static std::string makeMD5String(uint8_t[] md5);
        static uint8_t[] fromMD5String(std::string md5Str);
        static std::string getMD5String(std::string file);
        static std::string getMD5String(std::ostringstream stream);
        static std::string getMD5String(uint8_t[] buf);
/* VESTIGIAL MACRO
 *  #endregion
 */

/* VESTIGIAL MACRO
 *  #region Legacy mode
 *  #if LEGACY || DEBUG
 */
        static IDictionary<std::string, std::string> readOldDatabase(std::string path);

        static IEnumerable<Tuple<std::string, uint8_t[]>> findAlternateVersions(std::string file);

/* VESTIGIAL MACRO
 *  #endif
 *  #endregion
 */

        static std::string truncate(std::string value, int maxLength);
        static bool patternSearch(std::ostringstream inStream, std::string pattern, std::string result);
        static void copyFolder(std::string inFolder, std::string destFolder);
        static void assertElevated(void);
    }
}
