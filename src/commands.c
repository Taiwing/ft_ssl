#include "commands.h"
#include "options.h"

const t_command	g_command[] = {
	{ "md5", NULL, g_mdc_options, g_mdc_usage, g_mdc_help },
	{ "sha256", NULL, g_mdc_options, g_mdc_usage, g_mdc_help },
	{ "base64", NULL, g_base64_options, g_base64_usage, g_base64_help },
	{ "des", NULL, g_cc_options, g_cc_usage, g_cc_help },
	{ "des-ecb", NULL, g_cc_options, g_cc_usage, g_cc_help },
	{ "des-cbc", NULL, g_cc_options, g_cc_usage, g_cc_help },
	{ "genrsa", NULL, g_genrsa_options, g_genrsa_usage, g_genrsa_help },
	{ "rsa", NULL, g_rsa_options, g_rsa_usage, g_rsa_help },
	{ "rsautl", NULL, g_rsautl_options, g_rsautl_usage, g_rsautl_help },
	{ NULL, NULL, NULL, NULL, NULL }
};
