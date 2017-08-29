DIRCS=cs/LC_MESSAGES
DIRSK=sk/LC_MESSAGES

all:
	msgfmt -c -o $(DIRCS)/hello.mo $(DIRCS)/hello.po
	msgfmt -c -o $(DIRCS)/file.mo $(DIRCS)/file.po
	msgfmt -c -o $(DIRSK)/hello.mo $(DIRSK)/hello.po
	msgfmt -c -o $(DIRSK)/file.mo $(DIRSK)/file.po

clean:
	@- rm $(DIRCS)/hello.mo $(DIRCS)/file.mo
	@- rm $(DIRSK)/hello.mo $(DIRSK)/file.mo
