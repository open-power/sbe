#! /usr/bin/perl
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/sbeGitTool.pl $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016
# [+] International Business Machines Corp.
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
use lib "src/tools/utils/modules";
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
my $bbuild = "";
my $authorFile = "git_author_emails.csv";

# We do not want hooks to run in this tool
$ENV{MIRROR} = 1;
$ENV{DISABLEHOOKS} = 1;

my %commands = ( "extract" => \&execute_extract,
                 "get-commits" => \&execute_get_commits,
                 "discover" => \&execute_discover);

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

# sub execute_help
#
# Display the help text for the tool
#
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
    Create the SBE binaries with the patch and its GIT dependencies.

    Options:
        --patches=<changeId:patch-set>      CSV of changeId:patch-set's [required].
        --sbbase=<full-path-to-sb-base>     Sandbox base for FSP-CI [required].
        --sbname=<name>                     Sandbox name [required].
        --rc=<rc file name>                 RC file for the sandbox with absolute path [optional].
        --bbuild=<full-path-to-bb>          Backing build of sandbox [required].
),
           "get-commits" =>
q(
    Given a patch, find the corresponding commit id.

    Options:
        --patches=<changeId:patch-set>      CSV of changeId:patch-set's [required].
        --sbbase=<full-path-to-sb-base>     Sandbox base for FSP-CI [required].
        --sbname=<name>                     Sandbox name [required].
        --release=<name>                    Fips release for patch [required].
),
           "discover" =>
q(
    Discover if there are any CMVC dependencies

    Options:
        --patches=<changeId:patch-set>      CSV of changeId:patch-set's [required].
        --sbbase=<full-path-to-sb-base>     Sandbox base for FSP-CI [required].
        --sbname=<name>                     Sandbox name [required].
        --bbuild=<full-path-to-bb>          Backing build of sandbox [required].
)
    );
        print "sbeGitTool $command:";
        print $help{$command};
    }
}

