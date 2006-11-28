/*
 *   Copyright 2003-2004 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <stdio.h>
#include <oxs_constants.h>
#include <oxs_error.h>
#include <oxs_token_x509_issuer_serial.h>
#include <axiom_element.h>


AXIS2_EXTERN axiom_node_t* AXIS2_CALL
oxs_token_build_x509_issuer_serial_element(const axis2_env_t *env,
        axiom_node_t *parent)
{
    axiom_node_t *x509_issuer_serial_node = NULL;
    axiom_element_t *x509_issuer_serial_ele = NULL;
    axiom_namespace_t *ns_obj = NULL;

    ns_obj = axiom_namespace_create(env, OXS_DSIG_NS,
            OXS_DS);


    x509_issuer_serial_ele = axiom_element_create(env, parent, OXS_NODE_X509_ISSUER_SERIAL, ns_obj, &x509_issuer_serial_node);
    if (!x509_issuer_serial_ele)
    {
        oxs_error(ERROR_LOCATION,
                OXS_ERROR_ELEMENT_FAILED, "Error creating X509IssuerSerial element");
        return NULL;
    }

    return x509_issuer_serial_node;
}
