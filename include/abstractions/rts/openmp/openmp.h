#pragma once

#include "traits.h"

#define OMP_RTS_INCLUDED

namespace MOGSLib { namespace RTS {

/**
 *  @class OpenMP
 *  @brief Abstraction of the OpenMP runtime system functionalities, types and API.
 */
struct OpenMP {

  /// @brief Id type is defined by an external Traits structure.
  using Id = typename MOGSLib::Traits::Id;
  /// @brief Load type is defined by an external Traits structure.
  using Load = typename MOGSLib::Traits::Load;

  static Id chunks;
  static Id threads;

  /**
   *  @brief Set the amount of chunks generated by the OpenMP runtime.
   *  @param nchunks The amount of chunks.
   *
   *  This method is designed to be called from within the OpenMP runtime.
   */
  static inline void nchunks(decltype(chunks) nchunks) { chunks = nchunks; }

  /**
   *  @brief Set the amount of threads generated by the OpenMP runtime.
   *  @param nthreads The amount of running threads managed by OpenMP.
   *
   *  This method is designed to be called from within the OpenMP runtime.
   */
  static inline void nthreads(decltype(threads) nthreads) { threads = nthreads; }
};

}}