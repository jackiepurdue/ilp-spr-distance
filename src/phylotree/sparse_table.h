#ifndef CSPR_SPARSE_TABLE_H
#define CSPR_SPARSE_TABLE_H
/* ---- MIT LICENSE ----

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
------------------------*/

#ifndef SPARSE_TABLE_HPP_INCLUDED

#define SPARSE_TABLE_HPP_INCLUDED

#include <algorithm>
#include <vector>
#include <stdexcept>

#if __cplusplus > 199711L
    #include <initializer_list>
#endif

template < typename T, typename Comp = std::less< T > >
class sparse_table {
private:
    std::vector< T > data;
    std::vector < std::vector < size_t > > table;
    void build_table();
    inline static unsigned mlog(size_t _n);
public:
    const size_t size() const { return data.size(); }
    sparse_table(const std::vector< T >& _data) : data(_data) {
        build_table();
    }
    template < typename ItT >
    sparse_table(ItT begin, ItT end) {
        while (begin != end) {
            data.push_back(*begin);
            ++begin;
        }
        build_table();
    }

#if __cplusplus > 199711L
    sparse_table(std::initializer_list< T > data_list) {
        data = std::vector< T >(data_list.begin(), data_list.end());
        build_table();
    }
#endif

    const int get_min(const size_t L, const size_t R) const;
};

template < typename T, typename Comp >
void sparse_table< T, Comp >::build_table() {
    if (!data.size()) {
        throw std::invalid_argument("Empty set is passet as a parameter.");
    }
    table.resize(1);
    table[0].resize(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        table[0][i] = i;
    }
    unsigned pow = 1;
    const unsigned max_pow = mlog(data.size());
    table.resize(max_pow + 1);
    const Comp cmp;
    for (; pow <= max_pow; ++pow) {
        const size_t cur_step_len = static_cast< size_t >(1) << pow,
            prev_step_len = static_cast< size_t >(1) << (pow - 1);
        table[pow].resize(data.size() - cur_step_len + 1);
        for (int i = 0; i <= data.size() - cur_step_len; ++i) {
            table[pow][i] = cmp(data[table[pow - 1][i]], data[table[pow - 1][i + prev_step_len]]) ?
                table[pow - 1][i] : table[pow - 1][i + prev_step_len];
        }
    }
}

template < typename T, typename Comp >
inline unsigned sparse_table< T, Comp >::mlog(const size_t _n) {
    register size_t n = _n, k = 0;
    while ((static_cast< size_t >(1) << k) <= n) {
        ++k;
    }
    return k - 1;
}

template < typename T, typename Comp >
const int sparse_table< T, Comp >::get_min(const size_t L, const size_t R) const {


    if (L > data.size() || R == 0 || R > data.size()) {
        throw std::out_of_range("Wrong index is used.");
    }
    if (L >= R) {
        throw std::invalid_argument("Wrong arguments in query.");
    }
    const unsigned pow = mlog(R - L);
    const Comp cmp;
    const size_t check_len = static_cast< size_t >(1) << pow;
    return cmp(data[table[pow][L]], data[table[pow][R - check_len]]) ?
        table[pow][L] : table[pow][R - check_len];
}


#endif
#endif
