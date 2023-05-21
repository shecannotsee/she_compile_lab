//
// Created by shecannotsee on 23-5-22.
//

#ifndef SHE_COMPILE_LAB_MAKE_UNIQUE_H
#define SHE_COMPILE_LAB_MAKE_UNIQUE_H

#include <memory>

namespace CPP11 {

template<typename T, typename ...Args>
std::unique_ptr<T> make_unique(Args &&...args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

};// namespace CPP11

#endif //SHE_COMPILE_LAB_MAKE_UNIQUE_H
