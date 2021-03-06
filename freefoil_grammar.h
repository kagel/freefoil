#ifndef FREEFOIL_GRAMMAR_H_
#define FREEFOIL_GRAMMAR_H_

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_symbols.hpp>
#include <boost/spirit/include/classic_distinct.hpp>
#include <boost/spirit/utility/lists.hpp>

#include <boost/spirit/tree/ast.hpp>


#define GRAMMAR_RULE(RULE_ID) rule<ScannerT, parser_context<>, parser_tag<RULE_ID> >

namespace Freefoil {
	namespace Private {
		
		using BOOST_SPIRIT_CLASSIC_NS::symbols;
		using BOOST_SPIRIT_CLASSIC_NS::distinct_parser;
		using BOOST_SPIRIT_CLASSIC_NS::grammar;
		using BOOST_SPIRIT_CLASSIC_NS::rule;
		using BOOST_SPIRIT_CLASSIC_NS::parser_context;
		using BOOST_SPIRIT_CLASSIC_NS::lexeme_d;
		using BOOST_SPIRIT_CLASSIC_NS::token_node_d;
		using BOOST_SPIRIT_CLASSIC_NS::parser_id;
		using BOOST_SPIRIT_CLASSIC_NS::space_p;
		using BOOST_SPIRIT_CLASSIC_NS::list_p;
		using BOOST_SPIRIT_CLASSIC_NS::ch_p;
		using BOOST_SPIRIT_CLASSIC_NS::parser_tag;
		using BOOST_SPIRIT_CLASSIC_NS::no_node_d;
		using BOOST_SPIRIT_CLASSIC_NS::eps_p;
		using BOOST_SPIRIT_CLASSIC_NS::alpha_p;
		using BOOST_SPIRIT_CLASSIC_NS::alnum_p;
		using BOOST_SPIRIT_CLASSIC_NS::discard_node_d;
		using BOOST_SPIRIT_CLASSIC_NS::tree_parse_info;
				
		struct freefoil_keywords : symbols<int>{
			freefoil_keywords(){
				add
					("void")
					("string")
					("float")
					("int")
					("ref")
					("bool")
					//TODO: add all the left keywords
					;
			}
		} freefoil_keywords_p;
		
		const distinct_parser<> keyword_p("a-zA-Z0-9_");
		
		struct freefoil_grammar : public grammar<freefoil_grammar> {
			
		enum ruleID{
			script_ID = 0,
			func_decl_ID,
			func_impl_ID,
			ident_ID,
			stmt_end_ID,
			func_head_ID,
			func_body_ID,
			params_list_ID,
			param_ID,
			ref_ID,
			func_type_ID,
			var_type_ID,
			stmt_ID,
		};
		
			template <typename ScannerT>
			struct definition{
				definition(freefoil_grammar const &/*self*/){
					
					script = 
							*(func_decl | func_impl) >> no_node_d[eps_p];
						
					ident = lexeme_d[
										token_node_d[
											(((alpha_p | ch_p('_')) >> *(alnum_p | ch_p('_')))) 
											-  
											freefoil_keywords_p
										]
									] 
									;
				
					stmt_end = discard_node_d[ch_p(';')];
				
					func_decl = func_head >> stmt_end;				
					
					func_impl = func_head >> discard_node_d[ch_p('{')] >> func_body >> discard_node_d[ch_p('}')];  
					
					func_head = func_type >> ident >> discard_node_d[ch_p('(')] >> !params_list >> discard_node_d[ch_p(')')];
					
					func_body = *stmt;
					
					func_type = keyword_p("string") | keyword_p("void") | keyword_p("float") | keyword_p("int") | keyword_p("bool");
					
					params_list = list_p(param, no_node_d[ch_p(',')]);
					
					var_type = keyword_p("string") | keyword_p("float") | keyword_p("int") | keyword_p("bool");
					
					param = var_type >> !ref >> !ident;
					
					ref = discard_node_d[keyword_p("ref")];
					
					stmt = stmt_end; //TODO: add other alternatives
					
					 // turn on the debugging info.
		            BOOST_SPIRIT_DEBUG_RULE(integer);
		            BOOST_SPIRIT_DEBUG_RULE(factor);
		            BOOST_SPIRIT_DEBUG_RULE(term);
		            BOOST_SPIRIT_DEBUG_RULE(expression);
				}	
												
				GRAMMAR_RULE(script_ID) const &start() const{
					return script;
				}
				
				/////////////////////////////////////////////////////////////////////				
				GRAMMAR_RULE(script_ID) script;
				GRAMMAR_RULE(func_decl_ID) func_decl;
				GRAMMAR_RULE(func_impl_ID) func_impl;
				GRAMMAR_RULE(ident_ID) ident;
				GRAMMAR_RULE(stmt_end_ID) stmt_end;
				GRAMMAR_RULE(func_head_ID) func_head;
				GRAMMAR_RULE(func_body_ID) func_body;
				GRAMMAR_RULE(params_list_ID) params_list;
				GRAMMAR_RULE(param_ID) param;
				GRAMMAR_RULE(func_type_ID) func_type;
				GRAMMAR_RULE(var_type_ID) var_type;
				GRAMMAR_RULE(ref_ID) ref;
				GRAMMAR_RULE(stmt_ID) stmt;
			};
		};
	}
}

#endif /*SYNTAX_H_*/
