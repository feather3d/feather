/***********************************************************************
 *
 * Filename: deps.hpp
 *
 * Description: Contains all the dependencies.
 *
 * Copyright (C) 2015 Richard Layman, rlayman2000@yahoo.com 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#ifndef DEPS_HPP
#define DEPS_HPP

// C
#include <dlfcn.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// C++
#include <cstddef>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <memory>
#include <utility>
#include <chrono>

// OpenGL
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/freeglut.h>

// boost
#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/config.hpp>
#include <boost/system/error_code.hpp>
#include <boost/filesystem.hpp>

// boost::fusion
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/container/vector/vector_fwd.hpp>
#include <boost/fusion/include/vector_fwd.hpp>
#include <boost/fusion/algorithm/transformation/push_front.hpp>
#include <boost/fusion/include/push_front.hpp>
#include <boost/fusion/algorithm/transformation/push_back.hpp>
#include <boost/fusion/include/push_back.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/include/make_vector.hpp>

// boost::bgl
#include <boost/graph/dag_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "boost/graph/visitors.hpp"
#include "boost/graph/breadth_first_search.hpp"
#include "boost/graph/depth_first_search.hpp"

// boost::tuple
#include <boost/tuple/tuple.hpp>

// boost::spirit
#include <boost/spirit/include/lex.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>

// boost::variant
#include <boost/variant/recursive_variant.hpp>

#endif
