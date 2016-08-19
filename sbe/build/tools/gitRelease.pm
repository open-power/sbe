#!/usr/bin/perl
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: sbe/build/tools/gitRelease.pm $
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

use Getopt::Long qw(:config pass_through);
use POSIX;
use Text::Wrap;
use List::Util 'max';
use Term::ReadKey;
use File::Temp qw/tempfile/;

use Data::Dumper;

my $debug = 0;
my $help = 0;

# default globals
my %globals = ( branch => "master");
# Token used by git-CI-tool @execute_discover, update there too
my $TOKEN = "!@#%^";

GetOptions("debug!" => \$debug,
           "help" => \$help,
           "branch:s" => \$globals{"branch"});

my %commands = ( "define" => \&execute_define,
                 "undef" => \&execute_undef,
                 "list-levels" => \&config_print_levels,
                 "query-gerrit" => \&execute_gerrit_query,
                 "query-git" => \&execute_git_query,
                 "query-level" => \&execute_level_query,
                 "add-patch" => \&execute_add_patch,
                 "add-forcedep" => \&execute_add_forcedep,
                 "verify-patches" => \&execute_verify_patches,
                 "release" => \&execute_release,
                 "build-name" => \&execute_build_name,
                 "gerrit-commit" => \&execute_gerrit_commit,
                 "help" => \&execute_help,
               );

if ($help)
{
    execute_help();
}

my $command = shift @ARGV;
if ($commands{$command})
{
    &{$commands{$command}}();
}
else
{
    execute_help();
}

foreach my $arg (@ARGV)
{
    print "Unprocessed arg: $arg\n" if $debug;
}

############################## Begin Actions ##################################

sub execute_define
{
    print "Defining new level...\n";

    my %level = ();

    GetOptions("level:s" => \$level{name},
               "name:s" => \$level{name},
               "baseline:s" => \$level{base},
               "released:s" => \$level{released});

    die "Missing level name" if ($level{name} eq "");
    die "Missing baseline name" if ($level{base} eq "");
    die "Missing released level name" if ($level{released} eq "");

    print "New level: ".$level{name}.":".$level{base}.":".$level{released}."\n"
        if $debug;

    $level{base} = git_resolve_ref($level{base});
    $level{released} = git_resolve_ref($level{released});

    config_add_level(\%level);
}

sub execute_undef
{
    my $level = shift @ARGV;

    die "Level to undefine not given" if ($level eq "");

    my $levels = config_list_levels();
    die "Level $level does not exist" if (not defined $levels->{$level});

    print "Undefining level $level...\n";
    config_del_level($level);
}

sub execute_gerrit_query
{
    my $project = "";

    GetOptions("project:s" => \$project);

    if ("" eq $project)
    {
        $project = config_project();
    }

    my $items = gerrit_query("status:open project:$project ".
                             "branch:".$globals{"branch"});

    foreach my $item (@$items)
    {
        if (defined $item->{"project"})
        {
            print wrap("","",$item->{"subject"}) . "\n";
            print "\t" . $item->{"id"} . "\n";
            print "\n";
        }
    }
}

sub execute_git_query
{
    my $level = "";

    GetOptions("name:s" => \$level,
               "level:s" => \$level);

    die "Missing level name" if ($level eq "");

    $globals{"branch"} = git_resolve_ref($globals{"branch"});

    my $level = config_get_level($level);

    my $commits = git_commit_history($globals{"branch"}, $level->{base});

    foreach my $commit (@{$commits})
    {
        my $subject = git_get_subject($commit);
        print "$subject\n\t$commit\n\n";
    }
}

sub execute_level_query
{
    my $level = "";

    GetOptions("name:s" => \$level,
               "level:s" => \$level);

    die "Missing level name" if ($level eq "");

    my $level_info = config_get_level($level);

    print "Level $level\n";
    print "    Base: \n";
    print "        ".git_name_rev($level_info->{base})."\n";
    print "    Released:\n";
    print "        ".git_name_rev($level_info->{released})."\n";
    if ($globals{"branch"} ne "master")
    {
        print "    Branch:\n";
        print "        ".$globals{"branch"}."\n";
    }
    print "    Patches:\n";
    foreach my $patch (sort @{$level_info->{patches}})
    {
        print "        $patch\n";
    }
    print "    Forced Deps:\n";
    foreach my $dep (sort keys %{$level_info->{forceDeps}})
    {
        my $deps = $level_info->{forceDeps};
        print "        $dep =>\n";
        print "            ".$deps->{$dep}."\n";
    }
}


sub execute_add_patch
{
    my $level = "";
    my $patch = "";

    GetOptions("name:s" => \$level,
               "level:s" => \$level,
               "patch:s" => \$patch);

    die "Missing level name" if ($level eq "");
    die "Missing patch name" if ($patch eq "");

    config_add_patch($level, $patch);
}

sub execute_add_forcedep
{
    my $level = "";
    my $from = "";
    my $to = "";

    GetOptions("name:s" => \$level,
               "level:s" => \$level,
               "from:s" => \$from,
               "to:s" => \$to);

    die "Missing level name" if ($level eq "");
    die "Missing from depend" if ($from eq "");
    die "Missing to depend" if ($to eq "");

    config_add_dep($level, $from, $to);
}

sub execute_verify_patches
{
    my $level = "";

    GetOptions("name:s" => \$level,
               "level:s" => \$level);

    die "Missing level name" if ($level eq "");

    my $level_info = config_get_level($level);
    my $patches = $level_info->{patches};

    $patches = gerrit_resolve_patches($patches);

    config_verify_patches($level_info->{base}, $patches);
}

sub execute_release
{
    my $level = "";

    GetOptions("name:s" => \$level,
               "level:s" => \$level);

    die "Missing level name" if ($level eq "");

    my $level_info = config_get_level($level);

    config_release($level_info,1);
}

sub execute_build_name
{
    # default release - 910, build letter - a, build prefix - sbe
    my $release = "910";
    my $build_letter = "a";
    my $build_prefix = "sbe";

    GetOptions("release:s" => \$release,
               "letter:s" => \$build_letter,
               "prefix:s" => \$build_prefix);

    system ("date +".$build_prefix."%m%d".$build_letter."_%g%V.$release");
}

sub execute_gerrit_commit
{
    my $patches = "";

    GetOptions("patches:s" => \$patches);

    die "Missing patches" if ($patches eq "");

    # Parse out csv list of patches
    my @patches = split(/,+/, $patches);

    my $commits = gerrit_resolve_patchset(\@patches);
    foreach my $commit (@$commits)
    {
        print $commit;
        print "," if( \$commit != \$commits->[-1] )
    }
    print "\n";
}

sub execute_help
{
    my $command = shift @ARGV;

    if ($command eq "")
    {
        print "gitRelease:\n";
        print "    Prepare the git codebase for release.\n";
        print "\n";
        print "    Syntax:\n";
        print "        gitRelease [options] <tool>\n";
        print "\n";
        print "    Available subtools:\n";
        foreach my $key (sort keys %commands)
        {
            print "        $key\n";
        }
        print "\n";
        print "    Global options:\n";
        print "        --debug     Enable debug mode.\n";
        print "        --help      Display help on a specific tool.\n";
        print "        --branch    Branch to use for release.\n";
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
            "define" =>
q(
        Define a new level for release.

    Options:
        --level=<name>          Name for the new level [required].
        --base=<commit>         Baseline commit [required].
        --released=<commit>     Commit of previous release [required].
),
            "undef" =>
q(
        Delete a previously defined release level.

    Options:
        --level=<name>          Name for the level to delete [required].
),
            "list-levels" =>
q(
        Displays a list of currently defined levels.
),

            "query-gerrit" =>
q(
        Displays a list of open change-sets from the Gerrit server.
),
            "query-git" =>
q(
        Displays a list of merged commits which are NOT currently destined
        for a release level.

    Options:
        --level=<name>          Name for the level to query [required].
        --branch=<commit>       Branch to query against [default=master].
),
            "query-level" =>
q(
        Displays information about a defined release level.

    Options:
        --level=<name>          Name for the level to query [required].
),
            "add-patch" =>
q(
        Adds a commit to the patch-list for a release.

    Options:
        --level=<name>          Release to add patch to [required].
        --patch=<commit>        Commit to add to patch-list [required].
),
            "add-forcedep" =>
q(
        Add a commit-pair as a forced dependency for a release.

    Options:
        --level=<name>          Release to add dependency to [required].
        --from=<commit>         Decendent commit in the dependency [required].
        --to=<commit>           Ancestor commit in the dependency [required].
),
            "verify-patches" =>
q(
        Verify patch-list to ensure all dependencies are met.

        This tool will give a list of dependency candidates if an ancestor
        commit is found modifying the same files as a commit in the
        patch-list.

    Options:
        --level=<name>          The level to verify [required].
),
            "release" =>
q(
        Create a branch / tag based on the definition of a release.

    Options:
        --level=<name>          The level to release [required].
),
            "build-name" =>
q(
        Display a properly formatted build name based on the date.

        Ex: sbe0402a_1412.910

    Options:
        --release=<id>          Release name [default=910].
        --letter=[a-z]          Build letter [default=a].
        --prefix=[a-z]          Build prefix [default=sbe]
),
            "gerrit-commit" =>
q(
        Get commit number of gerrit change-id, patch-set pairs

    Options:
        --patches=<change-id:patchset>  CSV of change-id:patchset [required].
),
        );

    my $release = "";
    my $level = "";
    my $checkInDir = "";

        print "gitRelease $command:";
        print $help{$command};

    }
}


#########################  Begin Utility Subroutines ###########################



# sub create_release_notes
#
# Generates an HTML file (releaseNotes.html) with the release notes for a
# release.
#
# @param [in] level - The level name to release.
# @param [in] level_info - The level_info hash (see config_get_level).
#
sub create_release_notes
{
    my $level = shift;
    my $level_info = shift;

    my $commits = git_commit_history("HEAD", $level_info->{released});

    open RELNOTE, "> ".git_root()."/releaseNotes.html";
    print RELNOTE "<html>\n";
    print RELNOTE "    <head><title>Release notes for $level</title></head>\n";
    print RELNOTE <<STYLESHEET;
    <style type="text/css">
        table.release {
            border-width: 1px;
            border-spacing: 2px;
            border-style: outset;
            border-color: gray;
            border-collapse: separate;
            background-color: white;
        }
        table.release th {
            border-width: 1px;
            padding: 1px;
            border-style: inset;
            border-color: gray;
            background-color: white;
        }
        table.release td {
            border-width: 1px;
            padding: 1px;
            border-style: inset;
            border-color: gray;
            background-color: white;
        }
    </style>
STYLESHEET
    print RELNOTE "    <body>\n";

    print RELNOTE "<h1>Level: $level</h1>\n";
    print RELNOTE "<h2>Included commits:</h2>\n";
    print RELNOTE "<table class='release'>\n";
    print RELNOTE "    <tr>\n";
    print RELNOTE "        <th>RTC/CQ Number(s)</th>\n";
    print RELNOTE "        <th>Subject</th>\n";
    print RELNOTE "        <th>Git Commit</th>\n";
    print RELNOTE "    </tr>\n";


    foreach my $commit (@{$commits})
    {
        my $subject = git_get_subject($commit);
        my $rtc = rtc_workitem_num($commit);
        my $rtc_hyper = "";
        my $cq = cq_workitem_num($commit);
        my $cq_hyper = "";

        if ($rtc ne "")
        {
            $rtc_hyper = rtc_hyperlink($rtc);
            $rtc_hyper = "<a href='$rtc_hyper' target='_blank'>$rtc</a>";
        }
        if ($cq ne "")
        {
            $cq_hyper = cq_hyperlink($cq);
            $cq_hyper = "<a href='$cq_hyper' target='_blank'>$cq</a>";

            if ($rtc_hyper ne "")
            {
                $cq_hyper = "<br>$cq_hyper";
            }
        }

        print RELNOTE "    <tr>\n";
        print RELNOTE "        <td>$rtc_hyper $cq_hyper</td>\n";
        print RELNOTE "        <td>$subject</td>\n";
        print RELNOTE "        <td>$commit</td>\n";
        print RELNOTE "    </tr>\n";
    }
    print RELNOTE "</table>\n";

    print RELNOTE "    </body>\n";
    print RELNOTE "</html>\n";

    close RELNOTE;

    system "git add ".git_root()."/releaseNotes.html";
    system "git commit -m \"Release notes for $level\"";

}

