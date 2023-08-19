#ifndef _NGX_BMS_H_INCLUDED_
#define _NGX_BMS_H_INCLUDED_

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>
#include <ngx_event_connect.h>





typedef struct {
    void                  **main_conf;
    void                  **srv_conf;
} ngx_bms_conf_ctx_t;


typedef struct {
    struct sockaddr        *sockaddr;
    socklen_t               socklen;
    ngx_str_t               addr_text;

    /* server ctx */
    ngx_bms_conf_ctx_t    *ctx;

#if (NGX_HAVE_KEEPALIVE_TUNABLE)
    int                     tcp_keepidle;
    int                     tcp_keepintvl;
    int                     tcp_keepcnt;
#endif
    int                     backlog;
    int                     rcvbuf;
    int                     sndbuf;
} ngx_bms_listen_t;

typedef struct {
    ngx_array_t             servers;     /* ngx_bms_core_srv_conf_t */
    ngx_array_t             listen;      /* ngx_bms_listen_t */
} ngx_bms_core_main_conf_t;


typedef struct {
    ngx_msec_t              timeout;
    ngx_msec_t              resolver_timeout;

    ngx_uint_t              max_errors;

    ngx_str_t               server_name;

    u_char                 *file_name;
    ngx_uint_t              line;

    ngx_resolver_t         *resolver;
    ngx_log_t              *error_log;

    /* server ctx */
    ngx_bms_conf_ctx_t    *ctx;

    ngx_uint_t              listen;  /* unsigned  listen:1; */
} ngx_bms_core_srv_conf_t;


typedef struct {
    //ngx_mail_protocol_t        *protocol;

    void                       *(*create_main_conf)(ngx_conf_t *cf);
    char                       *(*init_main_conf)(ngx_conf_t *cf, void *conf);

    void                       *(*create_srv_conf)(ngx_conf_t *cf);
    char                       *(*merge_srv_conf)(ngx_conf_t *cf, void *prev,
                                                  void *conf);
} ngx_bms_module_t;

#define NGX_BMS_MODULE         0x00534D42    /* "BMS" */

#define NGX_BMS_MAIN_CONF      0x20000000
#define NGX_BMS_SRV_CONF       0x40000000

#define NGX_BMS_MAIN_CONF_OFFSET  offsetof(ngx_bms_conf_ctx_t, main_conf)
#define NGX_BMS_SRV_CONF_OFFSET   offsetof(ngx_bms_conf_ctx_t, srv_conf)

extern ngx_uint_t    ngx_bms_max_module;
extern ngx_module_t  ngx_bms_core_module;

#endif