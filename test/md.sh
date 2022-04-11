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

for md in md5 sha256; do
	hash_label="${md^^}"

	str="42 is nice"
	orig=$(echo "$str" | openssl $md)
	mine=$(echo "$str" | ./ft_ssl $md)
	test_result 1 "$orig" "$mine"
	
	orig="(\"$str\")= "$(echo $str | openssl $md | awk '{print $2}')
	mine=$(echo "$str" | ./ft_ssl $md -p)
	test_result 2 "$orig" "$mine"
	
	str="Pity the living."
	orig=$(echo "$str" | openssl $md | awk '{print $2}')
	mine=$(echo "$str" | ./ft_ssl $md -q -r)
	test_result 3 "$orig" "$mine"
	
	echo "And above all," > file
	orig="$hash_label (file) = $(openssl $md file | awk '{print $2}')"
	mine=$(./ft_ssl $md file)
	test_result 4 "$orig" "$mine"

	orig="$(openssl $md file | awk '{print $2}') file"
	mine=$(./ft_ssl $md -r file)
	test_result 5 "$orig" "$mine"
	
	str="pity those that aren't following baerista on spotify."
	mine=$(./ft_ssl $md -s "$str")
	orig="$hash_label (\"$str\") = "$(echo -n "$str" | openssl $md | awk '{print $2}')
	test_result 6 "$orig" "$mine"
	
	str="be sure to handle edge cases carefully"
	str_result="(\"$str\")= "$(echo $str | openssl $md | awk '{print $2}')
	file_result=$(openssl $md file | awk '{print $2}')
	orig=$(echo $str_result && echo $hash_label \(file\) \= $file_result)
	mine=$(echo "$str" | ./ft_ssl $md -p file)
	test_result 7 "$orig" "$mine"
	
	str="some of this will not make sense at first"
	orig="$hash_label (file) = $(openssl $md file | awk '{print $2}')"
	mine=$(echo $str | ./ft_ssl $md file)
	test_result 8 "$orig" "$mine"
	
	str="but eventually you will understand"
	str_result="(\"$str\")= "$(echo $str | openssl $md | awk '{print $2}')
	file_result=$(openssl $md file | awk '{print $2}')
	orig=$(echo $str_result && echo $file_result file)
	mine=$(echo $str | ./ft_ssl $md -p -r file)
	test_result 9 "$orig" "$mine"
	
	str="GL HF let's go"
	mine=$(echo "$str" | ./ft_ssl $md -p -s "foo" file)
	stdin_result="(\"$str\")= "$(echo $str | openssl $md | awk '{print $2}')
	str_result=$(echo -n "foo" | openssl $md | awk '{print $2}')
	file_result=$(openssl $md file | awk '{print $2}')
	orig=$(echo $stdin_result \
		&& echo $hash_label \(\"foo\"\) \= $str_result \
		&& echo $hash_label \(file\) = $file_result)
	test_result 10 "$orig" "$mine"
	
	str="one more thing"
	mine=$(echo "$str" | ./ft_ssl $md -r -p -s "foo" file -s "bar")
	stdin_result="(\"$str\")= "$(echo $str | openssl $md | awk '{print $2}')
	str_result=$(echo -n "foo" | openssl $md | awk '{print $2}')
	file_result=$(openssl $md file | awk '{print $2}')
	orig=$(echo $stdin_result \
		&& echo $str_result \"foo\" \
		&& echo $file_result file)
	test_result 11 "$orig" "$mine"
	
	str="just to be extra clear"
	mine=$(echo "$str" | ./ft_ssl $md -r -q -p -s "foo" file)
	stdin_result=$(echo "$str" | openssl $md | awk '{print $2}')
	str_result=$(echo -n "foo" | openssl $md | awk '{print $2}')
	file_result=$(openssl $md file | awk '{print $2}')
	orig=$(echo $str && echo $stdin_result \
		&& echo $str_result && echo $file_result)
	test_result 12 "$orig" "$mine"
	
	echo "https://www.42.fr/" > website
	orig=$(openssl $md website | awk '{print $2}')
	mine=$(./ft_ssl $md -q website)
	test_result 13 "$orig" "$mine"
	
	str="42 is nice"
	orig="$hash_label (\"$str\") = "$(echo -n "$str" | openssl $md | awk '{print $2}')
	mine=$(./ft_ssl $md -s "$str")
	test_result 14 "$orig" "$mine"
done
