/* lstools-showcase
 * Copyright (C) 2018  EPFL
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*!
  \file abc.cpp
  \brief Alice CLI for ABC

  \author Mathias Soeken
*/

#include <alice/alice.hpp>
#include <fmt/format.h>

#include <aig/gia/gia.h>
#include <aig/gia/giaAig.h>
#include <base/wlc/wlc.h>

namespace alice
{

/* And-inverter graphs */
ALICE_ADD_STORE( abc::Gia_Man_t*, "aig", "a", "AIG", "AIGs" )

ALICE_DESCRIBE_STORE( abc::Gia_Man_t*, aig )
{
  const auto name = abc::Gia_ManName( aig );
  const auto pi_num = abc::Gia_ManPiNum( aig );
  const auto po_num = abc::Gia_ManPoNum( aig );
  return fmt::format( "{} i/o = {}/{}", name, pi_num, po_num );
}

ALICE_PRINT_STORE_STATISTICS( abc::Gia_Man_t*, os, aig )
{
  abc::Gps_Par_t Pars;
  memset( &Pars, 0, sizeof(abc::Gps_Par_t) );
  abc::Gia_ManPrintStats( aig, &Pars );
}

ALICE_LOG_STORE_STATISTICS( abc::Gia_Man_t*, aig )
{
  return nlohmann::json{
    {"name", abc::Gia_ManName( aig )},
    {"inputs", abc::Gia_ManPiNum( aig )},
    {"outputs", abc::Gia_ManPoNum( aig )},
    {"nodes", abc::Gia_ManAndNum( aig )},
    {"levels", abc::Gia_ManLevelNum( aig )}
  };
}

ALICE_ADD_FILE_TYPE( aiger, "Aiger" )

ALICE_READ_FILE( abc::Gia_Man_t*, aiger, filename, cmd )
{
  return abc::Gia_AigerRead( (char*)filename.c_str(), 0, 0, 0 );
}

ALICE_WRITE_FILE( abc::Gia_Man_t*, aiger, aig, filename, cmd )
{
  abc::Gia_AigerWrite( aig, (char*)filename.c_str(), 1, 0 );
}

ALICE_COMMAND(syn3, "Optimization", "Performs AIG optimization")
{
  auto& aigs = store<abc::Gia_Man_t*>();
  auto aig_new = abc::Gia_ManAigSyn3( aigs.current(), 0, 0 );
  abc::Gia_ManStop( aigs.current() );
  aigs.current() = aig_new;
}

ALICE_COMMAND(syn4, "Optimization", "Performs AIG optimization")
{
  auto& aigs = store<abc::Gia_Man_t*>();
  auto aig_new = abc::Gia_ManAigSyn4( aigs.current(), 0, 0 );
  abc::Gia_ManStop( aigs.current() );
  aigs.current() = aig_new;
}

/* Word-level networks */
ALICE_ADD_STORE( abc::Wlc_Ntk_t*, "wlc", "w", "Word-level network", "Word-level networks" )

ALICE_DESCRIBE_STORE( abc::Wlc_Ntk_t*, ntk )
{
  const auto name = ntk->pName;
  const auto pi_num = abc::Wlc_NtkPiNum( ntk );
  const auto po_num = abc::Wlc_NtkPoNum( ntk );
  return fmt::format( "{} i/o = {}/{}", name, pi_num, po_num );
}

ALICE_PRINT_STORE_STATISTICS( abc::Wlc_Ntk_t*, os, ntk )
{
  abc::Wlc_NtkPrintStats( ntk, 0, 0, 0 );
}

ALICE_ADD_FILE_TYPE( verilog, "Verilog" )

ALICE_READ_FILE( abc::Wlc_Ntk_t*, verilog, filename, cmd )
{
  return abc::Wlc_ReadVer( (char*)filename.c_str(), nullptr );
}

ALICE_WRITE_FILE( abc::Wlc_Ntk_t*, verilog, ntk, filename, cmd )
{
  abc::Wlc_WriteVer( ntk, (char*)filename.c_str(), 0, 0 );
}

ALICE_CONVERT( abc::Wlc_Ntk_t*, wlc, abc::Gia_Man_t* )
{
  return abc::Wlc_NtkBitBlast( wlc, nullptr );
}

} // namespace alice

ALICE_MAIN( abc )
