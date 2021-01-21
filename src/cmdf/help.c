/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 12:27:53 by yforeau           #+#    #+#             */
/*   Updated: 2020/12/30 17:45:35 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

const char g_mdc_help[] = "[options] [file...]\n"
"\t-p\t\t\techo STDIN to STDOUT and append the checksum to STDOUT\n"
"\t-q\t\t\tquiet mode\n"
"\t-r\t\t\treverse the format of the output\n"
"\t-s val\t\t\tprint the sum of the given string\n"
"\t-help\t\t\tprint this\n";

const char g_base64_help[] = "[options]\n"
"\t-d\t\t\tdecode mode\n"
"\t-e\t\t\tencode mode (default)\n"
"\t-i file\t\t\tinput file\n"
"\t-o file\t\t\toutput file\n"
"\t-help\t\t\tprint this\n";

const char g_cc_help[] = "[options]\n"
"\t-a\t\t\tdecode/encode the input/output in base64 depending on the encrypt mode\n"
"\t-d\t\t\tdecrypt mode\n"
"\t-e\t\t\tencypt mode (default)\n"
"\t-i file\t\t\tinput file for message\n"
"\t-k val\t\t\tkey in hex is the next arguement\n"
"\t-o file\t\t\toutput file for message\n"
"\t-p val\t\t\tpassword in ascii is the next argument\n"
"\t-s val\t\t\tthe salt in hex is the next argument\n"
"\t-v val\t\t\tinitialization vector in hex is the next argument\n"
"\t-help\t\t\tprint this\n";

const char g_genrsa_help[] = "[options]\n"
"\t-i file\t\t\tno idea what this does (maybe it's supposed to be -rand...)\n"
"\t-o file\t\t\toutput the key to specified file\n"
"\t-help\t\t\tprint this\n";

const char g_rsa_help[] = "[options]\n"
"\t-inform val\t\t\tinput format, one of DER PEM\n"
"\t-outform val\t\t\toutput format, one of DER PEM PVK\n"
"\t-in file\t\t\tinput file\n"
"\t-passin val\t\t\tinput file pass phrase source\n"
"\t-out file\t\t\toutput file\n"
"\t-passout val\t\t\toutput file pass phrase source\n"
"\t-des\t\t\tuse des as a cypher\n"
"\t-text\t\t\tprint the key in text\n"
"\t-noout\t\t\tdon't print key out\n"
"\t-modulus\t\t\tprint the RSA key modulus\n"
"\t-check\t\t\tverify key consistency\n"
"\t-pubin\t\t\texpect a public key in input file\n"
"\t-pubout\t\t\toutput a public key\n"
"\t-help\t\t\tprint this\n";

const char g_rsautl_help[] = "[options]\n"
"\t-in file\t\t\tinput file\n"
"\t-out file\t\t\toutput file\n"
"\t-inkey val\t\t\tinput key\n"
"\t-pubin\t\t\tinput is an RSA public\n"
"\t-encrypt\t\t\tencrypt with public key\n"
"\t-decrypt\t\t\tdecrypt with private key\n"
"\t-hexdump\t\t\thex dump output\n"
"\t-help\t\t\tprint this\n";
