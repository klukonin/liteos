#!/usr/bin/perl -w
use strict;

my %G_opts = ( sortby => "name", verbose => 0 );

my @opts = @ARGV; @ARGV = ();
while (@opts) {
  my $opt = shift @opts;
  if( $opt !~ /^-/ ) { push( @ARGV, $opt ); }
  elsif( $opt eq "-v" ) { $G_opts{verbose} = 1; }
  elsif( $opt eq "-n" ) { $G_opts{sortby} = "name"; }
  elsif( $opt eq "-s" ) { $G_opts{sortby} = "size"; }
  else { die "Unknown command line opion $opt\n"; }
}

my $SortKeys = $G_opts{sortby} eq "name" ? \&keysbyname : \&keysbysize;

if( @ARGV < 1 ) {
  print "usage: module_ram_usage (-v|-s|-n) [exe] (module) (module) (...)\n";
  exit 0;
}

my $exe = shift @ARGV;
my %modules = map { $_ => 1 } @ARGV;

my %total = ();
my %vars = ();

my $cmd = "avr-objdump -x \"$exe\"";
open( FH, "$cmd |" ) or die "ERROR, $cmd failed: $!\n";

while( <FH> ) {
  if( /^(\S+)\s+\w+\s+\w+\s+\.bss\s+(\S+)\s+([a-zA-Z_]+\S+)/ ) {
    print "$1  $2  $3\n"
    
    }
  }

close( FH );



sub keysbyname {
  my $h = shift;
  return sort keys %$h;
}

sub keysbysize {
  my $h = shift;
  return sort {$h->{$a} <=> $h->{$b} || $a cmp $b} keys %$h;
}

