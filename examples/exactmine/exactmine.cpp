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
  \file exactmine.cpp
  \brief Mine exact logic networks

  \author Mathias Soeken
*/

#include <cmath>
#include <string>

#include <alice/alice.hpp>
#include <kitty/kitty.hpp>
#include <lorina/lorina.hpp>

class optimum_network
{
public:
  optimum_network() = default;

  optimum_network( const kitty::dynamic_truth_table& function )
      : function( function )
  {
  }

  optimum_network( kitty::dynamic_truth_table&& function )
      : function( std::move( function ) )
  {
  }

  bool exists() const
  {
    static std::vector<std::unordered_set<kitty::dynamic_truth_table, kitty::hash<kitty::dynamic_truth_table>>> hash;

    if ( function.num_vars() >= hash.size() )
    {
      hash.resize( function.num_vars() + 1 );
    }

    return !hash[function.num_vars()].insert( function ).second;
  }

public: /* field access */
  kitty::dynamic_truth_table function{0};
  std::string network;
};

namespace alice
{

ALICE_ADD_STORE( optimum_network, "opt", "o", "network", "networks" )

ALICE_DESCRIBE_STORE( optimum_network, opt )
{
  if ( opt.network.empty() )
  {
    return fmt::format( "{}", kitty::to_hex( opt.function ) );
  }
  else
  {
    return fmt::format( "{}, optimum network computed", kitty::to_hex( opt.function ) );
  }
}

void add_optimum_network_entry( command& cmd, kitty::dynamic_truth_table& function )
{
  if ( cmd.env->variable( "npn" ) != "" )
  {
    function = std::get<0>( kitty::exact_npn_canonization( function ) );
  }

  optimum_network entry( function );

  if ( !entry.exists() )
  {
    cmd.store<optimum_network>().extend();
    cmd.store<optimum_network>().current() = entry;
  }
}

class load_command : public command
{
public:
  load_command( const environment::ptr& env ) : command( env, "load new entry" )
  {
    add_option( "truth_table,--tt", truth_table, "truth table in hex format" );
    add_flag( "--binary,-b", "read truth table as binary string" );
  }

protected:
  void execute()
  {
    auto function = [this]() {
      if ( is_set( "binary" ) )
      {
        const unsigned num_vars = ::log( truth_table.size() ) / ::log( 2.0 );
        kitty::dynamic_truth_table function( num_vars );
        kitty::create_from_binary_string( function, truth_table );
        return function;
      }
      else
      {
        const unsigned num_vars = ::log( truth_table.size() * 4 ) / ::log( 2.0 );
        kitty::dynamic_truth_table function( num_vars );
        kitty::create_from_hex_string( function, truth_table );
        return function;
      }
    }();

    add_optimum_network_entry( *this, function );
  }

private:
  std::string truth_table;
};

ALICE_ADD_COMMAND( load, "Loading" );

class load_bench_command : public alice::command
{
public:
  load_bench_command( const environment::ptr& env ) : command( env, "load entries from LUT functions in BENCH file" )
  {
    add_option( "filename,--filename", filename, "BENCH filename" )->check( ExistingFileWordExp );
  }

  class lut_parser : public lorina::bench_reader
  {
  public:
    lut_parser( load_bench_command& cmd ) : cmd( cmd ) {}

    void on_gate( const std::vector<std::string>& inputs, const std::string& output, const std::string& type ) const override
    {
      (void)output;

      const auto num_vars = inputs.size();

      if ( num_vars > 6u )
      {
        std::cout << "[w] ignore gate with " << num_vars << " inputs\n";
        return;
      }

      if ( !( type.size() > 2u && type[0] == '0' && type[1] == 'x' ) )
      {
        std::cout << "[w] ignore gate '" << type << "'\n";
        return;
      }

      kitty::dynamic_truth_table function( num_vars );
      kitty::create_from_hex_string( function, type.substr( 2u ) );

      add_optimum_network_entry( cmd, function );
    }

  private:
    load_bench_command& cmd;
  };

protected:
  void execute()
  {
    lorina::read_bench( filename, lut_parser( *this ) );
  }

private:
  std::string filename;
};

ALICE_ADD_COMMAND( load_bench, "Loading" );

} // namespace alice

ALICE_MAIN( exactmine )
