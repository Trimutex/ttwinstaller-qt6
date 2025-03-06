#include "sais.hpp"

namespace PatchMaker {
    /*- Suffixsorting -*/
    /* byte */
    /// <summary>
    /// Constructs the suffix array of a given string in linear time.
    /// </summary>
    /// <param name="T">input string</param>
    /// <param name="SA">output suffix array</param>
    /// <param name="n">length of the given string</param>
    /// <returns>0 if no error occurred, -1 or -2 otherwise</returns>
    static int[] sufSort(byte[] T) {
        if (T == null)
            throw new ArgumentNullException("T");

        var SA = new int[T.Length];

        if (T.Length <= 1) {
            if (T.Length == 1)
                SA[0] = 0;
        } else
            sais_main(new IntAccessor(T), SA, 0, T.Length, 256);

        return SA;
    }


    /* VESTIGIAL MACRO
     *  [MethodImpl(MethodImplOptions.AggressiveInlining)]
     */
    static void getCounts(IList<int> T, IList<int> C, int n, int k) {
        int i;
        for (i = 0; i < k; ++i) {
            C[i] = 0;
        }

        for (i = 0; i < n; ++i) {
            C[T[i]] = C[T[i]] + 1;
        }
    }

    /* VESTIGIAL MACRO
     *  [MethodImpl(MethodImplOptions.AggressiveInlining)]
     */
    static void getBuckets(IList<int> C, IList<int> B, int k, bool end) {
        int i, sum = 0;
        if (end != false) {
            for (i = 0; i < k; ++i) {
                sum += C[i];
                B[i] = sum;
            }
        } else {
            for (i = 0; i < k; ++i) {
                sum += C[i];
                B[i] = sum - C[i];
            }
        }
    }

    /* sort all type LMS suffixes */
    static void lmsSort(IList<int> T, int[] SA, IList<int> C, IList<int> B, int n, int k) {
        int b, i, j;
        int c0, c1;

        /* compute SAl */
        if (C == B)
            getCounts(T, C, n, k);
        getBuckets(C, B, k, false); /* find starts of buckets */
        j = n - 1;
        b = B[c1 = T[j]];
        --j;
        SA[b++] = (T[j] < c1) ? ~j : j;
        for (i = 0; i < n; ++i) {
            if (0 < (j = SA[i])) {
                if ((c0 = T[j]) != c1) {
                    B[c1] = b;
                    b = B[c1 = c0];
                } --j;
                SA[b++] = (T[j] < c1) ? ~j : j;
                SA[i] = 0;
            } else if (j < 0)
                SA[i] = ~j;
        }

        /* compute SAs */
        if (C == B)
            getCounts(T, C, n, k);
        getBuckets(C, B, k, true); /* find ends of buckets */
        for (i = n - 1, b = B[c1 = 0]; 0 <= i; --i) {
            if (0 < (j = SA[i])) {
                if ((c0 = T[j]) != c1) {
                    B[c1] = b;
                    b = B[c1 = c0];
                } --j;
                SA[--b] = (T[j] > c1) ? ~(j + 1) : j;
                SA[i] = 0;
            }
        }
    }

    static int lmsPostProc(IList<int> T, int[] SA, int n, int m) {
        int i, j, p, q, plen, qlen, name;
        int c0, c1;
        bool diff;

        /* compact all the sorted substrings into the first m items of SA
           2*m must be not larger than n (proveable) */
        for (i = 0; (p = SA[i]) < 0; ++i) {
            SA[i] = ~p;
        }

        if (i < m) {
            for (j = i, ++i; ; ++i) {
                if ((p = SA[i]) < 0) {
                    SA[j++] = ~p;
                    SA[i] = 0;
                    if (j == m)
                        break;
                }
            }
        }

        /* store the length of all substrings */
        i = n - 1;
        j = n - 1;
        c0 = T[n - 1];
        do {
            c1 = c0;
        } while ((0 <= --i) && ((c0 = T[i]) >= c1));
        for (; 0 <= i; ) {
            do {
                c1 = c0;
            } while ((0 <= --i) && ((c0 = T[i]) <= c1));
            if (0 <= i) {
                SA[m + ((i + 1) >> 1)] = j - i;
                j = i + 1;
                do {
                    c1 = c0;
                } while ((0 <= --i) && ((c0 = T[i]) >= c1));
            }
        }

        /* find the lexicographic names of all substrings */
        for (i = 0, name = 0, q = n, qlen = 0; i < m; ++i) {
            p = SA[i];
            plen = SA[m + (p >> 1)];
            diff = true;
            if ((plen == qlen) && ((q + plen) < n)) {
                for (j = 0;
                        (j < plen) && (T[p + j] == T[q + j]); ++j) { }
                if (j == plen)
                    diff = false;
            }
            if (diff != false) {
                ++name;
                q = p;
                qlen = plen;
            }
            SA[m + (p >> 1)] = name;
        }

        return name;
    }

