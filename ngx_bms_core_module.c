/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>
#include <ngx_bms.h>


static void *ngx_bms_core_create_main_conf(ngx_conf_t *cf);
static void *ngx_bms_core_create_srv_conf(ngx_conf_t *cf);
static char *ngx_bms_core_merge_srv_conf(ngx_conf_t *cf, void *parent, void *child);
static char *ngx_bms_core_server(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char *ngx_bms_core_listen(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_command_t  ngx_bms_core_commands[] = {

    { ngx_string("server"),
      NGX_BMS_MAIN_CONF|NGX_CONF_BLOCK|NGX_CONF_NOARGS,
      ngx_bms_core_server,
      0,
      0,
      NULL },

    { ngx_string("listen"),
      NGX_BMS_SRV_CONF|NGX_CONF_1MORE,
      ngx_bms_core_listen,
      NGX_BMS_SRV_CONF_OFFSET,
      0,
      NULL },

    ngx_null_command
};

static ngx_bms_module_t  ngx_bms_core_module_ctx = {
    //NULL,                                  /* protocol */

    ngx_bms_core_create_main_conf,        /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_bms_core_create_srv_conf,         /* create server configuration */
    ngx_bms_core_merge_srv_conf           /* merge server configuration */
};


ngx_module_t  ngx_bms_core_module = {
    NGX_MODULE_V1,
    &ngx_bms_core_module_ctx,             /* module context */
    ngx_bms_core_commands,                /* module directives */
    NGX_BMS_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


static void *ngx_bms_core_create_main_conf(ngx_conf_t *cf)
{
    return NULL;
}

static void *ngx_bms_core_create_srv_conf(ngx_conf_t *cf)
{
    return NULL;
}

static char *ngx_bms_core_merge_srv_conf(ngx_conf_t *cf, void *parent, void *child)
{
    return NULL;
}

static char *ngx_bms_core_server(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    return NULL;
}

static char *ngx_bms_core_listen(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    return NULL;
}
