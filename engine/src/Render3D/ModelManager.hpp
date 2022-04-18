#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include "FlatDictionary.hpp"
#include "Model.hpp"

namespace Engine {

class ModelManager {
  public:
    void RegisterModel(const std::string &name, const std::shared_ptr<Model> &model) { 
      m_Data.add(name, model);
    }

    std::shared_ptr<Model> GetModel(const std::string &name) const {
        assert(m_Data.hasKey(name) && "no model.");
        return m_Data[name];
    }

    template <typename TModel, typename = std::enable_if_t<std::is_base_of_v<Model, TModel>>>
    std::shared_ptr<TModel> GetModel(const std::string &name) const {
        assert(m_Data.hasKey(name) && "no model.");
        return std::static_pointer_cast<TModel>(m_Data[name]);
    }

    template <typename TModel, typename = std::enable_if_t<std::is_base_of_v<Model, TModel>>>
    bool Is(const std::string &name) const {
        assert(m_Data.hasKey(name) && "no model.");

        return typeid(TModel).name() == typeid(*(m_Data[name])).name();
    }

    bool HasModel(const std::string &name) const { return m_Data.hasKey(name); }

    const std::vector<std::string> &keys() { return m_Data.keys(); }

  private:
    FlatDictionary<std::string, std::shared_ptr<Model>> m_Data;
};

} // namespace Engine
