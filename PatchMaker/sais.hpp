#pragma once

namespace PatchMaker {
    /// <summary>
    /// An implementation of the induced sorting based suffix array construction algorithm.
    /// </summary>
    // NOTE: public class
    static class SAIS {
    public:
        static int[] sufsort(byte[] T);

    private:
        const int MINBUCKETSIZE = 256;

        // NOTE: private class
        class IntAccessor : IList<int> {
        public:
            IntAccessor(byte[] buf);

            int IndexOf(int item);
            void Insert(int index, int item);
            void RemoveAt(int index);

            int this[int index];

            void Add(int item);
            void Clear();
            bool Contains(int item);
            void CopyTo(int[] array, int arrayIndex);

            int Count;
            bool IsReadOnly;

            bool Remove(int item);
            IEnumerator<int> GetEnumerator();

            IEnumerator IEnumerable.GetEnumerator();

        private:
            // NOTE: read-only block
            byte[] _buffer;

        };

        static void getCounts(IList<int> T, IList<int> C, int n, int k);
        static void getBuckets(IList<int> C, IList<int> B, int k, bool end);
        static void LMSsort(IList<int> T, int[] SA, IList<int> C, IList<int> B, int n, int k);
        static int LMSpostproc(IList<int> T, int[] SA, int n, int m);
        static void induceSA(IList<int> T, int[] SA, IList<int> C, IList<int> B, int n, int k);
        static void sais_main(IList<int> T, int[] SA, int fs, int n, int k);

    };
}
