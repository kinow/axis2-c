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

#include <axis2_addr_mod.h>
#include <axis2_addr.h>
#include <axis2_handler_desc.h>
#include <axis2_array_list.h>
#include <axis2_soap_envelope.h>
#include <axis2_soap_header.h>
#include <axis2_soap_header_block.h>
#include <axis2_op.h>
#include <axis2_msg_ctx.h>
#include <axis2_msg_info_headers.h>
#include <axis2_property.h>

axis2_status_t AXIS2_CALL
axis2_addr_in_handler_invoke(struct axis2_handler *handler, 
                         axis2_env_t **env,
                         struct axis2_msg_ctx *msg_ctx);
                                             
axis2_bool_t 
axis2_addr_in_check_element(axis2_env_t **env, axis2_qname_t *expected_qname,
              axis2_qname_t *actual_qname);
                         
axis2_status_t 
axis2_addr_in_extract_svc_grp_ctx_id(axis2_env_t **env, 
                       axis2_soap_header_t *soap_header, 
                       axis2_msg_ctx_t *msg_ctx);
                         
axis2_status_t
axis2_addr_in_extract_ref_params(axis2_env_t **env,
                   axis2_soap_header_t *soap_header,
                   axis2_msg_info_headers_t *msg_info_headers);
                            
axis2_status_t 
axis2_addr_in_extract_addr_final_info(axis2_env_t **env,
                        axis2_soap_header_t *soap_header,
                        axis2_msg_info_headers_t **msg_info_headers,
                        axis2_array_list_t *addr_headers);
                         
                         
axis2_status_t 
axis2_addr_in_extract_to_epr_ref_params(axis2_env_t **env,
                          axis2_endpoint_ref_t * to_epr,
                          axis2_soap_header_t *soap_header,
                          axis2_char_t *addr_ns);
                          
axis2_status_t 
axis2_addr_in_extract_epr_information(axis2_env_t **env,
                        axis2_soap_header_block_t *soap_header_block,
                        axis2_endpoint_ref_t *endpoint_ref,
                        axis2_char_t *addr_ns);
                        
axis2_status_t
axis2_addr_in_extract_addr_params(axis2_env_t **env,
                    axis2_soap_header_t *soap_header,
                    axis2_msg_info_headers_t **msg_info_headers,
                    axis2_array_list_t *addr_headers,
                    axis2_char_t *addr_ns);

axis2_status_t axis2_addr_in_extract_addr_submission_info(axis2_env_t **env,
        axis2_soap_header_t *soap_header,
        axis2_msg_info_headers_t **msg_info_headers,
        axis2_array_list_t *addr_headers);

/******************************************************************************/                         

AXIS2_DECLARE(axis2_handler_t*)
axis2_addr_in_handler_create(axis2_env_t **env, 
                         axis2_qname_t *qname) 
{
    axis2_handler_t *handler = NULL;
    axis2_handler_desc_t *handler_desc = NULL;
    axis2_qname_t *handler_qname = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    if (qname)
    {
        handler_qname = AXIS2_QNAME_CLONE(qname, env);
        if (!(handler_qname))
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;        
        }
    }
    else
    {
        /* create default qname */
        handler_qname = axis2_qname_create(env, "addr_in_handler", 
                                                "http://axis.ws.apache.org",
                                                NULL);
        if (!handler_qname)
        {
            return NULL;        
        }           
    }
    
    handler = axis2_handler_create(env);
    if (!handler)
    {
        return NULL;
    }
    
    /* handler desc of base handler */
    handler_desc = axis2_handler_desc_create_with_qname(env, handler_qname);
    if (!handler_desc)
    {
        AXIS2_HANDLER_FREE(handler, env);
        return NULL;
    }
    
    AXIS2_HANDLER_INIT(handler, env, handler_desc);
    
    /* set the base struct's invoke op */
    if (handler->ops) 
        handler->ops->invoke = axis2_addr_in_handler_invoke;

    return handler;
}


