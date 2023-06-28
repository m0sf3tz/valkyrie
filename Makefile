CC        = gcc
SRCFILES  = server.c
OUTPUT    = server
CFLAGS    += $(DEPFLAGS)

.PHONY: clean clean_img all 
all: $(OUTPUT)

$(OUTPUT) : server.c
	$(CC) server.c -o $@ 

clean_img:
	rm -f img_*

clean:
	rm -f $(OUTPUT)
	rm -f img_*
