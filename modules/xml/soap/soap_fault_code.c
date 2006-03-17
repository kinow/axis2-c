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
 
 #include <_axis2_soap_fault_code.h>
 #include <axis2_soap_fault_sub_code.h>
 #include <axis2_soap_fault_value.h>
 #include <axis2_soap_builder.h>
 #include <_axis2_soap_fault.h>

 /***************** impl struct ***********************************************/
 
 typedef struct axis2_soap_fault_code_impl_t
 {
    axis2_soap_fault_code_t fault_code;
    
    axis2_om_node_t *om_ele_node;
    
    axis2_soap_fault_sub_code_t *subcode;
    
    axis2_soap_fault_value_t *value;
    
    axis2_soap_builder_t *builder;
 
 }axis2_soap_fault_code_impl_t;
 
 /******************** Macro **************************************************/
 
 #define AXIS2_INTF_TO_IMPL(code) ((axis2_soap_fault_code_impl_t*)code)
 
 /******************** function prototypes ************************************/
 
axis2_status_t AXIS2_CALL 
axis2_soap_fault_code_free(axis2_soap_fault_code_t *fault_code,
                           axis2_env_t **env);

axis2_soap_fault_sub_code_t* AXIS2_CALL 
axis2_soap_fault_code_get_sub_code(axis2_soap_fault_code_t *fault_code,
                                  axis2_env_t **env);
                                     
axis2_soap_fault_value_t* AXIS2_CALL
axis2_soap_fault_code_get_value(axis2_soap_fault_code_t *fault_code,
                                axis2_env_t **env);
                                     

axis2_om_node_t* AXIS2_CALL 
axis2_soap_fault_code_get_base_node(axis2_soap_fault_code_t *fault_code,
                                    axis2_env_t **env);
 
/********************* function implementation ********************************/

AXIS2_DECLARE(axis2_soap_fault_code_t *)
axis2_soap_fault_code_create(axis2_env_t **env)
{
    axis2_soap_fault_code_impl_t *fault_code_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    fault_code_impl = (axis2_soap_fault_code_impl_t*) AXIS2_MALLOC(
                            (*env)->allocator,
                            sizeof(axis2_soap_fault_code_impl_t));
    if(!fault_code_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    fault_code_impl->fault_code.ops = NULL;
    fault_code_impl->om_ele_node = NULL;
    fault_code_impl->subcode = NULL;
    fault_code_impl->value = NULL;
    fault_code_impl->builder = NULL;
    
    fault_code_impl->fault_code.ops = 
            (axis2_soap_fault_code_ops_t*)AXIS2_MALLOC((*env)->allocator,
                sizeof(axis2_soap_fault_code_ops_t));
                
    if(!(fault_code_impl->fault_code.ops))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE((*env)->allocator, fault_code_impl);
        return NULL;
    }                                                                  
    
    fault_code_impl->fault_code.ops->free_fn = 
        axis2_soap_fault_code_free;
        
    fault_code_impl->fault_code.ops->get_sub_code =
        axis2_soap_fault_code_get_sub_code;
        
    fault_code_impl->fault_code.ops->get_value =
        axis2_soap_fault_code_get_value;              
        
    fault_code_impl->fault_code.ops->get_base_node =
        axis2_soap_fault_code_get_base_node;
        
  return  &(fault_code_impl->fault_code);  
}

AXIS2_DECLARE(axis2_soap_fault_code_t *)
axis2_soap_fault_code_create_with_parent(axis2_env_t **env,
                            axis2_soap_fault_t *fault)
{
    axis2_soap_fault_code_impl_t *fault_code_impl = NULL;
    axis2_soap_fault_code_t *fault_code = NULL;
    
    axis2_om_element_t *this_ele = NULL;
    axis2_om_node_t *this_node = NULL;
    
    axis2_om_node_t *parent_node = NULL;
    axis2_om_element_t *parent_ele = NULL;
    
    axis2_om_namespace_t *parent_ns = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, fault, NULL);
    
    fault_code = axis2_soap_fault_code_create(env);
    if(!fault_code)
        return NULL;
        
    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);
    
    parent_node = AXIS2_SOAP_FAULT_GET_BASE_NODE(fault, env);
    if(!parent_node)
    {
        AXIS2_SOAP_FAULT_CODE_FREE(fault_code, env);
        return NULL;
    }        
    parent_ele  = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
                        parent_node, env);
    
    if(!parent_ele)
    {
        AXIS2_SOAP_FAULT_CODE_FREE(fault_code, env);
        return NULL;
    }
    
    parent_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(parent_ele, env);
    
    this_ele = axis2_om_element_create(env, 
                                       parent_node,                             
                                       AXIS2_SOAP12_SOAP_FAULT_CODE_LOCAL_NAME,
                                       parent_ns,
                                       &this_node);
    if(!this_ele)
    {
        AXIS2_SOAP_FAULT_CODE_FREE(fault_code, env);
        return NULL;
    }
    
    fault_code_impl->om_ele_node = this_node;    
    
    axis2_soap_fault_set_code (fault, env, fault_code);
    
    return  &(fault_code_impl->fault_code);            
}


