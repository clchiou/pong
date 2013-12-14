// Copyright (C) 2013 Che-Liang Chiou.

#ifndef BASE_BASE_H_
#define BASE_BASE_H_

#include <exception>
#include <string>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

class Exception : public std::exception {
 public:
  explicit Exception(std::string message) : message_(std::move(message)) {}

  virtual ~Exception() noexcept(true) {}

  virtual const char* what() const throw() override { return message_.c_str(); }

 private:
  std::string message_;
};

#endif  // BASE_BASE_H_
