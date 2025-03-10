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
