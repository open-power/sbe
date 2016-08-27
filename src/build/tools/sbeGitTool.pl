#! /usr/bin/perl
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/tools/sbeGitTool.pl $
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
#use lib "$ENV{'PERLMODULES'}";
use lib "sbe/build/tools/perl.modules";
use Getopt::Long qw(:config pass_through);
use Data::Dumper;
use Cwd;
use gitUtil;

# Global variables & constants
my $debug = 0;
my $help = 0;
my %globals = ();
my @references = ();
my @patchList = ();
my @commits = ();
my $patches = "";
my $sbbase = "";
my $sbname = "";
my $sbrc = "";
my $release = "";

my %commands = ( "extract" => \&execute_extract,
                 "get-commits" => \&execute_get_commits);

# Handle input command
GetOptions("debug!" => \$debug,
           "help" => \$help);

if ($help)
{
    execute_help();
}
else
{
    my $command = shift @ARGV;
    if ($commands{$command})
    {
        &{$commands{$command}}();
    }
    else
    {
        execute_help();
    }
}

foreach my $arg (@ARGV)
{
    print "Unprocessed arg: $arg\n" if $debug;
}

############################## Begin Actions ##################################

sub execute_help
{
 my $command = shift @ARGV;

    if ($command eq "")
    {
        print "sbeGitTool\n";
        print "    Run FSP-CI from a set of git commits\n";
        print "\n";
        print "    Syntax:\n";
        print "        sbeGitTool <tool> [options]\n";
        print "\n";
        print "    Available subtools:\n";
        foreach my $key (sort keys %commands)
        {
            print "        $key\n";
        }
        print "\n";
        print "    Global options:\n";
        print "        --debug              Enable debug mode.\n";
        print "        --help               Display help on a specific tool.\n";
        print "\n";
        print "    Note: Generally a <commit> can be any git or gerrit\n";
        print "          reference.  A git commit number, tag, branch, or\n";
        print "          a gerrit change-id are all valid.\n";
    }
    elsif (not defined $commands{$command})
    {
        die "Unknown subcommand: $command.\n";
    }
    else
    {
        my %help = (
           "extract" =>
q(
    Create the SBE binaries with the patch and its CMVC/GIT dependencies.

    Options:
        --patches=<changeId:patch-set>      CSV of changeId:patch-set's [required].
        --sbbase=<full-path-to-sb-base>     Sandbox base for FSP-CI [required].
        --sbname=<name>                     Sandbox name [required].
        --rc=<rc file name>                 RC file for the sandbox with absolute path [optional].
),
           "get-commits" =>
q(
    Given a patch, find the corresponding commit id.

    Options:
        --patches=<changeId:patch-set>      CSV of changeId:patch-set's [required].
        --sbbase=<full-path-to-sb-base>     Sandbox base for FSP-CI [required].
        --sbname=<name>                     Sandbox name [required].
)
        );

        print "sbeGitTool $command:";
        print $help{$command};
    }
}

sub execute_get_commits
{

    # Set GIT environment
    git_environ_init();

    # Obtain the list of patches
    retrivePatchList();

    # Fetch the commits for the patches
    fetchCommits();

    # Prepare commit string
    my $commitStr = prepareCommitStr();

    print "The set of commits: $commitStr" if $debug;
    print $commitStr;
    return $commitStr;
}

sub execute_extract
{
    # Set GIT environment
    git_environ_init();

    # Obtain the list of patches
    retrivePatchList();

    # Fetch the references for the patches
    fetchRefs();

    # Apply the patches on the GIT repo
    applyRefs();

    # Compile the SBE and copy binaries to sandbox
    compileAndCopy();
}

sub git_environ_init
{
    # Handle the i/p to the function
    GetOptions("patches:s" => \$patches,
               "sbbase:s" => \$sbbase,
               "sbname:s" => \$sbname,
               "rc:s" => \$sbrc);

    die "Missing patch list" if ($patches eq "");
    die "Missing sandbox base path" if ($sbbase eq "");
    die "Missing sandbox name" if ($sbname eq "");

    # Set global variables
    $globals{sandbox} = $sbbase."/".$sbname;
    $globals{sbe_git_root} = $globals{sandbox}."/git-ci";

    print "Sandbox: $globals{sandbox}\n" if $debug;
    print "GIT repository path: $globals{sbe_git_root}\n" if $debug;

    chdir($globals{sbe_git_root});
    die "ERROR $?: Invalid GIT repository path in the sandbox" if $? ;
}

