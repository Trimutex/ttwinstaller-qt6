#include "Util.hpp"

namespace TaleOfTwoWastelands {
        
/* VESTIGIAL MACRO
 *  #region GetMD5 overloads
 */
        // NOTE: byte[] is an 8-bit **unsigned** integer
        // TODO: Replace ComputeHash() and co to openssl library
        //       ... also add openssl library
        static byte[] getMD5(string file) {
            using (var stream = File.OpenRead(file))
                return GetMD5(stream);
        }

        static byte[] getMD5(Stream stream) {
            using (var fileHash = MD5.Create())
            using (stream)
                return fileHash.ComputeHash(stream);
        }

        static byte[] getMD5(byte[] buf) {
            using (var fileHash = MD5.Create())
                return fileHash.ComputeHash(buf);
        }

        static string makeMD5String(byte[] md5) {
            return BitConverter.ToString(md5).Replace("-", "");
        }

        static byte[] fromMD5String(string md5Str) {
            byte[] data = new byte[md5Str.Length / 2];
            for (int i = 0; i < data.Length; i++)
                data[i] = Convert.ToByte(md5Str.Substring(i * 2, 2), 16);

            return data;
        }

        static string getMD5String(string file) {
            return MakeMD5String(GetMD5(file));
        }

        static string getMD5String(Stream stream) {
            return MakeMD5String(GetMD5(stream));
        }

        static string getMD5String(byte[] buf) {
            return MakeMD5String(GetMD5(buf));
        }
        
/* VESTIGIAL MACRO
 *  #endregion
 *
 *  #region Legacy mode
 */
        

/* VESTIGIAL MACRO
 *  #if LEGACY || DEBUG
 */
        static IDictionary<string, string> readOldDatabase(string path) {
            Debug.Assert(File.Exists(path));

            using (var stream = File.OpenRead(path))
                return (IDictionary<string, string>)new BinaryFormatter().Deserialize(stream);
        }

        // NOTE: simple iteration over a non-generic collection
        //      tldr; enumeration over tuples of string && byte[]
        static IEnumerable<Tuple<string, byte[]>> findAlternateVersions(string file) {
            var justName = Path.GetFileName(file);
            var split = justName.Split('.');
            split[split.Length - 3] = "*";
            //combatshotgun.nif.8154C65E957F6A29B36ADA24CFBC1FDE.1389525E123CD0F8CD5BB47EF5FD1901.diff
            //end[0] = diff, end[1] = newChk, end[2] = oldChk, end[3 ...] = fileName

            var justDir = Path.GetDirectoryName(file);
            if (!Directory.Exists(justDir))
                return null;

            return from other in Directory.EnumerateFiles(justDir, string.Join(".", split))
                   where other != file
                   let splitOther = Path.GetFileName(other).Split('.')
                   select Tuple.Create(other, FromMD5String(splitOther[splitOther.Length - 3]));
        }
        
/* VESTIGIAL MACRO
 *  #endif
 *
 *  #endregion
 */

        // NOTE: std::string value was originally `this string value`
        //      Does this even matter?
        static string truncate(std::string value, int maxLength) {
            if (string.IsNullOrEmpty(value)) return value;
            return value.Length <= maxLength ? value : value.Substring(0, maxLength);
        }

        // NOTE: std::string result was originally `out string result`
        //      Why the need for extra return?
        static bool patternSearch(Stream inStream, string pattern, std::string result) {
            if (!inStream.CanRead)
                throw new ArgumentException("Stream must be readable");

            var sb = new StringBuilder();
            bool wild = false, found = false;

            int matchLen = 0, cur;
            while ((cur = inStream.ReadByte()) != -1) {
                if (wild || pattern[matchLen] == cur) {
                    matchLen++;
                    sb.Append((char)cur);

                    if (wild) {
                        found = true;
                        for (int i = 0; i < pattern.Length; i++) {
                            var soFar = sb[sb.Length - pattern.Length + i];
                            if (soFar != pattern[i]) {
                                found = false;
                                break;
                            }
                        }

                        if (found)
                            break;
                    }
                }
                else if (pattern[matchLen] == '*') {
                    sb.Append((char)cur);

                    pattern = pattern.Substring(matchLen + 1);
                    matchLen = 0;
                    wild = true;
                }
            }

            result = sb.ToString();
            return found;
        }

        static void copyFolder(string inFolder, string destFolder) {
            Directory.CreateDirectory(destFolder);

            foreach (string folder in Directory.EnumerateDirectories(inFolder, "*", SearchOption.AllDirectories)) {
                var justFolder = folder.Replace(inFolder, "").TrimStart(Path.DirectorySeparatorChar);
                Directory.CreateDirectory(Path.Combine(destFolder, justFolder));
            }

            foreach (string file in Directory.EnumerateFiles(inFolder, "*", SearchOption.AllDirectories)) {
                var justFile = file.Replace(inFolder, "").TrimStart(Path.DirectorySeparatorChar);

                try {
                    File.Copy(file, Path.Combine(destFolder, justFile), true);
                } catch (UnauthorizedAccessException error) {
                    var Log = DependencyRegistry.Container.GetInstance<ILog>();
                    Log.Dual("ERROR: " + file.Replace(inFolder, "") + " did not copy successfully");
                    Log.File(error.ToString());
                }
            }
        }

        static void assertElevated() {
            Trace.Assert(Program.IsElevated, string.Format(Localization.MustBeElevated, Localization.TTW));
        }
    }
}
