#!/usr/bin/perl
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/conv_rel_branch.pl $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied. See the License for the specific language governing
# permissions and limitations under the License.
#
# IBM_PROLOG_END_TAG

use strict;
use warnings;
use Data::Dumper;
use Getopt::Long qw(:config pass_through);

# Globals
my %power_platforms = ();
my %relations = ();
my $latest_power_platform = "";
my $fsp_ci_jenkins_rel_file = "/gsa/ausgsa/home/f/s/fspcibld/patches/cipatch_xml";
use constant MASTER => "master";
my $debug = 0;
my $help = 0;

# Set local variables based on ENV variables
my $PROJECT_ROOT = `git rev-parse --show-toplevel`;

# Parse command line parameters
GetOptions("debug!" => \$debug,
           "help" => \$help);

# List of commands tool can run
my %commands = ( "rtob" => \&execute_rel_to_branch,
                 "btor" => \&execute_branch_to_rel,
               );

if ($help)
{
    execute_help();
}
else
{
    my $command = shift @ARGV;
    if ($commands{$command})
    {
        system("git remote -v | grep gerrit -q");
        die "Gerrit remote DNE, must run in repo with gerrit remote" if $?;
        # Build release to branch relation hash.
        build_relations();
        &{$commands{$command}}();
    }
    else
    {
        execute_help();
    }
}

############################## Begin Actions ###################################

sub execute_rel_to_branch
{
    my $release = "";

    GetOptions("release:s" => \$release);
    die "Missing release" if $release eq "";

    # Get power platform associated with release
    my $power_platform = get_power_platform($release);
    # Find release in relations hash.
    my $branch = $relations{$power_platform}{$release};
    die "Fips release => $release has no corresponding gerrit branch" if (!$branch);
    print "$branch \n";
}

sub execute_branch_to_rel
{
    my $branch = "";

    GetOptions("branch:s" => \$branch);
    die "Missing branch" if $branch eq "";

    # Get power platform associated with branch
    my $power_platform = get_power_platform($branch);

    # Find branch in relations hash.
    my $release = "";
    if( $power_platform )
    {
        foreach my $rel (sort keys %{$relations{$power_platform}})
        {
            if ($relations{$power_platform}{$rel} eq "$branch")
            {
                $release = $rel;
                last;
            }
        }
    }
    die "Gerrit branch => $branch has no corresponding fips release" if ($release eq "");
    print "$release \n";
}

sub execute_help
{
    my $command = shift @ARGV;

    if ($command eq "")
    {
        print "  Usage:\n";
        print "      conv_rel_branch <subtool> [options]\n\n";
        print "  Tool to convert fips release to branches and vice versa\n\n";
        print "  Requires:\n";
        print "      Tool to run in git repo that has gerrit remote\n\n";
        print "  Available subtools:\n";
        foreach my $key (sort keys %commands)
        {
            print "      $key\n";
        }
        print "\n";
        print "  Global options:\n";
        print "      --debug     Enable debug mode.\n";
        print "      --help      Display help on a specific tool.\n";
    }
    elsif (not defined $commands{$command})
    {
        die "Unknown subcommand: $command.\n";
    }
    else
    {
        my %help = (
        "rtob" =>
q(
  Convert release to branch

  Options:
    --release=<release>     Fips release name (e.g. fips810) [required].
),
        "btor" =>
q(
  Convert branch to fips release

  Options:
    --branch=<remote-gerrit-branch>     Remote gerrit branch (e.g. release-fips910, master) [required].
),
        );

        print "rel_branch $command:";
        print $help{$command};
    }
}

############################## Begin Sub Routines ##############################

# sub get_release_branches
#
# Get all branches in gerrit that are prefixed 'release-' and remove the prefix
# *Note branches with string 'master' are removed from this to result in direct
# matches of fips releases only. Master branches will be dervied later.
#
# @return array - sorted names of branches (e.g release-fips810 ...)
#
sub get_release_branches
{
    chdir($PROJECT_ROOT);
    die $? if ($?);

    # Parse for remote gerrit branches associated directly with a release
    my $cmd = "git branch -a | grep -e remotes/gerrit/release";
    $cmd .= " | sed -e 's/^[ \\t]*remotes\\/gerrit\\///'";

    my @release_branches = sort (split('\n',`$cmd`));
    print "Release Branches:\n" if $debug;
    print Dumper \@release_branches if $debug;
    return @release_branches;
}