# sub git_resolve_ref
#
# Transforms a symbolic git reference into a commit number.
#
# @param [in] ref - The reference to resolve.
#
# @return string - Resolved git commit number.
#
sub git_resolve_ref
{
    my $ref = shift;
    my $resolve = "";

    if (gerrit_is_patch($ref))
    {
        my $gerrit = gerrit_resolve_patches([$ref]);
        $resolve = @{$gerrit}[0];
    }
    else
    {
        open COMMAND, "git log -n1 --pretty=\"%H\" $ref |";
        $resolve = <COMMAND>;
        close COMMAND;
        chomp $resolve;
    }

    die "Unable to resolve ref $ref" if ($resolve eq "");
    print "Resolved $ref as $resolve\n" if $debug;

    return $resolve;
}

# sub git_root
#
# Determines the path of the root of the git repository.
#
# @return string - Root of the git repository.
sub git_root
{
    return $globals{git_root} if (defined $globals{git_root});

    open COMMAND, "git rev-parse --show-toplevel |";
    my $root = <COMMAND>;
    close COMMAND;
    chomp $root;

    die "Unable to determine git_root" if ($root eq "");
    print "Found git_root at $root\n" if $debug;

    $globals{git_root} = $root;
    return $root;
}

# sub git_commit_history
#
# Determines all the commits between two points in git history.
#
# @param[in] start - Beginning commit.
# @param[in, optional] not_including - Starting point to exclude.
#
# @return array - Commit history.
#
sub git_commit_history
{
    my $start = shift;
    my $not_including = shift;

    my @commits = ();

    unless ($not_including eq "") { $not_including = "^".$not_including; }

    open COMMAND, "git rev-list --cherry-pick $start $not_including |";
    while (my $line = <COMMAND>)
    {
        chomp $line;
        push @commits, $line;
    }
    close COMMAND;

    return \@commits;
}

# sub git_log_changeId
#
# Determines if a changeId exists in the base
#
# @param[in] base
# @param[in] changeId
#
# @return bool - True if in commit history, False otherwise.
#
sub git_log_changeId
{
    my $base = shift;
    my $changeId = shift;
    my $exists = 0;
    open COMMAND, "git log $base | grep \'Change-Id: $changeId\' |";
    if(<COMMAND> ne "")
    {
        $exists = 1;
    }
    close COMMAND;

    return $exists;
}

# sub git_name_rev
#
# Transforms a git commit number to a symbolic name for human readability.
#
# @param[in] rev - Git revision (commit number) to name.
# @return string - The symbolic name git uses for that commit number.
#
sub git_name_rev
{
    my $rev = shift;

    open COMMAND, "git name-rev $rev |";
    my $line = <COMMAND>; chomp $line;
    close COMMAND;

    return $line;
}

# sub git_commit_deps
#
# Determines a list of dependent commits based on common files touched.
#
# @param[in] base - The end point, in git history, of commits to compare.
# @param[in] commit - The commit to find dependents of.
#
# @return array - List of dependent commits.
#
sub git_commit_deps
{
    my $base = shift;
    my $commit = shift;
    chomp($base);
    chomp($commit);

    my @deps = ();

    print "Searching for deps for $commit against $base\n" if $debug;

    my @files = split('\n',`git diff-tree --name-only --no-commit-id -r $commit`);
    foreach my $file (@files)
    {
        # If a commit introduces a new file, don't run rev-list as it fails
        # when the file does not exists in base.
        my $file_in_base = `git log $base -n1 --oneline -- $file`;
        next if ($file_in_base eq "");

        my $dep_commit = `git rev-list $commit~1 ^$base $file`;
        if ($dep_commit ne "")
        {
            print "Found dep: $dep_commit" if $debug;

            chomp $dep_commit;
            push @deps, $dep_commit;
        }
    }

    return \@deps;
}

# sub git_commit_files
#
# Find the files touched by a commit.
#
# @param[in] commit - The commit to examine.
# @return array - List of files touched by the commit.
#
sub git_commit_files
{
    my $commit = shift;

    my @files = ();
    open COMMAND, "git diff-tree --name-only --no-commit-id -r $commit |";
    while (my $line = <COMMAND>)
    {
        chomp $line;
        push @files, $line;
    }
    close COMMAND;

    return \@files;
}

# sub git_get_subject
#
# Get the subject of the commit message associated with a commit.
# See git log --oneline.
#
# @param[in] commit - The commit to examine.
# @return string - The subject of the commit.
#
sub git_get_subject
{
    my $commit = shift;

    open COMMAND, "git log -n1 --pretty=\"%s\" $commit |";
    my $subject = <COMMAND>; chomp($subject);
    close COMMAND;

    return $subject;
}

# sub git_commit_msg
#
# Get the entire commit message associated with a commit.
#
# @param[in] commit - The commit to examine.
# @return string - The commit message.
#
sub git_commit_msg
{
    my $commit = shift;

    open COMMAND, "git log -n1 --pretty=%B $commit |";
    my $message = "";
    while (my $line = <COMMAND>)
    {
        $message = $message.$line;
    }
    close COMMAND;

    return $message;
}

# sub git_create_branch
#
# Create a branch for a release-level.
#
# @param[in] level - The release-level to use as basis for the branch name.
# @param[in] base - The commit to use as the base for the new branch.
#
sub git_create_branch
{
    my $level = shift;
    my $base = shift;

    system("git checkout -b __sbeRelease_$level $base");
    die "Could not create branch for $level" if ($?);
}