sub retrivePatchList
{
    # Parse out the CSV patch list
    @patchList = split(/,+/, $patches);

    print ">>>Patches\n" if $debug;
    print Dumper @patchList if $debug;
    print "<<<End of Patches\n" if $debug;
}

sub fetchRefs
{
    my $currentRef = "";
    my $validPatchCount = 0;

    foreach my $patch (@patchList)
    {
        my ($changeId,$patchSet) = split(":",$patch);
        if (gitUtil::gerritIsPatch($changeId))
        {
            $validPatchCount = $validPatchCount + 1;
            print "Fetching reference for the patch : $patch \n" if $debug;
            if (gitUtil::patchMergeStatus($changeId) == 0)
            {
                my $currentRef = gitUtil::gerritQueryReference($changeId, $patchSet);
                push @references, $currentRef;
                print "(patchset -> reference) = $patch -> $currentRef\n" if $debug;
            }
        }
        else
        {
            print "\n Warning : Patchset $patch is invalid.. Continuing to check if there is any other valid patch \n";
        }
    }
    die "ERROR: No valid patches given..\n" if ($validPatchCount == 0);
}

sub applyRefs
{
    my $statusFile = $globals{sbe_git_root}."/patchApply.status";

    foreach my $ref (@references)
    {
        print "Cherrypicking reference $ref \n" if $debug;
        open  SBWORKON, " | ./sb workon";
        print SBWORKON  "git fetch gerrit $ref  && echo \"Fetch Done \" > $statusFile \n";
        #print SBWORKON  "git cherry-pick FETCH_HEAD  && echo \"Cherry-pick Done \" >> $statusFile \n"; // will be reused once appropriate support is there
        print SBWORKON  "git checkout FETCH_HEAD  && echo \"Checkout Done \" >> $statusFile \n";
        print SBWORKON  "exit \n";
        close SBWORKON;

        print "\nChecking cherrypick status for $ref...\n" if $debug;
        my $ch_status = `cat $statusFile`;
        if( ($ch_status =~ m/Fetch/) && ($ch_status =~ m/Checkout/))
        {
            print "Checkout successful\n";
        }
        else
        {
            die "ERROR: Checkout of $ref failed\n";
        }
    }
}

sub compileAndCopy
{
    my $statusFile = $globals{sbe_git_root}."/compile.status";
    my $compile_path = $globals{sbe_git_root};

    print "Compiling and copying the generated binaries to sandbox\n" if $debug;
    open  SBWORKON, " | ./sb workon";
    print SBWORKON  "cd $compile_path \n";
    print SBWORKON  "make install && echo \"Compile Passed\" > $statusFile \n";
    print SBWORKON  "export SANDBOXBASE=$globals{sandbox} \n";
    print SBWORKON  "cd $globals{sbe_git_root} \n";
    print SBWORKON  "./sb prime --sb $sbname --no_build && echo \"Prime Passed\" >> $statusFile \n";
    print SBWORKON  "exit \n";
    close SBWORKON;

    print "\nChecking compile status...\n" if $debug;
    my $compile_status = `cat $statusFile | grep "Compile"`;
    if ($compile_status =~ m/Compile/)
    {
        print "SBE compile successful\n";
    }
    else
    {
        die "ERROR: SBE compile failed\n";
    }

    print "\nChecking sandbox status...\n" if $debug;
    my $sb_status = `cat $statusFile | grep "Prime"`;
    if ($sb_status =~ m/Prime/)
    {
        print "SBE prime successful\n";
    }
    else
    {
        die "ERROR: SBE prime failed\n";
    }
}

sub fetchCommits
{
    my $currentCommit = "";

    foreach my $patch (@patchList)
    {
        my ($changeId,$patchSet) = split(":",$patch);
        if (gitUtil::gerritIsPatch($changeId))
        {
            print "Fetching commit for the patch : $patch \n" if $debug;
            my $currentCommit = gitUtil::gerritQueryCommit($changeId, $patchSet);
            push @commits, $currentCommit;
            print "(patchset -> commit) = $patch -> $currentCommit\n" if $debug;
        }
        else
        {
            print "\n Warning : Patchset $patch is invalid.. Continuing to check if there is any other valid patch \n";
        }
    }
    die "ERROR: No valid patches given..\n" if (scalar @commits == 0);
}

sub prepareCommitStr
{
    my $commitStr = "";

    foreach my $commit (@commits)
    {
        $commitStr = $commitStr.",".$commit;
    }
    $commitStr =~ s/^,//g;
    return $commitStr;
}
