#pragma once

namespace PatchMaker {
    /// <summary>
    /// An implementation of the induced sorting based suffix array construction algorithm.
    /// </summary>
    // NOTE: public class
    static class SAIS {
    public:
        static int[] sufSort(byte[] T);

    private:
        const int MINBUCKETSIZE = 256;

        // NOTE: private class
        class IntAccessor : IList<int> {
        public:
            IntAccessor(byte[] buf);

            int indexOf(int item);
            void insert(int index, int item);
            void removeAt(int index);

            int getThis(int index);
            void setThis(int index, int value);

            void add(int item);
            void clear();
            bool contains(int item);
            void copyTo(int[] array, int arrayIndex);

            int m_count;
            bool m_isReadOnly;

            bool remove(int item);
            IEnumerator<int> getEnumerator();

            IEnumerator IEnumerable.retEnumerator();

        private:
            // NOTE: read-only block
            byte[] _m_buffer;

        };

        static void getCounts(IList<int> T, IList<int> C, int n, int k);
        static void getBuckets(IList<int> C, IList<int> B, int k, bool end);
        static void lmsSort(IList<int> T, int[] SA, IList<int> C, IList<int> B, int n, int k);
        static int lmsPostProc(IList<int> T, int[] SA, int n, int m);
        static void induceSA(IList<int> T, int[] SA, IList<int> C, IList<int> B, int n, int k);
        static void sais_main(IList<int> T, int[] SA, int fs, int n, int k);

    };
}
