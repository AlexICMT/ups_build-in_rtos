/* ups_build_in_rtos
 * /ups_build_in_rtos/hacks/inc/eclipse_header_wrapper.hpp
 * eclipse_header_wrapper.hpp
 * 
 *  * @file eclipse_header_wrapper.hpp
 * @date 25.04.2016
 * @author vlunyov
 * @brief Exclude C symbols from Eclipse C/C++ Index
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1461 $
 * $Date: 2017-01-11 16:28:24 +0300 (б№, 11 џэт 2017) $
 *
 */

#ifndef HACKS_INC_ECLIPSE_HEADER_WRAPPER_HPP_
#define HACKS_INC_ECLIPSE_HEADER_WRAPPER_HPP_

/*
 * For use this hack need to uncheck following checkbox in the Eclipse Indexer properties:
 *  - Index source files not included in the build
 *  - Index unused headers
 *
 * Usage example:
 * #include "eclipse_header_wrapper.hpp"
 * C_HEADERS_WRAPPER_BEGIN(module)
 * #include "c_language_header1.h"
 * #include "c_language_header2.h"
 * #include "c_language_header3.h"
 * C_HEADERS_WRAPPER_END(module)
 */

#ifndef __cplusplus
#error This file should be included only in C++ files!
#endif // __cplusplus

#ifdef __CDT_PARSER__

#define C_HEADERS_WRAPPER_BEGIN(module_name)	namespace c_wrapper_##module_name {
#define C_HEADERS_WRAPPER_END(module_name)		} using namespace c_wrapper_##module_name;

#else // __CDT_PARSER__

#define C_HEADERS_WRAPPER_BEGIN(module_name)
#define C_HEADERS_WRAPPER_END(module_name)

#endif // __CDT_PARSER__

#endif /* HACKS_INC_ECLIPSE_HEADER_WRAPPER_HPP_ */
