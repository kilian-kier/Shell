IDIR 	:= include
SDIR 	:= source

SRC 	:= $(wildcard $(SDIR)/*.c)
HEAD 	:= $(wildcard $(IDIR)/*.h)
OBJS 	:= $(SRC:.c=.o)

.PHONY: all
all: $(OBJS) shell clean

$(SDIR)/%.o: $(SDIR)/%.c $(HEAD)
	gcc -c $< -o $@

help: $(SDIR)/help.txt
	ld -r -b binary $(SDIR)/help.txt -o $(SDIR)/help.o

shell: $(OBJS) $(SDIR)/help.o
	gcc $(OBJS) $(SDIR)/help.o -o shell

clean:
	rm $(OBJS)



