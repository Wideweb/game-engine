#pragma once

#include <unordered_map>
#include <utility>
#include <vector>

namespace Engine {

template <typename TKey, typename TValue> class FlatDictionary {
  public:
    void add(TKey key, TValue obj) {
        uint32_t index = m_Values.size();
        m_KeyToIndex[key] = index;
        m_Keys.push_back(key);
        m_Values.push_back(obj);
    }

    template <typename... Args> void emplace(TKey key, Args &&...args) {
        uint32_t index = m_Values.size();
        m_KeyToIndex[key] = index;
        m_Keys.push_back(key);
        m_Values.emplace_back(std::forward<Args>(args)...);
    }

    void update(TKey id, TValue obj) {
        uint32_t index = m_KeyToIndex[id];
        m_Values[index] = obj;
    }

    void remove(TKey id) {
        uint32_t index = m_KeyToIndex[id];

        uint32_t lastIndex = m_Values.size() - 1;
        auto lastId = m_Keys[lastIndex];

        m_Values[index] = m_Values[lastIndex];
        m_Keys[index] = lastId;
        m_KeyToIndex[lastId] = index;

        m_Values.pop_back();
        m_Keys.pop_back();
        m_KeyToIndex.erase(id);
    }

    void swapByIndex(uint32_t first, uint32_t second) {
        if (first == second) {
            return;
        }

        std::swap(m_KeyToIndex[m_Keys[first]], m_KeyToIndex[m_Keys[second]]);
        std::swap(m_Keys[first], m_Keys[second]);
        std::swap(m_Values[first], m_Values[second]);
    }

    TValue &getValue(TKey key) { return m_Values[m_KeyToIndex[key]]; }

    const TValue &getValue(TKey key) const { return m_Values.at(m_KeyToIndex.at(key)); }

    uint32_t getIndex(TKey key) const { return m_KeyToIndex.at(key); }

    TValue &operator[](TKey key) { return getValue(key); }

    const TValue &operator[](TKey key) const { return getValue(key); }

    const std::vector<TValue> &values() const { return m_Values; }

    const std::vector<TKey> &keys() const { return m_Keys; }

    std::vector<TValue> &values() { return m_Values; }

    std::vector<TKey> &keys() { return m_Keys; }

    bool hasKey(TKey key) const { return m_KeyToIndex.find(key) != m_KeyToIndex.end(); }

    bool changeKey(TKey prevKey, TKey newKey) {
        if (hasKey(newKey)) {
            return false;
        }

        uint32_t index = m_KeyToIndex[prevKey];

        m_Keys[index] = newKey;
        m_KeyToIndex[newKey] = index;
        m_KeyToIndex.erase(prevKey);
    }

    size_t size() const { return m_Values.size(); }

    void clear() {
        m_Values.clear();
        m_KeyToIndex.clear();
        m_Keys.clear();
    }

    auto begin() const { return m_Values.cbegin(); }
    auto end() const { return m_Values.cend(); }

  private:
    std::vector<TValue> m_Values;
    std::vector<TKey> m_Keys;
    std::unordered_map<TKey, uint32_t> m_KeyToIndex;
};

} // namespace Engine
