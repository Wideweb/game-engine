#pragma once

#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>

namespace Engine {

class IBlackboardData {
  public:
    virtual ~IBlackboardData() = default;
};

template <typename T> class BlackboardData : public IBlackboardData {
  public:
    void setValue(const std::string &key, const T &value) {
        m_Data[key] = value;
    }

    bool hasValue(const std::string &key) {
        const auto &it = m_Data.find(key);
        return it != m_Data.end();
    }

    T &getValue(const std::string &key) {
        assert(hasValue(key) && "no value.");

        return m_Data[key];
    }

  private:
    std::unordered_map<std::string, T> m_Data;
};

class Blackboard {
  private:
    std::shared_ptr<Blackboard> m_Parent;
    std::unordered_map<const std::type_info *, std::shared_ptr<IBlackboardData>>
        m_Data;

  public:
    Blackboard() {}

    virtual ~Blackboard() = default;

    void setParent(const std::shared_ptr<Blackboard> parent) {
        m_Parent = parent;
    }

    template <typename T> void setValue(const std::string &key, T value) {
        getDataSet<T>()->setValue(key, value);
    }

    template <typename T> T &getValue(const std::string &key) {
        const auto &dataSet = getDataSet<T>();

        if (dataSet->hasValue(key)) {
            return dataSet->getValue(key);
        }

        if (m_Parent != nullptr) {
            return m_Parent->getValue<T>(key);
        }

        throw std::runtime_error("no value");
    }

    template <typename T> std::shared_ptr<BlackboardData<T>> getDataSet() {
        const auto *key = &typeid(T);

        if (m_Data.find(key) == m_Data.end()) {
            m_Data[key] = std::make_shared<BlackboardData<T>>();
        }

        return std::static_pointer_cast<BlackboardData<T>>(m_Data[key]);
    }
};

} // namespace Engine