# sub get_fips_releases
#
# Get all fips releases that fsp-ci-jenkins uses in sorted order.
#
# @return array - sorted names of releases (e.g fips910, fips920, etc)
#
sub get_fips_releases
{
    chdir($PROJECT_ROOT);
    die $? if ($?);

    # Parse fsp-ci-jenkins xml file for releases
    my $cmd = "cat $fsp_ci_jenkins_rel_file | grep release | ";
    $cmd .= "sed -e 's/^[ \\t]*<release>//' -e 's/<\\/release>[ \\t]*//'";

    my @fips_releases = sort (split('\n',`$cmd`));
    print "Fips Release:\n" if $debug;
    print Dumper \@fips_releases if $debug;
    return @fips_releases;
}

# sub get_power_platform
#
# Takes a references (release or gerrit branch) and determines the power
# platform it belongs to.
#
# @return string - power platform (e.g. p8, p9)
#
sub get_power_platform
{
    my $reference = shift;

    my $power_platform = "";
    if ($reference =~ m/master/)
    {
        # Remove prefix from older platforms (e.g. master-p8). If nothing is
        # replaced then it is the latest power platform.
        $reference =~ s/master-//;
        if ($reference eq MASTER)
        {
            $power_platform = $latest_power_platform;
        }
        else
        {
            $power_platform = $reference;
        }
    }
    else
    {
        ($power_platform) = $reference =~ /fips(.*)[0-9][0-9]/;
        if ( $power_platform )
        {
            $power_platform = "p$power_platform";
            $power_platforms{$power_platform} = 1;
        }
    }
    return $power_platform;
}


# sub branchExists

sub branchExists
{
    my $branch = shift;
    chomp($branch);
    my $brChk = `git branch -a | grep $branch`;
    if ($brChk eq "")
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

# sub build_relations
#
# Build a relationship hash between fips releases and gerrit branches using
# fsp-ci-jenkins xml and git branch command within PPE.
#
# Structure:
#   power_platform =>
#       fips-release => gerrit-branch
# Example:
#   p9 =>
#       fips910 => master
#
sub build_relations
{
    my @releases = get_fips_releases();
    my @branches = get_release_branches();

    # Fill in fips release keys
    foreach my $release (@releases)
    {
        my $power_platform = get_power_platform($release);
        $relations{$power_platform}{$release} = "";
    }

    # Fill in fips release values, which are gerrit release branches.
    foreach my $branch (@branches)
    {
        my $power_platform = get_power_platform($branch);
        my $release = $branch;
        $release =~ s/release-//;
        $relations{$power_platform}{$release} = $branch;
    }

    # Handle master branches for each platform
    my @platforms = sort keys %power_platforms;
    foreach my $i (0 .. $#platforms)
    {
        my $power_platform = $platforms[$i];

        # Lastest power platform matches branch master
        my $master = MASTER;
        # Previous power platforms match branch "master-$platform"
        if ($i < $#platforms)
        {
            $master = MASTER."-$power_platform";
        }
        else
        {
            # Set latest power platform
            $latest_power_platform = $power_platform;
        }

        # Check for first fips release without a gerrit branch. Due to sort
        # order, it will be the next in order release. It is done this way
        # to avoid issues when fips releases are ahead of gerrit branches. In
        # other words it is possible to have fips releases past gerrit master.
        foreach my $release (sort keys %{$relations{$power_platform}})
        {
            if ($relations{$power_platform}{$release} eq "")
            {
                if (branchExists($master))
                {
                    $relations{$power_platform}{$release} = $master;
                }
                last;
            }
        }
    }

    print "Relations:\n" if $debug;
    print Dumper \%relations if $debug;
}