axis2_status_t AXIS2_CALL
axis2_addr_in_handler_invoke(struct axis2_handler *handler, 
                         axis2_env_t **env,
                         struct axis2_msg_ctx *msg_ctx)
{
    axis2_soap_envelope_t *soap_envelope = NULL;
    axis2_soap_header_t *soap_header = NULL;
    axis2_property_t *property = NULL;
    
    AXIS2_ENV_CHECK( env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
    
    /*op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    svc_ctx = AXIS2_MSG_CTX_GET_SVC_CTX(msg_ctx, env);*/
    
    AXIS2_LOG_INFO((*env)->log, "Starting addressing in handler .........");
    
    soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);
    
    if (soap_envelope)
    {
        soap_header = AXIS2_SOAP_ENVELOPE_GET_HEADER(soap_envelope, env);
        if (soap_header)
        {
            axis2_array_list_t *addr_headers = NULL;
            axis2_ctx_t *ctx = NULL;
            axis2_char_t *addr_ns_str = NULL;
            axis2_msg_info_headers_t *msg_info_headers = AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS(msg_ctx, env);
            
            addr_headers = AXIS2_SOAP_HEADER_GET_HEADER_BLOCKS_WITH_NAMESPACE_URI(soap_header, env, AXIS2_WSA_NAMESPACE_SUBMISSION);
            if (addr_headers)
            {
                addr_ns_str = AXIS2_STRDUP(AXIS2_WSA_NAMESPACE_SUBMISSION, env);
                axis2_addr_in_extract_addr_submission_info(env, 
                        soap_header,
                        &msg_info_headers,
                        addr_headers);
            } 
            else 
            {
                addr_headers = AXIS2_SOAP_HEADER_GET_HEADER_BLOCKS_WITH_NAMESPACE_URI(soap_header, env, AXIS2_WSA_NAMESPACE);
                if (addr_headers)
                {
                    addr_ns_str = AXIS2_STRDUP(AXIS2_WSA_NAMESPACE, env);
                    axis2_addr_in_extract_addr_final_info(env, 
                            soap_header,
                            &msg_info_headers,
                            addr_headers);
                    axis2_addr_in_extract_ref_params(env, soap_header, AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS(msg_ctx, env));
    
                } 
                else 
                {
                    /* addressing headers are not present in the SOAP message*/
                    /*logger.debug(
                            "No Addressing Headers present in the IN message. Addressing In Handler does nothing.");
                    */
                    /** TODO : set the error */
                    AXIS2_LOG_ERROR((*env)->log, AXIS2_LOG_SI, "No Addressing Headers present in the IN message. Addressing In Handler cannot do anything.");
                    return AXIS2_FAILURE;
                }
            }
            
            ctx = AXIS2_MSG_CTX_GET_BASE(msg_ctx, env);
            if (ctx)
            {
                property = axis2_property_create(env);
                AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_REQUEST);
                AXIS2_PROPERTY_SET_VALUE(property, env, addr_ns_str);
                AXIS2_CTX_SET_PROPERTY(ctx, env, AXIS2_WSA_VERSION, property, AXIS2_TRUE);
            }
    
            /* extract service group context, if available */
            axis2_addr_in_extract_svc_grp_ctx_id(env, soap_header, msg_ctx);
            
            AXIS2_ARRAY_LIST_FREE(addr_headers, env);
            return AXIS2_SUCCESS;
        }
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t 
axis2_addr_in_extract_svc_grp_ctx_id(axis2_env_t **env, 
                       axis2_soap_header_t *soap_header, 
                       axis2_msg_ctx_t *msg_ctx)
{
    axis2_om_node_t *node = NULL;
    axis2_om_element_t *element = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    node = AXIS2_SOAP_HEADER_GET_BASE_NODE(soap_header, env);
    if (node && AXIS2_OM_NODE_GET_NODE_TYPE(node, env) == AXIS2_OM_ELEMENT)
    {
        axis2_qname_t *qname = NULL;
        element = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env);
        qname = axis2_qname_create(env, AXIS2_SVC_GRP_ID, AXIS2_NAMESPACE_URI,
            AXIS2_NAMESPACE_PREFIX);
        if (qname)
        {
            axis2_om_node_t *child_node = NULL;
            axis2_om_element_t *child_element = NULL;
            child_element = AXIS2_OM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(element, env, qname, node, &child_node);
            if (child_element)
            {
                axis2_conf_ctx_t * conf_ctx = NULL;
                axis2_char_t *grp_id = AXIS2_OM_ELEMENT_GET_TEXT(child_element, env,
                    child_node);
                conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
                if (conf_ctx && grp_id)
                {
                    axis2_svc_grp_ctx_t *svc_ctx_grp_ctx = AXIS2_CONF_CTX_GET_SVC_GRP_CTX(conf_ctx, env, grp_id);
                    if (!svc_ctx_grp_ctx)
                    {
                        /** TODO, set error */
                        return AXIS2_FAILURE;
                    }
                    AXIS2_MSG_CTX_SET_SVC_GRP_CTX_ID(msg_ctx, env, grp_id);
                    return AXIS2_SUCCESS;
                }
            }
        }
        AXIS2_QNAME_FREE(qname, env);
    }
    /** TODO, set error */
    return AXIS2_FAILURE;
}

    

