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
  \file spectral6.cpp
  \brief Find spectral equivalence classes for 6-input functions

  \author Mathias Soeken
*/

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>

#include <fmt/format.h>

#include <alice/alice.hpp>
#include <kitty/kitty.hpp>
#include <lorina/lorina.hpp>

class spectral_store
{
public:
  using truth_table = kitty::static_truth_table<8>;

  std::pair<bool, bool> is_computed( const truth_table& function ) const
  {
    const auto it = cache.find( function );

    if ( it == cache.end() )
    {
      return {false, false};
    }
    else
    {
      return {true, representatives.at( it->second ) };
    }
  }

  void classify( const truth_table& function )
  {
    bool computed, exact;
    std::tie( computed, exact ) = is_computed( function );

    if ( !computed )
    {
      truth_table cls;
      bool exact;
      std::tie( cls, exact ) = kitty::exact_spectral_canonization_limit( function, _limit );
      cache.insert( {function, cls} );
      representatives.insert( {cls, exact} );
    }
  }

  void retry()
  {
    std::vector<truth_table> to_be_removed;
    std::vector<std::pair<truth_table, bool>> to_be_added;

    for ( auto& p : representatives )
    {
      /* entry is already exact */
      if ( p.second ) continue;

      std::cout << "retry with " << kitty::to_hex( p.first ) << std::flush;

      truth_table cls;
      bool exact;
      std::tie( cls, exact ) = kitty::exact_spectral_canonization_limit( p.first, _limit );

      std::cout << ", got " << kitty::to_hex( cls ) << " " << ( exact ? "exact" : "heuristic" ) << std::endl;

      /* nothing changed */
      if ( cls == p.first && !exact ) continue;

      /* update function cache */
      std::for_each( cache.begin(), cache.end(), [&]( auto& entry ) { if ( entry.second == p.first ) entry.second = cls; } );

      to_be_removed.push_back( p.first );
      to_be_added.emplace_back( cls, exact );
    }

    for ( const auto& t : to_be_removed )
    {
      representatives.erase( t );
    }

    for ( const auto& p : to_be_added )
    {
      representatives[p.first] = p.second;
    }
  }

  auto size() const { return representatives.size(); }

  auto cache_size() const { return cache.size(); }

  auto size_heuristic() const { return std::count_if( representatives.begin(), representatives.end(), []( auto p ) { return !p.second; } ); }

  auto limit() const { return _limit; }

  auto set_limit( uint32_t limit ) { _limit = limit; }

  void write_to_file( const std::string& filename ) const
  {
    std::ofstream out( filename.c_str(), std::ofstream::out );
    for ( const auto& p : cache )
    {
      out << kitty::to_hex( p.first ) << " " << kitty::to_hex( p.second ) << " "
          << ( representatives.at( p.second ) ? 1 : 0 ) << "\n";
    }
    out.close();
  }

  void read_from_file( const std::string& filename )
  {
    std::ifstream in( filename.c_str(), std::ifstream::in );
    std::string line;

    while ( std::getline( in, line ) )
    {
      truth_table function, representative;
      kitty::create_from_hex_string( function, line.substr( 0, 16 ) );
      kitty::create_from_hex_string( representative, line.substr( 17, 16 ) );
      bool exact = line[34] == '1';

      cache.insert( {function, representative} );
      representatives.insert( {representative, exact} );
    }
  }

private:
  /* contains all representatives, maps to Boolean value that indicates whether
     representative is exact or heuristic */
  std::unordered_map<truth_table, bool, kitty::hash<truth_table>> representatives;

  /* cache that maps already computed functions to their representative */
  std::unordered_map<truth_table, truth_table, kitty::hash<truth_table>> cache;

  /* limit for spectral classification */
  uint32_t _limit{200000u};
};

namespace alice
{

ALICE_ADD_STORE( spectral_store, "spectral", "s", "spectral database", "spectral databases" );

ALICE_DESCRIBE_STORE( spectral_store, store )
{
  return fmt::format( "{} representatives ({} heuristic, limit: {}, functions: {})", store.size(), store.size_heuristic(), store.limit(), store.cache_size() );
}

class from_file_command : public command
{
public:
  from_file_command( const environment::ptr& env ) : command( env, "Load truth tables from K-LUT network" )
  {
    add_option( "filename,--filename", filename, "BENCH filename" )->check( ExistingFileWordExp );
  }

  class lut_parser : public lorina::bench_reader
  {
  public:
    lut_parser( from_file_command& cmd ) : cmd( cmd ) {}