# sub execute_discover
#
# Discover if there are any CMVC dependencies
#
sub execute_discover
{
    # Set GIT environment
    git_environ_init();

    # Set CI environment
    git_ci_init();

    # Use Release tool to discover possible CMVC requirements
    print "Discovering CMVC requirements for $globals{sandbox}\n" if $debug;
    my $output = `./src/tools/utils/gitRelease.pl fsp-ci --discover --level $sbname --patches $patches --branch $globals{branch} --basestr \"gerrit/$globals{branch}\" --bbuild-Rel $globals{Build_Rel} --debug`;
    die "sbeRelease tool failed\n" if($?);

    my $bbuild_tracks = `cat $bbuild/logs/tracks.txt`;
    my @lines = split /\n/, $output;
    my @tracks = ();
    foreach my $line (@lines)
    {
        # Look for lines indicating tracks (e.g. !@#%^ Need CoReq:)
        my $cmvc_line = $line;
        if ($cmvc_line =~ m/^\!\@\#\%\^/)
        {
            $cmvc_line =~ s/^.*\:\s*//;

            # Remove CMVC tracks that are already in the sb bbuild
            if ($bbuild_tracks !~ m/$cmvc_line/)
            {
                print $cmvc_line;
                print "," if(\$line != \$lines[-1]);
            }
        }
    }

    # Remove level=$sbname from config file as extract() will
    # actually create a release with this name
    system("./src/tools/utils/gitRelease.pl undef $sbname > /dev/null");
    die "sbeRelease tool failed\n" if($?);
}

# sub execute_get_commits
#
# Given a patch, find the asociated corresponding commits
#
sub execute_get_commits
{
    # Set GIT environment
    git_environ_init();

    die "Missing release" if ($release eq "");

    # Set CI environemnt
    git_ci_init($release);

    # Use gitRelease tool to discover all associated commits
    chdir $globals{tool_path};
    print `./src/tools/utils/gitRelease.pl gerrit-commit --patches $patches --branch $globals{branch}`;
    die "$?" if ($?);
}

# sub execute_extract
#
# Create the SBE binaries with the patch and its GIT dependencies.
# Copy the binaries to the sandbox.
#
sub execute_extract
{
    # Set GIT environment
    git_environ_init();

    # Set CI environment
    git_ci_init();

    print "Extracting Code for $globals{sandbox}\n";
    system("./src/tools/utils/gitRelease.pl fsp-ci --level $sbname --patches $patches --branch $globals{branch} --basestr \"gerrit/$globals{branch}\" --bbuild-Rel $globals{Build_Rel}");
    die "sbeRelease tool failed\n" if($?);

    # Compile the SBE and copy binaries to sandbox
    compileAndCopy();
}

# sub git_environ_init
#
# Fetch the parameters and setup environment for further functions
#
sub git_environ_init
{
    # Handle the i/p to the function
    GetOptions("patches:s" => \$patches,
               "sbbase:s" => \$sbbase,
               "sbname:s" => \$sbname,
               "rc:s" => \$sbrc,
               "bbuild:s" => \$bbuild,
               "release:s" => \$release);

    die "Missing patch list" if ($patches eq "");
    die "Missing sandbox base path" if ($sbbase eq "");
    die "Missing sandbox name" if ($sbname eq "");
    die "Missing bbuild path" if ($bbuild eq "" && $release eq "");
    exit "Workaround for Patch list = none" if ($patches eq "none");

    # Set global variables
    $globals{sandbox} = $sbbase."/".$sbname;
    $globals{sbe_git_root} = $globals{sandbox}."/git-ci";
    $globals{tool_path} = $globals{sb_git_root}."/src/tools/utils";

    print "Sandbox: $globals{sandbox}\n" if $debug;
    print "GIT repository path: $globals{sbe_git_root}\n" if $debug;

    chdir($globals{sbe_git_root});
    die "ERROR $?: Invalid GIT repository path in the sandbox" if $? ;
}

# sub git_ci_init
#
# Fetch the GIT branch to work with given the release
#
# @param[in] release - release to convert to branch, indicates no bbuild was
#                      passed in so the default is blank as the release is
#                      derived from the bbuild
#
sub git_ci_init
{

    my $release = shift;

    # If a release is passed in, there is no bbuild to do parsing for
    if ($release eq "")
    {
        # Get SBE release associated with bbuild
        parse_release_notes($bbuild);

        # Parse release out of bbuild path ("fipsXXX")
        ($release) = $bbuild =~ /\/esw\/(fips.*)\/Builds/;
        chomp($release);
    }

    # Convert release to gerrit branch
    release_to_branch($release);
    print "\nConverted release:\n" if $debug;
    print "    Release=$release -> branch=$globals{branch}\n\n" if $debug;
    die "Gerrit branch missing" if  $globals{branch} eq "";

    # Create commit author file
    create_author_file($patches);

}

# sub parse_release_notes
#
# Parse bbuilds SBE release notes to determine the sbeRelease in the bbuild
#
# @param[in] bbuild - bbuild of the sandbox
#
sub parse_release_notes
{
    my $bbuild = shift;

    print "Parsing Release Notes...\n" if $debug;

    # Check if a new SBE release was pulled into the sandbox first, then check the bbuild
    open(RELNOTE, $globals{sandbox}."/src/sbei/sbfw/releaseNotes.html") or
    open(RELNOTE, $bbuild."/src/sbei/sbfw/releaseNotes.html") or
    die("Cannot find releaseNotes.html in the following directories $globals{sandbox} or $bbuild; : $!");
    my $Release = "";
    while (my $line = <RELNOTE>)
    {
        if ($line =~ m/<title>/)
        {
            print $line if $debug;
            ($Release) = $line =~ /Release notes for (.*)<\/title>/;
            chomp $Release;
            last;
        }
    }
    $globals{Build_Rel} = $Release;
    close(RELNOTE);
}

# sub release_to_branch
#
# Assign global branch by converting a release name to a SBE branch
#
# @param[in] release - The fips release to convert to a branch
#
sub release_to_branch
{
    my ($release) = @_;

    die "Release missing while trying to convert to branch" if $release eq "";

    # Run conversion tool to translate a fips-release to gerrit branch
    $globals{branch} = `./src/tools/utils/conv_rel_branch.pl rtob --release $release`;
    die "Conversion from fips-release = $release to gerrit branch failed" if $globals{branch} eq "";
    chomp($globals{branch});
}

# sub create_author_file
#
# Get patches from gerrit and create a CSV file with author emails
# for FSP-CI notification purposes.
#
# @param[in] patches - List of patches to determine authors of
#
sub create_author_file
{
    my ($patches) = @_;

    # Get commits via gitRelease.pl tool
    my $commitsCSV = `./src/tools/utils/gitRelease.pl gerrit-commit --patches $patches --branch $globals{branch}`;
    chomp($commitsCSV);
    my @commits = split(',',$commitsCSV);
    print Dumper \@commits if $debug;

    # Write author email to file
    system("mkdir -p $globals{sandbox}/logs");
    my $file = $globals{sandbox}."/logs/".$authorFile;
    print "Creating CSV author file - $file\n" if $debug;
    open(FILE, ">", $file) or die("Cannot open: $file: $!");
    foreach my $commit (@commits)
    {
        my $email =  `git log -n1 --pretty="%ae" $commit`;
        chomp ($email);
        print FILE $email;
        print FILE "," if ($commit != $commits[-1]);
    }
    system("chmod 755 $file");
    close FILE;
}

# sub compileAndCopy
#
# utility that builds and copies the SBE image.
#
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
