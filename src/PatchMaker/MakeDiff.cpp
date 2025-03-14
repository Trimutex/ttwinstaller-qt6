﻿#include "MakeDiff.hpp"

namespace PatchMaker {

        /// <summary>
        /// Creates a binary patch (in <a href="http://www.daemonology.net/bsdiff/">bsdiff</a> format) that can be used
        /// (by <see cref="Apply"/>) to transform <paramref name="oldData"/> into <paramref name="newData"/>.
        /// </summary>
        /// <param name="oldData">The original binary data.</param>
        /// <param name="newData">The new binary data.</param>
        /// <param name="output">A <see cref="Stream"/> to which the patch will be written.</param>
        // NOTE: originally unsafe
        static void create(uint8_t[] oldBuf, uint8_t[] newBuf, long signature, std::ostreamstring output) {
            // check arguments
            if (oldBuf == null)
                throw new ArgumentNullException("oldData");
            if (newBuf == null)
                throw new ArgumentNullException("newData");
            if (output == null)
                throw new ArgumentNullException("output");
            if (!output.CanSeek)
                throw new ArgumentException("Output stream must be seekable.", "output");
            if (!output.CanWrite)
                throw new ArgumentException("Output stream must be writable.", "output");

            /* Header is
                0	8	 "BSDIFF40"
                8	8	length of bzip2ed ctrl block
                16	8	length of bzip2ed diff block
                24	8	length of new file */
            /* File is
                0	32	Header
                32	??	Bzip2ed ctrl block
                ??	??	Bzip2ed diff block
                ??	??	Bzip2ed extra block */
            uint8_t[] header = new uint8_t[HEADER_SIZE];
            fixed (uint8_t* pHead = header)
            {
                WriteInt64(signature, pHead);
                WriteInt64(newBuf.LongLength, pHead + 24);
            }

            long startPosition = output.Position;
            output.Write(header, 0, header.Length);

            //backing for ctrl writes
            uint8_t[] buf = new uint8_t[8];
            int[] bufI = SAIS.sufsort(oldBuf);

            fixed (uint8_t* oldData = oldBuf)
            fixed (uint8_t* newData = newBuf)
            fixed (uint8_t* pB = buf)
            fixed (int* I = bufI)
            {
                using (MemoryStream msControl = new MemoryStream())
                using (MemoryStream msDiff = new MemoryStream())
                using (MemoryStream msExtra = new MemoryStream()) {
                    using (var ctrlStream = GetEncodingStream(msControl, signature, true))
                    using (var diffStream = GetEncodingStream(msDiff, signature, true))
                    using (var extraStream = GetEncodingStream(msExtra, signature, true)) {
                        int scan = 0;
                        int pos = 0;
                        int len = 0;
                        int lastscan = 0;
                        int lastpos = 0;
                        int lastoffset = 0;

                        // compute the differences, writing ctrl as we go
                        while (scan < newBuf.Length) {
                            int oldscore = 0;

                            for (int scsc = scan += len; scan < newBuf.Length; scan++) {
                                len = Search(I, oldData, oldBuf.Length, newData, newBuf.Length, scan, 0, oldBuf.Length, out pos);

                                for (; scsc < scan + len; scsc++) {
                                    if ((scsc + lastoffset < oldBuf.Length) && (oldData[scsc + lastoffset] == newData[scsc]))
                                        oldscore++;
                                }

                                if ((len == oldscore && len != 0) || (len > oldscore + 8))
                                    break;

                                if ((scan + lastoffset < oldBuf.Length) && (oldData[scan + lastoffset] == newData[scan]))
                                    oldscore--;
                            }

                            if (len != oldscore || scan == newBuf.Length) {
                                int s = 0;
                                int sf = 0;
                                int lenf = 0;
                                for (int i = 0; (lastscan + i < scan) && (lastpos + i < oldBuf.Length); ) {
                                    if (oldData[lastpos + i] == newData[lastscan + i])
                                        s++;
                                    i++;
                                    if (s * 2 - i > sf * 2 - lenf) {
                                        sf = s;
                                        lenf = i;
                                    }
                                }

                                int lenb = 0;
                                if (scan < newBuf.Length) {
                                    s = 0;
                                    int sb = 0;
                                    for (int i = 1; (scan >= lastscan + i) && (pos >= i); i++) {
                                        if (oldData[pos - i] == newData[scan - i])
                                            s++;
                                        if (s * 2 - i > sb * 2 - lenb) {
                                            sb = s;
                                            lenb = i;
                                        }
                                    }
                                }

                                if (lastscan + lenf > scan - lenb) {
                                    int overlap = (lastscan + lenf) - (scan - lenb);
                                    s = 0;
                                    int ss = 0;
                                    int lens = 0;
                                    for (int i = 0; i < overlap; i++) {
                                        if (newData[lastscan + lenf - overlap + i] == oldData[lastpos + lenf - overlap + i])
                                            s++;
                                        if (newData[scan - lenb + i] == oldData[pos - lenb + i])
                                            s--;
                                        if (s > ss)
                                        {
                                            ss = s;
                                            lens = i + 1;
                                        }
                                    }

                                    lenf += lens - overlap;
                                    lenb -= lens;
                                }

                                //write diff string
                                for (int i = 0; i < lenf; i++)
                                    diffStream.WriteByte((uint8_t)(newData[lastscan + i] - oldData[lastpos + i]));

                                //write extra string
                                var extraLength = (scan - lenb) - (lastscan + lenf);
                                if (extraLength > 0)
                                    extraStream.Write(newBuf, lastscan + lenf, extraLength);

                                //write ctrl block
                                WriteInt64(lenf, pB);
                                ctrlStream.Write(buf, 0, 8);

                                WriteInt64(extraLength, pB);
                                ctrlStream.Write(buf, 0, 8);

                                WriteInt64((pos - lenb) - (lastpos + lenf), pB);
                                ctrlStream.Write(buf, 0, 8);

                                lastscan = scan - lenb;
                                lastpos = pos - lenb;
                                lastoffset = pos - scan;
                            }
                        }
                    }

                    //write compressed ctrl data
                    msControl.Seek(0, SeekOrigin.Begin);
                    msControl.CopyTo(output);

                    fixed (uint8_t* pHead = header)
                    {
                        // compute size of compressed ctrl data
                        WriteInt64(msControl.Length, &pHead[8]);

                        // write compressed diff data
                        msDiff.Seek(0, SeekOrigin.Begin);
                        msDiff.CopyTo(output);
                        
                        // compute size of compressed diff data
                        WriteInt64(msDiff.Length, &pHead[16]);
                    }

                    // write compressed extra data
                    msExtra.Seek(0, SeekOrigin.Begin);
                    msExtra.CopyTo(output);
                }

                // seek to the beginning, write the header, then seek back to end
                long endPosition = output.Position;
                output.Position = startPosition;
                output.Write(header, 0, header.Length);
                output.Position = endPosition;
            }
        }

