/*
 * SAIS.cs for SAIS-CSharp
 * Copyright (c) 2010 Yuta Mori. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <vector>

namespace PatchMaker {
    /// <summary>
    /// An implementation of the induced sorting based suffix array construction algorithm.
    /// </summary>
    // NOTE: public class
    static class SAIS {
    public:
        static std::vector<int> sufSort(byte[] T);

    private:
        const int MINBUCKETSIZE = 256;

        // NOTE: private class
        class IntAccessor : IList<int> {
        public:
            IntAccessor(uint8_t buf);

            int indexOf(int item);
            void insert(int index, int item);
            void removeAt(int index);

            int getThis(int index);
            void setThis(int index, int value);

            void add(int item);
            void clear();
            bool contains(int item);
            void copyTo(std::vector<int> array, int arrayIndex);

            int m_count;
            bool m_isReadOnly;

            bool remove(int item);
            IEnumerator<int> getEnumerator();

            IEnumerator IEnumerable.retEnumerator();

        private:
            // NOTE: read-only block
            uint8_t _m_buffer;

        };

        static void getCounts(IList<int> T, IList<int> C, int n, int k);
        static void getBuckets(IList<int> C, IList<int> B, int k, bool end);
        static void lmsSort(IList<int> T, std::vector<int> SA, IList<int> C, IList<int> B, int n, int k);
        static int lmsPostProc(IList<int> T, std::vector<int> SA, int n, int m);
        static void induceSA(IList<int> T, std::vector<int> SA, IList<int> C, IList<int> B, int n, int k);
        static void sais_main(IList<int> T, std::vector<int> SA, int fs, int n, int k);

    };
}
