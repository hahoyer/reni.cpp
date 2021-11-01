#pragma once

namespace HWLib
{
  template <typename TResult, typename TTarget, typename ... TArguments>
  function<TResult(const TTarget&)> Pivot(TResult (*targetFunction )(const TTarget&, TArguments ...), TArguments ... arguments)
  {
    return [=](const TTarget& target)
    {
      return targetFunction(target, arguments...);
    };
  }

  template <typename TResult, typename TTarget, typename ... TArguments>
  function<TResult(const TTarget&)> Pivot(function<TResult (const TTarget&, TArguments ...)> targetFunction, TArguments ... arguments)
  {
    return [=](const TTarget& target)
    {
      return targetFunction(target, arguments...);
    };
  }

  template <typename TResult, typename TTarget>
  TResult operator |(const TTarget& target, TResult (*extensionMethod)(const TTarget&))
  {
    return extensionMethod(target);
  }

  template <typename TResult, typename TTarget>
  TResult operator |(const TTarget& target, function<TResult (const TTarget&)> extensionMethod)
  {
    return extensionMethod(target);
  }
}