    /* compute SA and BWT */
    static void induceSA(IList<int> T, int[] SA, IList<int> C, IList<int> B, int n, int k) {
        int b, i, j;
        int c0, c1;

        /* compute SAl */
        if (C == B)
            getCounts(T, C, n, k);
        getBuckets(C, B, k, false); /* find starts of buckets */
        j = n - 1;
        b = B[c1 = T[j]];
        SA[b++] = ((0 < j) && (T[j - 1] < c1)) ? ~j : j;
        for (i = 0; i < n; ++i) {
            j = SA[i];
            SA[i] = ~j;
            if (0 < j) {
                if ((c0 = T[--j]) != c1) {
                    B[c1] = b;
                    b = B[c1 = c0];
                }
                SA[b++] = ((0 < j) && (T[j - 1] < c1)) ? ~j : j;
            }
        }

        /* compute SAs */
        if (C == B)
            getCounts(T, C, n, k);
        getBuckets(C, B, k, true); /* find ends of buckets */
        for (i = n - 1, b = B[c1 = 0]; 0 <= i; --i) {
            if (0 < (j = SA[i])) {
                if ((c0 = T[--j]) != c1) {
                    B[c1] = b;
                    b = B[c1 = c0];
                }
                SA[--b] = ((j == 0) || (T[j - 1] > c1)) ? ~j : j;
            } else
                SA[i] = ~j;
        }
    }

