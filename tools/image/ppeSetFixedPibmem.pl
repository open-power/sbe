#!/usr/bin/perl
# Purpose:  This perl script will parse the attribute and default list and
# and set the default values into the image.
# TODO via RTC 142365
# Remove this file once we have support to copy fixed section
# from SEEPROM to PIBMEM.

use strict;


#------------------------------------------------------------------------------
my $numArgs = $#ARGV + 1;
if ($numArgs < 2)
{
    print ("Usage: ppeSetFixedPibmem.pl <attributes and default list> <attribute file> ...\n");
    print ("  This perl script will the attributes and default list to lookup the defaults\n");
    print ("  and parse the attribute file to lookup the types.\n");
    print ("  The default values will be set in the image.\n");
    print ("  This is WORKAROUND\n");
    print (". \\\n" );
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



#------------------------------------------------------------------------------
# Element names
#------------------------------------------------------------------------------
my $attribute = 'attribute';

#------------------------------------------------------------------------------
# For each argument
#------------------------------------------------------------------------------
my $argfile = $ARGV[0];
my $entries = $xml->XMLin($argfile, ForceArray => ['entry']);

foreach my $entr (@{$entries->{entry}}) {

    my $inname = $entr->{name};
    # read XML file. The ForceArray option ensures that there is an array of
    # elements even if there is only one such element in the file

    foreach my $argnum (1 .. $#ARGV)
    {
        my $infile = $ARGV[$argnum];

        if ( ! -e $infile) {die "ppeSetFixedPibmem.pl: $infile $!"};

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

                if($attr->{targetType} eq "TARGET_TYPE_SYSTEM") {

                    push(@attrSystemIds, $entr);

                } elsif($attr->{targetType} eq "TARGET_TYPE_PROC_CHIP") {

                    push(@attrChipIds, $entr);

                } elsif($attr->{targetType} eq "TARGET_TYPE_CORE") {

                    push(@attrCoreIds, $entr);

                } elsif($attr->{targetType} eq "TARGET_TYPE_EQ") {

                    push(@attrEqIds, $entr);

                } elsif($attr->{targetType} eq "TARGET_TYPE_EX") { 

                    push(@attrExIds, $entr);

                } elsif($attr->{targetType} eq "TARGET_TYPE_PERV") { 

                    push(@attrPervIds, $entr);

                } else {

                    print ("ppeSetFixedPibmem.pl ERROR. Wrong attribute type: $attr->{targetType}\n");
                    exit(1);

                }
            }
        }
    }
}
my $file = "../sbefw/attr.C";
unless(open FILE, '>'.$file) {
#Die with error message 
             die "\nUnable to create $file\n";
            }
    #
print FILE "#include <fapi2.H>\n";
print FILE "extern fapi2attr::SystemAttributes_t G_system_attributes;\n";
print FILE "extern fapi2attr::ProcChipAttributes_t G_proc_chip_attributes;\n";
print FILE "extern fapi2attr::PervAttributes_t G_perv_attributes;\n";
print FILE "extern fapi2attr::CoreAttributes_t G_core_attributes;\n";
print FILE "extern fapi2attr::EQAttributes_t G_eq_attributes;\n";
print FILE "extern fapi2attr::EXAttributes_t G_ex_attributes;\n";

print FILE "void initAttrWA() \n";
print FILE "{ \n";
print FILE "////// workaround start \n";
print FILE "#ifndef SBE_SEEPROM_FIXED_SECTION\n";


setFixed("TARGET_TYPE_SYSTEM", @attrSystemIds);
setFixed("TARGET_TYPE_PROC_CHIP", @attrChipIds);
setFixed("TARGET_TYPE_CORE", @attrCoreIds);
setFixed("TARGET_TYPE_EQ", @attrEqIds);
setFixed("TARGET_TYPE_EX", @attrExIds);
setFixed("TARGET_TYPE_PERV", @attrPervIds);
print FILE "#endif //SBE_SEEPROM_FIXED_SECTION \n";
print FILE "} \n";
close FILE;


sub setFixed {
 
    my ($string, @entries) =  @_;

    foreach my $attr (@entries)
    {

        my $inname = $attr->{name};

        my @values = $attr->{value};


        if(scalar @values > 0) {
            foreach my $val (@values)
            {
                my $prefixVal1 = "";
                
                if($string eq "TARGET_TYPE_SYSTEM") {
                     $prefixVal1 = "G_system_attributes.fapi2attr::SystemAttributes_t::";
                } elsif($string eq "TARGET_TYPE_PROC_CHIP") {
                     $prefixVal1 = "G_proc_chip_attributes.fapi2attr::ProcChipAttributes_t::";
                } elsif($string eq "TARGET_TYPE_PERV") { 
                     $prefixVal1 = "G_perv_attributes.fapi2attr::PervAttributes_t::";
                } elsif($string eq "TARGET_TYPE_CORE") {
                     $prefixVal1 = "G_core_attributes.fapi2attr::CoreAttributes_t::";
                } elsif($string eq "TARGET_TYPE_EQ") {
                     $prefixVal1 = "G_eq_attributes.fapi2attr::EQAttributes_t::";
                } elsif($string eq "TARGET_TYPE_EX") { 
                     $prefixVal1 = "G_ex_attributes.fapi2attr::EXAttributes_t::";
                }
                if(defined $val && ref($val) eq "") {

                    if ($val =~ /(0x)?[0-9a-fA-F]+/) {

                        my $line = "$prefixVal1$inname=$val;\n";
                        print FILE $line;
                    } else {
                        print ("ppeSetFixedPibmem.pl ERROR. not hex\n");
                        exit(1);
                    }

                } elsif(defined $val && ref($val) eq "ARRAY") {

                    my $index = 0;

                    foreach my $arr (@{$val}) {

                        if(defined $arr && ref($arr) eq "") {
                            if ($arr =~ /(0x)?[0-9a-fA-F]+/) {

                                my $line = "$prefixVal1$inname\[$index\]=$arr;\n";
                                print FILE $line;

                            }
                        }
                        $index++;
                    }
                }
            }
        }
    }
}