# sub git_create_tag
#
# Create a tag for a release-level.
#
# @param[in] level - The release-level to create a tag for.
# @param[in] level_info - The level-info associated with the level.
#
sub git_create_tag
{
    my $level = shift;
    my $level_info = shift;

    # Create an annotated tag, taking annotation from stdin.
    open COMMAND, "| git tag -a $level -F -" || die;

    # Add information about the level to the tag.
    print COMMAND "Release: $level\n\n";
    print COMMAND "Base: ".$level_info->{base}."\n";
    print COMMAND "Previous-Release: ".$level_info->{released}."\n";
    print COMMAND "Branch: ".$globals{"branch"}."\n";
    print COMMAND "\n";
    foreach my $patch (@{$level_info->{patches}})
    {
        print COMMAND "Patch: $patch\n";
    }
    my $forceDeps = $level_info->{forceDeps};
    foreach my $from (keys %{$forceDeps})
    {
        print COMMAND "Forced-Dep: $from:".$forceDeps->{$from}."\n";
    }

    # Commit annotated tag.
    close COMMAND;
}

# sub git_cherry_pick
#
# Cherry-pick a commit onto the current branch.
#
# @param[in] commit - The commit to cherry-pick.
#
# @retval false - Error occurred during cherry-pick.
sub git_cherry_pick
{
    my $commit = shift;

    system("git cherry-pick -x $commit");
    return ($? == 0);
}

# sub git_order_commits
#
# Order a list of commits so that they are in a good order with regard to
# dependencies.  The order returned should be the most likely to not fail
# a cherry-pick sequence.
#
# @param[in] patches - The list of commits to order.
# @param[in] level_info - The level_info for the release-level being created.
#
# @return array - Re-ordered list of commits (from patches).
#
sub git_order_commits
{
    my $patches = shift;
    my $level_info = shift;
    my $forceDeps = $level_info->{forceDeps};
    my %patch_dep = ();

    # Create patch -> { distance -> 0, deps -> [] } hash.
    my %patch_hash =
        map { $_ => \{ distance => 0, deps => [] }} @{$patches};

    # Determine dependencies and distance for each patch.
    foreach my $patch (@{$patches})
    {
        # Add dependencies for each patch to the hash.
        my $deps = git_commit_deps($level_info->{base}, $patch);
        push @{${$patch_hash{$patch}}->{deps}}, @{$deps};

        # Add dependencies to hash for circular depends check later
        foreach my $dep (@{$deps})
        {
            $patch_dep{$patch}{$dep} = 1;
        }

        # Determine the distance from previous release for each patch.
        ${$patch_hash{$patch}}->{distance} =
            scalar @{git_commit_history($patch, $level_info->{released})};
    }

    # Determine forced dependencies for each patch.
    foreach my $patch (keys %{$forceDeps})
    {
        my $resolve_from = @{gerrit_resolve_patches([$patch])}[0];
        my $resolve_to =
                @{gerrit_resolve_patches([$forceDeps->{$patch}])}[0];

        print "Force dep: $resolve_from : $resolve_to\n" if ($debug);

        push @{${$patch_hash{$resolve_from}}->{deps}}, $resolve_to;
        # Add dependencies to hash for circular depends check later
        $patch_dep{$resolve_from}{$resolve_to} = 1;
    }

    # Calculate Dijkstra's on the patches.
    my $changed = 1;
    while ($changed != 0)
    {
        $changed = 0;
        foreach my $patch (@{$patches})
        {
            my $distance = 1 + max( map
                {
                    # If patches have a circular dependency, ignore distance check.
                    next if ($patch_dep{$_}{$patch} && $patch_dep{$patch}{$_});
                    ${$patch_hash{$_}}->{distance}
                }
                @{${$patch_hash{$patch}}->{deps}});
            if ($distance > ${$patch_hash{$patch}}->{distance})
            {
                $changed = 1;
                ${$patch_hash{$patch}}->{distance} = $distance;
            }
        }
    }

    # Sort the patches based on shortest distance from previous release
    # (after Dijkstra).
    my @commit_order =
        sort { ${$patch_hash{$a}}->{distance} <=>
                    ${$patch_hash{$b}}->{distance} }
        @{$patches};

    return \@commit_order;
}

# sub config_filename
#
# @return The location of the gitRelease config file.
#
sub config_filename
{
    return git_root()."/.git/gitRelease.config";
}

# sub config_init
#
# Ensures the gitRelease tool is initialized properly.
#
sub config_init
{
    return if (defined $globals{config_init});

    unless (-e config_filename())
    {
        open COMMAND, "git config --list | grep remote.*ssh |";
        my $url = <COMMAND>;
        close COMMAND;
        chomp $url;

        die "Undefined git-remote 'gerrit'" if ($url eq "");

        die "Unexpected url found: $url" if (not ($url =~ m/ssh:\/\/.*\/.*/));

        my $server = $url;
        my $project = $url;

        # match first occurance of '/' after ssh://
        # eg: remote.hostboot.url=ssh://hostboot.gerrit/hostboot
        # $2 is 'hostboot.gerrit'
        $server =~ s/(.*)ssh:\/\/(.*?)\/(.*)/$2/;
        # eg: remote.ppe.url=ssh://hw.gerrit/hw/ppe
        # $3 is 'hw/ppe'
        $project =~ s/(.*)ssh:\/\/(.*?)\/(.*)/$3/;

        print "Gerrit Server: ".$server."\n" if $debug;
        print "Gerrit Project: ".$project."\n" if $debug;

        open(UNUSED, ">".config_filename()) || die;
        close UNUSED;

        system("git config --file ".config_filename().
               " --add releaseLevels.server $server");
        system("git config --file ".config_filename().
               " --add releaseLevels.project $project");
    }
    $globals{config_init} = 1;

}

# sub config_list_levels
#
# Determines the previously defined release-levels.
#
# @return hash - { level => 1 } for each defined level.
#
sub config_list_levels
{
    return $globals{config_list_levels}
        if (defined $globals{config_list_levels});

    config_init();

    open COMMAND, "git config --file ".config_filename().
                  " --get-all releaseLevels.levelname |";
    my $names = {};
    while (my $line = <COMMAND>)
    {
        chomp $line;
        $names->{$line} = 1;
    }
    close COMMAND;

    $globals{config_list_levels} = $names;
    return $names;
}

