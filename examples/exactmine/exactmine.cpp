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
#include <unordered_set>
#include <vector>

#include <alice/alice.hpp>
#include <kitty/kitty.hpp>
#include <lorina/lorina.hpp>
#include <percy/percy.hpp>

class optimum_network
{
public:
  optimum_network() = default;

  optimum_network( const kitty::dynamic_truth_table& function )
      : function( function ) {}

  optimum_network( kitty::dynamic_truth_table&& function )
      : function( std::move( function ) ) {}

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

ALICE_PRINT_STORE( optimum_network, os, opt )
{
  os << fmt::format( "function (hex): {}\nfunction (bin): {}\n", kitty::to_hex( opt.function ), kitty::to_binary( opt.function ) );

  if ( opt.network.empty() )
  {
    os << "no optimum network computed\n";
  }
  else
  {
    os << fmt::format( "optimum network: {}\n", opt.network );
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
  load_command( const environment::ptr& env ) : command( env, "Load new entry" )
  {
    add_option( "truth_table,--tt", truth_table, "truth table in hex format" );
    add_flag( "--binary,-b", "read truth table as binary string" );
  }

protected:
  void execute() override
  {
    auto function = [this]() {
      if ( is_set( "binary" ) )
      {
        const unsigned num_vars = std::log2( truth_table.size() );
        kitty::dynamic_truth_table function( num_vars );
        kitty::create_from_binary_string( function, truth_table );
        return function;
      }
      else
      {
        const unsigned num_vars = std::log2( truth_table.size() * 4 );
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

class load_bench_command : public command
{
public:
  load_bench_command( const environment::ptr& env ) : command( env, "Load entries from LUT functions in BENCH file" )
  {
    add_option( "filename,--filename", filename, "BENCH filename" )->check( ExistingFileWordExp );
    add_option( "--threshold,-t", threshold, "skip functions with more than this number of inputs", 4u );
  }

  class lut_parser : public lorina::bench_reader
  {
  public:
    lut_parser( load_bench_command& cmd ) : cmd( cmd ) {}

    void on_gate( const std::vector<std::string>& inputs, const std::string& output, const std::string& type ) const override
    {
      (void)output;

      const auto num_vars = inputs.size();

      if ( num_vars > cmd.threshold )
        return;

      if ( !( type.size() > 2u && type[0] == '0' && type[1] == 'x' ) )
      {
        cmd.env->out() << "[w] ignore gate '" << type << "'\n";
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
  void execute() override
  {
    lorina::read_bench( filename, lut_parser( *this ) );
  }

private:
  std::string filename;
  unsigned threshold = 6u;
};

ALICE_ADD_COMMAND( load_bench, "Loading" );

class find_network_command : public command
{
public:
  find_network_command( const environment::ptr& env ) : command( env, "Find optimum network" )
  {
    add_flag( "--verify", "verifies whether found network matches specification" );
    add_flag( "--force,-f", "recompute optimum network if it exists" );
    add_flag( "--verbose,-v", "be verbose" );
  }

protected:
  rules validity_rules() const override
  {
    return {
        has_store_element<optimum_network>( env ),
        {[this]() { return store<optimum_network>().current().network.empty() || is_set( "force" ); }, "optimum network already computed (use -f to override)"}};
  }

  void execute() override
  {
    auto& opt = store<optimum_network>().current();

    percy::spec spec;
    spec.verbosity = is_set( "verbose" ) ? 1 : 0;
    spec[0] = opt.function;

    percy::chain c;

    if ( percy::synthesize( spec, c ) != percy::success )
    {
      env->out() << "[e] could not find optimum network \n";
      return;
    }

    if ( is_set( "verify" ) )
    {
      if ( c.simulate(spec)[0] == opt.function )
      {
        env->out() << "[i] synthesized chain matches specification\n";
      }
      else
      {
        env->err() << "[e] synthesized chain does not match specification\n";
        return;
      }
    }

    std::stringstream str;
    c.to_expression( str );
    opt.network = str.str();
  }
};

ALICE_ADD_COMMAND( find_network, "Exact synthesis" )

} // namespace alice

ALICE_MAIN( exactmine )
