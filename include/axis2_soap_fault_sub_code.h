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
 
 #ifndef AXIS2_SOAP_FAULT_SUB_CODE_H
 #define AXIS2_SOAP_FAULT_SUB_CODE_H
 
  
   /**
    * @file axis2_soap_fault_sub_code.h
    * @brief axis2_soap_fault_sub_code struct
    */
#include <axis2_env.h>
#include <axis2_soap_fault_code.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axis2_soap_fault_sub_code_ops axis2_soap_fault_sub_code_ops_t;
    typedef struct axis2_soap_fault_sub_code axis2_soap_fault_sub_code_t;
    struct axis2_soap_fault_value;
    struct axis2_soap_builder;
/**
 * @defgroup axis2_soap_fault_sub_code
 * @ingroup axis2_soap
 * @{
 */

/**
 *   \brief soap_fault_sub_code operations struct
 *   ops Encapsulator struct of axis2_soap_fault_sub_code
 */
 AXIS2_DECLARE_DATA   struct axis2_soap_fault_sub_code_ops
    {
      /**
        * Free an axis2_soap_fault_sub_code
        * @param  fault_sub_code pointer to soap_fault_sub_code struct
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *free_fn)
                             (axis2_soap_fault_sub_code_t *fault_sub_code,
                              axis2_env_t **env);
                                     
        axis2_soap_fault_sub_code_t* (AXIS2_CALL *get_sub_code)                                         
                             (axis2_soap_fault_sub_code_t *fault_sub_code,
                              axis2_env_t **env);
                                     
        struct axis2_soap_fault_value* (AXIS2_CALL *get_value)                                         
                             (axis2_soap_fault_sub_code_t *fault_sub_code,
                              axis2_env_t **env);
                                     
        axis2_om_node_t* (AXIS2_CALL *get_base_node)
                             (axis2_soap_fault_sub_code_t *fault_sub_code,
                              axis2_env_t **env);

        };      

  /**
    * \brief soap_fault_sub_code struct
    * represent a soap_fault_sub_code
    */
    struct axis2_soap_fault_sub_code
    {
        /** operation of axis2_soap_fault_sub_code struct */
        axis2_soap_fault_sub_code_ops_t *ops;
       
    };

  /**
    * creates a soap struct 
    * @param env Environment. MUST NOT be NULL
    */
AXIS2_DECLARE(axis2_soap_fault_sub_code_t *)
axis2_soap_fault_sub_code_create_with_parent(axis2_env_t **env,
                            axis2_soap_fault_code_t *fault_code);
                            

/******************** Macros **************************************************/
    
    
/** free soap_fault_sub_code */
#define AXIS2_SOAP_FAULT_SUB_CODE_FREE(fault_sub_code , env) \
        ((fault_sub_code)->ops->free_fn(fault_sub_code, env))
        
#define AXIS2_SOAP_FAULT_SUB_CODE_GET_VALUE(fault_sub_code , env) \
        ((fault_sub_code)->ops->get_node(fault_sub_code, env)) 
        
#define AXIS2_SOAP_FAULT_SUB_CODE_GET_SUB_CODE(fault_sub_code , env) \
        ((fault_sub_code)->ops->get_subcode(fault_sub_code, env))        
        
#define AXIS2_SOAP_FAULT_SUB_CODE_GET_BASE_NODE(fault_sub_code, env) \
        ((fault_sub_code)->ops->get_base_node(fault_sub_code, env))         

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_SOAP_FAULT_SUB_CODE_H */
