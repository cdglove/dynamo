// ****************************************************************************
// dynamo/parse/parser.hpp
//
// Parser object for dynamo syntax.
// 
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#ifndef _DYNAMO_PARSE_PARSER_HPP_
#define _DYNAMO_PARSE_PARSER_HPP_
#pragma once

#include "dynamo/config.hpp"
#include "dynamo/ast/ast.hpp"
#include "dynamo/diagnostic/source_index.hpp"
#include <string>
#include <boost/optional.hpp>

namespace dynamo 
{
	template<typename Iterator>
	class source_index;

	class diagnostic_sink;
}

namespace dynamo { namespace parse
{
	///////////////////////////////////////////////////////////////////////////
	// Iterator parser
	// Consumes a sequence of characters and returns an ast on success
	// or boost::none on failure.
	// Builds a source_index of iterators used to map ast items to the original
	// source location.
	///////////////////////////////////////////////////////////////////////////
	template<typename Iterator>
	class parser
	{
	public:

		parser(diagnostic_sink& diagnostic)
			: sink_(diagnostic)
		{}

		boost::optional<
			ast::statement_list
		> parse(Iterator first, Iterator last)
		{
			return parse_impl(first, last);
		}

		source_index<Iterator> const& get_indexed_source()
		{
			return indexed_source_;
		}

	private:

		friend class string_parser;

		// The parser definition is intentionally excluded to reduce compile
		// times for those who only need the const char* version.
		// If you're getting a linker error because of this, include 
		// parser_def.hpp
		boost::optional<
			ast::statement_list
		> parse_impl(Iterator first, Iterator last);

		diagnostic_sink& sink_;
		source_index<Iterator> indexed_source_;
	};

	///////////////////////////////////////////////////////////////////////////
	// The Parser
	// Consumes a sequence of characters and returns an ast on success
	// or boost::none on failure.
	// Builds a source_index of iterators used to map ast items to the original
	// source location.
	// Makes a copy of the stirng to ensure iterators remain valid.
	///////////////////////////////////////////////////////////////////////////
	class string_parser
	{
	public:

		typedef std::string::const_iterator iterator_type;

		string_parser(diagnostic_sink& diagnostic)
			: sink_(diagnostic)
		{}

		boost::optional<
			ast::statement_list
		> parse(std::string src);

		source_index<iterator_type> const& get_indexed_source()
		{
			return indexed_source_;
		}

	private:

		diagnostic_sink& sink_;
		source_index<iterator_type> indexed_source_;
		std::string src_;
	};
}}

#endif // _DYNAMO_PARSE_PARSER_HPP_