# sub config_add_level
#
# Add a new level definition to the config file.
#
# @param level_def - A level info with the name/base/released for the new level.
#
sub config_add_level
{
    config_init();

    my $level_def = shift;
    my $levels = config_list_levels();

    if (defined $levels->{$level_def->{name}})
    {
        die "Level ".$level_def->{name}." is already defined";
    }

    system("git config --file ".config_filename().
           " --add releaseLevels.levelname ".$level_def->{name});

    system("git config --file ".config_filename().
           " --add level.".$level_def->{name}.".base ".$level_def->{base});

    system("git config --file ".config_filename().
           " --add level.".$level_def->{name}.".released ".
           $level_def->{released});

    if ($globals{"branch"} ne "master")
    {
        system("git config --file ".config_filename().
               " --add level.".$level_def->{name}.".branch ".
               $globals{"branch"});
    }
}

# sub config_del_level
#
# Delete a level definition from the config file.
#
# @param level - The level name to delete.
#
sub config_del_level
{
    config_init();

    my $level = shift;

    system("git config --file ".config_filename().
           " --unset releaseLevels.levelname ^".$level."\$");

    system("git config --file ".config_filename().
           " --remove-section level.".$level);
}

# sub config_add_patch
#
# Add a patch to a level definition.
#
# @param level - The level to add patch to.
# @param patch - The patch to add.
#
sub config_add_patch
{
    my $level = shift;
    my $patch = shift;

    config_get_level($level);

    unless (gerrit_is_patch($patch))
    {
        $patch = git_resolve_ref($patch);
    }
    die "Unknown patch requested" if ($patch eq "");

    system("git config --file ".config_filename().
           " --add level.$level.patch $patch");
}

# sub config_add_dep
#
# Add a forced dependency to a level definition.
#
# @param level - The level to add to.
# @param from - The decendent patch.
# @param to - THe ancestor patch.
#
sub config_add_dep
{
    my $level = shift;
    my $from = shift;
    my $to = shift;

    config_get_level($level);

    unless (gerrit_is_patch($from))
    {
        $from = git_resolve_ref($from);
    }
    die "Unknown patch requested for 'from' dep" if ($from eq "");

    unless (gerrit_is_patch($to))
    {
        $to = git_resolve_ref($to);
    }
    die "Unknown patch requested for 'to' dep" if ($to eq "");

    system("git config --file ".config_filename().
           " --add level.$level.forceDep $from:$to");
}

# sub config_get_level
#
# Reads a level's information from the config file.
#
# @param level - The level to read.
#
# @return hash - { name => level, base => base release,
#                  released => previous release,
#                  patches => array of patches,
#                  forceDep => hash of { from => to } pairs }.
#
sub config_get_level
{
    config_init();

    my $level = shift;
    my %level_data = ();

    open COMMAND, "git config --file ".config_filename().
                  " --get releaseLevels.levelname $level |";
    my $found_level = <COMMAND>; chomp($found_level);
    close COMMAND;

    die "Level $level not defined" if ($found_level eq "");

    $level_data{name} = $level;

    open COMMAND, "git config --file ".config_filename().
                  " --get level.$level.base |";
    my $base = <COMMAND>; chomp($base);
    close COMMAND;

    $level_data{base} = $base;

    open COMMAND, "git config --file ".config_filename().
                  " --get level.$level.released |";
    my $released = <COMMAND>; chomp($released);
    close COMMAND;

    $level_data{released} = $released;

    open COMMAND, "git config --file ".config_filename().
                  " --get level.$level.branch |";
    my $branch = <COMMAND>; chomp($branch);
    close COMMAND;

    if ($branch ne "")
    {
        $globals{"branch"} = $branch;
    }

    my @patches = ();
    open COMMAND, "git config --file ".config_filename().
                  " --get-all level.$level.patch |";
    while (my $patch = <COMMAND>)
    {
        chomp($patch);
        push @patches, $patch;
    }
    close COMMAND;

    $level_data{patches} = \@patches;

    my %forceDeps = ();
    open COMMAND, "git config --file ".config_filename().
                  " --get-all level.$level.forceDep |";
    while (my $forceDep = <COMMAND>)
    {
        $forceDep =~ m/(.*):(.*)/;
        $forceDeps{$1} = $2;
    }
    close COMMAND;

    $level_data{forceDeps} = \%forceDeps;

    return \%level_data;
}

# sub config_print_levels
#
# Displays the name of each defined level.
#
sub config_print_levels
{
    my $levels = config_list_levels();
    foreach my $level (sort keys %$levels)
    {
        print $level."\n";
    }
}

# sub config_server
#
# Gets the Gerrit server name / address from the config file.
#
# @return string - The location of the Gerrit server.
#
sub config_server
{
    return $globals{config_server} if (defined $globals{config_server});

    config_init();

    open COMMAND, "git config --file ".config_filename().
                  " --get releaseLevels.server |";
    my $server = <COMMAND>; chomp($server);
    close COMMAND;

    die "Server config does not exist" if ($server eq "");

    $globals{config_server} = $server;
    return $server;

}

# sub config_project
#
# Gets the Gerrit project managed by this repository from the config file.
#
# @return string - Project managed by this repository.
#
sub config_project
{
    return $globals{config_project} if (defined $globals{config_project});

    config_init();

    open COMMAND, "git config --file ".config_filename().
                  " --get releaseLevels.project |";
    my $project = <COMMAND>; chomp($project);
    close COMMAND;

    die "Project config does not exist" if ($project eq "");

    $globals{config_project} = $project;
    return $project;
}

