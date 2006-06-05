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
 
 
#include <xml_schema/axis2_xml_schema_constants.h>
#include <xml_schema/axis2_xml_schema_type.h>
#include <xml_schema/axis2_xml_schema_data_type.h>
#include <xml_schema/axis2_xml_schema_derivation_method.h>
#include <xml_schema/axis2_xml_schema.h>
#include <axis2_qname.h>

typedef struct axis2_xml_schema_type_impl 
                axis2_xml_schema_type_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension  
 */ 
struct axis2_xml_schema_type_impl
{
    axis2_xml_schema_type_t type;
    /** parent type */
    void *base_schema_type;
    
    axis2_xml_schema_annotated_t *annotated;
    
    axis2_hash_t *methods;
    
    axis2_xml_schema_data_type_t *data_type;
    
    axis2_xml_schema_derivation_method_t *derive_by;
    
    axis2_xml_schema_derivation_method_t *final_derivation;
    
    axis2_xml_schema_derivation_method_t *final_resolved;
    
    axis2_bool_t is_mixed;
    /* Name of the type */
    
    axis2_char_t *name;
    
    axis2_xml_schema_t *schema;
    
    axis2_qname_t *qname;
    
    axis2_hash_t *ht_super;
    
    axis2_xml_schema_types_t obj_type;
};

#define AXIS2_INTF_TO_IMPL(type) \
        ((axis2_xml_schema_type_impl_t *) type)

/************************** function prototypes ******************************/