axis2_status_t AXIS2_CALL 
axis2_soap_fault_code_free(axis2_soap_fault_code_t *fault_code,
                           axis2_env_t **env)
{
    axis2_soap_fault_code_impl_t *fault_code_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);
    
    if(NULL != fault_code_impl->subcode)
    {
        AXIS2_SOAP_FAULT_SUB_CODE_FREE(fault_code_impl->subcode, env);
        fault_code_impl->subcode = NULL;
    }
    if(NULL != fault_code_impl->value)
    {
        AXIS2_SOAP_FAULT_VALUE_FREE(fault_code_impl->value, env);
        fault_code_impl->value = NULL;
    }
    if(NULL != fault_code->ops)
    {
        AXIS2_FREE((*env)->allocator, fault_code->ops);
        fault_code->ops = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, fault_code_impl);
    fault_code_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_soap_fault_code_set_value(axis2_soap_fault_code_t *fault_code,
                                axis2_env_t **env,
                                axis2_soap_fault_value_t *fault_val)
{
    axis2_soap_fault_code_impl_t *fault_code_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   
    AXIS2_PARAM_CHECK((*env)->error, fault_val, AXIS2_FAILURE);
    
    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);
    if(!(fault_code_impl->value))
    {
        fault_code_impl->value = fault_val;
        return AXIS2_SUCCESS;    
    }
    else
    {
        AXIS2_LOG_DEBUG((*env)->log , AXIS2_LOG_SI, 
        "trying to set fault value to fault code more than once");
    }
    return AXIS2_FAILURE;    
}

axis2_status_t AXIS2_CALL 
axis2_soap_fault_code_set_sub_code(axis2_soap_fault_code_t *fault_code,
                                  axis2_env_t **env,
                                  axis2_soap_fault_sub_code_t *fault_subcode)
{
    axis2_soap_fault_code_impl_t *fault_code_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, fault_subcode, AXIS2_FAILURE);
    
    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);
    
    if(!(fault_code_impl->subcode))
    {
        fault_code_impl->subcode = fault_subcode;
        return AXIS2_SUCCESS;    
    }
    else
    {
        AXIS2_LOG_DEBUG((*env)->log , AXIS2_LOG_SI, 
            "trying to set fault subcode to fault code more than once ");
    }
    return AXIS2_FAILURE;
}                                                                         
        
axis2_soap_fault_sub_code_t* AXIS2_CALL 
axis2_soap_fault_code_get_sub_code(axis2_soap_fault_code_t *fault_code,
                                  axis2_env_t **env)
{
    axis2_soap_fault_code_impl_t *fault_code_impl = NULL;
    int status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, NULL);
    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);
    
    if(NULL != fault_code_impl->subcode)
    {
        return fault_code_impl->subcode;
    }
    else if(NULL != fault_code_impl->builder)
    {
        while(!(fault_code_impl->subcode) && 
            !(AXIS2_OM_NODE_GET_BUILD_STATUS(fault_code_impl->om_ele_node, env)))
        {
            status = AXIS2_SOAP_BUILDER_NEXT(fault_code_impl->builder, env);
            if(status == AXIS2_FAILURE)
            {
                break;
            }
        }
    }
    return fault_code_impl->subcode;
}
                                     
axis2_soap_fault_value_t* AXIS2_CALL
axis2_soap_fault_code_get_value(axis2_soap_fault_code_t *fault_code,
                                axis2_env_t **env)
{
    axis2_soap_fault_code_impl_t *fault_code_impl = NULL;
    
    int status = AXIS2_SUCCESS;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);
    
    if(NULL != fault_code_impl->value)
    {
        return fault_code_impl->value;
    }
    else if(fault_code_impl->builder)
    {
        while(!(fault_code_impl->value) && 
            !(AXIS2_OM_NODE_GET_BUILD_STATUS(fault_code_impl->om_ele_node, env)))
        {
            status = AXIS2_SOAP_BUILDER_NEXT(fault_code_impl->builder, env);
            if(status == AXIS2_FAILURE)
            {
                break;
            }
        }
    }
    return fault_code_impl->value;    
}
                                     
axis2_status_t AXIS2_CALL 
axis2_soap_fault_code_set_base_node(axis2_soap_fault_code_t *fault_code,
                                    axis2_env_t **env,
                                    axis2_om_node_t *node)
{
   axis2_soap_fault_code_impl_t *fault_code_impl = NULL;
   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK((*env)->error, node, AXIS2_FAILURE);
   
   fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);
   
   if(AXIS2_OM_NODE_GET_NODE_TYPE(node, env) != AXIS2_OM_ELEMENT)
   {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
   }
   fault_code_impl->om_ele_node = node;
   return AXIS2_SUCCESS;

}

axis2_om_node_t* AXIS2_CALL 
axis2_soap_fault_code_get_base_node(axis2_soap_fault_code_t *fault_code,
                                    axis2_env_t **env)
{
   AXIS2_ENV_CHECK(env, NULL);
   return AXIS2_INTF_TO_IMPL(fault_code)->om_ele_node;
}
 
axis2_status_t AXIS2_CALL
axis2_soap_fault_code_set_builder(axis2_soap_fault_code_t *fault_code,
                                  axis2_env_t **env,
                                  axis2_soap_builder_t *soap_builder)
{
    axis2_soap_fault_code_impl_t *fault_code_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_builder, AXIS2_FAILURE);
    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);
    fault_code_impl->builder = soap_builder;
    return AXIS2_SUCCESS;
}
