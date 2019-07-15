#!/usr/bin/perl
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/scripts/ppeSetFixed.pl $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2018
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
# Purpose:  This perl script will parse the attribute and default list and
# and set the default values into the image.

use strict;

#------------------------------------------------------------------------------
# Print Command Line Help
#------------------------------------------------------------------------------
my $numArgs = $#ARGV + 1;
if ($numArgs < 3)
{
    print ("Usage: ppeSetFixed.pl <image> <attributes and default list> <attribute file> ...\n");
    print ("  This perl script will the attributes and default list to lookup the defaults\n");
    print ("  and parse the attribute file to lookup the types.\n");
    print ("  The default values will be set in the image.\n");
    print ("example:\n");
    print ("./ppeSetFixed.pl \\\n" );
    print (". \\\n" );
    print (" ../../obj/seeprom_main.bin \\\n" );
    print ("p9_ppe_attributes.xml \\\n" );
    print ("perv_attributes.xml \\\n" );
    print ("proc_attributes.xml \\\n" );
    print ("ex_attributes.xml \\\n" );
    print ("eq_attributes.xml \\\n" );
    print ("core_attributes.xml \\ \n");
    print ("nest_attributes.xml \\ \n");
    print ("ocmb_attributes.xml \n");
    exit(1);
}

#------------------------------------------------------------------------------
# Specify perl modules to use
#------------------------------------------------------------------------------
use XML::Simple;
my $xml = new XML::Simple (KeyAttr=>[]);


my $xmlFiles = 0;
my $attCount = 0;
my $numIfAttrFiles = 0;
my @attrSystemIds;
my @attrChipIds;
my @attrExIds;
my @attrCoreIds;
my @attrEqIds;
my @attrPervIds;
my @attrOcmbChipIds; 



#------------------------------------------------------------------------------
# Element names
#------------------------------------------------------------------------------
my $attribute = 'attribute';

#------------------------------------------------------------------------------
# For each argument
#------------------------------------------------------------------------------
my $sbedefaultpath = $ARGV[0];
my $image = $ARGV[1];
my $argfile = $ARGV[2];
my $entries = $xml->XMLin($argfile, ForceArray => ['entry']);

if ( ! -e $image) {die "ppeSetFixed.pl: $image $!"};

foreach my $entr (@{$entries->{entry}}) {

    # Skip virtual attributes
    if(exists $entr->{virtual})
    {
        next;
    }

    my $inname = $entr->{name};

    # read XML file. The ForceArray option ensures that there is an array of
    # elements even if there is only one such element in the file

    foreach my $argnum (3 .. $#ARGV)
    {
        my $infile = $ARGV[$argnum];

        if ( ! -e $infile) {die "ppeSetFixed.pl: $infile $!"};

        my $attributes = $xml->XMLin($infile, ForceArray => ['attribute']);

        #--------------------------------------------------------------------------
        # For each Attribute
        #--------------------------------------------------------------------------
        foreach my $attr (@{$attributes->{attribute}})
        {
            if($attr->{id} eq $inname) {

                #------------------------------------------------------------------
                # Check that the AttributeId exists
                #------------------------------------------------------------------
                if (! exists $attr->{id})
                {
                    print ("ppeSbeFixed.pl ERROR. Att 'id' missing\n");
                    exit(1);
                }

                my @targets = split(",", $attr->{targetType});

                my $targetTypeMatched = 0;

                foreach my $target (@targets)
                {
                    # Remove newlines and leading/trailing whitespace
                    $target =~ s/\n//;
                    $target =~ s/^\s+//;
                    $target =~ s/\s+$//;

                    if($target eq "TARGET_TYPE_SYSTEM") {

                        push(@attrSystemIds, $entr);
                        $targetTypeMatched = 1;
                        last;

                    } elsif($target eq "TARGET_TYPE_PROC_CHIP") {

                        push(@attrChipIds, $entr);
                        $targetTypeMatched = 1;
                        last;

                    } elsif($target eq "TARGET_TYPE_OCMB_CHIP") {

                        push(@attrOcmbChipIds, $entr);
                        $targetTypeMatched = 1;
                        last;

                    } elsif($target eq "TARGET_TYPE_CORE") {

                        push(@attrCoreIds, $entr);
                        $targetTypeMatched = 1;
                        last;

                    } elsif($target eq "TARGET_TYPE_EQ") {

                        push(@attrEqIds, $entr);
                        $targetTypeMatched = 1;
                        last;

                    } elsif($target eq "TARGET_TYPE_EX") {

                        push(@attrExIds, $entr);
                        $targetTypeMatched = 1;
                        last;

                    } elsif($target eq "TARGET_TYPE_PERV") {

                        push(@attrPervIds, $entr);
                        $targetTypeMatched = 1;
                        last;

                    } else {

                        print ("ppeSetFixed.pl WARNING. Ignoring unsupported".
                            " target type: $target for attribute: $inname\n");
                        next;

                    }
                }
                if($targetTypeMatched eq 0)
                {
                    print ("ppeSetFixed.pl ERROR. No matching target type ".
                        "found for attribute: $inname\n");
                    exit(1);
                }

            }
        }
    }
}


setFixed("TARGET_TYPE_SYSTEM", @attrSystemIds);
setFixed("TARGET_TYPE_PROC_CHIP", @attrChipIds);
setFixed("TARGET_TYPE_CORE", @attrCoreIds);
setFixed("TARGET_TYPE_EQ", @attrEqIds);
setFixed("TARGET_TYPE_EX", @attrExIds);
setFixed("TARGET_TYPE_PERV", @attrPervIds);
setFixed("TARGET_TYPE_OCMB_CHIP", @attrOcmbChipIds);



sub setFixed {

    my ($string, @entries) =  @_;

    foreach my $attr (@entries)
    {

        my $inname = $attr->{name};

        my @values = $attr->{value};


        if(scalar @values > 0) {
            foreach my $val (@values)
            {

                if(defined $val && ref($val) eq "") {

                    if ($val =~ /(0x)?[0-9a-fA-F]+/) {

                        my $systemRc = system("$sbedefaultpath/sbe_default_tool $image $inname $val $string 0");

                        if ($systemRc) {
                            print "sbe_default_tool: error in execution\n";
                            exit 1;
                        }

                    } else {
                        print ("ppeSetFixed.pl ERROR. not hex\n");
                        exit(1);
                    }

                } elsif(defined $val && ref($val) eq "ARRAY") {

                    my $index = 0;

                    foreach my $arr (@{$val}) {

                        if(defined $arr && ref($arr) eq "") {
                            if ($arr =~ /(0x)?[0-9a-fA-F]+/) {

                                my $systemRc = system("$sbedefaultpath/sbe_default_tool $image $inname $arr $string $index");

                                if ($systemRc) {
                                    print "sbe_default_tool: error in execution\n";
                                    exit 1;
                                }


                            }
                        }
                        $index++;
                    }
                }
            }
        }
    }
}


