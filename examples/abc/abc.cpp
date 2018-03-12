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

/* Adds And-inverter graphs (ABC type Gia_Man_t*) as store element type to
 * alice.
 *
 * One can access AIGs in general store commands using the long --aig flag or
 * the short -a flag.
 */
ALICE_ADD_STORE( abc::Gia_Man_t*, "aig", "a", "AIG", "AIGs" )

/* Implements the short string to describe a store element in store -a */
ALICE_DESCRIBE_STORE( abc::Gia_Man_t*, aig )
{
  const auto name = abc::Gia_ManName( aig );
  const auto pi_num = abc::Gia_ManPiNum( aig );
  const auto po_num = abc::Gia_ManPoNum( aig );
  return fmt::format( "{} i/o = {}/{}", name, pi_num, po_num );
}

/* Implements the functionality of ps -a */
ALICE_PRINT_STORE_STATISTICS( abc::Gia_Man_t*, os, aig )
{
  abc::Gps_Par_t Pars{};
  abc::Gia_ManPrintStats( aig, &Pars );
}

/* Implements the log returned by ps -a */
ALICE_LOG_STORE_STATISTICS( abc::Gia_Man_t*, aig )
{
  return {
      {"name", abc::Gia_ManName( aig )},
      {"inputs", abc::Gia_ManPiNum( aig )},
      {"outputs", abc::Gia_ManPoNum( aig )},
      {"nodes", abc::Gia_ManAndNum( aig )},
      {"levels", abc::Gia_ManLevelNum( aig )}};
}

/* Add an Aiger file type, will create two commands read_aiger and
 * write_aiger.
 */
ALICE_ADD_FILE_TYPE( aiger, "Aiger" )

/* Implements the functionality of read_aiger -a */
ALICE_READ_FILE( abc::Gia_Man_t*, aiger, filename, cmd )
{
  return abc::Gia_AigerRead( (char*)filename.c_str(), 0, 0, 0 );
}

/* Implements the functionality of write_aiger -a */
ALICE_WRITE_FILE( abc::Gia_Man_t*, aiger, aig, filename, cmd )
{
  abc::Gia_AigerWrite( aig, (char*)filename.c_str(), 1, 0 );
}

/* Implements the command syn3 */
ALICE_COMMAND( syn3, "Optimization", "Performs AIG optimization" )
{
  auto& aig = store<abc::Gia_Man_t*>().current();
  auto aig_new = abc::Gia_ManAigSyn3( aig, 0, 0 );
  abc::Gia_ManStop( aig );
  aig = aig_new;
}

/* Implements the command syn4 */
ALICE_COMMAND( syn4, "Optimization", "Performs AIG optimization" )
{
  auto& aigs = store<abc::Gia_Man_t*>();
  auto aig_new = abc::Gia_ManAigSyn4( aigs.current(), 0, 0 );
  abc::Gia_ManStop( aigs.current() );
  aigs.current() = aig_new;
}

/* Adds word-level networks (ABC type Wlc_Ntk_t*) as store element type to
 * alice.
 *
 * One can access word-level networks in general store commands using the long
 * --wlc flag or the short -w flag.
 */
ALICE_ADD_STORE( abc::Wlc_Ntk_t*, "wlc", "w", "Word-level network", "Word-level networks" )

/* Implements the short string to describe a store element in store -w */
ALICE_DESCRIBE_STORE( abc::Wlc_Ntk_t*, ntk )
{
  const auto name = ntk->pName;
  const auto pi_num = abc::Wlc_NtkPiNum( ntk );
  const auto po_num = abc::Wlc_NtkPoNum( ntk );
  return fmt::format( "{} i/o = {}/{}", name, pi_num, po_num );
}

/* Implements the functionality of ps -w */
ALICE_PRINT_STORE_STATISTICS( abc::Wlc_Ntk_t*, os, ntk )
{
  abc::Wlc_NtkPrintStats( ntk, 0, 0, 0 );
}

/* Add a Verilog file type, will create two commands read_verilog and
 * write_verilog.
 */
ALICE_ADD_FILE_TYPE( verilog, "Verilog" )

/* Implements the functionality of read_verilog -w */
ALICE_READ_FILE( abc::Wlc_Ntk_t*, verilog, filename, cmd )
{
  return abc::Wlc_ReadVer( (char*)filename.c_str(), nullptr );
}

/* Implements the functionality of write_verilog -w */
ALICE_WRITE_FILE( abc::Wlc_Ntk_t*, verilog, ntk, filename, cmd )
{
  abc::Wlc_WriteVer( ntk, (char*)filename.c_str(), 0, 0 );
}

/* Implements the functionality of convert --wlc_to_aig */
ALICE_CONVERT( abc::Wlc_Ntk_t*, ntk, abc::Gia_Man_t* )
{
  return abc::Wlc_NtkBitBlast( ntk, nullptr );
}

} // namespace alice

/* Main method for the Alice shell (with prefix) */
ALICE_MAIN( abc2 )
