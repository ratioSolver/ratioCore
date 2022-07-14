#pragma once
#include "scope.h"
#include "env.h"

namespace ratio::core
{
  class type;

  class core : public scope, public env
  {
  public:
    ORATIOCORE_EXPORT core();
    core(const core &orig) = delete;
    ORATIOCORE_EXPORT ~core();

    /**
     * @brief Parses the given riddle script.
     *
     * @param script the riddle script to parse.
     */
    ORATIOCORE_EXPORT virtual void read(const std::string &script);
    /**
     * @brief Parses the given riddle files.
     *
     * @param files the riddle files to parse.
     */
    ORATIOCORE_EXPORT virtual void read(const std::vector<std::string> &files);

    inline core &get_core() const override { return const_cast<core &>(*this); }

    ORATIOCORE_EXPORT expr get(const std::string &name) noexcept override;

    ORATIOCORE_EXPORT type &get_type(const std::string &name) const override;

  private:
    std::map<std::string, type *> types;
  };
} // namespace ratio::core