    void on_gate( const std::vector<std::string>& inputs, const std::string& output, const std::string& type ) const override
    {
      (void)output;

      if ( inputs.size() != 8 || !( type.size() > 2u && type[0] == '0' && type[1] == 'x' ) )
      {
        return;
      }

      spectral_store::truth_table function;
      kitty::create_from_hex_string( function, type.substr( 2u ) );

      cmd.env->out() << kitty::to_hex( function ) << std::endl;
      cmd.store<spectral_store>().current().classify( function );
    }

  private:
    from_file_command& cmd;
  };

  class blif_parser : public lorina::blif_reader
  {
  public:
    blif_parser( from_file_command& cmd ) : cmd( cmd ) {}

    void on_gate( const std::vector<std::string>& inputs, const std::string& output, const std::vector<std::pair<std::string, std::string>>& tt ) const override
    {
      (void) output;

      if ( inputs.size() != 8 )
      {
        return;
      }

      std::vector<kitty::cube> cubes;

      for ( auto p : tt )
      {
        if ( p.second == "0" )
        {
          return;
        }

        cubes.push_back( p.first );
      }

      spectral_store::truth_table function;
      kitty::create_from_cubes( function, cubes );

      cmd.env->out() << kitty::to_hex( function ) << std::endl;
      cmd.store<spectral_store>().current().classify( function );
    }

  private:
    from_file_command& cmd;
  };

protected:
  command::rules validity_rules() const override
  {
    return {has_store_element<spectral_store>( env )};
  }

  void execute() override
  {
    const auto tick = std::chrono::high_resolution_clock::now();
    lorina::read_bench( filename, lut_parser( *this ) );
    const auto tock = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>( tock - tick ).count() / 1000.;

    env->out() << fmt::format( "[i] run-time: {:.2f} secs\n", runtime );
  }

private:
  std::string filename;
};

ALICE_ADD_COMMAND( from_file, "Loading" );

class retry_command : public command
{
public:
  retry_command( const environment::ptr& env ) : command( env, "retries to classify heuristic representatives" )
  {
  }

protected:
  command::rules validity_rules() const override
  {
    return {has_store_element<spectral_store>( env )};
  }

  void execute() override
  {
    const auto tick = std::chrono::high_resolution_clock::now();
    store<spectral_store>().current().retry();
    const auto tock = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>( tock - tick ).count() / 1000.;

    env->out() << fmt::format( "[i] run-time: {:.2f} secs\n", runtime );
  }
};

ALICE_ADD_COMMAND( retry, "Database" )

class random_command : public command
{
public:
  random_command( const environment::ptr& env ) : command( env, "retries to classify heuristic representatives" )
  {
    add_option( "count,--count", count, "number of random functions to create", true );
  }

protected:
  command::rules validity_rules() const override
  {
    return {has_store_element<spectral_store>( env )};
  }

  void execute() override
  {
    const auto tick = std::chrono::high_resolution_clock::now();

    spectral_store::truth_table tt;

    for ( auto i = 0u; i < count; ++i )
    {
      kitty::create_random( tt );
      env->out() << fmt::format( "{:<3} {}\n", i + 1, kitty::to_hex( tt ) );
      env->out() << std::flush;
      store<spectral_store>().current().classify( tt );
    }

    const auto tock = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>( tock - tick ).count() / 1000.;

    env->out() << fmt::format( "[i] run-time: {:.2f} secs\n", runtime );
  }

private:
  uint32_t count{100};
};

ALICE_ADD_COMMAND( random, "Loading" )

ALICE_COMMAND( empty, "Database", "Creates empty representative database" )
{
  store<spectral_store>().extend();
}

class set_limit_command : public command
{
public:
  set_limit_command( const environment::ptr& env ) : command( env, "sets the classification limit" )
  {
    add_option( "limit,--limit", limit, "spectral classification limit", true );
  }

protected:
  command::rules validity_rules() const override
  {
    return {has_store_element<spectral_store>( env )};
  }

  void execute() override
  {
    store<spectral_store>().current().set_limit( limit );
  }

private:
  uint32_t limit;
};

ALICE_ADD_COMMAND( set_limit, "Database" )

ALICE_ADD_FILE_TYPE( db, "spectral database" );

ALICE_WRITE_FILE( spectral_store, db, store, filename, cmd )
{
  store.write_to_file( filename );
}

ALICE_READ_FILE( spectral_store, db, filename, cmd )
{
  spectral_store store;
  store.read_from_file( filename );
  return store;
}

} /* namespace alice */

ALICE_MAIN( spectral6 )
