#pragma once

#include "System.hpp"

#include "HPathFinderResultHandler.hpp"
#include "Node.hpp"

#include <memory>
#include <vector>

namespace Engine {

class MapUnitSystem : public System {
  private:
    class PathResultHandler : public HPathFinderResultHandler {
      public:
        std::vector<Point> path;

        bool onAbstractPathFound(const std::vector<std::shared_ptr<Node>>& abstractPath) override {
            return true;
        }

        void onPathFound(const std::vector<Point>& path) override {
            this->path = path;
        }

        void onPathNotFound() override {
            this->path.clear();
        }
    };

  std::shared_ptr<PathResultHandler> m_ResultHandler = std::make_shared<PathResultHandler>();

  public:
    virtual void Update(ComponentManager &components) const override;
};

} // namespace Engine