# sub config_resolve_level_dep
#
# Resolves dependencies for patches by parsing the commit messages for the
# depends-on tag and checking if there are any open parents of a commit.
# If a patch is dependent on a patch not already in the level, the patch is
# added.
#
# @param[in] - level name
# @param[in] - Array of patches to process.
#
# @TODO RTC:125235 - improve this to support cross project dependencies
sub config_resolve_level_dep
{
    print "Resolving level dependencies...\n";
    my $level = shift;
    my $base = shift;
    my @patches = @_;
    my %level_patches = ();

    while (@patches)
    {
        my $patchPair = shift @patches;
        my ($patch,$patchSet) = split (":", $patchPair);

        # Check if patch has already been added to level
        if ($level_patches{$patch})
        {
            print "Skipping - already added patch = $patch to level\n" if $debug;
            next;
        }
        # Check if patch already exists in release base
        if (git_log_changeId($base, $patch))
        {
            print "Skipping - patch = $patch already exists in release base = $base\n" if $debug;
            next;
        }

        # Mark patch as processed
        $level_patches{$patch} = 1;

        print "\n===========\nFirst time seeing patch = $patch\n" if $debug;

        # Force use of changeId's
        if (!gerrit_is_patch($patch))
        {
            die "Added patch: $patch is not of type changeId\n";
        }

        # Add patch to level with resolved git commit.
        print "Adding patch - $patchPair\n" if $debug;
        my $commits = gerrit_resolve_patchset([$patchPair]);
        config_add_patch($level, $commits->[0]);

        # Get commit message
        my $patchInfo = gerrit_query_commit($patch);
        my @commitMsgArray = split(/\\n/,$patchInfo->{commitMessage});
        print Dumper @commitMsgArray if $debug;

        # Check for OPEN parent
        my $commit_info = gerrit_query_commit($patch);
        my $parent_commit = $commit_info->{currentPatchSet}->{parents}[0];
        my $parent_info = gerrit_query_commit($parent_commit);
        if ($parent_info->{status} eq "NEW")
        {
            my $parent_id = $parent_info->{id};
            # Add dependency if dependency is not already in base release
            if(!git_log_changeId($base, $parent_id))
            {
                print "Adding forced dependency $patch:$parent_id\n" if $debug;
                config_add_dep($level, $patch, $parent_id);
            }

            # Add dependent patch if not already added to level
            if (!exists($level_patches{$parent_id}) )
            {
                push @patches, $parent_id;
            }
        }

        # Search commit message for dependencies
        foreach my $line (@commitMsgArray)
        {
            # Check for forced dependencies
            if ($line =~ m/depends-on:/i)
            {
                $line =~ s/([^:]*):\s*//;
                chomp($line);
                print "Found depends-on: $line\n" if $debug;

                # Add dependency if dependency is not already in base release
                if(!git_log_changeId($base, $line))
                {
                    print "Adding forced dependency $patch:$line\n" if $debug;
                    config_add_dep($level, $patch, $line);
                }

                # Add dependent patch if not already added to level
                if (!exists($level_patches{$line}) )
                {
                    push @patches, $line;
                }
            }
            # Print out CMVC dependencies
            if ($line =~ m/cmvc-([a-zA-Z]+):/i)
            {
                print "$TOKEN Need ".$line."\n";
            }
        }
    }
}

# sub config_verify_patches
#
# Verify patch-list to ensure all dependencies are met
#
# @param[in] - level base patch
# @param[in] - Array of patches to verify.
#
sub config_verify_patches
{
    print "Verifying patches...\n";

    config_init();

    my $base = shift;
    my $patches = shift;

    foreach my $patch (@{$patches})
    {
        print "Deps for $patch\n" if $debug;
        my $displayed_header = 0;

        my $deps = git_commit_deps($base, $patch);

        foreach my $dep (@{$deps})
        {
            unless (grep {$_ eq $dep} @{$patches})
            {
                unless ($displayed_header)
                {
                    print "-------------------------------------------------\n";
                    print "Potential missing dependency for:\n";
                    print wrap("    ","    ",git_get_subject($patch)."\n");
                    print "\t$patch\n\n";
                    $displayed_header = 1;
                }

                print wrap("    ", "    ", git_get_subject($dep)."\n");
                print "\t$dep\n";

                my $files = array_intersect(git_commit_files($patch),
                                            git_commit_files($dep));

                foreach my $file (@{$files})
                {
                    print "\t$file\n";
                }

                print "\n";
            }
        }

        if ($displayed_header)
        {
            print "-------------------------------------------------\n";
        }
    }

}

# sub config_release
#
# Create a branch / tag based on the definition of a release.
#
# @param[in] - level info
# @param[in] - bool to create tag
#
sub config_release
{
    my $level_info = shift;
    my $create_tag = shift;

    print "Creating release branch...\n";
    git_create_branch($level_info->{name}, $level_info->{base});

    my $patches = $level_info->{patches};

    print "Resolving and ordering patches...\n";
    print Dumper $level_info->{patches} if $debug;
    $patches = gerrit_resolve_patches($level_info->{patches});
    $patches = git_order_commits($patches, $level_info);

    print "\n========\nDetermined patch order as:\n";
    my $i = 1;
    foreach my $patch (@{$patches})
    {
        print "$i. $patch\n";
        $i++;
    }

    print "\n========\nApplying patches...\n";
    $i = 1;
    foreach my $patch (@{$patches})
    {
        print "\n$i. Cherry-picking commit = $patch.\n\n";
        unless (git_cherry_pick($patch))
        {
            print `git status`;
            system("git reset HEAD --hard");
            die "Cherry-pick of $patch failed";
        }
        $i++;
    }

    print "\nGenerating release notes...\n";
    create_release_notes($level_info->{name}, $level_info);

    if ($create_tag)
    {
        print "\nCreating tag...\n";
        git_create_tag($level_info->{name}, $level_info);
    }
}

# sub gerrit_ssh_command
#
# Creates a properly formed ssh command based on the server address.
#
# @return string - The basic ssh command to connect to the server.
#
sub gerrit_ssh_command
{
    return $globals{gerrit_ssh_command}
        if (defined $globals{gerrit_ssh_command});

    my $server = config_server();
    my $port = "";

    if ($server =~ m/.*:.*/)
    {
        $port = $server;
        $server =~ s/(.*):.*/$1/;
        $port =~ s/.*:(.*)/$1/;

        $port = "-p $port";
    }

    my $command = "ssh -qx $port $server gerrit";
    print "SSH command: $command\n" if $debug;

    $globals{gerrit_ssh_command} = $command;
    return $command;
}

# sub gerrit_query
#
# Performs a gerrit query and parses the resulting JSON.
#
# @param[in] query - The query to perform.
#
# @return array - A list of items from the JSON query.  Each item is a
#                 hash (key-value pair) for the item attributes.
#
sub gerrit_query
{
    my $query = shift;

    my @items = ();

    open COMMAND, gerrit_ssh_command()." query $query --current-patch-set".
                  " --patch-sets  --format=JSON |";

    while (my $line = <COMMAND>)
    {
        chomp $line;
        push @items, json_parse($line);
    }

    return \@items;
}

