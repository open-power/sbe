
# This Makefile compiles all of the cache hardware procedure code. See the
# "cachehcdfiles.mk" file in this directory.

#all generated files from this makefile will end up in obj/cache
export SUB_OBJDIR = /cache

include img_defs.mk
include cachehcdfiles.mk


OBJS := $(addprefix $(OBJDIR)/, $(CACHE_OBJECTS))

libcache.a: cache
	$(AR) crs $(OBJDIR)/libcache.a $(OBJDIR)/*.o

.PHONY: clean cache
cache: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -fr $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif
