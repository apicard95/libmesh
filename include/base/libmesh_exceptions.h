// The libMesh Finite Element Library.
// Copyright (C) 2002-2020 Benjamin S. Kirk, John W. Peterson, Roy H. Stogner

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



#ifndef LIBMESH_LIBMESH_EXCEPTIONS_H
#define LIBMESH_LIBMESH_EXCEPTIONS_H

#include "libmesh/libmesh_config.h"

#ifdef LIBMESH_ENABLE_EXCEPTIONS
#include <stdexcept>
#include <string>
#include <sstream>

#define libmesh_noexcept noexcept

namespace libMesh {

/**
 * A class to represent the internal "this should never happen"
 * errors, to be thrown by "libmesh_error();"
 */
class LogicError : public std::logic_error
{
public:
  LogicError() : std::logic_error( "Error in libMesh internal logic" ) {}
  LogicError(const std::string & msg) : std::logic_error( msg ) {}
};


/**
 * A class to stub for features that should be in libMesh, but
 * haven't been written yet, to be thrown by
 * "libmesh_not_implemented();"
 */
class NotImplemented : public std::logic_error
{
public:
  NotImplemented() : std::logic_error( "Error: not implemented!" ) {}
};


/**
 * A class representing a failed attempt by the library to open a
 * file (or construct an fstream, etc), to be thrown by
 * "libmesh_file_error(filename);" For ease of debugging, "filename"
 * should include any (absolute or relative or implicit) pathname
 * that was part of the failed open.
 */
class FileError : public std::runtime_error
{
public:
  FileError(const std::string & filename) : std::runtime_error( "Error accessing file: " + filename ) {}
};


/**
 * A class representing a solver's failure to converge, to be thrown
 * by "libmesh_convergence_failure();"  This should be a last
 * resort; more often, a solve which has failed should be
 * reattempted after switching to a smaller timestep, adding
 * underrelaxation, taking a smaller continuation step, etc.
 */
class ConvergenceFailure : public std::runtime_error
{
public:
  ConvergenceFailure() : std::runtime_error( "Unrecoverable failure to converge" ) {}
};


/**
 * A class representing that a dynamic cast failed to produce expected output.
 */
class DynamicCastFailure:  public std::runtime_error
{
public:
  DynamicCastFailure() : std::runtime_error( "Failed dynamic cast!" ) {}
};

/**
 * A class representing a floating point exception.
 */
class FloatingPointException: public std::runtime_error
{
public:
  FloatingPointException() : std::runtime_error( "libmesh FPE!" ) {}
};

/**
 * A class representing an exception during a solve.
 */
class SolverException: public std::exception
{
public:
  SolverException(int error_code_in) :
    std::exception(),
    error_code(error_code_in)
  {
    std::ostringstream oss;
    oss << "Error code " << error_code << " during solve." << std::endl;
    what_message = oss.str();
  }

  /**
   * Virtual destructor, gotta have one of those.
   */
  virtual ~SolverException() noexcept {};

  /**
   * Override the what() function to provide a generic error message.
   */
  virtual const char * what() const noexcept
  {
    // std::string::c_str() is noexcept in C++11, so it's safe to call
    // in what() because it can't throw.
    return what_message.c_str();
  }

  /**
   * The error code generated by the solver.
   */
  int error_code;

  /**
   * string which holds the message built in the constructor.
   */
  std::string what_message;
};

}

#define LIBMESH_THROW(e) do { throw e; } while (0)
#define libmesh_try try
#define libmesh_catch(e) catch(e)

#else

#define LIBMESH_THROW(e) do { std::abort(); } while (0)
#define libmesh_try
#define libmesh_catch(e) if (0)

#endif // LIBMESH_ENABLE_EXCEPTIONS

#endif // LIBMESH_LIBMESH_EXCEPTIONS_H