axis2_status_t 
axis2_addr_in_extract_addr_final_info(axis2_env_t **env,
                        axis2_soap_header_t *soap_header,
                        axis2_msg_info_headers_t **msg_info_headers,
                        axis2_array_list_t *addr_headers)
{
    return axis2_addr_in_extract_addr_params(env,
            soap_header,
            msg_info_headers,
            addr_headers,
            AXIS2_WSA_NAMESPACE);
}

axis2_status_t axis2_addr_in_extract_addr_submission_info(axis2_env_t **env,
        axis2_soap_header_t *soap_header,
        axis2_msg_info_headers_t **msg_info_headers,
        axis2_array_list_t *addr_headers)
{
    return axis2_addr_in_extract_addr_params(env,
            soap_header,
            msg_info_headers,
            addr_headers,
            AXIS2_WSA_NAMESPACE_SUBMISSION);
}

axis2_status_t
axis2_addr_in_extract_addr_params(axis2_env_t **env,
                    axis2_soap_header_t *soap_header,
                    axis2_msg_info_headers_t **msg_info_headers_p,
                    axis2_array_list_t *addr_headers,
                    axis2_char_t *addr_ns_str)
{
    axis2_hash_t *header_block_ht = NULL;
    axis2_hash_index_t *hash_index =  NULL;
    axis2_msg_info_headers_t *msg_info_headers = *(msg_info_headers_p);
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_header, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_info_headers_p, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, addr_headers, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, addr_ns_str, AXIS2_FAILURE);
    
    

    if (!msg_info_headers)
        msg_info_headers = axis2_msg_info_headers_create(env, NULL, NULL);
    if (!msg_info_headers)
        return AXIS2_FAILURE;
    
    header_block_ht = AXIS2_SOAP_HEADER_GET_ALL_HEADER_BLOCKS(soap_header, env);
    if(!header_block_ht)
        return AXIS2_FAILURE;            
    
    for(hash_index = axis2_hash_first(header_block_ht, env); hash_index;
            hash_index = axis2_hash_next(env, hash_index))
    {   
        void *hb = NULL;
        axis2_soap_header_block_t *header_block =    NULL;
        axis2_om_node_t *header_block_node = NULL;
        axis2_om_element_t *header_block_ele = NULL;
        axis2_char_t *ele_localname = NULL;
        axis2_endpoint_ref_t *epr = NULL;        
        
        axis2_hash_this(hash_index, NULL, NULL, &hb);
        
        header_block = (axis2_soap_header_block_t *)hb;
        header_block_node = AXIS2_SOAP_HEADER_BLOCK_GET_BASE_NODE(header_block, env);
        header_block_ele  = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(header_block_node, env);
        ele_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(header_block_ele, env);
        
        if(AXIS2_STRCMP(ele_localname, AXIS2_WSA_TO) == 0)
        {
             /* here the addressing epr overidde what ever already there in the message context */   
                
                epr = axis2_endpoint_ref_create(env, AXIS2_OM_ELEMENT_GET_TEXT(header_block_ele, env, header_block_node));
                AXIS2_MSG_INFO_HEADERS_SET_TO(msg_info_headers, env, epr);
                
                axis2_addr_in_extract_to_epr_ref_params(env, epr, soap_header, addr_ns_str);
                AXIS2_SOAP_HEADER_BLOCK_SET_PRECESSED(header_block, env);
        }
        else if(AXIS2_STRCMP(ele_localname, AXIS2_WSA_FROM) == 0)
        {
                epr = AXIS2_MSG_INFO_HEADERS_GET_FROM(msg_info_headers, env);
                if(!epr)
                {
                    /* I don't know the address now. Let me pass the empty 
                        string now and fill this once I process the Elements under this. */
                    
                    
                    epr = axis2_endpoint_ref_create(env, "");
                    AXIS2_MSG_INFO_HEADERS_SET_FROM(msg_info_headers, env, epr);
                }     
                axis2_addr_in_extract_epr_information(env, header_block, epr, addr_ns_str);
                AXIS2_SOAP_HEADER_BLOCK_SET_PRECESSED(header_block, env);
        }
        else if( AXIS2_STRCMP(ele_localname, AXIS2_WSA_REPLY_TO) == 0)
        {
            epr = AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO(msg_info_headers, env);
            if(!epr)
            {
                epr = axis2_endpoint_ref_create(env, "");
                AXIS2_MSG_INFO_HEADERS_SET_REPLY_TO(msg_info_headers, env, epr);                
            }
            axis2_addr_in_extract_epr_information(env, header_block, epr, addr_ns_str);
            AXIS2_SOAP_HEADER_BLOCK_SET_PRECESSED(header_block, env);
        }
        else if(AXIS2_STRCMP(ele_localname, AXIS2_WSA_FAULT_TO) == 0)
        {
            epr = AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO(msg_info_headers , env);
            if(!epr)
            {
               epr = axis2_endpoint_ref_create(env, "");
               AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO(msg_info_headers, env, epr);
             }                    
            axis2_addr_in_extract_epr_information(env, header_block, epr, addr_ns_str);
            AXIS2_SOAP_HEADER_BLOCK_SET_PRECESSED(header_block, env);         
        }
        else if(AXIS2_STRCMP(ele_localname, AXIS2_WSA_MESSAGE_ID) == 0)
        {
            axis2_char_t *text = NULL;
            text = AXIS2_OM_ELEMENT_GET_TEXT(header_block_ele, env, header_block_node);
            AXIS2_MSG_INFO_HEADERS_SET_MESSAGE_ID(msg_info_headers, env, text);
            AXIS2_SOAP_HEADER_BLOCK_SET_PRECESSED(header_block, env);              
            if(NULL != text)
                AXIS2_FREE((*env)->allocator, text);
        }
        else if(AXIS2_STRCMP(ele_localname, AXIS2_WSA_ACTION) == 0)
        {
            axis2_char_t *text = NULL;
            text = AXIS2_OM_ELEMENT_GET_TEXT(header_block_ele, env, header_block_node);
            AXIS2_MSG_INFO_HEADERS_SET_ACTION(msg_info_headers, env, text); 
            AXIS2_SOAP_HEADER_BLOCK_SET_PRECESSED(header_block, env);       
            if(NULL != text)
                AXIS2_FREE((*env)->allocator, text);
        }
        else if(AXIS2_STRCMP(ele_localname, AXIS2_WSA_RELATES_TO) == 0)
        {
            axis2_char_t *address = NULL;
            axis2_qname_t *rqn = NULL;
            axis2_om_attribute_t *relationship_type = NULL;
            axis2_char_t *relationship_type_default_value =  NULL;
            axis2_char_t *relationship_type_value = NULL;
            axis2_relates_to_t *relates_to =  NULL;
            if(AXIS2_STRCMP(AXIS2_WSA_NAMESPACE_SUBMISSION , addr_ns_str) == 0)
            {
                relationship_type_default_value = 
                    AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE_DEFAULT_VALUE_SUBMISSION;
            }
            else
            {
                relationship_type_default_value = AXIS2_WSA_ANONYMOUS_URL_SUBMISSION;                
            }                
            rqn = axis2_qname_create(env, AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE, NULL, NULL);
            
            relationship_type = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(header_block_ele, env, rqn);
            
            if(!relationship_type)
            {
                relationship_type_value = 
                    AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE_DEFAULT_VALUE_SUBMISSION;    
            }
            else 
            {
                relationship_type_value = AXIS2_OM_ATTRIBUTE_GET_VALUE(relationship_type, env);    
            }
            
            address = AXIS2_OM_ELEMENT_GET_TEXT(header_block_ele, env, header_block_node);
            relates_to = axis2_relates_to_create(env, address, relationship_type_value);
            AXIS2_MSG_INFO_HEADERS_SET_RELATES_TO(msg_info_headers, env, relates_to);
            AXIS2_SOAP_HEADER_BLOCK_SET_PRECESSED(header_block, env);       
            
            AXIS2_QNAME_FREE(rqn, env);
        }
     }
     return AXIS2_SUCCESS;
}

