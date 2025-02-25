#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: public class
    static class Util {
    public:
        #region GetMD5 overloads
        static byte[] GetMD5(string file);
        static byte[] GetMD5(Stream stream);
        static byte[] GetMD5(byte[] buf);
        static string MakeMD5String(byte[] md5);
        static byte[] FromMD5String(string md5Str);
        static string GetMD5String(string file);
        static string GetMD5String(Stream stream);
        static string GetMD5String(byte[] buf);
        #endregion

        #region Legacy mode
#if LEGACY || DEBUG
        static IDictionary<string, string> ReadOldDatabase(string path);

        static IEnumerable<Tuple<string, byte[]>> FindAlternateVersions(string file);
#endif
        #endregion

        static string Truncate(this string value, int maxLength);
        static bool PatternSearch(Stream inStream, string pattern, out string result);
        static void CopyFolder(string inFolder, string destFolder);
        static void AssertElevated();
    }
}
