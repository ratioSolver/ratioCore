#pragma once
#include "scope.h"
#include "rational.h"

namespace riddle::ast
{
  class statement;
} // namespace riddle::ast

namespace ratio::core
{
  class conjunction : public scope
  {
  public:
    conjunction(scope &scp, context ctx, semitone::rational cst, const std::vector<std::unique_ptr<const riddle::ast::statement>> &stmnts);
    conjunction(const conjunction &orig) = delete;
    virtual ~conjunction();

    inline semitone::rational get_cost() const noexcept { return cost; } // returns the cost for applying this conjunction..

    /**
     * @brief Execute this conjunction within the stored context.
     *
     */
    RATIOCORE_EXPORT void execute();

  private:
    context ctx;                                                                  // the context within which the conjunction can be executed..
    const semitone::rational cost;                                                // the cost for applying this conjunction..
    const std::vector<std::unique_ptr<const riddle::ast::statement>> &statements; // the statements within the conjunction's body..
  };
} // namespace ratio::core
