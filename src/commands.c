#include "commands.h"
#include "options.h"

const t_command	g_command[] = {
	{ "md5", NULL, g_mdc_options, g_mdc_usage, g_mdc_help, MDC },
	{ "sha256", NULL, g_mdc_options, g_mdc_usage, g_mdc_help, MDC },
	{ "base64", NULL, g_base64_options, g_base64_usage, g_base64_help, CIC },
	{ "des", NULL, g_cc_options, g_cc_usage, g_cc_help, CIC },
	{ "des-ecb", NULL, g_cc_options, g_cc_usage, g_cc_help, CIC },
	{ "des-cbc", NULL, g_cc_options, g_cc_usage, g_cc_help, CIC },
	{ "genrsa", NULL, g_genrsa_options, g_genrsa_usage, g_genrsa_help, STC },
	{ "rsa", NULL, g_rsa_options, g_rsa_usage, g_rsa_help, STC },
	{ "rsautl", NULL, g_rsautl_options, g_rsautl_usage, g_rsautl_help, STC },
	{ NULL, NULL, NULL, NULL, NULL, NONE }
};
