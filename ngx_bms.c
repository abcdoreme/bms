/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>
#include <ngx_bms.h>


static char *ngx_bms_block(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);


ngx_uint_t  ngx_bms_max_module;


static ngx_command_t  ngx_bms_commands[] = {

    { ngx_string("bms"),
      NGX_MAIN_CONF|NGX_CONF_BLOCK|NGX_CONF_NOARGS,
      ngx_bms_block,
      0,
      0,
      NULL },

      ngx_null_command
};


static ngx_core_module_t  ngx_bms_module_ctx = {
    ngx_string("bms"),
    NULL,
    NULL
};


ngx_module_t  ngx_bms_module = {
    NGX_MODULE_V1,
    &ngx_bms_module_ctx,                  /* module context */
    ngx_bms_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};



static char *ngx_bms_block(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char                         *rv;
    ngx_uint_t                   s, mi, m;
    ngx_conf_t                   pcf;
    //ngx_array_t                  ports;
    //ngx_bms_listen_t             *listen;
    ngx_bms_module_t             *module;
    ngx_bms_conf_ctx_t           *ctx;
    ngx_bms_core_srv_conf_t      **cscfp;
    ngx_bms_core_main_conf_t     *cmcf;

    if (*(ngx_bms_conf_ctx_t **) conf) {
        return "is duplicate";
    }

    /* the main bms context */

    ctx = ngx_pcalloc(cf->pool, sizeof(ngx_bms_conf_ctx_t));
    if (ctx == NULL) {
        return NGX_CONF_ERROR;
    }

    *(ngx_bms_conf_ctx_t **) conf = ctx;

    /* count the number of the bms modules and set up their indices */

    ngx_bms_max_module = ngx_count_modules(cf->cycle, NGX_BMS_MODULE);


    /* the bms main_conf context, it is the same in the all bms contexts */

    ctx->main_conf = ngx_pcalloc(cf->pool,
                                 sizeof(void *) * ngx_bms_max_module);
    if (ctx->main_conf == NULL) {
        return NGX_CONF_ERROR;
    }

    /*
     * the bms null srv_conf context, it is used to merge
     * the server{}s' srv_conf's
     */

    ctx->srv_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_bms_max_module);
    if (ctx->srv_conf == NULL) {
        return NGX_CONF_ERROR;
    }

    /*
     * create the main_conf's and the null srv_conf's of the all bms modules
     */

    for (m = 0; cf->cycle->modules[m]; m++) {
        if (cf->cycle->modules[m]->type != NGX_BMS_MODULE) {
            continue;
        }

        module = cf->cycle->modules[m]->ctx;
        mi = cf->cycle->modules[m]->ctx_index;

        if (module->create_main_conf) {
            ctx->main_conf[mi] = module->create_main_conf(cf);
            if (ctx->main_conf[mi] == NULL) {
                return NGX_CONF_ERROR;
            }
        }

        if (module->create_srv_conf) {
            ctx->srv_conf[mi] = module->create_srv_conf(cf);
            if (ctx->srv_conf[mi] == NULL) {
                return NGX_CONF_ERROR;
            }
        }
    }

    /* init bms{} main_conf's, merge the server{}s' srv_conf's */

    cmcf = ctx->main_conf[ngx_bms_core_module.ctx_index];
    cscfp = cmcf->servers.elts;

    for (m = 0; cf->cycle->modules[m]; m++) {
        if (cf->cycle->modules[m]->type != NGX_BMS_MODULE) {
            continue;
        }

        module = cf->cycle->modules[m]->ctx;
        mi = cf->cycle->modules[m]->ctx_index;

        /* init bms{} main_conf's */

        cf->ctx = ctx;

        if (module->init_main_conf) {
            rv = module->init_main_conf(cf, ctx->main_conf[mi]);
            if (rv != NGX_CONF_OK) {
                *cf = pcf;
                return rv;
            }
        }

        for (s = 0; s < cmcf->servers.nelts; s++) {

            /* merge the server{}s' srv_conf's */

            cf->ctx = cscfp[s]->ctx;

            if (module->merge_srv_conf) {
                rv = module->merge_srv_conf(cf,
                                            ctx->srv_conf[mi],
                                            cscfp[s]->ctx->srv_conf[mi]);
                if (rv != NGX_CONF_OK) {
                    *cf = pcf;
                    return rv;
                }
            }
        }
    }

    *cf = pcf;

    return "bms";
}

