#!/usr/bin/env bash

function test_result() {
	local id=$1
	local orig=$2
	local mine=$3

	echo
	echo ====== TEST $id \(expected\) ======
	echo $orig
	echo ===================================
	if [ "$orig" != "$mine" ]; then
		echo ====== FAIL $id \(got\) ======
		echo $mine
		echo ==============================
		exit 1
	fi
	echo "PASS"
	echo
}

str="toto"
orig=$(echo $str | openssl base64 -e)
mine=$(echo $str | ./ft_ssl base64 -e)
test_result 1 "$orig" "$mine"

str="dG90bwo="
orig=$(echo $str | openssl base64 -d)
mine=$(echo $str | ./ft_ssl base64 -d)
test_result 2 "$orig" "$mine"

str="d G90 bwo  ="
mine=$(echo "$str" | ./ft_ssl base64 -d)
test_result 3 "$orig" "$mine"

str="foobar"
mine=$(echo "$str" | ./ft_ssl base64 -e)
orig=$(echo "$str" | openssl base64 -e)
test_result 4 "$orig" "$mine"

str="Zm9vYmFyCg=="
mine=$(echo "$str" | ./ft_ssl base64 -d)
orig=$(echo "$str" | openssl base64 -d)
test_result 5 "$orig" "$mine"

orig="one deep secret ecb"
key="6162636461626364"
iv="0011223344556677"
mine=$(echo $orig | ./ft_ssl des-ecb -a -k $key -v $iv \
	| openssl des-ecb -d -a -K $key -iv $iv)
test_result 6 "$orig" "$mine"

orig="one deep secret cbc"
mine=$(echo $orig | ./ft_ssl des-cbc -a -k $key -v $iv \
	| openssl des-cbc -d -a -K $key -iv $iv)
test_result 7 "$orig" "$mine"

orig="one deep secret cfb"
mine=$(echo $orig | ./ft_ssl des-cfb -a -k $key -v $iv \
	| openssl des-cfb -d -a -K $key -iv $iv)
test_result 8 "$orig" "$mine"

orig="one deep secret ofb"
mine=$(echo $orig | ./ft_ssl des-ofb -a -k $key -v $iv \
	| openssl des-ofb -d -a -K $key -iv $iv)
test_result 9 "$orig" "$mine"
