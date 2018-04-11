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
  \file countluts.cpp
  \brief Count lookup tables in mapped netlists

  \author Heinz Riener
  \author Mathias Soeken
*/

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <kitty/kitty.hpp>
#include <lorina/lorina.hpp>

class lut_counter : public lorina::bench_reader
{
public:
  lut_counter() : functions( 7u ), npn_classes( 7u )
  {
  }

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

    functions[num_vars][function]++;
    npn_classes[num_vars][std::get<0>( kitty::exact_npn_canonization( function ) )]++;
  }

  void print_statistics() const
  {
    for ( auto i = 0u; i < functions.size(); ++i )
    {
      if ( functions[i].empty() ) continue;

      std::cout << "LUTs with " << i << " variables: "
                << std::setw( 4 ) << functions[i].size() << "   classes: "
                << std::setw( 4 ) << npn_classes[i].size() << '\n';
    }

    std::cout << std::flush;
  }

private:
  using truth_table_t = kitty::dynamic_truth_table;
  using truth_table_hash_t = kitty::hash<truth_table_t>;
  using truth_table_map_t = std::unordered_map<truth_table_t, unsigned, truth_table_hash_t>;
  using truth_table_vec_t = std::vector<truth_table_map_t>;

  mutable truth_table_vec_t functions;
  mutable truth_table_vec_t npn_classes;
};

int main( int argc, char** argv )
{
  if ( argc != 2 )
  {
    std::cout << "usage: " << argv[0] << " file.bench\n";
    return 0;
  }

  lut_counter reader;
  lorina::read_bench( argv[1], reader );

  reader.print_statistics();

  return 1;
}
