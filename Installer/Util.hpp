#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: public class
    static class Util {
    public:
/* VESTIGIAL MACRO
 *  #region GetMD5 overloads
 */
        static byte[] getMD5(string file);
        static byte[] getMD5(Stream stream);
        static byte[] getMD5(byte[] buf);
        static string makeMD5String(byte[] md5);
        static byte[] fromMD5String(string md5Str);
        static string getMD5String(string file);
        static string getMD5String(Stream stream);
        static string getMD5String(byte[] buf);
/* VESTIGIAL MACRO
 *  #endregion
 */

/* VESTIGIAL MACRO
 *  #region Legacy mode
 */
#if LEGACY || DEBUG
        static IDictionary<string, string> readOldDatabase(string path);

        static IEnumerable<Tuple<string, byte[]>> findAlternateVersions(string file);
#endif
/* VESTIGIAL MACRO
 *  #endregion
 */

        static string truncate(this string value, int maxLength);
        static bool patternSearch(Stream inStream, string pattern, out string result);
        static void copyFolder(string inFolder, string destFolder);
        static void assertElevated();
    }
}