# sub gerrit_query_commit
#
# Performs a gerrit query on a specific commit.
#
# @param[in] commit - The commit to query.
#
# @return hash - The parsed JSON for the queried commit.
#
sub gerrit_query_commit
{
    my $commit = shift;

    my $project = config_project();

    my $query_result = gerrit_query("$commit project:$project ".
                                    "branch:".$globals{"branch"});
    foreach my $result (@{$query_result})
    {
        if ($result->{id} eq $commit  ||
            $result->{currentPatchSet}->{revision} =~ m/$commit/)
        {
            return $result;
        }
        else
        {
            # If all patchsets queried, search all of them for the commit
            foreach my $patchset (@{$result->{patchSets}})
            {
                if ($patchset->{revision} =~ m/$commit/)
                {
                    return $result;
                }
            }
        }
    }

    die "Cannot find $commit in $project/$globals{\"branch\"}";
}

# sub gerrit_is_patch
#
# Determines if a patch identifier is a Gerrit patch or not.
#
# @param[in] patch - The patch to make determination about.
#
# @retval true - Patch is a Gerrit patch ID.
# @retval false - Patch does not appear to be a Gerrit patch ID.
sub gerrit_is_patch
{
    my $patch = shift;
    return 1 if ($patch =~ m/I[0-9a-f]+/);
    return 0;
}

# sub gerrit_resolve_patches
#
# Resolves gerrit patch IDs to git commit numbers and ensures the git
# commits are fetched from the gerrit server.
#
# Any git commit number is left unchanged.
#
# @param[in] patches - An array of patches.
# @return array - An array of git commit numbers.
#
sub gerrit_resolve_patches
{
    my $patches = shift;
    my @result = ();

    foreach my $patch (@{$patches})
    {
        if (gerrit_is_patch($patch))
        {
            my $patch_info = gerrit_query_commit($patch);
            gerrit_fetch($patch_info->{currentPatchSet}->{ref});
            push @result, $patch_info->{currentPatchSet}->{revision};
        }
        else
        {
            push @result, $patch;
        }
    }

    return \@result;
}

# sub gerrit_resolve_patchset
#
# Resolves an array of gerrit change-id and patch-set pairs to git commit
# numbers and and ensures the git commits are fetched from the gerrit server.
#
# @param[in] patches - An array of change-id, patch-set pairs.
# @return array - An array of git commit numbers.
#
sub gerrit_resolve_patchset
{
    my $patches = shift;

    my @result = ();
    foreach my $patchPair (@{$patches})
    {
        my ($changeId,$patchSet) = split(":",$patchPair);

        if (gerrit_is_patch($changeId))
        {
            my $patch_info = gerrit_query_commit($changeId);
            # Fail if patchset DNE
            if ($patchSet > $patch_info->{currentPatchSet}->{number})
            {
                die "$patchSet does not have patch number $patchSet";
            }
            # JSON creates array of patchSets in number order
            my $index = $patchSet - 1;
            gerrit_fetch($patch_info->{patchSets}[$index]->{ref});
            push @result, $patch_info->{patchSets}[$index]->{revision};
        }
        else
        {
            die "Requires gerrit change-id and patch-set";
        }
    }

    return \@result;
}

# sub gerrit_fetch
#
# Fetches the contents of a Gerrit revision (refs/changes/*) to the local
# git repository.
#
# @param[in] ref - The revision to fetch from the Gerrit server.
#
sub gerrit_fetch
{
    my $ref = shift;
    open COMMAND, "git config --list | grep remote.*ssh |";
    my $projecturl = <COMMAND>;
    close COMMAND;
    chomp $projecturl;
    $projecturl =~ s/(.*?)\=(.*)/$2/;
    system("git fetch $projecturl $ref -q");
}

# sub rtc_workitem_num
#
# Determines the RTC WorkItem associated with a git commit.
#
# @param[in] commit - The git commit.
#
# @return string - RTC WorkItem number (or "").
#
sub rtc_workitem_num
{
    my $commit = shift;
    my $message = git_commit_msg($commit);

    if ($message =~ m/RTC:\s*([0-9]+)/)
    {
        return $1;
    }
    else
    {
        return "";
    }
}

# sub cq_workitem_num
#
# Determine the CQ WorkItem associated with a git commit.
#
# @param[in] commit - The git commit.
#
# @return string - CQ WorkItem number (or "").
#
sub cq_workitem_num
{
    my $commit = shift;
    my $message = git_commit_msg($commit);

    if ($message =~ m/CQ:\s*([A-Z][A-Z][0-9]+)/)
    {
        return $1;
    }
    else
    {
        return "";
    }
}

# sub coreq_workitem_num
#
# Search through a git commit for all coReq instances.
#
# @param[in] commit - The git commit.
#
# @return array of strings - CMVC-Coreq numbers or "".
#

sub coreq_workitem_num
{
    my $commit = shift;

    my @msg_lines = split('\n',git_commit_msg($commit));
    my @coreqs = ();

    foreach my $line (@msg_lines)
    {
        if ($line =~ m/CMVC-Coreq:\s*([0-9]+)/i)
        {
            push @coreqs, $1;
        }
    }
    return @coreqs;
}

# sub prereq_workitem_num
#
# Search through a git commit for all preReq instances.
#
# @param[in] commit - The git commit.
#
# @return array of strings - CMVC-Prereq numbers or "".
#

sub prereq_workitem_num
{
    my $commit = shift;

    my @msg_lines = split('\n',git_commit_msg($commit));
    my @prereqs = ();

    foreach my $line (@msg_lines)
    {
        if($line =~  m/CMVC-Prereq:\s*([0-9]+)/i)
        {
            push @prereqs, $1;
        }
    }
    return @prereqs;
}

# sub gerrit_changeid_num
#
# Determine the Gerrit Change-Id associated with a git commit.
#
# @param[in] commit - The git commit.
#
# @return string - Gerrit Change-Id number (or "").
#
sub gerrit_changeid_num
{
    my $commit = shift;
    my $message = git_commit_msg($commit);

    if ($message =~ m/Change-Id:\s*(I[0-9a-z]+)/)
    {
        return $1;
    }
    else
    {
        return "";
    }
}