axis2_status_t 
axis2_addr_in_extract_epr_information(axis2_env_t **env,
                        axis2_soap_header_block_t *soap_header_block,
                        axis2_endpoint_ref_t *endpoint_ref,
                        axis2_char_t *addr_ns_str) 
{
    axis2_qname_t *epr_addr_qn = NULL;
    axis2_qname_t *epr_ref_qn = NULL;
    axis2_qname_t *wsa_meta_qn = NULL;
    axis2_om_node_t *header_block_node = NULL;
    axis2_om_element_t *header_block_ele = NULL;
    axis2_om_child_element_iterator_t *child_ele_iter = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_header_block, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, endpoint_ref, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, addr_ns_str, AXIS2_FAILURE);
    
    header_block_node = AXIS2_SOAP_HEADER_BLOCK_GET_BASE_NODE(soap_header_block, env);
    header_block_ele  = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(header_block_node, env);
    
    
    child_ele_iter = AXIS2_OM_ELEMENT_GET_CHILD_ELEMENTS(header_block_ele, env, header_block_node);
    if(!child_ele_iter)
        return AXIS2_FAILURE;
    epr_addr_qn = axis2_qname_create(env, EPR_ADDRESS, addr_ns_str, NULL); 
    epr_ref_qn = axis2_qname_create(env, EPR_REFERENCE_PARAMETERS, addr_ns_str, NULL);
    wsa_meta_qn = axis2_qname_create(env, AXIS2_WSA_METADATA, addr_ns_str, NULL);
    while(AXIS2_OM_CHILD_ELEMENT_ITERATOR_HAS_NEXT(child_ele_iter, env))
    {
        axis2_om_node_t *child_node = NULL;
        axis2_om_element_t *child_ele = NULL;
        axis2_qname_t *child_qn = NULL;
        child_node = AXIS2_OM_CHILD_ELEMENT_ITERATOR_NEXT(child_ele_iter, env);
        child_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(child_node, env);
       
        
        child_qn = AXIS2_OM_ELEMENT_GET_QNAME(child_ele, env);
        if(axis2_addr_in_check_element(env, epr_addr_qn, child_qn ))
        {
            AXIS2_ENDPOINT_REF_SET_ADDRESS(endpoint_ref, env,
                AXIS2_OM_ELEMENT_GET_TEXT(child_ele, env, child_node));            
        }
        else if(axis2_addr_in_check_element(env, epr_ref_qn , child_qn))
        {
            axis2_om_child_element_iterator_t *ref_param_iter = NULL;       
            
            axis2_any_content_type_t *any_content_type = NULL;
            any_content_type = AXIS2_ENDPOINT_REF_GET_REF_PARAMS(endpoint_ref, env);
            if(any_content_type == NULL)
            {
                any_content_type = axis2_any_content_type_create(env);
                AXIS2_ENDPOINT_REF_SET_REF_PARAMS(endpoint_ref, env, any_content_type);
            }            

            ref_param_iter = AXIS2_OM_ELEMENT_GET_CHILD_ELEMENTS(child_ele, env, child_node);
            if(ref_param_iter)
            {
                while(AXIS2_OM_CHILD_ELEMENT_ITERATOR_HAS_NEXT(ref_param_iter, env))
                {
                   axis2_om_node_t *om_node =  NULL;
                   axis2_om_element_t *om_ele = NULL;
                   axis2_any_content_type_t *any_content = NULL;
                   om_node = AXIS2_OM_CHILD_ELEMENT_ITERATOR_NEXT(ref_param_iter, env);
                   om_ele  = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(om_node, env);
                   any_content = AXIS2_ENDPOINT_REF_GET_REF_PARAMS(endpoint_ref, env);
                   if(any_content && om_ele)
                   {
                       AXIS2_ANY_CONTENT_TYPE_ADD_VALUE(any_content, env, 
                            AXIS2_OM_ELEMENT_GET_QNAME(om_ele, env), AXIS2_OM_ELEMENT_GET_TEXT(om_ele, env, om_node));
                       
                   }                       
                }                    
            }
            
        }else if(axis2_addr_in_check_element(env, wsa_meta_qn, child_qn))        
        {
           axis2_any_content_type_t *any_content_type = AXIS2_ENDPOINT_REF_GET_METADATA(endpoint_ref, env);
           if(any_content_type)
           {
               AXIS2_ANY_CONTENT_TYPE_ADD_VALUE(any_content_type, env, 
                            child_qn , AXIS2_OM_ELEMENT_GET_TEXT(child_ele, env, child_node));
           }               
            
        }
    }        
    AXIS2_QNAME_FREE(epr_addr_qn, env);
    AXIS2_QNAME_FREE(epr_ref_qn, env);
    AXIS2_QNAME_FREE(wsa_meta_qn, env);
    return AXIS2_SUCCESS;
}
 




