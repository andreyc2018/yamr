#!/usr/bin/env python
"""
https://codereview.stackexchange.com/a/58273
"""

import random
import string
import sys
import argparse

domains = [ "hotmail.com", "gmail.com", "aol.com", "mail.com" , "mail.kz", "yahoo.com"]
letters = string.ascii_lowercase[:12]


def get_random_domain(domains):
    return random.choice(domains)


def get_random_name(letters, length):
    return ''.join(random.choice(letters) for i in range(length))


def generate_random_emails(num_addr, max_len, min_len):
    return [get_random_name(letters, random.randint(min_len, max_len)) + '@' + get_random_domain(domains) for i in range(num_addr)]


def main(num_addr, max_len, min_len):
    for address in generate_random_emails(num_addr, max_len, min_len):
        print(address)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generate email addresses')
    parser.add_argument('-n', '--num', type=int, default=10, help='Number of the addresses. Default: 10')
    parser.add_argument('-l', '--max', type=int, default=7, help='Maximum address length. Default: 7')
    parser.add_argument('-m', '--min', type=int, default=1, help='Minimum address length. Default: 1')
    opts = parser.parse_args()
    main(opts.num, opts.max, opts.min)

