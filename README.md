# ft\_ssl

This program implements functions related to cryptographic security. It takes
data, it hashes it, passes it through a cypher or an encoding function and
returns the result.

<br />
<p align="center">
  <img src="https://github.com/Taiwing/ft_ssl/blob/master/resources/cipher.jpg?raw=true" alt="cipher" style="width: 50%;"/>
</p>

## Setup

```shell
# clone it with the libft submodule
git clone --recurse-submodules https://github.com/Taiwing/ft_ssl
# build it
cd ft_ssl/ && make
# run it
./ft_ssl
```

## Message Digest commands

These commands are hashing algorithms. They implement a one-way compression
function. This means that it is difficult to compute an input from a given
output of the function. Ideally they would be collision-proof, meaning that it
would be extremely rare for two inputs to have the same output, but md5 and
sha256 are known to be broken in that regard. Nowadays they are mostly used to
check file integrity as they are fast and cheap to execute.

### md5

The md5 algorithm takes an arbitrary long input and turns it into a 128bit hash.

#### usage:

```shell
# get some help
./ft_ssl md5 -help
```

```
Usage: md5 [options] [file...]
	-p		echo STDIN to STDOUT and append the checksum to STDOUT
	-q		quiet mode
	-r		reverse the format of the output
	-s val		print the sum of the given string
	-help		print this
```

#### example:

```shell
# run md5 hash on Makefile
./ft_ssl md5 Makefile
# prints --> MD5 (Makefile) = 78f0a46e4140abb6d12afccf3b68bbc7
```

### sha256

The sha256 algorithm takes an arbitrary long input and turns it into a 256bit
hash.

#### usage:

```shell
# get some help
./ft_ssl sha256 -help
```

```
Usage: sha256 [options] [file...]
	-p		echo STDIN to STDOUT and append the checksum to STDOUT
	-q		quiet mode
	-r		reverse the format of the output
	-s val		print the sum of the given string
	-help		print this
```

#### example:

```shell
# run sha256 hash with a string
./ft_ssl sha256 -q -s 'Hello sha256!'
# prints --> 5b83d7b46ad7678f9fdd416309e9ff324edb30e424a409f751ebe39c313b05c5
```
