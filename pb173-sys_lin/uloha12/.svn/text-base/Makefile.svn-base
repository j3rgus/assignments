SUBDIRS=libs src locale
SUBCLEAN=$(addsuffix .clean,$(SUBDIRS))

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
		$(MAKE) -C $@

.PHONY: clean $(SUBCLEAN)

clean: $(SUBCLEAN)

$(SUBCLEAN): %.clean:
		$(MAKE) -C $* clean