/**
 * WSA 1.0 specification mandates all the reference parameters to have a attribute as wsa:Type=???parameter???. So
 * here this will check for soap_header blocks with the above attribute and will put them in message information soap_header collection
 *
 * @param soap_header
 * @param messageInformationHeaders
 */
axis2_status_t 
axis2_addr_in_extract_ref_params(axis2_env_t **env, 
                   axis2_soap_header_t *soap_header,
                    axis2_msg_info_headers_t* msg_info_headers) 
{
    axis2_hash_t *header_block_ht = NULL;
    axis2_hash_index_t *hash_index = NULL;
    axis2_qname_t *wsa_qname = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_header, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_info_headers, AXIS2_FAILURE);

    header_block_ht = AXIS2_SOAP_HEADER_GET_ALL_HEADER_BLOCKS(soap_header, env);
    if(!header_block_ht)
        return AXIS2_FAILURE;            
    wsa_qname = axis2_qname_create(env, AXIS2_WSA_IS_REFERENCE_PARAMETER_ATTRIBUTE, 
            AXIS2_WSA_NAMESPACE, NULL);
    
    for(hash_index = axis2_hash_first(header_block_ht, env); hash_index;
            hash_index = axis2_hash_next(env, hash_index))
    {   
        void *hb = NULL;
        axis2_soap_header_block_t *header_block =    NULL;
        axis2_om_node_t *header_block_node = NULL;
        axis2_om_element_t *header_block_ele = NULL;
        
        axis2_hash_this(hash_index, NULL, NULL, &hb);
        
        header_block = (axis2_soap_header_block_t *)hb;
        header_block_node = AXIS2_SOAP_HEADER_BLOCK_GET_BASE_NODE(header_block, env);
        
        if(header_block_node && 
            (AXIS2_OM_NODE_GET_NODE_TYPE(header_block_node, env) == AXIS2_OM_ELEMENT))
        {
            axis2_om_attribute_t *om_attr = NULL;
            axis2_char_t *attr_value = NULL;
            header_block_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(header_block_node, env);
            om_attr = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(header_block_ele, env, wsa_qname);
            if(om_attr)
            {
                attr_value = AXIS2_OM_ATTRIBUTE_GET_LOCALNAME(om_attr, env);
                if(AXIS2_STRCMP(attr_value, AXIS2_WSA_TYPE_ATTRIBUTE_VALUE) == 0)
                {
                    AXIS2_MSG_INFO_HEADERS_ADD_REF_PARAM(msg_info_headers, env, header_block_node);    
                }
            }
        }
    }
        
    AXIS2_QNAME_FREE(wsa_qname, env);

    return AXIS2_SUCCESS;
}