# sub rtc_hyperlink
#
# Turn an RTC WorkItem number into the https:// address to the RTC server.
#
# @param[in] workitem - RTC workitem number.
#
# @return string - The https:// address of the RTC item on the server.
#
sub rtc_hyperlink
{
    my $workitem = shift;
    return "https://jazz07.rchland.ibm.com:13443/jazz/oslc/workitems/".
           "$workitem.hover.html";
}

# sub cq_hyperlink
#
# Turn a CQ WorkItem number into the http:// address to the BQ server.
#
# @param[in] workitem - CQ workitem number.
#
# @return string - The http:// address of the CQ item on the server.
#
sub cq_hyperlink
{
    my $workitem = shift;
    return "http://w3.rchland.ibm.com/projects/bestquest/?defect=$workitem";
}

# sub json_parse
#
# Parse a line of JSON into an hash-object.
#
# @param[in] line - The JSON content.
#
# @return hash - The parsed object.
#
# @note There are perl modules for doing this but they are not installed on
#       the pool machines.  The parsing for JSON (at least the content from
#       the Gerrit server) isn't so bad...
#
sub json_parse
{
    my $line = shift;

    die "Invalid JSON format: $line" unless ($line =~ m/^\{.*\}$/);
    $line =~ s/^\{(.*)}$/$1/;

    my %object = ();

    while($line ne "")
    {
        my $key;
        my $value;

        ($key, $line) = json_get_string($line);
        $key =~ s/^"(.*)"$/$1/;

        $line =~ s/^://;
        if ($line =~ m/^"/)
        {
            ($value, $line) = json_get_string($line);
            $value =~ s/^"(.*)"$/$1/;
        }
        elsif ($line =~ m/^{/)
        {
            ($value, $line) = json_get_object($line);
            $value = json_parse($value);
        }
        elsif ($line =~ m/^\[/)
        {
            ($value, $line) = json_get_array($line);
            $value = json_parse_array($value);
        }
        else
        {
            $line =~ s/([^,]*)//;
            $value = $1;
        }

        $object{$key} = $value;
    }

    return \%object;
}

# sub json_parse_array
#
# Utility function for json_parse.
#
sub json_parse_array
{
    my $line = shift;

    $line =~ s/^\[(.*)\]$/$1/;

    my @array = ();

    while ($line ne "")
    {
        my $value;

        if ($line =~ m/^"/)
        {
            ($value, $line) = json_get_string($line);
            $value =~ s/^"(.*)"$/$1/;
        }
        elsif ($line =~ m/^\{/)
        {
            ($value, $line) = json_get_object($line);
            $value = json_parse($value);
        }
        elsif ($line =~ m/^\[/)
        {
            ($value, $line) = json_get_array($line);
            $value = json_parse_array($value);
        }
        else
        {
            $line =~ s/([^,]*)//;
            $value = $1;
        }

        push @array, $value;
        $line =~ s/^,//;
    }

    return \@array;
}

# sub json_get_string
#
# Utility function for json_parse.
#
sub json_get_string
{
    my $line = shift;

    $line =~ /("[^"]*")(.*)/;
    my $first = $1;
    my $second = $2;

    if ($first =~ m/\\"$/)
    {
        my ($more, $rest) = json_get_string($second);
        return ($first.$more , $rest);
    }
    else
    {
        return ($first, $second);
    }
}

# sub json_get_object
#
# Utility function for json_parse.
#
sub json_get_object
{
    my $line = shift;

    $line =~ s/^{//;
    my $object = "{";
    my $frag = "";

    my $found_object = 0;

    until ((not $found_object) && ($object =~ m/}$/))
    {
        $found_object = 0;

        if ($line =~ m/^\{/)
        {
            ($frag, $line) = json_get_object($line);
            $object = $object.$frag;
            $found_object = 1;
        }
        elsif ($line =~ m/^"/)
        {
            ($frag, $line) = json_get_string($line);
            $object = $object.$frag;
        }
        elsif ($line =~ m/^\[/)
        {
            ($frag, $line) = json_get_array($line);
            $object = $object.$frag;
        }
        elsif ($line =~ m/^[:,}]/)
        {
            $line =~ s/^([:,}])//;
            $frag = $1;
            $object = $object.$frag;
        }
        else
        {
            $line =~ s/([^,}]*)//;
            $frag = $1;
            $object = $object.$frag;
        }
    }

    return ($object, $line);
}

# sub json_get_array
#
# Utility function for json_parse.
#
sub json_get_array
{
    my $line = shift;

    $line =~ s/^\[//;
    my $array = "[";
    my $frag = "";

    my $found_array = 0;

    until ((not $found_array) && ($array =~ m/]$/))
    {
        $found_array = 0;

        if ($line =~ m/^\[/)
        {
            ($frag, $line) = json_get_array($line);
            $array = $array.$frag;
            $found_array;
        }
        elsif ($line =~ m/^\{/)
        {
            ($frag, $line) = json_get_object($line);
            $array = $array.$frag;
        }
        elsif ($line =~ m/^"/)
        {
            ($frag, $line) = json_get_string($line);
            $array = $array.$frag;
        }
        elsif ($line =~ m/^[:,\]]/)
        {
            $line =~ s/^([:,\]])//;
            $frag = $1;
            $array = $array.$frag;
        }
        else
        {
            $line =~ s/([^,]*)//;
            $frag = $1;
            $array = $array.$frag;
        }
    }

    return ($array, $line);
}

# sub array_intersect
#
# Perform set intersection on two arrays.
#
# @param[in] one - The first array.
# @param[in] two - The second array.
#
# @return array - The set intersection.
#
sub array_intersect
{
    my $one = shift;
    my $two = shift;

    my %set = {};

    map { $set{$_}++ } (@{$one}, @{$two});

    my @result = map { ($set{$_} > 1) ? $_ : () } (keys %set);

    return \@result;
}

# sub run_system_command
#
# Execute a system command, handle printing command and debug info, and return
# system output for caller processing
#
# E.g. Execute a CMVC line command and return the results.
#
# @param[in] cmd - system command to be executed.
#
# @return string - output returned from running system command.
#
sub run_system_command
{
    my $cmd = shift;

    print "$cmd\n";
    my $output = `$cmd`;
    die "failed running system command $cmd - $?" if ($?);
    print $output if $debug;

    return $output;
}
