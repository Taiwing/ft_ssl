/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 12:27:53 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/22 19:52:10 by yforeau          ###   ########.fr       */
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
"\t-a\t\t\tdecode/encode in base64 depending on the encrypt mode\n"
"\t-d\t\t\tdecrypt mode\n"
"\t-e\t\t\tencypt mode (default)\n"
"\t-i file\t\t\tinput file for message\n"
"\t-k val\t\t\tkey in hex is the next arguement\n"
"\t-o file\t\t\toutput file for message\n"
"\t-p val\t\t\tpassword in ascii is the next argument\n"
"\t-s val\t\t\tthe salt in hex is the next argument\n"
"\t-v val\t\t\tinitialization vector in hex is the next argument\n"
"\t-help\t\t\tprint this\n";

const char g_genrsa_help[] = "[options] [keysize]\n"
"\t-i file\t\t\tload the file into the random number generator\n"
"\t-o file\t\t\toutput the key to specified file\n"
"\t-des\t\t\tencrypt the output with DES-CBC\n"
"\t-passout val\t\toutput file pass phrase source\n"
"\t-help\t\t\tprint this\n"
"\n\tkeysize: The length in bits of the key to be generated.\n"
"\tEvery value from 3 to 64 (inclusive) is valid. keysize\n"
"\twill be equal to 64 if it is not given.\n";

const char g_rsa_help[] = "[options]\n"
"\t-inform val\t\tinput format - PEM (already default)\n"
"\t-outform val\t\toutput format - PEM (already default)\n"
"\t-in file\t\tinput file\n"
"\t-passin val\t\tinput file pass phrase source\n"
"\t-out file\t\toutput file\n"
"\t-passout val\t\toutput file pass phrase source\n"
"\t-des\t\t\tuse des as a cypher\n"
"\t-text\t\t\tprint the key in text\n"
"\t-noout\t\t\tdon't print key out\n"
"\t-modulus\t\tprint the RSA key modulus\n"
"\t-check\t\t\tverify key consistency\n"
"\t-pubin\t\t\texpect a public key in input file\n"
"\t-pubout\t\t\toutput a public key\n"
"\t-help\t\t\tprint this\n"
"\n"
"Manually modify the output key:\n"
"\n"
"\tThese options allow the user to set arbirary values\n"
"\tfor the output key. This works with private and public\n"
"\tkeys. However it will not have any effect to modify\n"
"\ta private key only value for a public key output.\n"
"\tValues are in hexadecimal without prefix and case\n"
"\tagnostic (example: F86cE09dDe).\n"
"\n"
"\t-n\thex\t\tset modulus\n"
"\t-e\thex\t\tset public exponent\n"
"\t-d\thex\t\tset private exponent\n"
"\t-p\thex\t\tset first prime\n"
"\t-q\thex\t\tset second prime\n"
"\t-exp1\thex\t\tset exponent1\n"
"\t-exp2\thex\t\tset exponent2\n"
"\t-coeff\thex\t\tset coefficient\n";

const char g_rsautl_help[] = "[options]\n"
"\t-in file\t\tinput file\n"
"\t-out file\t\toutput file\n"
"\t-inkey val\t\tinput key\n"
"\t-pubin\t\t\tinput is an RSA public\n"
"\t-encrypt\t\tencrypt with public key\n"
"\t-decrypt\t\tdecrypt with private key\n"
"\t-hexdump\t\thex dump output\n"
"\t-raw\t\t\tno padding (already default)\n"
"\t-passin val\t\tinput file pass phrase source\n"
"\t-help\t\t\tprint this\n";