axis2_status_t AXIS2_CALL 
axis2_xml_schema_type_free(void *type,
                        const axis2_env_t *env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_type_get_base_impl(void *type,
                                        const axis2_env_t *env);
                                        
axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_type_type(void *type,
                           const axis2_env_t *env); 
                                        
axis2_hash_t*  AXIS2_CALL
axis2_xml_schema_type_super_objs(void *type,
                                const axis2_env_t *env);                                                                               

void *AXIS2_CALL
axis2_xml_schema_type_get_base_schema_type(void *type,
                                            const axis2_env_t *env);

axis2_xml_schema_data_type_t *AXIS2_CALL
axis2_xml_schema_type_get_data_type(void *type,
                                        const axis2_env_t *env);

axis2_xml_schema_derivation_method_t *AXIS2_CALL
axis2_xml_schema_type_get_derive_by(void *type,
                                    const axis2_env_t *env);

axis2_xml_schema_derivation_method_t *AXIS2_CALL
axis2_xml_schema_type_get_final(void *type,
                                const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_type_set_final(void *type,
                        const axis2_env_t *env,
                        axis2_xml_schema_derivation_method_t *final_derivation);

axis2_xml_schema_derivation_method_t *AXIS2_CALL
axis2_xml_schema_type_get_final_resolved(void *type,
                                            const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
axis2_xml_schema_type_is_mixed(void *type,
                                const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_type_set_mixed(void *type,
                                const axis2_env_t *env,
                                axis2_bool_t is_mixed); 

axis2_char_t *AXIS2_CALL
axis2_xml_schema_type_get_name(void *type,
                                const axis2_env_t *env); 

axis2_status_t AXIS2_CALL
axis2_xml_schema_type_set_name(void *type,
                                const axis2_env_t *env,
                                axis2_char_t *name); 

axis2_qname_t *AXIS2_CALL
axis2_xml_schema_type_get_qname(void *type,
                                const axis2_env_t *env);
                                
/************************** end       ****************************************/                                

AXIS2_EXTERN axis2_xml_schema_type_t * AXIS2_CALL
axis2_xml_schema_type_create(const axis2_env_t *env,
                                axis2_xml_schema_t *schema)
{
    axis2_xml_schema_type_impl_t *type_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    type_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_type_impl_t));
    if(!type_impl)
    {
        AXIS2_ERROR_SET(env->error,
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    type_impl->type.base.ops = NULL;
    type_impl->name = NULL;
    type_impl->derive_by = NULL;
    type_impl->final_derivation = NULL;
    type_impl->final_resolved = NULL;
    type_impl->schema = NULL;
    type_impl->data_type = NULL;
    type_impl->methods = NULL;
    type_impl->annotated = NULL;
    type_impl->base_schema_type = NULL;
    type_impl->qname = NULL;
    type_impl->ht_super = NULL;
    type_impl->obj_type = AXIS2_XML_SCHEMA_TYPE;
    type_impl->schema = schema;

    type_impl->type.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_type_ops_t));


    if(!type_impl->type.ops)
    {
        axis2_xml_schema_type_free(&(type_impl->type), env);
        AXIS2_ERROR_SET(env->error,
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    type_impl->type.ops->free = 
            axis2_xml_schema_type_free;
    type_impl->type.ops->get_base_impl = 
            axis2_xml_schema_type_get_base_impl;
    type_impl->type.ops->type =
            axis2_xml_schema_type_type;
    type_impl->type.ops->super_objs =
            axis2_xml_schema_type_super_objs;
    type_impl->type.ops->get_base_schema_type = 
            axis2_xml_schema_type_get_base_schema_type;
    type_impl->type.ops->get_data_type = 
            axis2_xml_schema_type_get_data_type;
    type_impl->type.ops->get_derive_by = 
            axis2_xml_schema_type_get_derive_by;
    type_impl->type.ops->get_final = 
            axis2_xml_schema_type_get_final;
    type_impl->type.ops->set_final = 
            axis2_xml_schema_type_set_final;
    type_impl->type.ops->get_final_resolved = 
            axis2_xml_schema_type_get_final_resolved;
    type_impl->type.ops->is_mixed = 
            axis2_xml_schema_type_is_mixed;
    type_impl->type.ops->set_mixed = 
            axis2_xml_schema_type_set_mixed;
    type_impl->type.ops->get_name = 
            axis2_xml_schema_type_get_name;
    type_impl->type.ops->set_name = 
            axis2_xml_schema_type_set_name;
    type_impl->type.ops->get_qname = 
            axis2_xml_schema_type_get_qname;
   
    type_impl->methods = axis2_hash_make(env);
    if(!type_impl->methods)
    {
        axis2_xml_schema_type_free(&(type_impl->type), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    type_impl->final_derivation = axis2_xml_schema_derivation_method_create(env, 
            "none");
    if(!(type_impl->final_derivation))
    {
        axis2_xml_schema_type_free(&(type_impl->type), env);
        return NULL;
    }            
    
    axis2_hash_set(type_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_type_free);
    axis2_hash_set(type_impl->methods, "get_base_schema_type", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_type_get_base_schema_type);
    axis2_hash_set(type_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_type_type);
    axis2_hash_set(type_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_type_super_objs);            
    axis2_hash_set(type_impl->methods, "get_data_type", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_type_get_data_type);
    axis2_hash_set(type_impl->methods, "get_derive_by", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_type_get_derive_by);
    axis2_hash_set(type_impl->methods, "get_final", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_type_get_final);
    axis2_hash_set(type_impl->methods, "set_final", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_type_set_final);
    axis2_hash_set(type_impl->methods, "get_final_resolved", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_type_get_final_resolved);
    axis2_hash_set(type_impl->methods, "is_mixed", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_type_is_mixed);
    axis2_hash_set(type_impl->methods, "set_mixed", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_type_set_mixed);
    axis2_hash_set(type_impl->methods, "get_name", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_type_get_name);
    axis2_hash_set(type_impl->methods, "set_name", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_type_set_name);
    axis2_hash_set(type_impl->methods, "get_qname", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_type_get_qname);
    
    type_impl->annotated = axis2_xml_schema_annotated_create(env);
    if(!type_impl->annotated)
    {
        axis2_xml_schema_type_free(&(type_impl->annotated), env);
        return NULL;
    }
    type_impl->ht_super = axis2_hash_make(env);
    if(!type_impl->ht_super)
    {
        axis2_xml_schema_type_free(&(type_impl->annotated), env);
        return NULL;
    }
    axis2_hash_set(type_impl->ht_super, "AXIS2_XML_SCHEMA_TYPE", 
        AXIS2_HASH_KEY_STRING, &(type_impl->type));
        
    axis2_hash_set(type_impl->ht_super, "AXIS2_XML_SCHEMA_ANNOTATED", 
        AXIS2_HASH_KEY_STRING, type_impl->annotated);
            
    axis2_hash_set(type_impl->ht_super, "AXIS2_XML_SCHEMA_OBJ", 
        AXIS2_HASH_KEY_STRING, 
        AXIS2_XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(type_impl->annotated, env));
            
    
    status = axis2_xml_schema_annotated_resolve_methods(
            &(type_impl->type.base), env, type_impl->annotated, 
            type_impl->methods);
    
    return &(type_impl->type);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_type_free(void *type,
                                    const axis2_env_t *env)
{
    axis2_xml_schema_type_impl_t *type_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    type_impl = AXIS2_INTF_TO_IMPL(type);

    if(NULL != type_impl->methods)
    {
        axis2_hash_free(type_impl->methods, env);
        type_impl->methods = NULL;
    }

    if(NULL != type_impl->annotated)
    {
        AXIS2_XML_SCHEMA_ANNOTATED_FREE(type_impl->annotated, env);
        type_impl->annotated = NULL;
    }
    
    if(NULL != type_impl->type.ops)
    {
        AXIS2_FREE(env->allocator, type_impl->type.ops);
        type_impl->type.ops = NULL;
    }
    if(NULL != type_impl->type.base.ops)
    {
        AXIS2_FREE(env->allocator, type_impl->type.base.ops);
        type_impl->type.base.ops = NULL;
    }
    AXIS2_FREE(env->allocator, type_impl);
    type_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_type_get_base_impl(void *type,
                                const axis2_env_t *env)
{
    axis2_xml_schema_type_impl_t *type_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    type_impl = AXIS2_INTF_TO_IMPL(type);
    return type_impl->annotated;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_type_resolve_methods(
                                axis2_xml_schema_type_t *type,
                                const axis2_env_t *env,
                                axis2_xml_schema_type_t *type_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_type_impl_t *sch_type_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, type_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    
    sch_type_impl = (axis2_xml_schema_type_impl_t *) type_impl;
    
    type->ops = AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_xml_schema_type_ops_t));
    if(!type->ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;     
    }       
            
    type->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    type->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);
    type->ops->super_objs = axis2_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);                        
            

    type->ops->get_base_schema_type = axis2_hash_get(methods, "get_base_schema_type",
         AXIS2_HASH_KEY_STRING);            
    if(!type->ops->get_base_schema_type)           
        type->ops->get_base_schema_type = 
            sch_type_impl->type.ops->get_base_schema_type;

    type->ops->get_data_type = axis2_hash_get(methods, "get_data_type",
         AXIS2_HASH_KEY_STRING);            
    if(!type->ops->get_data_type)           
        type->ops->get_data_type = 
            sch_type_impl->type.ops->get_data_type;

    type->ops->get_derive_by = axis2_hash_get(methods, "get_derive_by",
         AXIS2_HASH_KEY_STRING);            
    if(!type->ops->get_derive_by)           
        type->ops->get_derive_by = 
            sch_type_impl->type.ops->get_derive_by;
            
    type->ops->get_final = axis2_hash_get(methods, "get_final",
         AXIS2_HASH_KEY_STRING);            
    if(!type->ops->get_final)         
    type->ops->get_final = 
            sch_type_impl->type.ops->get_final;
            
    type->ops->set_final = axis2_hash_get(methods, "set_final",
        AXIS2_HASH_KEY_STRING);            
    if(!type->ops->set_final)          
        type->ops->set_final = 
            sch_type_impl->type.ops->set_final;

    type->ops->get_final_resolved = axis2_hash_get(methods, "get_final_resolved",
        AXIS2_HASH_KEY_STRING);            
    if(!type->ops->get_final_resolved)            
    type->ops->get_final_resolved = 
            sch_type_impl->type.ops->get_final_resolved;
            
    type->ops->is_mixed = axis2_hash_get(methods, "is_mixed",
        AXIS2_HASH_KEY_STRING);            
    if(!type->ops->is_mixed)  
    type->ops->is_mixed = 
            sch_type_impl->type.ops->is_mixed;
     
    type->ops->set_mixed = axis2_hash_get(methods, "set_mixed",
        AXIS2_HASH_KEY_STRING);            
    if(!type->ops->set_mixed)              
    type->ops->set_mixed = 
            sch_type_impl->type.ops->set_mixed;
            
    type->ops->get_name = axis2_hash_get(methods, "get_name",
        AXIS2_HASH_KEY_STRING);            
    if(!type->ops->get_name)             
        type->ops->get_name = 
            sch_type_impl->type.ops->get_name;
            
    type->ops->set_name = axis2_hash_get(methods, "set_name",
        AXIS2_HASH_KEY_STRING);            
    if(!type->ops->set_name)             
    type->ops->set_name = 
            sch_type_impl->type.ops->set_name;
            
    type->ops->get_qname = axis2_hash_get(methods, "get_qname",
        AXIS2_HASH_KEY_STRING);            
    if(!type->ops->get_qname)            
    type->ops->get_qname = 
            sch_type_impl->type.ops->get_qname;
    
    return axis2_xml_schema_annotated_resolve_methods(&(type->base), 
            env, sch_type_impl->annotated, methods);
}

void *AXIS2_CALL
axis2_xml_schema_type_get_base_schema_type(void *type,
                                            const axis2_env_t *env)
{
    axis2_xml_schema_type_impl_t *type_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if(NULL != ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
                    "AXIS2_XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if(!type_impl)                
            return NULL;
    }
    return type_impl->base_schema_type;
}

axis2_xml_schema_data_type_t *AXIS2_CALL
axis2_xml_schema_type_get_data_type(void *type,
                                    const axis2_env_t *env)
{
    axis2_xml_schema_type_impl_t *type_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if(NULL != ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
                    "AXIS2_XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if(!type_impl)                
            return NULL;
    }
    return type_impl->data_type;
}

axis2_xml_schema_derivation_method_t *AXIS2_CALL
axis2_xml_schema_type_get_derive_by(void *type,
                                    const axis2_env_t *env)
{
    axis2_xml_schema_type_impl_t *type_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if(NULL != ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
                    "AXIS2_XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if(!type_impl)                
            return NULL;
    }
    return type_impl->derive_by;
}

axis2_xml_schema_derivation_method_t *AXIS2_CALL
axis2_xml_schema_type_get_final(void *type,
                                const axis2_env_t *env)
{
    axis2_xml_schema_type_impl_t *type_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if(NULL != ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
                    "AXIS2_XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if(!type_impl)                
            return NULL;
    }
    return type_impl->final_derivation;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_type_set_final(void *type,
                            const axis2_env_t *env,
                            axis2_xml_schema_derivation_method_t *final_derivation)
{
    axis2_xml_schema_type_impl_t *type_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, final_derivation, AXIS2_FAILURE);
    ht_super = AXIS2_XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if(NULL != ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
                    "AXIS2_XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if(!type_impl)                
            return AXIS2_FAILURE;
    }
    if(NULL != type_impl->final_derivation)
    {
        AXIS2_XML_SCHEMA_DERIVATION_METHOD_FREE(type_impl->final_derivation, env);
        type_impl->final_derivation = NULL;
    }
    type_impl->final_derivation = final_derivation;
    return AXIS2_SUCCESS;    
}

axis2_xml_schema_derivation_method_t *AXIS2_CALL
axis2_xml_schema_type_get_final_resolved(void *type,
                                            const axis2_env_t *env) 
{
    axis2_xml_schema_type_impl_t *type_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if(NULL != ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
                    "AXIS2_XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if(!type_impl)                
            return NULL;
    }
    return type_impl->final_resolved;

}

axis2_bool_t AXIS2_CALL
axis2_xml_schema_type_is_mixed(void *type,
                                const axis2_env_t *env) 
{
    axis2_xml_schema_type_impl_t *type_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = AXIS2_XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if(NULL != ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
                    "AXIS2_XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if(!type_impl)                
            return AXIS2_FAILURE;
    }
    return type_impl->is_mixed;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_type_set_mixed(void *type,
                                const axis2_env_t *env,
                                axis2_bool_t is_mixed) 
{
    axis2_xml_schema_type_impl_t *type_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = AXIS2_XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if(NULL != ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
                    "AXIS2_XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if(!type_impl)                
            return AXIS2_FAILURE;
    }
    return type_impl->is_mixed = is_mixed;
}

axis2_char_t *AXIS2_CALL
axis2_xml_schema_type_get_name(void *type,
                                const axis2_env_t *env) 
{
    axis2_xml_schema_type_impl_t *type_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if(NULL != ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
                    "AXIS2_XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if(!type_impl)                
            return NULL;
    }
    return type_impl->name;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_type_set_name(void *type,
                                const axis2_env_t *env,
                                axis2_char_t *name) 
{
    axis2_xml_schema_type_impl_t *type_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);
    ht_super = AXIS2_XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if(NULL != ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
                    "AXIS2_XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if(!type_impl)                
            return AXIS2_FAILURE;
    }
    if(NULL != type_impl->name)
    {
        AXIS2_FREE(env->allocator, type_impl->name);
        type_impl->name = NULL;
    }
    type_impl->name = AXIS2_STRDUP(name, env);
    if(!type_impl->name)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
axis2_xml_schema_type_get_qname(void *type,
                                const axis2_env_t *env) 
{
    axis2_xml_schema_type_impl_t *type_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    axis2_char_t *target_ns = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if(NULL != ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
                    "AXIS2_XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if(!type_impl)                
            return NULL;
    }

    if(!(type_impl->name))
    {
        return NULL;
    }
    if(NULL != type_impl->qname)
    {
        return type_impl->qname;
    }
    
    if(NULL != type_impl->schema)
    {
        target_ns = 
            AXIS2_XML_SCHEMA_GET_TARGET_NAMESPACE(type_impl->schema, env);
    }
    
    type_impl->qname = axis2_qname_create(env, type_impl->name, target_ns, NULL);
    return type_impl->qname;
}

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_type_type(void *type,
                           const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(type)->obj_type;
}                           
                                        
axis2_hash_t*  AXIS2_CALL
axis2_xml_schema_type_super_objs(void *type,
                                const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(type)->ht_super;
}                                