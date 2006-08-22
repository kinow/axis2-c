/*
* Copyright 2004,2005 The Apache Software Foundation.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef AXIS2_DEFINES_H
#define AXIS2_DEFINES_H

/**
 * @file axis2_defines.h
 * @brief Useful definitions, which may have platform concerns
 */

#include <stddef.h>
#include <axis2_utils_defines.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** Axis2 in flow */
#define  AXIS2_IN_FLOW  1
/** Axis2 out flow */
#define  AXIS2_OUT_FLOW 2
/** Axis2 fault in flow */
#define  AXIS2_FAULT_IN_FLOW 3
/** Axis2 fault out flow */
#define  AXIS2_FAULT_OUT_FLOW 4

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_DEFINES_H */