    /* find the suffix array SA of T[0..n-1] in {0..k-1}^n
       use a working space (excluding T and SA) of at most 2n+O(1) for a constant alphabet */
    static void sais_main(IList<int> T, int[] SA, int fs, int n, int k) {
        IList<int> C, B, RA;
        int i, j, b, m, p, q, name, newfs;
        int c0, c1;
        uint flags = 0;

        if (k <= MINBUCKETSIZE) {
            C = new int[k];
            if (k <= fs) {
                B = new ArraySegment<int>(SA, n + fs - k, SA.Length - (n + fs - k));
                flags = 1;
            } else {
                B = new int[k];
                flags = 3;
            }
        } else if (k <= fs) {
            C = new ArraySegment<int>(SA, n + fs - k, SA.Length - (n + fs - k));
            if (k <= (fs - k)) {
                B = new ArraySegment<int>(SA, n + fs - k * 2, SA.Length - (n + fs - k * 2));
                flags = 0;
            } else if (k <= (MINBUCKETSIZE * 4)) {
                B = new int[k];
                flags = 2;
            } else {
                B = C;
                flags = 8;
            }
        } else {
            C = B = new int[k];
            flags = 4 | 8;
        }

        /* stage 1: reduce the problem by at least 1/2
           sort all the LMS-substrings */
        getCounts(T, C, n, k);
        getBuckets(C, B, k, true); /* find ends of buckets */
        for (i = 0; i < n; ++i) {
            SA[i] = 0;
        }

        b = -1;
        i = n - 1;
        j = n;
        m = 0;
        c0 = T[n - 1];
        do {
            c1 = c0;
        } while ((0 <= --i) && ((c0 = T[i]) >= c1));

        for (; 0 <= i; ) {
            do {
                c1 = c0;
            } while ((0 <= --i) && ((c0 = T[i]) <= c1));
            if (0 <= i) {
                if (0 <= b)
                    SA[b] = j;
                b = --B[c1];
                j = i;
                ++m;
                do {
                    c1 = c0;
                } while ((0 <= --i) && ((c0 = T[i]) >= c1));
            }
        }
        if (1 < m) {
            LMSsort(T, SA, C, B, n, k);
            name = LMSpostproc(T, SA, n, m);
        } else if (m == 1) {
            SA[b] = j + 1;
            name = 1;
        } else
            name = 0;

        /* stage 2: solve the reduced problem
           recurse if names are not yet unique */
        if (name < m) {
            if ((flags & 4) != 0) {
                C = null;
                B = null;
            }
            if ((flags & 2) != 0)
                B = null;
            newfs = (n + fs) - (m * 2);
            if ((flags & (1 | 4 | 8)) == 0) {
                if ((k + name) <= newfs)
                    newfs -= k;
                else
                    flags |= 8;
            }
            for (i = m + (n >> 1) - 1, j = m * 2 + newfs - 1; m <= i; --i) {
                if (SA[i] != 0)
                    SA[j--] = SA[i] - 1;
            }

            RA = new ArraySegment<int>(SA, m + newfs, SA.Length - (m + newfs));
            sais_main(RA, SA, newfs, m, name);
            RA = null;

            i = n - 1;
            j = m * 2 - 1;
            c0 = T[n - 1];
            do {
                c1 = c0;
            } while ((0 <= --i) && ((c0 = T[i]) >= c1));

            for (; 0 <= i; ) {
                do {
                    c1 = c0;
                } while ((0 <= --i) && ((c0 = T[i]) <= c1));

                if (0 <= i) {
                    SA[j--] = i + 1;
                    do {
                        c1 = c0;
                    } while ((0 <= --i) && ((c0 = T[i]) >= c1));
                }
            }

            for (i = 0; i < m; ++i) {
                SA[i] = SA[m + SA[i]];
            }
            if ((flags & 4) != 0)
                C = B = new int[k];
            if ((flags & 2) != 0)
                B = new int[k];
        }

        /* stage 3: induce the result for the original problem */
        if ((flags & 8) != 0)
            getCounts(T, C, n, k);
        /* put all left-most S characters into their buckets */
        if (1 < m) {
            getBuckets(C, B, k, true); /* find ends of buckets */
            i = m - 1;
            j = n;
            p = SA[m - 1];
            c1 = T[p];
            do {
                q = B[c0 = c1];
                while (q < j) {
                    SA[--j] = 0;
                }
                do {
                    SA[--j] = p;
                    if (--i < 0)
                        break;
                    p = SA[i];
                } while ((c1 = T[p]) == c0);
            } while (0 <= i);
            while (0 < j) {
                SA[--j] = 0;
            }
        }

        induceSA(T, SA, C, B, n, k);
    }

    IntAccessor(byte[] buf) {
        _buffer = buf;
    }

    int indexOf(int item) {
        throw new NotImplementedException();
    }

    void insert(int index, int item) {
        throw new NotImplementedException();
    }

    void removeAt(int index) {
        throw new NotImplementedException();
    }

    int getThis(int index) {
        return _m_buffer[index];
    }

    void setThis(int index, int value) {
        _m_buffer[index] = (byte)value;
    }

    void add(int item) {
        throw new NotImplementedException();
    }

    void clear() {
        throw new NotImplementedException();
    }

    bool contains(int item) {
        throw new NotImplementedException();
    }

    void copyTo(int[] array, int arrayIndex) {
        throw new NotImplementedException();
    }

    int count {
        get { return _buffer.Length; }
    }

    bool isReadOnly {
        get { return false; }
    }

    bool remove(int item) {
        throw new NotImplementedException();
    }

    IEnumerator<int> getEnumerator() {
        throw new NotImplementedException();
    }

    IEnumerator IEnumerable.getEnumerator() {
        throw new NotImplementedException();
    }
}
