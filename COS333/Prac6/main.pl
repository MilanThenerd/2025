#!/usr/bin/perl

use strict;
use warnings;

if (@ARGV != 2) {
    die "Usage: $0 <input_file> <name_count>\n";
}

my ($input_file, $name_count) = @ARGV;

unless ($name_count =~ /^\d+$/) {
    die "Second parameter must be a non-negative integer\n";
}

open(my $fh, '<', $input_file) or die "Cannot open file $input_file: $!";

my @matching_students;

while (my $line = <$fh>) {
    chomp $line;
    next if $line eq '';

    if ($line =~ /^(\d+),\s*(.+)$/) {
        my ($student_number, $full_name) = ($1, $2);

        my @name_parts = split(/\s+/, $full_name);
        my $last_name = pop @name_parts;

        if (@name_parts == $name_count) {
            push @matching_students, {
                student_number => $student_number,
                last_name => $last_name
            };
        }
    }
}

close($fh);

my $result;
if (@matching_students == 0) {
    $result = "None found";
} elsif (@matching_students == 1) {
    $result = $matching_students[0]{student_number};
} else {
    @matching_students = sort { $a->{last_name} cmp $b->{last_name} } @matching_students;
    $result = $matching_students[0]{student_number};
}

print "$result\n";