        static long readInt64(uint8_t* buf) {
            return Diff::readInt64(buf);
        }

        static Stream getEncodingStream(Stream stream, long signature, bool output) {
            return Diff::getEncodingStream(stream, signature, output);
        }

        /// <summary>
        /// Used only in PatchMaker
        /// </summary>
        // NOTE: originally unsafe
        // NOTE: internal function
        static uint8_t[] convertPatch(uint8_t* pPatch, long length, long inputSig, long outputSig) {
            if (inputSig == outputSig)
                throw new ArgumentException("output must be different from input");

            Func<long, long, Stream> openPatchStream = (u_offset, u_length) =>
                new UnmanagedMemoryStream(pPatch + u_offset, u_length > 0 ? u_length : length - u_offset);

            /*
             File format:
                 0	    8	"BSDIFF40"
                 8	    8	X
                 16	    8	Y
                 24	    8	sizeof(newfile)
                 32      X	bzip2(control block)
                 32+X	Y	bzip2(diff block)
                 32+X+Y	???	bzip2(extra block)
             with control block a set of triples (x,y,z) meaning "add x bytes
             from oldfile to x bytes from the diff block; copy y bytes from the
             extra block; seek forwards in oldfile by z bytes".
             */

            uint8_t[] header = new uint8_t[HEADER_SIZE];
            using (var inputStream = openPatchStream(0, HEADER_SIZE))
                inputStream.Read(header, 0, HEADER_SIZE);

            long controlLength, diffLength, newSize;
            fixed (uint8_t* pHead = header)
            {
                // check for appropriate magic
                long signature = ReadInt64(pHead);
                if (signature != inputSig)
                    throw new InvalidOperationException("Corrupt patch.");

                // read lengths from header
                controlLength = ReadInt64(pHead + 8); //8 = sizeof(long)
                diffLength = ReadInt64(pHead + 16);
                newSize = ReadInt64(pHead + 24);
            }

            if (controlLength < 0 || diffLength < 0 || newSize < 0)
                throw new InvalidOperationException("Corrupt patch.");

            uint8_t[] outControlBytes, outDiffBytes, outExtraBytes;

            using (MemoryStream
                msControlStream = new MemoryStream(),
                msDiffStream = new MemoryStream(),
                msExtraStream = new MemoryStream()) {
                using (Stream
                    inControlStream = openPatchStream(HEADER_SIZE, controlLength),
                    inDiffStream = openPatchStream(HEADER_SIZE + controlLength, diffLength),
                    inExtraStream = openPatchStream(HEADER_SIZE + controlLength + diffLength, -1),

                    bz2ControlStream = GetEncodingStream(inControlStream, inputSig, false),
                    bz2DiffStream = GetEncodingStream(inDiffStream, inputSig, false),
                    bz2ExtraStream = GetEncodingStream(inExtraStream, inputSig, false),

                    lzmaControlStream = GetEncodingStream(msControlStream, outputSig, true),
                    lzmaDiffStream = GetEncodingStream(msDiffStream, outputSig, true),
                    lzmaExtraStream = GetEncodingStream(msExtraStream, outputSig, true)) {
                    bz2ControlStream.CopyTo(lzmaControlStream);
                    bz2DiffStream.CopyTo(lzmaDiffStream);
                    bz2ExtraStream.CopyTo(lzmaExtraStream);
                }

                outControlBytes = msControlStream.ToArray();
                outDiffBytes = msDiffStream.ToArray();
                outExtraBytes = msExtraStream.ToArray();
            }

            using (var msOut = new MemoryStream())
            {
                var buf = new uint8_t[8];
                fixed (uint8_t* pB = buf)
                {
                    WriteInt64(outputSig, pB);
                    msOut.Write(buf, 0, buf.Length);

                    WriteInt64(outControlBytes.LongLength, pB);
                    msOut.Write(buf, 0, buf.Length);

                    WriteInt64(outDiffBytes.LongLength, pB);
                    msOut.Write(buf, 0, buf.Length);

                    WriteInt64(newSize, pB);
                    msOut.Write(buf, 0, buf.Length);
                }

                msOut.Write(outControlBytes, 0, outControlBytes.Length);
                msOut.Write(outDiffBytes, 0, outDiffBytes.Length);
                msOut.Write(outExtraBytes, 0, outExtraBytes.Length);

                return msOut.ToArray();
            }
        }