axis2_status_t 
axis2_addr_in_extract_to_epr_ref_params(axis2_env_t **env,
                          axis2_endpoint_ref_t * to_epr,
                          axis2_soap_header_t *soap_header,
                          axis2_char_t *addr_ns_str)
{
    axis2_hash_t *header_blocks_ht = NULL;
    axis2_hash_index_t *hash_index = NULL;
    axis2_qname_t *is_ref_qn = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, to_epr, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_header, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, addr_ns_str, AXIS2_FAILURE);
    
    header_blocks_ht = AXIS2_SOAP_HEADER_GET_ALL_HEADER_BLOCKS(soap_header, env);
    if(!header_blocks_ht)
        return AXIS2_FAILURE;
    
    is_ref_qn = axis2_qname_create(env, "IsReferenceParameter",addr_ns_str, NULL);
    if(!is_ref_qn)
            return AXIS2_FAILURE;
    
    for(hash_index = axis2_hash_first(header_blocks_ht, env); hash_index;
            hash_index = axis2_hash_next( env, hash_index))
    {
        axis2_om_element_t *header_block_ele = NULL;
        axis2_om_node_t *header_block_node = NULL;
        axis2_soap_header_block_t *header_block = NULL;
        void *hb = NULL;
        axis2_om_attribute_t *is_ref_param_attr = NULL;
        axis2_char_t *attr_value = NULL;
        
        axis2_hash_this(hash_index, NULL, NULL, &hb);
        if(hb)
        {
              header_block = (axis2_soap_header_block_t*)hb; 
              header_block_node = AXIS2_SOAP_HEADER_BLOCK_GET_BASE_NODE(header_block, env);
              header_block_ele = (axis2_om_element_t*)
                    AXIS2_OM_NODE_GET_DATA_ELEMENT(header_block_node, env);
              is_ref_param_attr  = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(header_block_ele, env, is_ref_qn);
              if(is_ref_param_attr)
              {
                    attr_value = AXIS2_OM_ATTRIBUTE_GET_LOCALNAME(is_ref_param_attr, env);
                    if(AXIS2_STRCMP("true", attr_value) == 0)
                    {
                        AXIS2_ENDPOINT_REF_ADD_REF_PARAM(to_epr, env,
                            AXIS2_OM_ELEMENT_GET_QNAME(header_block_ele, env), 
                            AXIS2_OM_ELEMENT_GET_TEXT(header_block_ele, env, header_block_node));
                    }
              }                    
        }
    }
    
    AXIS2_QNAME_FREE(is_ref_qn, env);
    return AXIS2_SUCCESS;
}


axis2_bool_t 
axis2_addr_in_check_element(axis2_env_t **env, 
                axis2_qname_t *expected_qname,
                axis2_qname_t *actual_qname)
{
    axis2_char_t *exp_qn_lpart =  NULL;
    axis2_char_t *act_qn_lpart = NULL;
    axis2_char_t *exp_qn_nsuri = NULL;
    axis2_char_t *act_qn_nsuri = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, expected_qname, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, actual_qname, AXIS2_FAILURE);
    
    exp_qn_lpart = AXIS2_QNAME_GET_LOCALPART(expected_qname, env);
    act_qn_lpart = AXIS2_QNAME_GET_LOCALPART(actual_qname, env);
    
    exp_qn_nsuri = AXIS2_QNAME_GET_LOCALPART(expected_qname, env);
    act_qn_nsuri = AXIS2_QNAME_GET_LOCALPART(actual_qname, env);
    
    return ((AXIS2_STRCMP(exp_qn_lpart, act_qn_lpart) == 0) && 
        (AXIS2_STRCMP(exp_qn_nsuri,act_qn_nsuri) == 0));
}
