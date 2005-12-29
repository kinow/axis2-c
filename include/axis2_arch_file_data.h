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

#ifndef AXIS2_ARCH_FILE_DATA_H
#define AXIS2_ARCH_FILE_DATA_H

/**
 * @file axis2_arch_file_data.h
 * @brief Axis2 Arch File Data interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_conf.h>
#include <axis2_array_list.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_conf;
    
typedef struct axis2_arch_file_data axis2_arch_file_data_t;
typedef struct axis2_arch_file_data_ops axis2_arch_file_data_ops_t;

/** @defgroup axis2_arch_file_data Arch File Data
 * @ingroup axis2_deployment
 * @{
 */

/** 
 * @brief Arch File Data ops struct
 * Encapsulator struct for ops of axis2_arch_file_data
 */
AXIS2_DECLARE_DATA struct axis2_arch_file_data_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free)(axis2_arch_file_data_t *arch_file_data,
	        axis2_env_t **env);
    
    axis2_char_t *(AXIS2_CALL *
    get_msg_recv)(axis2_arch_file_data_t *file_data,
                                        axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_msg_recv)(axis2_arch_file_data_t *file_data,
                                        axis2_env_t **env,
                                        axis2_char_t *msg_recv);
    
    axis2_char_t *(AXIS2_CALL *
    get_name)(axis2_arch_file_data_t *file_data,
                                        axis2_env_t **env);
    
    axis2_char_t *(AXIS2_CALL *
    get_svc_name)(axis2_arch_file_data_t *file_data,
                                        axis2_env_t **env);
    
    int (AXIS2_CALL *
    get_type)(axis2_arch_file_data_t *file_data,
                                        axis2_env_t **env);
    
    axis2_char_t *(AXIS2_CALL *
    get_file_name)(axis2_arch_file_data_t *file_data,
                                        axis2_env_t **env);
    
    axis2_char_t *(AXIS2_CALL *
    get_module_dll_name)(axis2_arch_file_data_t *file_data,
                                                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_module_dll_name)(axis2_arch_file_data_t *file_data,
                                        axis2_env_t **env,
                                        axis2_char_t *module_dll_name);
    axis2_status_t (AXIS2_CALL *
    add_svc)(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env,
                                    struct axis2_svc *svc_desc);
    
    struct axis2_svc *(AXIS2_CALL *
    get_svc)(axis2_arch_file_data_t *file_data,
                                        axis2_env_t **env,
                                        axis2_char_t *svc_name);
    
    axis2_hash_t *(AXIS2_CALL *
    get_svc_map)(axis2_arch_file_data_t *file_data,
                                        axis2_env_t **env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_deployable_svcs)(axis2_arch_file_data_t *file_data,
                                                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_deployable_svcs) (axis2_arch_file_data_t *file_data,
                                        axis2_env_t **env,
                                        axis2_array_list_t *deployable_svcs);    
 

};

/** 
 * @brief Arch File Data struct 
 */  
AXIS2_DECLARE_DATA struct axis2_arch_file_data
{
	axis2_arch_file_data_ops_t *ops;
};

/**
 * Creates arch file data struct
 * @param repos_path
 * @return pointer to newly created arch file data
 */
AXIS2_DECLARE(axis2_arch_file_data_t *) 
axis2_arch_file_data_create_with_repos_name (
                                        axis2_env_t **env, 
                                        axis2_char_t *repos_path);

/*************************** Function macros **********************************/

#define AXIS2_ARCH_FILE_DATA_FREE(arch_file_data, env) \
		((arch_file_data->ops)->free (arch_file_data, env))

#define AXIS2_ARCH_FILE_DATA_GET_MSG_RECV(arch_file_data, env) \
		((arch_file_data->ops)->get_msg_recv (arch_file_data, env))  

#define AXIS2_ARCH_FILE_DATA_SET_MSG_RECV(arch_file_data, env, msg_recv) \
		((arch_file_data->ops)->set_msg_recv (arch_file_data, env, msg_recv))  
        
#define AXIS2_ARCH_FILE_DATA_GET_NAME(arch_file_data, env) \
		((arch_file_data->ops)->get_name (arch_file_data, env))          

#define AXIS2_ARCH_FILE_DATA_GET_SVC_NAME(arch_file_data, env) \
		((arch_file_data->ops)->get_svc_name (arch_file_data, env)) 

#define AXIS2_ARCH_FILE_DATA_GET_TYPE(arch_file_data, env) \
		((arch_file_data->ops)->get_type (arch_file_data, env)) 
        
#define AXIS2_ARCH_FILE_DATA_GET_FILE_NAME(arch_file_data, env) \
		((arch_file_data->ops)->get_file_name (arch_file_data, env)) 

#define AXIS2_ARCH_FILE_DATA_GET_MODULE_DLL_NAME(arch_file_data, env) \
		((arch_file_data->ops)->get_module_dll_name (arch_file_data, env)) 
        
#define AXIS2_ARCH_FILE_DATA_SET_MODULE_DLL_NAME(arch_file_data, env, module_dll_name) \
		((arch_file_data->ops)->set_module_dll_name (arch_file_data, env, module_dll_name)) 

#define AXIS2_ARCH_FILE_DATA_ADD_SVC(arch_file_data, env, svc_desc) \
		((arch_file_data->ops)->add_svc (arch_file_data, env, svc_desc)) 
        
#define AXIS2_ARCH_FILE_DATA_GET_SVC(arch_file_data, env, svc_name) \
		((arch_file_data->ops)->get_svc (arch_file_data, env, svc_name)) 

#define AXIS2_ARCH_FILE_DATA_GET_SVC_MAP(arch_file_data, env) \
		((arch_file_data->ops)->get_svc_map (arch_file_data, env)) 
        
#define AXIS2_ARCH_FILE_DATA_GET_DEPLOYABLE_SVCS(arch_file_data, env) \
		((arch_file_data->ops)->get_deployable_svcs (arch_file_data, env)) 

#define AXIS2_ARCH_FILE_DATA_SET_DEPLOYABLE_SVCS(arch_file_data, env, deployable_svcs) \
		((arch_file_data->ops)->set_deployable_svcs (arch_file_data, env, deployable_svcs)) 
        
/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_ARCH_FILE_DATA_H */
