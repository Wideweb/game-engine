#pragma once

#include <map>
#include <memory>
#include <type_traits>
#include <vector>

#include "Model.hpp"

namespace Engine {

class ModelManager {
  public:
    void RegisterModel(const std::string &name,
                       const std::shared_ptr<Model> &model) {
        m_Models.push_back(model);
        m_NameToModel[name] = model;
    }

    std::shared_ptr<Model> GetModel(const std::string &name) const {
        const auto &it = m_NameToModel.find(name);

        assert(it != m_NameToModel.end() && "no model.");

        return it->second;
    }

    template <typename TModel,
              typename = std::enable_if_t<std::is_base_of_v<Model, TModel>>>
    std::shared_ptr<TModel> GetModel(const std::string &name) const {
        const auto &it = m_NameToModel.find(name);

        assert(it != m_NameToModel.end() && "no model.");

        return std::static_pointer_cast<TModel>(it->second);
    }

  private:
    std::vector<std::shared_ptr<Model>> m_Models;
    std::unordered_map<std::string, std::shared_ptr<Model>> m_NameToModel;
};

} // namespace Engine
