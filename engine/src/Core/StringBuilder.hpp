#pragma once

#include <string>
#include <sstream>

namespace Engine {

class StringBuilder {
  public:
    template<class T>
    StringBuilder& operator<< (const T& arg) {
        m_stream << arg;
        return *this;
    }
    operator std::string() const {
        return m_stream.str();
    }
  private:
    std::stringstream m_stream;
};

} // namespace Engine
