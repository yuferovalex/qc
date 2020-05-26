#ifndef QC_LINKEDHASHSET_H
#define QC_LINKEDHASHSET_H

#include <cstdint>
#include <vector>
#include <unordered_map>

template <typename T>
class LinkedHashSet {
public:
    typedef T value_type;
    typedef uint32_t index_type;
    typedef typename std::vector<std::pair<index_type, value_type>>::const_iterator iterator_type;
    typedef typename std::vector<std::pair<index_type, value_type>>::size_type size_type;

    index_type indexOf(const value_type &value) const {
        return m_map.find(value)->second->first;
    }

    size_type size() const {
        return m_vector.size();
    }

    index_type push(const value_type &value) {
        auto found = m_map.find(value);
        if (found != m_map.end()) {
            return found->second->first;
        }
        index_type index = m_vector.size();
        m_vector.push_back(std::make_pair(index, value));
        m_map[value] = m_vector.end() - 1;
        return index;
    }

    iterator_type begin() const {
        return m_vector.cbegin();
    }

    iterator_type end() const {
        return m_vector.cend();
    }

private:
    std::vector<std::pair<index_type, value_type>> m_vector;
    std::unordered_map<value_type, iterator_type> m_map;
};

#endif //QC_LINKEDHASHSET_H
