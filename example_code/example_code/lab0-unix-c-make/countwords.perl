#!/usr/bin/perl
# $Id: countwords.perl,v 1.1 2020-01-02 14:32:14-08 - - $
map {++$words{lc $_}} m/([[:alpha:]]+)/g while <>;
print "$_ $words{$_}\n" for sort keys %words;
