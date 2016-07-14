#!/usr/bin/perl
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: sbe/build/tools/perl.modules/gitUtil.pm $
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

package gitUtil;

use strict;

my %globals = {};

# Function     : gitRoot
#
# @brief       : Determine the root of the GIT repository
#
# @return root : Root of GIT repository
#
sub gitRoot
{
    return $globals{git_root} if (defined $globals{git_root});

    open COMMAND, "git rev-parse --show-toplevel |";
    my $root = <COMMAND>;
    close COMMAND;
    chomp $root;

    die "Unable to determine git_root" if ($root eq "");

    $globals{git_root} = $root;
    return $root;
}

#################### Begin Gerrit JSON Utility Subroutines #####################

# @note There are perl modules for doing this but they are not installed on
#       the pool machines.  The parsing for JSON (at least the content from
#       the Gerrit server) isn't so bad...

# Function        : jsonParse
#
# @brief          : Parse a line of JSON into an hash-object.
#
# @param[in] line : The JSON content.
#
# @return hash    : The parsed object.
#
sub jsonParse
{
    my $line = shift;

    die "Invalid JSON format: $line" unless ($line =~ m/^\{.*\}$/);
    $line =~ s/^\{(.*)}$/$1/;

    my %object = ();

    while($line ne "")
    {
        my $key;
        my $value;

        ($key, $line) = jsonGetString($line);
        $key =~ s/^"(.*)"$/$1/;

        $line =~ s/^://;
        if ($line =~ m/^"/)
        {
            ($value, $line) = jsonGetString($line);
            $value =~ s/^"(.*)"$/$1/;
        }
        elsif ($line =~ m/^{/)
        {
            ($value, $line) = jsonGetObject($line);
            $value = jsonParse($value);
        }
        elsif ($line =~ m/^\[/)
        {
            ($value, $line) = jsonGetArray($line);
            $value = jsonParseArray($value);
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

# Function        : jsonGetString
#
# @brief          : Utility function for jsonParse that extracts
#                   the string data in a given object
#
# @param[in] line : The JSON line containing the strings.
#
# @return strings : The parsed strings.
#
sub jsonGetString
{
    my $line = shift;

    $line =~ /("[^"]*")(.*)/;
    my $first = $1;
    my $second = $2;

    if ($first =~ m/\\"$/)
    {
        my ($more, $rest) = jsonGetString($second);
        return ($first.$more , $rest);
    }
    else
    {
        return ($first, $second);
    }
}

# Function        : jsonGetObject
#
# @brief          : Utility function for jsonParse that extracts
#                   the nested JSON object data in a given object
#
# @param[in] line : The JSON line containing the object
#
# @return  object : The nested object
#
sub jsonGetObject
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
            ($frag, $line) = jsonGetObject($line);
            $object = $object.$frag;
            $found_object = 1;
        }
        elsif ($line =~ m/^"/)
        {
            ($frag, $line) = jsonGetString($line);
            $object = $object.$frag;
        }
        elsif ($line =~ m/^\[/)
        {
            ($frag, $line) = jsonGetArray($line);
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

# Function        : jsonGetArray
#
# @brief          : Utility function for jsonParse that extracts
#                   the array in a given object
#
# @param[in] line : The JSON line containing the array
#
# @return  array  : The array object
#
sub jsonGetArray
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
            ($frag, $line) = jsonGetArray($line);
            $array = $array.$frag;
            $found_array;
        }
        elsif ($line =~ m/^\{/)
        {
            ($frag, $line) = jsonGetObject($line);
            $array = $array.$frag;
        }
        elsif ($line =~ m/^"/)
        {
            ($frag, $line) = jsonGetString($line);
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

# Function        : jsonParseArray
#
# @brief          : Utility function for jsonParse that parses
#                   the array object
#
# @param[in] line : The array
#
# @return  array  : The parsed array object
#
#
sub jsonParseArray
{
    my $line = shift;

    $line =~ s/^\[(.*)\]$/$1/;

    my @array = ();

    while ($line ne "")
    {
        my $value;

        if ($line =~ m/^"/)
        {
            ($value, $line) = jsonGetString($line);
            $value =~ s/^"(.*)"$/$1/;
        }
        elsif ($line =~ m/^\{/)
        {
            ($value, $line) = jsonGetObject($line);
            $value = jsonParse($value);
        }
        elsif ($line =~ m/^\[/)
        {
            ($value, $line) = jsonGetArray($line);
            $value = jsonParseArray($value);
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

#################### End Gerrit JSON Utility Subroutines #######################

# Function           : gerritIsPatch
#
# @brief             : Determine if patch identifier is a Gerrit patch or not.
#
# @param[in] i_patch : The patch to make determination about.
#
# @retval flag       : true/false (patch is/not a valid ID)
#
sub gerritIsPatch
{
    my $patch = shift;
    return 1 if ($patch =~ m/I[0-9a-f]+/);
    return 0;
}

# Function : configFilename
#
# @brief   : Create the file that stroes the GIT server details
#
# @return  : Location of the config file
#
sub configFilename
{
    return gitRoot()."/.git/gitRelease.config";
}

# Function : configInit
#
# @brief   : Fetch & write server details to the config file
#
sub configInit
{
    return if (defined $globals{configInit});

    unless (-e configFilename())
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
        # eg: remote.gerrit.url=ssh://hw.gerrit/hw/ppe
        # $2 is 'hw.gerrit'
        # $3 is 'hw/ppe'
        $server =~ s/(.*)ssh:\/\/(.*?)\/(.*)/$2/;
        $project =~ s/(.*)ssh:\/\/(.*?)\/(.*)/$3/;

        open(UNUSED, ">".configFilename()) || die;
        close UNUSED;

        system("git config --file ".configFilename().
               " --add releaseLevels.server $server");
        system("git config --file ".configFilename().
               " --add releaseLevels.project $project");
    }
    $globals{configInit} = 1;
}

# Function : configProject
#
# @brief   : Fetch the project name of the current configured repository
#
# @return  : GIT project name
#
sub configProject
{
    return $globals{config_project} if (defined $globals{config_project});

    configInit();

    open COMMAND, "git config --file ".configFilename().
                  " --get releaseLevels.project |";
    my $project = <COMMAND>; chomp($project);
    close COMMAND;

    die "Project config does not exist" if ($project eq "");

    $globals{config_project} = $project;

    return $project;
}

# Function : configServer
#
# @brief   : Fetch the server name of the current configured repository
#
# @return  : GIT server location
#
sub configServer
{
    return $globals{config_server} if (defined $globals{config_server});

    configInit();


    open COMMAND, "git config --file ".configFilename().
                  " --get releaseLevels.server |";
    my $server = <COMMAND>; chomp($server);
    close COMMAND;

    die "Server config does not exist" if ($server eq "");

    $globals{config_server} = $server;
    return $server;

}

# Function : gerritSSHCommand
#
# @brief   : Creates a properly formed ssh command based on the server address
#
# @return  : The basic ssh command to connect to the server.
#
sub gerritSSHCommand
{
    return $globals{gerrit_ssh_command}
        if (defined $globals{gerrit_ssh_command});

    my $server = configServer();
    my $port = "";

    if ($server =~ m/.*:.*/)
    {
        $port = $server;
        $server =~ s/(.*):.*/$1/;
        $port =~ s/.*:(.*)/$1/;

        $port = "-p $port";
    }

    my $command = "ssh -qx $port $server gerrit";

    $globals{gerrit_ssh_command} = $command;
    return $command;
}

# Function         : gerritQuery
#
# @brief           : Performs a gerrit query and parses the resulting JSON.
#
# @param[in] query : The query to perform.
#
# @return  item    : A list of items from the JSON query.  Each item is a
#                    hash (key-value pair) for the item attributes.
#
sub gerritQuery
{
    my $query = shift;
    my @items = ();

    $query = gerritSSHCommand()." query $query --current-patch-set --patch-sets --format=JSON |";

    open COMMAND, $query;
    while (my $line = <COMMAND>)
    {
        chomp $line;
        push @items, jsonParse($line);
    }

    return \@items;
}

# Function               : gerritQueryReference
#
# @brief                 : Retrieves reference for a patch id, patchset number
#
# @param[in] changeId    : Change id of the patch
# @param[in] patchNumber : Patch set number
#
# @return  reference     : The reference string
#
sub gerritQueryReference
{
    my $changeId = shift;
    my $patchNumber = shift;

    my $project = configProject();

    my $query_result = gerritQuery("$changeId project:$project");

    foreach my $result (@{$query_result})
    {
        if ($result->{id} eq $changeId)
        {
            # If all patchsets queried, search all of them for the commit
            foreach my $patchset (@{$result->{patchSets}})
            {
                if ($patchNumber eq "")
                {
                    return $patchset->{currentPatchSet}->{ref};
                }
                else
                {
                    if ($patchset->{number} =~ m/$patchNumber/)
                    {
                        return $patchset->{ref};
                    }
                }
            }
        }
    }
    die "Cannot find $changeId in $project";
}

# Function               : gerritQueryCommit
#
# @brief                 : Retrieves commit for a patch id, patchset number
#
# @param[in] changeId    : Change id of the patch
# @param[in] patchNumber : Patch set number
#
# @return  commit        : The commit string
#
sub gerritQueryCommit
{
    my $changeId = shift;
    my $patchNumber = shift;

    my $project = configProject();

    my $query_result = gerritQuery("$changeId project:$project");

    foreach my $result (@{$query_result})
    {
        if ($result->{id} eq $changeId)
        {
            # If all patchsets queried, search all of them for the commit
            foreach my $patchset (@{$result->{patchSets}})
            {
                if ($patchNumber eq "")
                {
                    return $patchset->{currentPatchSet}->{revision};
                }
                else
                {
                    if ($patchset->{number} =~ m/$patchNumber/)
                    {
                        return $patchset->{revision};
                    }
                }
            }
        }
    }
    die "Cannot find $changeId in $project";
}
