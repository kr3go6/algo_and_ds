#include <memory>

namespace nonstd
{
    template <typename RandomIt, typename Compare = 
            std::less<typename std::iterator_traits<RandomIt>::value_type>>
    void
    bubble_sort(RandomIt first, RandomIt last, Compare comp = Compare()) noexcept
    {
        for (RandomIt it1 = first; it1 < last; it1++) {
            for (RandomIt it2 = first; it2 + 1 < last; it2++) {
                if (!comp(*it2, *(it2 + 1))) {
                    std::swap(*it2, *(it2 + 1));
                }
            }
        }
    }
    

    template <typename RandomIt, typename Compare, typename valtype>
    void
    merge(RandomIt pt1_begin, RandomIt pt1_end,
            RandomIt pt2_begin, RandomIt pt2_end,
            std::unique_ptr<valtype>& dest, Compare comp) noexcept
    {
        int idx = 0;

        while (pt1_begin != pt1_end && pt2_begin != pt2_end) {
            if (comp(*pt1_begin, *pt2_begin)) {
                dest[idx] = *pt1_begin;
                pt1_begin++;
            } else {
                dest[idx] = *pt2_begin;
                pt2_begin++;
            }

            ++idx;
        }

        while (pt1_begin != pt1_end) {
            dest[idx] = *pt1_begin;
            pt1_begin++;
            ++idx;
        }

        while (pt2_begin != pt2_end) {
            dest[idx] = *pt2_begin;
            pt2_begin++;
            ++idx;
        }
    }

    template <typename RandomIt, typename Compare = 
            std::less<typename std::iterator_traits<RandomIt>::value_type>>
    void
    merge_sort(RandomIt first, RandomIt last, Compare comp = Compare()) noexcept
    {
        if (last - first < 2) {
            return;
        }

        merge_sort(first, first + (last - first) / 2, comp);
        merge_sort(first + (last - first) / 2, last, comp);

        typedef typename std::iterator_traits<RandomIt>::value_type valtype;

        std::unique_ptr<valtype[]> tmp_buf(new valtype[last - first]);
        RandomIt pt1_begin = first, pt2_begin = first + (last - first) / 2;
        RandomIt pt1_end = pt2_begin, pt2_end = last;

        merge(pt1_begin, pt1_end, pt2_begin, pt2_end, tmp_buf, comp);

        int i = 0;
        for (auto it = first; it != last; it++, ++i) { *it = std::move(tmp_buf[i]); } 
    }
}