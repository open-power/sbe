
# Process the Doxygen html using the stadard CTE doxygen
CTEPATH = /afs/awd.austin.ibm.com/projects/cte
#DOXYGEN = $(CTEPATH)/tools/gsiexe/eclipz/doxygen
DOXYGEN = doxygen

.PHONY : dox
dox: 
	$(DOXYGEN) hcd_core.dox >hcd_core.dox.lst 2>&1


#  Publish the Doxygen HTML (pending final decision on where to put it)

#POKDOC = /afs/apd/func/vlsi/eclipz/common/doc/www/sys/ras/scom/p8/poreve

#publish:
#	rm -rf $(POKDOC)/doxygen
#	cp -a html $(POKDOC)/doxygen