        // NOTE: originally unsafe
        static int compareBytes(uint8_t* left, int leftLength, uint8_t* right, int rightLength) {
            int diff = 0;
            for (int i = 0; i < leftLength && i < rightLength; i++) {
                diff = left[i] - right[i];
                if (diff != 0)
                    break;
            }
            return diff;
        }

        // NOTE: originally unsafe
        static int matchLength(uint8_t* oldData, int oldLength, uint8_t* newData, int newLength) {
            int i;
            for (i = 0; i < oldLength && i < newLength; i++) {
                if (oldData[i] != newData[i])
                    break;
            }

            return i;
        }

        // NOTE: originally unsafe
        static int search(int* I, uint8_t* oldData, int oldLength, uint8_t* newData, int newLength, int newOffset, int start, int end, out int pos) {
            if (end - start < 2) {
                int startLength = MatchLength((oldData + I[start]), oldLength, (newData + newOffset), newLength);
                int endLength = MatchLength((oldData + I[end]), oldLength, (newData + newOffset), newLength);

                if (startLength > endLength)
                {
                    pos = I[start];
                    return startLength;
                }
                else
                {
                    pos = I[end];
                    return endLength;
                }
            } else {
                int midPoint = start + (end - start) / 2;
                return CompareBytes((oldData + I[midPoint]), oldLength, (newData + newOffset), newLength) < 0 ?
                    Search(I, oldData, oldLength, newData, newLength, newOffset, midPoint, end, out pos) :
                    Search(I, oldData, oldLength, newData, newLength, newOffset, start, midPoint, out pos);
            }
        }

    /* VESTIGIAL MACRO
     *  [MethodImpl(MethodImplOptions.AggressiveInlining)]
     */
        // NOTE: originally unsafe
        static void writeInt64(long y, uint8_t* pb) {
            if (y < 0) {
                y = -y;

                pb[0] = (uint8_t)y;
                pb[1] = (uint8_t)(y >>= 8);
                pb[2] = (uint8_t)(y >>= 8);
                pb[3] = (uint8_t)(y >>= 8);
                pb[4] = (uint8_t)(y >>= 8);
                pb[5] = (uint8_t)(y >>= 8);
                pb[6] = (uint8_t)(y >>= 8);
                pb[7] = (uint8_t)((y >> 8) | 0x80);
            } else {
                pb[0] = (uint8_t)y;
                pb[1] = (uint8_t)(y >>= 8);
                pb[2] = (uint8_t)(y >>= 8);
                pb[3] = (uint8_t)(y >>= 8);
                pb[4] = (uint8_t)(y >>= 8);
                pb[5] = (uint8_t)(y >>= 8);
                pb[6] = (uint8_t)(y >>= 8);
                pb[7] = (uint8_t)(y >> 8);
            }
        }
}
